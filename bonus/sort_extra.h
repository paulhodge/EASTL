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
// EASTL/sort_extra.h
// Written by Paul Pedriana - 2005
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// This file implements additional sort algorithms beyond the basic set.
//////////////////////////////////////////////////////////////////////////////

#ifndef EASTL_SORT_EXTRA_H
#define EASTL_SORT_EXTRA_H

#include <EASTL/internal/config.h>
#include <EASTL/iterator.h>
#include <EASTL/algorithm.h>
#include <EASTL/functional.h>
#include <EASTL/heap.h>
#include <EASTL/allocator.h>

namespace eastl
{

    /// bubble_sort
    ///
    /// Implements the BubbleSort algorithm. This algorithm is only useful for 
    /// small range sizes, such as 10 or less items. You may be better off using
    /// insertion_sort for cases where bubble_sort works.
    ///
    template <typename ForwardIterator, typename StrictWeakOrdering>
    void bubble_sort_impl(ForwardIterator first, ForwardIterator last, StrictWeakOrdering compare, EASTL_ITC_NS::forward_iterator_tag)
    {
        ForwardIterator iCurrent, iNext;

        while(first != last)
        {
            iNext = iCurrent = first;
            
            for(++iNext; iNext != last; iCurrent = iNext, ++iNext) 
            {
                if(compare(*iNext, *iCurrent))
                {
                    EASTL_VALIDATE_COMPARE(!compare(*iCurrent, *iNext)); // Validate that the compare function is sane.
                    eastl::iter_swap(iCurrent, iNext);
                }
            }
            last = iCurrent;
        }
    }

    template <typename BidirectionalIterator, typename StrictWeakOrdering>
    void bubble_sort_impl(BidirectionalIterator first, BidirectionalIterator last, StrictWeakOrdering compare, EASTL_ITC_NS::bidirectional_iterator_tag)
    {
        if(first != last)
        {
            BidirectionalIterator iCurrent, iNext, iLastModified;

            last--;

            while(first != last)
            {
                iLastModified = iNext = iCurrent = first;
                
                for(++iNext; iCurrent != last; iCurrent = iNext, ++iNext)
                {
                    if(compare(*iNext, *iCurrent))
                    {
                        EASTL_VALIDATE_COMPARE(!compare(*iCurrent, *iNext)); // Validate that the compare function is sane.
                        iLastModified = iCurrent;
                        eastl::iter_swap(iCurrent, iNext);
                    }
                }

                last = iLastModified;
            }
        }
    }

    template <typename ForwardIterator, typename StrictWeakOrdering>
    inline void bubble_sort(ForwardIterator first, ForwardIterator last, StrictWeakOrdering compare)
    {
        typedef typename eastl::iterator_traits<ForwardIterator>::iterator_category IC;

        eastl::bubble_sort_impl<ForwardIterator, StrictWeakOrdering>(first, last, compare, IC());
    }

    template <typename ForwardIterator>
    inline void bubble_sort(ForwardIterator first, ForwardIterator last)
    {
        typedef eastl::less<typename eastl::iterator_traits<ForwardIterator>::value_type> Less;
        typedef typename eastl::iterator_traits<ForwardIterator>::iterator_category IC;

        eastl::bubble_sort_impl<ForwardIterator, Less>(first, last, Less(), IC());
    }

} // namespace eastl

#endif // Header include guard



















