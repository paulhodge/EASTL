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
// EASTL/sort.h
// Written by Paul Pedriana - 2005.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// This file implements sorting algorithms. Some of these are equivalent to 
// std C++ sorting algorithms, while others don't have equivalents in the 
// C++ standard. 
//////////////////////////////////////////////////////////////////////////////

#ifndef EASTL_SORT_H
#define EASTL_SORT_H



#include <EASTL/internal/config.h>
#include <EASTL/iterator.h>
#include <EASTL/memory.h>
#include <EASTL/algorithm.h>
#include <EASTL/functional.h>
#include <EASTL/heap.h>
#include <EASTL/allocator.h>
#include <EASTL/memory.h>



namespace eastl
{



    /// insertion_sort
    ///
    /// Implements the InsertionSort algorithm. 
    ///
    template <typename BidirectionalIterator, typename StrictWeakOrdering>
    void insertion_sort(BidirectionalIterator first, BidirectionalIterator last, StrictWeakOrdering compare)
    {
        typedef typename eastl::iterator_traits<BidirectionalIterator>::value_type value_type;

        if(first != last)
        {
            BidirectionalIterator iCurrent, iNext, iSorted = first;

            for(++iSorted; iSorted != last; ++iSorted)
            {
                const value_type temp(*iSorted);

                iNext = iCurrent = iSorted;

                for(--iCurrent; (iNext != first) && compare(temp, *iCurrent); --iNext, --iCurrent)
                {
                    EASTL_VALIDATE_COMPARE(!compare(*iCurrent, temp)); // Validate that the compare function is sane.
                    *iNext = *iCurrent;
                }

                *iNext = temp;
            }
        }
    } // insertion_sort



    template <typename BidirectionalIterator>
    void insertion_sort(BidirectionalIterator first, BidirectionalIterator last)
    {
        typedef typename eastl::iterator_traits<BidirectionalIterator>::value_type value_type;

        if(first != last)
        {
            BidirectionalIterator iCurrent, iNext, iSorted = first;

            for(++iSorted; iSorted != last; ++iSorted)
            {
                const value_type temp(*iSorted);

                iNext = iCurrent = iSorted;

                for(--iCurrent; (iNext != first) && (temp < *iCurrent); --iNext, --iCurrent)
                {
                    EASTL_VALIDATE_COMPARE(!(*iCurrent < temp)); // Validate that the compare function is sane.
                    *iNext = *iCurrent;
                }

                *iNext = temp;
            }
        }
    } // insertion_sort


    /////////////////////////////////////////////////////////////////////
    // quick_sort
    //
    // We do the "introspection sort" variant of quick sort which is now
    // well-known and understood. You can read about this algorithm in
    // many articles on quick sort, but briefly what it does is a median-
    // of-three quick sort whereby the recursion depth is limited to a
    // some value (after which it gives up on quick sort and switches to
    // a heap sort) and whereby after a certain amount of sorting the 
    // algorithm stops doing quick-sort and finishes the sorting via
    // a simple insertion sort.
    /////////////////////////////////////////////////////////////////////

    static const int kQuickSortLimit = 28; // For sorts of random arrays over 100 items, 28 - 32 have been found to be good numbers on VC++/Win32.

    namespace Internal
    {
        template <typename Size>
        inline Size Log2(Size n)
        {
            int i;
            for(i = 0; n; ++i)
                n >>= 1;
            return i - 1;
        }

        // To do: Investigate the speed of this bit-trick version of Log2.
        //        It may work better on some platforms but not others.
        //
        // union FloatUnion {
        //     float    f;
        //     uint32_t i;
        // };
        // 
        // inline uint32_t Log2(uint32_t x)
        // {
        //     const FloatInt32Union u = { x };
        //     return (u.i >> 23) - 127;
        // }
    }


    /// get_partition
    ///
    /// This function takes const T& instead of T because T may have special alignment
    /// requirements and some compilers (e.g. VC++) are don't respect alignment requirements
    /// for function arguments.
    ///
    template <typename RandomAccessIterator, typename T>
    inline RandomAccessIterator get_partition(RandomAccessIterator first, RandomAccessIterator last, const T& pivotValue)
    {
        const T pivotCopy(pivotValue); // Need to make a temporary because the sequence below is mutating.

        for(; ; ++first)
        {
            while(*first < pivotCopy)
            {
                EASTL_VALIDATE_COMPARE(!(pivotCopy < *first)); // Validate that the compare function is sane.
                ++first;
            }
            --last;

            while(pivotCopy < *last)
            {
                EASTL_VALIDATE_COMPARE(!(*last < pivotCopy)); // Validate that the compare function is sane.
                --last;
            }

            if(first >= last) // Random access iterators allow operator >=
                return first;

            eastl::iter_swap(first, last);
        }
    }


