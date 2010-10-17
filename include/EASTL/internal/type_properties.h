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
// EASTL/internal/type_properties.h
// Written and maintained by Paul Pedriana - 2005.
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_INTERNAL_TYPE_PROPERTIES_H
#define EASTL_INTERNAL_TYPE_PROPERTIES_H


#include <limits.h>


namespace eastl
{

    // The following properties or relations are defined here. If the given 
    // item is missing then it simply hasn't been implemented, at least not yet.

    ///////////////////////////////////////////////////////////////////////
    // is_const
    //
    // is_const<T>::value == true if and only if T has const-qualification.
    //
    ///////////////////////////////////////////////////////////////////////
    template <typename T> struct is_const_value                    : public false_type{};
    template <typename T> struct is_const_value<const T*>          : public true_type{};
    template <typename T> struct is_const_value<const volatile T*> : public true_type{};

    template <typename T> struct is_const : public is_const_value<T*>{};
    template <typename T> struct is_const<T&> : public false_type{}; // Note here that T is const, not the reference to T. So is_const is false. See section 8.3.2p1 of the C++ standard.



    ///////////////////////////////////////////////////////////////////////
    // is_volatile
    //
    // is_volatile<T>::value == true  if and only if T has volatile-qualification.
    //
    ///////////////////////////////////////////////////////////////////////

    template <typename T> struct is_volatile_value                    : public false_type{};
    template <typename T> struct is_volatile_value<volatile T*>       : public true_type{};
    template <typename T> struct is_volatile_value<const volatile T*> : public true_type{};

    template <typename T> struct is_volatile : public is_volatile_value<T*>{};
    template <typename T> struct is_volatile<T&> : public false_type{}; // Note here that T is volatile, not the reference to T. So is_const is false. See section 8.3.2p1 of the C++ standard.

} // namespace eastl


#endif // Header include guard




















