/*
Copyright (C) 2005,2009 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// EASTL/internal/fixed_pool.h
// Written and maintained by Paul Pedriana - 2005.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file implements the following
//     aligned_buffer
//     fixed_pool_base
//     fixed_pool
//     fixed_pool_with_overflow
//     fixed_hashtable_allocator
//     fixed_vector_allocator
//     fixed_swap
//
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_INTERNAL_FIXED_POOL_H
#define EASTL_INTERNAL_FIXED_POOL_H


#include <EASTL/internal/config.h>
#include <EASTL/functional.h>
#include <EASTL/memory.h>
#include <EASTL/allocator.h>
#include <EASTL/type_traits.h>

#ifdef _MSC_VER
    #pragma warning(push, 0)
    #include <new>
    #pragma warning(pop)
#else
    #include <new>
#endif



namespace eastl
{

    /// EASTL_FIXED_POOL_DEFAULT_NAME
    ///
    /// Defines a default allocator name in the absence of a user-provided name.
    ///
    #ifndef EASTL_FIXED_POOL_DEFAULT_NAME
        #define EASTL_FIXED_POOL_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixed_pool" // Unless the user overrides something, this is "EASTL fixed_pool".
    #endif



    ///////////////////////////////////////////////////////////////////////////
    // aligned_buffer
    ///////////////////////////////////////////////////////////////////////////

    /// aligned_buffer
    ///
    /// This is useful for creating a buffer of the same size and alignment 
    /// of a given struct or class. This is useful for creating memory pools
    /// that support both size and alignment requirements of stored objects
    /// but without wasting space in over-allocating. 
    ///
    /// Note that we implement this via struct specializations, as some 
    /// compilers such as VC++ do not support specification of alignments
    /// in any way other than via an integral constant.
    ///
    /// Example usage:
    ///    struct Widget{ }; // This class has a given size and alignment.
    ///
    ///    Declare a char buffer of equal size and alignment to Widget.
    ///    aligned_buffer<sizeof(Widget), EASTL_ALIGN_OF(Widget)> mWidgetBuffer; 
    ///
    ///    Declare an array this time.
    ///    aligned_buffer<sizeof(Widget), EASTL_ALIGN_OF(Widget)> mWidgetArray[15]; 
    ///
    typedef char EASTL_MAY_ALIAS aligned_buffer_char; 

    template <size_t size, size_t alignment>
    struct aligned_buffer { aligned_buffer_char buffer[size]; };

    template<size_t size>
    struct aligned_buffer<size, 2>    { EA_PREFIX_ALIGN(2) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(2); };

    template<size_t size>
    struct aligned_buffer<size, 4>    { EA_PREFIX_ALIGN(4) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(4); };

    template<size_t size>
    struct aligned_buffer<size, 8>    { EA_PREFIX_ALIGN(8) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(8); };

    template<size_t size>
    struct aligned_buffer<size, 16>   { EA_PREFIX_ALIGN(16) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(16); };

    template<size_t size>
    struct aligned_buffer<size, 32>   { EA_PREFIX_ALIGN(32) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(32); };

    template<size_t size>
    struct aligned_buffer<size, 64>   { EA_PREFIX_ALIGN(64) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(64); };

    template<size_t size>
    struct aligned_buffer<size, 128>  { EA_PREFIX_ALIGN(128) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(128); };

    template<size_t size>
    struct aligned_buffer<size, 256>  { EA_PREFIX_ALIGN(256) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(256); };

    template<size_t size>
    struct aligned_buffer<size, 512>  { EA_PREFIX_ALIGN(512) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(512); };

    template<size_t size>
    struct aligned_buffer<size, 1024> { EA_PREFIX_ALIGN(1024) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(1024); };

    template<size_t size>
    struct aligned_buffer<size, 2048> { EA_PREFIX_ALIGN(2048) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(2048); };

    template<size_t size>
    struct aligned_buffer<size, 4096> { EA_PREFIX_ALIGN(4096) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(4096); };



    ///////////////////////////////////////////////////////////////////////////
    // fixed_pool_base
    ///////////////////////////////////////////////////////////////////////////

    /// fixed_pool_base
    ///
    /// This is a base class for the implementation of fixed-size pools.
    /// In particular, the fixed_pool and fixed_pool_with_overflow classes
    /// are based on fixed_pool_base.
    ///
    struct EASTL_API fixed_pool_base
    {
    public:
        /// fixed_pool_base
        ///
        fixed_pool_base(void* pMemory = NULL)
            : mpHead((Link*)pMemory)
        {
            #if EASTL_FIXED_SIZE_TRACKING_ENABLED
                mnCurrentSize = 0;
                mnPeakSize    = 0;
            #endif
        }


        /// init
        ///
        /// Initializes a fixed_pool with a given set of parameters.
        /// You cannot call this function twice else the resulting 
        /// behaviour will be undefined. You can only call this function
        /// after constructing the fixed_pool with the default constructor.
        ///
        void init(void* pMemory, size_t memorySize, size_t nodeSize,
                  size_t alignment, size_t alignmentOffset = 0);


        /// peak_size
        ///
        /// Returns the maximum number of outstanding allocations there have been
        /// at any one time. This represents a high water mark for the allocation count.
        ///
        size_t peak_size() const
        {
            #if EASTL_FIXED_SIZE_TRACKING_ENABLED
                return mnPeakSize;
            #else
                return 0;
            #endif
        }


        /// can_allocate
        ///
        /// Returns true if there are any free links.
        ///
        bool can_allocate() const
        {
            return (mpHead != NULL);
        }

    public:
        /// Link
        /// Implements a singly-linked list.
        struct Link
        {
            Link* mpNext;
        };

        Link* mpHead;

        #if EASTL_FIXED_SIZE_TRACKING_ENABLED
            uint32_t mnCurrentSize; /// Current number of allocated nodes.
            uint32_t mnPeakSize;    /// Max number of allocated nodes at any one time.
        #endif

    }; // fixed_pool_base





    ///////////////////////////////////////////////////////////////////////////
    // fixed_pool
    ///////////////////////////////////////////////////////////////////////////

    /// fixed_pool
    ///
    /// Implements a simple fixed pool allocator for use by fixed-size containers. 
    /// This is not a generic eastl allocator which can be plugged into an arbitrary
    /// eastl container, as it simplifies some functions are arguments for the 
    /// purpose of efficiency.
    /// 
    class EASTL_API fixed_pool : public fixed_pool_base
    {
    public:
        /// fixed_pool
        ///
        /// Default constructor. User usually will want to call init() after  
        /// constructing via this constructor. The pMemory argument is for the 
        /// purposes of temporarily storing a pointer to the buffer to be used.
        /// Even though init may have a pMemory argument, this arg is useful 
        /// for temporary storage, as per copy construction.
        ///
        fixed_pool(void* pMemory = NULL)
            : fixed_pool_base(pMemory)
        {
        }


        /// fixed_pool
        ///
        /// Constructs a fixed_pool with a given set of parameters.
        ///
        fixed_pool(void* pMemory, size_t memorySize, size_t nodeSize, 
                    size_t alignment, size_t alignmentOffset = 0)
        {
            init(pMemory, memorySize, nodeSize, alignment, alignmentOffset);
        }


        /// allocate
        ///
        /// Allocates a new object of the size specified upon class initialization.
        /// Returns NULL if there is no more memory. 
        ///
        void* allocate()
        {
            Link* const pLink = mpHead;

            if(pLink) // If we have space...
            {
                #if EASTL_FIXED_SIZE_TRACKING_ENABLED
                    if(++mnCurrentSize > mnPeakSize)
                        mnPeakSize = mnCurrentSize;
                #endif

                mpHead = pLink->mpNext;
                return pLink;
            }

            // EASTL_ASSERT(false); To consider: enable this assert. However, we intentionally disable it because this isn't necessarily an assertable error.
            return NULL;
        }


        /// deallocate
        ///
        /// Frees the given object which was allocated by allocate(). 
        /// If the given node was not allocated by allocate() then the behaviour 
        /// is undefined.
        ///
        void deallocate(void* p)
        {
            #if EASTL_FIXED_SIZE_TRACKING_ENABLED
                --mnCurrentSize;
            #endif

            ((Link*)p)->mpNext = mpHead;
            mpHead = ((Link*)p);
        }


        using fixed_pool_base::can_allocate;


        const char* get_name() const
        {
            return EASTL_FIXED_POOL_DEFAULT_NAME;
        }


        void set_name(const char*)
        {
            // Nothing to do. We don't allocate memory.
        }

    }; // fixed_pool





    ///////////////////////////////////////////////////////////////////////////
    // fixed_pool_with_overflow
    ///////////////////////////////////////////////////////////////////////////

    /// fixed_pool_with_overflow
    ///
    template <typename Allocator = EASTLAllocatorType>
    class fixed_pool_with_overflow : public fixed_pool_base
    {
    public:
        fixed_pool_with_overflow(void* pMemory = NULL)
            : fixed_pool_base(pMemory),
              mOverflowAllocator(EASTL_FIXED_POOL_DEFAULT_NAME)
        {
            // Leave mpPoolBegin, mpPoolEnd uninitialized.
        }


        fixed_pool_with_overflow(void* pMemory, size_t memorySize, size_t nodeSize, 
                                 size_t alignment, size_t alignmentOffset = 0)
            : mOverflowAllocator(EASTL_FIXED_POOL_DEFAULT_NAME)
        {
            fixed_pool_base::init(pMemory, memorySize, nodeSize, alignment, alignmentOffset);

            mpPoolBegin = pMemory;
            mpPoolEnd   = (void*)((uintptr_t)pMemory + memorySize);
            mnNodeSize  = (eastl_size_t)nodeSize;
        }


        void init(void* pMemory, size_t memorySize, size_t nodeSize,
                    size_t alignment, size_t alignmentOffset = 0)
        {
            fixed_pool_base::init(pMemory, memorySize, nodeSize, alignment, alignmentOffset);

            mpPoolBegin = pMemory;
            mpPoolEnd   = (void*)((uintptr_t)pMemory + memorySize);
            mnNodeSize  = (eastl_size_t)nodeSize;
        }


        void* allocate()
        {
            void* p;

            if(mpHead) // If we have space...
            {
                p      = mpHead;
                mpHead = mpHead->mpNext;
            }
            else
                p = mOverflowAllocator.allocate(mnNodeSize);

            #if EASTL_FIXED_SIZE_TRACKING_ENABLED
                if(p && (++mnCurrentSize > mnPeakSize))
                    mnPeakSize = mnCurrentSize;
            #endif

            return p;
        }


        void deallocate(void* p)
        {
            #if EASTL_FIXED_SIZE_TRACKING_ENABLED
                --mnCurrentSize;
            #endif

            if((p >= mpPoolBegin) && (p < mpPoolEnd))
            {
                ((Link*)p)->mpNext = mpHead;
                mpHead = ((Link*)p);
            }
            else
                mOverflowAllocator.deallocate(p, (size_t)mnNodeSize);
        }


        using fixed_pool_base::can_allocate;


        const char* get_name() const
        {
            return mOverflowAllocator.get_name();
        }


        void set_name(const char* pName)
        {
            mOverflowAllocator.set_name(pName);
        }

    protected:
        Allocator          mOverflowAllocator;  // 
        void*              mpPoolBegin;         // To consider: We have these member variables and ideally we shouldn't need them. The problem is that 
        void*              mpPoolEnd;           //              the information about the pool buffer and object size is stored in the owning container 
        eastl_size_t       mnNodeSize;          //              and we can't have access to it without increasing the amount of code we need and by templating 
                                                //              more code. It may turn out that simply storing data here is smaller in the end.
    }; // fixed_pool_with_overflow              //              Granted, this class is usually used for debugging purposes, but perhaps there is an elegant solution.





    ///////////////////////////////////////////////////////////////////////////
    // fixed_node_pool
    ///////////////////////////////////////////////////////////////////////////

    /// fixed_node_pool
    ///
    /// Implements a fixed_pool with a given node count, alignment, and alignment offset.
    /// fixed_node_pool is like fixed_pool except it is templated on the node type instead
    /// of being a generic allocator. All it does is pass allocations through to
    /// the fixed_pool base. This functionality is separate from fixed_pool because there
    /// are other uses for fixed_pool.
    ///
    /// We template on kNodeSize instead of node_type because the former allows for the
    /// two different node_types of the same size to use the same template implementation.
    ///
    /// Template parameters:
    ///     nodeSize               The size of the object to allocate.
    ///     nodeCount              The number of objects the pool contains.
    ///     nodeAlignment          The alignment of the objects to allocate.
    ///     nodeAlignmentOffset    The alignment offset of the objects to allocate.
    ///     bEnableOverflow        Whether or not we should use the overflow heap if our object pool is exhausted.
    ///     Allocator              Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
    ///
    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename Allocator = EASTLAllocatorType>
    class fixed_node_pool
    {
    public:
        typedef typename type_select<bEnableOverflow, fixed_pool_with_overflow<Allocator>, fixed_pool>::type  pool_type;
        typedef fixed_node_pool<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>   this_type;

        enum
        {
            kNodeSize            = nodeSize,
            kNodeCount           = nodeCount,
            kNodesSize           = nodeCount * nodeSize,
            kBufferSize          = kNodesSize + ((nodeAlignment > 1) ? nodeSize : 0) + nodeAlignmentOffset,
            kNodeAlignment       = nodeAlignment,
            kNodeAlignmentOffset = nodeAlignmentOffset
        };

    public:
        pool_type mPool;

    public:
        //fixed_node_pool(const char* pName)
        //{
        //    mPool.set_name(pName);
        //}


        fixed_node_pool(void* pNodeBuffer)
            : mPool(pNodeBuffer, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset)
        {
        }


        /// fixed_node_pool
        ///
        /// Note that we are copying x.mpHead to our own fixed_pool. This at first may seem 
        /// broken, as fixed pools cannot take over ownership of other fixed pools' memory.
        /// However, we declare that this copy ctor can only ever be safely called when 
        /// the user has intentionally pre-seeded the source with the destination pointer.
        /// This is somewhat playing with fire, but it allows us to get around chicken-and-egg
        /// problems with containers being their own allocators, without incurring any memory
        /// costs or extra code costs. There's another reason for this: we very strongly want
        /// to avoid full copying of instances of fixed_pool around, espcially via the stack.
        /// Larger pools won't even be able to fit on many machine's stacks. So this solution
        /// is also a mechanism to prevent that situation from existing and being used. 
        /// Perhaps some day we'll find a more elegant yet costless way around this. 
        ///
        fixed_node_pool(const this_type& x)
            : mPool(x.mPool.mpHead, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset)
        {
        }


        this_type& operator=(const this_type&)
        {
            return *this; // Do nothing. Ignore the source type.
        }


        #if EASTL_ASSERT_ENABLED
        void* allocate(size_t   n,   int /*flags*/ = 0)
        #else
        void* allocate(size_t /*n*/, int /*flags*/ = 0)
        #endif
        {
            EASTL_ASSERT(n == kNodeSize);
            return mPool.allocate();
        }


        #if EASTL_ASSERT_ENABLED
        void* allocate(size_t   n,   size_t /*alignment*/, size_t /*offset*/, int /*flags*/ = 0)
        #else
        void* allocate(size_t /*n*/, size_t /*alignment*/, size_t /*offset*/, int /*flags*/ = 0)
        #endif
        {
            EASTL_ASSERT(n == kNodeSize);
            return mPool.allocate();
        }


        void deallocate(void* p, size_t)
        {
            mPool.deallocate(p);
        }


        /// can_allocate
        ///
        /// Returns true if there are any free links.
        ///
        bool can_allocate() const
        {
            return mPool.can_allocate();
        }


        /// reset
        ///
        /// This function unilaterally resets the fixed pool back to a newly initialized
        /// state. This is useful for using in tandem with container reset functionality.
        ///
        void reset(void* pNodeBuffer)
        {
            mPool.init(pNodeBuffer, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset);
        }


        const char* get_name() const
        {
            return mPool.get_name();
        }


        void set_name(const char* pName)
        {
            mPool.set_name(pName);
        }

    }; // fixed_node_pool


    ///////////////////////////////////////////////////////////////////////
    // global operators
    ///////////////////////////////////////////////////////////////////////

    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename Allocator>
    inline bool operator==(const fixed_node_pool<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>& a, 
                           const fixed_node_pool<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>& b)
    {
        return (&a == &b); // They are only equal if they are the same object.
    }


    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename Allocator>
    inline bool operator!=(const fixed_node_pool<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>& a, 
                           const fixed_node_pool<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>& b)
    {
        return (&a != &b); // They are only equal if they are the same object.
    }






    ///////////////////////////////////////////////////////////////////////////
    // fixed_hashtable_allocator
    ///////////////////////////////////////////////////////////////////////////

    /// fixed_hashtable_allocator
    ///
    /// Provides a base class for fixed hashtable allocations.
    /// To consider: Have this inherit from fixed_node_pool.
    ///
    /// Template parameters:
    ///     bucketCount            The fixed number of hashtable buckets to provide.
    ///     nodeCount              The number of objects the pool contains.
    ///     nodeAlignment          The alignment of the objects to allocate.
    ///     nodeAlignmentOffset    The alignment offset of the objects to allocate.
    ///     bEnableOverflow        Whether or not we should use the overflow heap if our object pool is exhausted.
    ///     Allocator              Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
    ///
    template <size_t bucketCount, size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename Allocator = EASTLAllocatorType>
    class fixed_hashtable_allocator
    {
    public:
        typedef typename type_select<bEnableOverflow, fixed_pool_with_overflow<Allocator>, fixed_pool>::type                                 pool_type;
        typedef fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>  this_type;

        enum
        {
            kBucketCount         = bucketCount + 1, // '+1' because the hash table needs a null terminating bucket.
            kBucketsSize         = bucketCount * sizeof(void*),
            kNodeSize            = nodeSize,
            kNodeCount           = nodeCount,
            kNodesSize           = nodeCount * nodeSize,
            kBufferSize          = kBucketsSize + kNodesSize + ((nodeAlignment > 1) ? nodeSize : 0) + nodeAlignmentOffset,
            kNodeAlignment       = nodeAlignment,
            kNodeAlignmentOffset = nodeAlignmentOffset,
            kAllocFlagBuckets    = 0x00400000               // Flag to allocator which indicates that we are allocating buckets and not nodes.
        };

    public:
        pool_type mPool;
        void*     mpBucketBuffer;

    public:
        //fixed_hashtable_allocator(const char* pName)
        //{
        //    mPool.set_name(pName);
        //}

        fixed_hashtable_allocator(void* pNodeBuffer)
            : mPool(pNodeBuffer, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset),
              mpBucketBuffer(NULL)
        {
            // EASTL_ASSERT(false); // As it stands now, this is not supposed to be called.
        }


        fixed_hashtable_allocator(void* pNodeBuffer, void* pBucketBuffer)
            : mPool(pNodeBuffer, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset),
              mpBucketBuffer(pBucketBuffer)
        {
        }


        /// fixed_hashtable_allocator
        ///
        /// Note that we are copying x.mpHead and mpBucketBuffer to our own fixed_pool. 
        /// See the discussion above in fixed_node_pool for important information about this.
        ///
        fixed_hashtable_allocator(const this_type& x)
            : mPool(x.mPool.mpHead, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset),
              mpBucketBuffer(x.mpBucketBuffer)
        {
        }


        fixed_hashtable_allocator& operator=(const fixed_hashtable_allocator&)
        {
            return *this; // Do nothing. Ignore the source type.
        }


        void* allocate(size_t n, int flags = 0)
        {
            // We expect that the caller uses kAllocFlagBuckets when it wants us to allocate buckets instead of nodes.
            EASTL_CT_ASSERT(kAllocFlagBuckets == 0x00400000); // Currently we expect this to be so, because the hashtable has a copy of this enum.
            if((flags & kAllocFlagBuckets) == 0) // If we are allocating nodes and (probably) not buckets...
            {
                EASTL_ASSERT(n == kNodeSize);
                return mPool.allocate();
            }

            EASTL_ASSERT(n <= kBucketsSize);
            return mpBucketBuffer;
        }


        void* allocate(size_t n, size_t /*alignment*/, size_t /*offset*/, int flags = 0)
        {
            // We expect that the caller uses kAllocFlagBuckets when it wants us to allocate buckets instead of nodes.
            if((flags & kAllocFlagBuckets) == 0) // If we are allocating nodes and (probably) not buckets...
            {
                EASTL_ASSERT(n == kNodeSize);
                return mPool.allocate();
            }

            // To consider: allow for bucket allocations to overflow.
            EASTL_ASSERT(n <= kBucketsSize);
            return mpBucketBuffer;
        }


        void deallocate(void* p, size_t)
        {
            if(p != mpBucketBuffer) // If we are freeing a node and not buckets...
                mPool.deallocate(p);
        }


        bool can_allocate() const
        {
            return mPool.can_allocate();
        }


        void reset(void* pNodeBuffer)
        {
            // No need to modify mpBucketBuffer, as that is constant.
            mPool.init(pNodeBuffer, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset);
        }


        const char* get_name() const
        {
            return mPool.get_name();
        }


        void set_name(const char* pName)
        {
            mPool.set_name(pName);
        }

    }; // fixed_hashtable_allocator


    ///////////////////////////////////////////////////////////////////////
    // global operators
    ///////////////////////////////////////////////////////////////////////

    template <size_t bucketCount, size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename Allocator>
    inline bool operator==(const fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>& a, 
                           const fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>& b)
    {
        return (&a == &b); // They are only equal if they are the same object.
    }


    template <size_t bucketCount, size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename Allocator>
    inline bool operator!=(const fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>& a, 
                           const fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>& b)
    {
        return (&a != &b); // They are only equal if they are the same object.
    }






    ///////////////////////////////////////////////////////////////////////////
    // fixed_vector_allocator
    ///////////////////////////////////////////////////////////////////////////

    /// fixed_vector_allocator
    ///
    /// Template parameters:
    ///     nodeSize               The size of individual objects.
    ///     nodeCount              The number of objects the pool contains.
    ///     nodeAlignment          The alignment of the objects to allocate.
    ///     nodeAlignmentOffset    The alignment offset of the objects to allocate.
    ///     bEnableOverflow        Whether or not we should use the overflow heap if our object pool is exhausted.
    ///     Allocator              Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
    ///
    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename Allocator = EASTLAllocatorType>
    class fixed_vector_allocator
    {
    public:
        typedef fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>  this_type;
        typedef Allocator overflow_allocator_type;

        enum
        {
            kNodeSize            = nodeSize,
            kNodeCount           = nodeCount,
            kNodesSize           = nodeCount * nodeSize,
            kBufferSize          = kNodesSize + ((nodeAlignment > 1) ? nodeSize : 0) + nodeAlignmentOffset,
            kNodeAlignment       = nodeAlignment,
            kNodeAlignmentOffset = nodeAlignmentOffset
        };

    public:
        overflow_allocator_type mOverflowAllocator;
        void*                   mpPoolBegin;         // To consider: Find some way to make this data unnecessary, without increasing template proliferation.

    public:
        //fixed_vector_allocator(const char* pName = NULL)
        //{
        //    mOverflowAllocator.set_name(pName);
        //}

        fixed_vector_allocator(void* pNodeBuffer)
            : mpPoolBegin(pNodeBuffer)
        {
        }

        fixed_vector_allocator& operator=(const fixed_vector_allocator&)
        {
            return *this; // Do nothing. Ignore the source type.
        }

        void* allocate(size_t n, int flags = 0)
        {
            return mOverflowAllocator.allocate(n, flags);
        }

        void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0)
        {
            return mOverflowAllocator.allocate(n, alignment, offset, flags);
        }

        void deallocate(void* p, size_t n)
        {
            if(p != mpPoolBegin)
                mOverflowAllocator.deallocate(p, n); // Can't do this to our own allocation.
        }

        const char* get_name() const
        {
            return mOverflowAllocator.get_name();
        }

        void set_name(const char* pName)
        {
            mOverflowAllocator.set_name(pName);
        }

        overflow_allocator_type& get_overflow_allocator()
        {
            return mOverflowAllocator;
        }

        void set_overflow_allocator(const overflow_allocator_type& allocator)
        {
            mOverflowAllocator = allocator;
        }

    }; // fixed_vector_allocator


    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, typename Allocator>
    class fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, false, Allocator>
    {
    public:
        typedef fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, false, Allocator>  this_type;
        typedef Allocator overflow_allocator_type;

        enum
        {
            kNodeSize            = nodeSize,
            kNodeCount           = nodeCount,
            kNodesSize           = nodeCount * nodeSize,
            kBufferSize          = kNodesSize + ((nodeAlignment > 1) ? nodeSize : 0) + nodeAlignmentOffset,
            kNodeAlignment       = nodeAlignment,
            kNodeAlignmentOffset = nodeAlignmentOffset
        };

        //fixed_vector_allocator(const char* = NULL) // This char* parameter is present so that this class can be like the other version.
        //{
        //}

        fixed_vector_allocator(void* /*pNodeBuffer*/)
        {
        }

        void* allocate(size_t /*n*/, int /*flags*/ = 0)
        {
            EASTL_ASSERT(false); // A fixed_vector should not reallocate, else the user has exhausted its space.
            return NULL;
        }

        void* allocate(size_t /*n*/, size_t /*alignment*/, size_t /*offset*/, int /*flags*/ = 0)
        {
            EASTL_ASSERT(false);
            return NULL;
        }

        void deallocate(void* /*p*/, size_t /*n*/)
        {
        }

        const char* get_name() const
        {
            return EASTL_FIXED_POOL_DEFAULT_NAME;
        }

        void set_name(const char* /*pName*/)
        {
        }

        overflow_allocator_type& get_overflow_allocator()
        {
            return *(overflow_allocator_type*)NULL; // This is not pretty.
        }

        void set_overflow_allocator(const overflow_allocator_type& /*allocator*/)
        {
            // We don't have an overflow allocator.
        }

    }; // fixed_vector_allocator


    ///////////////////////////////////////////////////////////////////////
    // global operators
    ///////////////////////////////////////////////////////////////////////

    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename Allocator>
    inline bool operator==(const fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>& a, 
                           const fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>& b)
    {
        return (&a == &b); // They are only equal if they are the same object.
    }


    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename Allocator>
    inline bool operator!=(const fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>& a, 
                           const fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, Allocator>& b)
    {
        return (&a != &b); // They are only equal if they are the same object.
    }





    ///////////////////////////////////////////////////////////////////////////
    // fixed_swap
    ///////////////////////////////////////////////////////////////////////////

    /// fixed_swap
    ///
    /// This function implements a swap suitable for fixed containers.
    /// This is an issue because the size of fixed containers can be very 
    /// large, due to their having the container buffer within themselves.
    /// Note that we are referring to sizeof(container) and not the total
    /// sum of memory allocated by the container from the heap. 
    ///
    template <typename Container>
    void fixed_swap(Container& a, Container& b)
    {
        // We must do a brute-force swap, because fixed containers cannot share memory allocations.
        eastl::less<size_t> compare;

        if(compare(sizeof(a), EASTL_MAX_STACK_USAGE)) // Using compare instead of just '<' avoids a stubborn compiler warning.
        {
            // Note: The C++ language does not define what happens when you declare 
            // an object in too small of stack space but the object is never created.
            // This may result in a stack overflow exception on some systems, depending
            // on how they work and possibly depending on enabled debug functionality.

            const Container temp(a); // Can't use global swap because that could
            a = b;                   // itself call this swap function in return.
            b = temp;
        }
        else
        {
            EASTLAllocatorType allocator(*EASTLAllocatorDefault(), EASTL_TEMP_DEFAULT_NAME);
            void* const pMemory = allocator.allocate(sizeof(a));

            if(pMemory)
            {
                Container* const pTemp = new(pMemory) Container(a);
                a = b;
                b = *pTemp;

                pTemp->~Container();
                allocator.deallocate(pMemory, sizeof(a));
            }
        }
    }



} // namespace eastl


#endif // Header include guard



