    template <typename RandomAccessIterator, typename T, typename Compare>
    inline RandomAccessIterator get_partition(RandomAccessIterator first, RandomAccessIterator last, const T& pivotValue, Compare compare)
    {
        const T pivotCopy(pivotValue); // Need to make a temporary because the sequence below is mutating.

        for(; ; ++first)
        {
            while(compare(*first, pivotCopy))
            {
                EASTL_VALIDATE_COMPARE(!compare(pivotCopy, *first)); // Validate that the compare function is sane.
                ++first;
            }
            --last;

            while(compare(pivotCopy, *last))
            {
                EASTL_VALIDATE_COMPARE(!compare(*last, pivotCopy)); // Validate that the compare function is sane.
                --last;
            }

            if(first >= last) // Random access iterators allow operator >=
                return first;

            eastl::iter_swap(first, last);
        }
    }


    template <typename RandomAccessIterator>
    inline void insertion_sort_simple(RandomAccessIterator first, RandomAccessIterator last)
    {
        for(RandomAccessIterator current = first; current != last; ++current)
        {
            typedef typename eastl::iterator_traits<RandomAccessIterator>::value_type value_type;

            RandomAccessIterator end(current), prev(current);
            const value_type     value(*current);

            for(--prev; value < *prev; --end, --prev)
            {
                EASTL_VALIDATE_COMPARE(!(*prev < value)); // Validate that the compare function is sane.
                *end = *prev;
            }

            *end = value;
        }
    }


    template <typename RandomAccessIterator, typename Compare>
    inline void insertion_sort_simple(RandomAccessIterator first, RandomAccessIterator last, Compare compare)
    {
        for(RandomAccessIterator current = first; current != last; ++current)
        {
            typedef typename eastl::iterator_traits<RandomAccessIterator>::value_type value_type;

            RandomAccessIterator end(current), prev(current);
            const value_type     value(*current);

            for(--prev; compare(value, *prev); --end, --prev)
            {
                EASTL_VALIDATE_COMPARE(!compare(*prev, value)); // Validate that the compare function is sane.
                *end = *prev;
            }

            *end = value;
        }
    }


    template <typename RandomAccessIterator>
    inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last)
    {
        typedef typename eastl::iterator_traits<RandomAccessIterator>::difference_type difference_type;
        typedef typename eastl::iterator_traits<RandomAccessIterator>::value_type      value_type;

        eastl::make_heap<RandomAccessIterator>(first, middle);

        for(RandomAccessIterator i = middle; i < last; ++i)
        {
            if(*i < *first)
            {
                EASTL_VALIDATE_COMPARE(!(*first < *i)); // Validate that the compare function is sane.
                const value_type temp(*i);
                *i = *first;
                eastl::adjust_heap<RandomAccessIterator, difference_type, value_type>
                                  (first, difference_type(0), difference_type(middle - first), difference_type(0), temp);
            }
        }

        eastl::sort_heap<RandomAccessIterator>(first, middle);
    }


    template <typename RandomAccessIterator, typename Compare>
    inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare compare)
    {
        typedef typename eastl::iterator_traits<RandomAccessIterator>::difference_type difference_type;
        typedef typename eastl::iterator_traits<RandomAccessIterator>::value_type      value_type;

        eastl::make_heap<RandomAccessIterator, Compare>(first, middle, compare);

        for(RandomAccessIterator i = middle; i < last; ++i)
        {
            if(compare(*i, *first))
            {
                EASTL_VALIDATE_COMPARE(!compare(*first, *i)); // Validate that the compare function is sane.
                const value_type temp(*i);
                *i = *first;
                eastl::adjust_heap<RandomAccessIterator, difference_type, value_type, Compare>
                                  (first, difference_type(0), difference_type(middle - first), difference_type(0), temp, compare);
            }
        }

        eastl::sort_heap<RandomAccessIterator, Compare>(first, middle, compare);
    }


    template<typename RandomAccessIterator>
    inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

        while((last - first) > 5)
        {
            const value_type           midValue(eastl::median<value_type>(*first, *(first + (last - first) / 2), *(last - 1)));
            const RandomAccessIterator midPos(eastl::get_partition<RandomAccessIterator, value_type>(first, last, midValue));

            if(midPos <= nth)
                first = midPos;
            else
                last = midPos;
        }

        eastl::insertion_sort<RandomAccessIterator>(first, last);
    }


    template<typename RandomAccessIterator, typename Compare>
    inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, Compare compare)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

        while((last - first) > 5)
        {
            const value_type           midValue(eastl::median<value_type, Compare>(*first, *(first + (last - first) / 2), *(last - 1), compare));
            const RandomAccessIterator midPos(eastl::get_partition<RandomAccessIterator, value_type, Compare>(first, last, midValue, compare));

            if(midPos <= nth)
                first = midPos;
            else
                last = midPos;
        }

        eastl::insertion_sort<RandomAccessIterator, Compare>(first, last, compare);
    }


    template <typename RandomAccessIterator, typename Size>
    inline void quick_sort_impl(RandomAccessIterator first, RandomAccessIterator last, Size kRecursionCount)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

        while(((last - first) > kQuickSortLimit) && (kRecursionCount > 0))
        {
            const RandomAccessIterator position(eastl::get_partition<RandomAccessIterator, value_type>(first, last, eastl::median<value_type>(*first, *(first + (last - first) / 2), *(last - 1))));

            eastl::quick_sort_impl<RandomAccessIterator, Size>(position, last, --kRecursionCount);
            last = position;
        }

        if(kRecursionCount == 0)
            eastl::partial_sort<RandomAccessIterator>(first, last, last);
    }


    template <typename RandomAccessIterator, typename Size, typename Compare>
    inline void quick_sort_impl(RandomAccessIterator first, RandomAccessIterator last, Size kRecursionCount, Compare compare)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

        while(((last - first) > kQuickSortLimit) && (kRecursionCount > 0))
        {
            const RandomAccessIterator position(eastl::get_partition<RandomAccessIterator, value_type, Compare>(first, last, eastl::median<value_type, Compare>(*first, *(first + (last - first) / 2), *(last - 1), compare), compare));

            eastl::quick_sort_impl<RandomAccessIterator, Size, Compare>(position, last, --kRecursionCount, compare);
            last = position;
        }

        if(kRecursionCount == 0)
            eastl::partial_sort<RandomAccessIterator, Compare>(first, last, last, compare);
    }

    /// quick_sort
    ///
    /// quick_sort sorts the elements in [first, last) into ascending order, 
    /// meaning that if i and j are any two valid iterators in [first, last) 
    /// such that i precedes j, then *j is not less than *i. quick_sort is not 
    /// guaranteed to be stable. That is, suppose that *i and *j are equivalent: 
    /// neither one is less than the other. It is not guaranteed that the 
    /// relative order of these two elements will be preserved by sort.
    ///
    /// We implement the "introspective" variation of quick-sort. This is 
    /// considered to be the best general-purpose variant, as it avoids 
    /// worst-case behaviour and optimizes the final sorting stage by 
    /// switching to an insertion sort.
    ///
    template <typename RandomAccessIterator>
    void quick_sort(RandomAccessIterator first, RandomAccessIterator last)
    {
        typedef typename eastl::iterator_traits<RandomAccessIterator>::difference_type difference_type;

        if(first != last)
        {
            eastl::quick_sort_impl<RandomAccessIterator, difference_type>(first, last, 2 * Internal::Log2(last - first));

            if((last - first) > (difference_type)kQuickSortLimit)
            {
                eastl::insertion_sort<RandomAccessIterator>(first, first + kQuickSortLimit);
                eastl::insertion_sort_simple<RandomAccessIterator>(first + kQuickSortLimit, last);
            }
            else
                eastl::insertion_sort<RandomAccessIterator>(first, last);
        }
    }


    template <typename RandomAccessIterator, typename Compare>
    void quick_sort(RandomAccessIterator first, RandomAccessIterator last, Compare compare)
    {
        typedef typename eastl::iterator_traits<RandomAccessIterator>::difference_type difference_type;

        if(first != last)
        {
            eastl::quick_sort_impl<RandomAccessIterator, difference_type, Compare>(first, last, 2 * Internal::Log2(last - first), compare);

            if((last - first) > (difference_type)kQuickSortLimit)
            {
                eastl::insertion_sort<RandomAccessIterator, Compare>(first, first + kQuickSortLimit, compare);
                eastl::insertion_sort_simple<RandomAccessIterator, Compare>(first + kQuickSortLimit, last, compare);
            }
            else
                eastl::insertion_sort<RandomAccessIterator, Compare>(first, last, compare);
        }
    }

    /// sort
    /// 
    /// We simply use quick_sort. See quick_sort for details.
    ///
    template <typename RandomAccessIterator>
    inline void sort(RandomAccessIterator first, RandomAccessIterator last)
    {
        eastl::quick_sort<RandomAccessIterator>(first, last);
    }

    template <typename RandomAccessIterator, typename Compare>
    inline void sort(RandomAccessIterator first, RandomAccessIterator last, Compare compare)
    {
        eastl::quick_sort<RandomAccessIterator, Compare>(first, last, compare);
    }

} // namespace eastl


#endif // Header include guard

















