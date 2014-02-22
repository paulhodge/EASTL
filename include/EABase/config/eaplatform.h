/*
Copyright (C) 2009, 2013 Electronic Arts, Inc.  All rights reserved.

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

/*-----------------------------------------------------------------------------
 * config/eaplatform.h
 *
 * Copyright (c) 2002 - 2005 Electronic Arts Inc. All rights reserved.
 * Maintained by Paul Pedriana, Maxis
 *
 *-----------------------------------------------------------------------------
 * Currently supported platform indentification defines include:
 *    EA_PLATFORM_MAC
 *    EA_PLATFORM_OSX
 *    EA_PLATFORM_LINUX
 *    EA_PLATFORM_WINDOWS
 *    EA_PLATFORM_WIN32
 *    EA_PLATFORM_WIN64
 *    EA_PLATFORM_HPUX
 *    EA_PLATFORM_SUN
 *    EA_PLATFORM_LRB (Larrabee)
 *    EA_PLATFORM_UNIX      (pseudo-platform; may be defined along with another platform like EA_PLATFORM_LINUX)
 *    EA_PLATFORM_CYGWIN    (pseudo-platform; may be defined along with another platform like EA_PLATFORM_LINUX)
 *    EA_PLATFORM_MINGW     (pseudo-platform; may be defined along with another platform like EA_PLATFORM_WINDOWS)
 *    EA_PLATFORM_MICROSOFT (pseudo-platform; may be defined along with another platform like EA_PLATFORM_WINDOWS)
 *    
 * Other definitions emanated from this file inclue:
 *    EA_PLATFORM_NAME = <string>
 *    EA_PLATFORM_DESCRIPTION = <string>
 *    EA_PROCESSOR_XXX
 *    EA_SYSTEM_LITTLE_ENDIAN | EA_SYSTEM_BIG_ENDIAN
 *    EA_ASM_STYLE_ATT | EA_ASM_STYLE_INTEL | EA_ASM_STYLE_MOTOROLA
 *    EA_PLATFORM_PTR_SIZE = <integer size in bytes>
 *    EA_PLATFORM_WORD_SIZE = <integer size in bytes>
 * 
 *---------------------------------------------------------------------------*/


#ifndef INCLUDED_eaplatform_H
#define INCLUDED_eaplatform_H


// Cygwin
// This is a pseudo-platform which will be defined along with EA_PLATFORM_LINUX when
// using the Cygwin build environment.
#if defined(__CYGWIN__)
    #define EA_PLATFORM_CYGWIN
    #define EA_PLATFORM_DESKTOP
#endif

// MinGW
// This is a pseudo-platform which will be defined along with EA_PLATFORM_WINDOWS when 
// using the MinGW Windows build environment.
#if defined(__MINGW32__) || defined(__MINGW64__)
    #define EA_PLATFORM_MINGW
    #define EA_PLATFORM_DESKTOP
#endif

#if   defined(EA_PLATFORM_LRB) || defined(__LRB__) || (defined(__EDG__) && defined(__ICC) && defined(__x86_64__))
    #undef  EA_PLATFORM_LRB
    #define EA_PLATFORM_LRB         1
    #define EA_PLATFORM_NAME        "Larrabee"
    #define EA_PLATFORM_DESCRIPTION "Larrabee on LRB1"
    #define EA_PROCESSOR_X86_64
    #if defined(BYTE_ORDER) && (BYTE_ORDER == 4321)
        #define EA_SYSTEM_BIG_ENDIAN
    #else
        #define EA_SYSTEM_LITTLE_ENDIAN
    #endif
    #define EA_PROCESSOR_LRB
    #define EA_PROCESSOR_LRB1       // Larrabee version 1
    #define EA_ASM_STYLE_ATT        // Both types of asm style 
    #define EA_ASM_STYLE_INTEL      // are supported.
    #define EA_PLATFORM_DESKTOP

// Android (Google phone OS)
#elif defined(__APPLE__) && __APPLE__ 
    #include <TargetConditionals.h>

    // Apple family of operating systems.
    #define EA_PLATFORM_APPLE
    
    // iPhone 
    // TARGET_OS_IPHONE will be undefined on an unknown compiler, and will be defined on gcc.
    #if defined(CS_UNDEFINED_STRING) || defined(__IPHONE__) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
        #undef  CS_UNDEFINED_STRING
        #define CS_UNDEFINED_STRING 1
        #define EA_PLATFORM_NAME "iPhone"
        #define EA_ASM_STYLE_ATT
        #define EA_POSIX_THREADS_AVAILABLE 1
        #if defined(__arm__)
            #define CS_UNDEFINED_STRING
            #define EA_SYSTEM_LITTLE_ENDIAN
            #define EA_PLATFORM_DESCRIPTION "iPhone on ARM"
        #elif defined(__i386__)
            #define CS_UNDEFINED_STRING
            #define EA_PROCESSOR_X86
            #define EA_SYSTEM_LITTLE_ENDIAN
            #define EA_PLATFORM_DESCRIPTION "iPhone simulator on x86"
        #else
            #error Unknown processor
        #endif
        #define CS_UNDEFINED_STRING

    // Macintosh OSX
    // TARGET_OS_MAC is defined by the Metrowerks and older AppleC compilers.
    // Howerver, TARGET_OS_MAC is defined to be 1 in all cases.
    // __i386__ and __intel__ are defined by the GCC compiler.
    // __dest_os is defined by the Metrowerks compiler.
    // __MACH__ is defined by the Metrowerks and GCC compilers.
    // powerc and __powerc are defined by the Metrowerks and GCC compilers.
    #elif defined(EA_PLATFORM_OSX) || defined(__MACH__) || (defined(__MSL__) && (__dest_os == __mac_os_x))
        #undef  EA_PLATFORM_OSX
        #define EA_PLATFORM_OSX 1
        #define EA_PLATFORM_UNIX 1
        #define EA_PLATFORM_NAME "OSX"
        #if defined(__i386__) || defined(__intel__)
            #define EA_PROCESSOR_X86
            #define EA_SYSTEM_LITTLE_ENDIAN
            #define EA_PLATFORM_DESCRIPTION "OSX on x86"
        #elif defined(__x86_64) || defined(__amd64)
            #define EA_PROCESSOR_X86_64
            #define EA_SYSTEM_LITTLE_ENDIAN
            #define EA_PLATFORM_DESCRIPTION "OSX on x86-64"
        #elif defined(__arm__)
            #define CS_UNDEFINED_STRING
            #define EA_SYSTEM_LITTLE_ENDIAN
            #define EA_PLATFORM_DESCRIPTION "OSX on ARM"
        #elif defined(__POWERPC64__) || defined(__powerpc64__)
            #define EA_PROCESSOR_POWERPC
            #define EA_PROCESSOR_POWERPC_64
            #define EA_SYSTEM_BIG_ENDIAN
            #define EA_PLATFORM_DESCRIPTION "OSX on PowerPC 64"
        #elif defined(__POWERPC__) || defined(__powerpc__)
            #define EA_PROCESSOR_POWERPC
            #define EA_PROCESSOR_POWERPC_32
            #define EA_SYSTEM_BIG_ENDIAN
            #define EA_PLATFORM_DESCRIPTION "OSX on PowerPC"
        #else
            #error Unknown processor
        #endif
        #if defined(__GNUC__)
            #define EA_ASM_STYLE_ATT
        #else
            #define EA_ASM_STYLE_MOTOROLA
        #endif
        #define EA_PLATFORM_DESKTOP

    #else
        #error Unknown Apple Platform
    #endif

// Linux
// __linux and __linux__ are defined by the GCC and Borland compiler.
// __i386__ and __intel__ are defined by the GCC compiler.
// __i386__ is defined by the Metrowerks compiler.
// _M_IX86 is defined by the Borland compiler.
// __sparc__ is defined by the GCC compiler.
// __powerpc__ is defined by the GCC compiler.
#elif defined(CS_UNDEFINED_STRING) || (defined(__linux) || defined(__linux__))
    #undef  CS_UNDEFINED_STRING
    #define CS_UNDEFINED_STRING 1
    #define EA_PLATFORM_UNIX 1
    #define EA_PLATFORM_NAME "Linux"
    #if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
        #define EA_PROCESSOR_X86
        #define EA_SYSTEM_LITTLE_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "Linux on x86"
    #elif defined(__x86_64__)
        #define EA_PROCESSOR_X86_64
        #define EA_SYSTEM_LITTLE_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "Linux on x86-64"
    #elif defined(__powerpc64__)
        #define EA_PROCESSOR_POWERPC
        #define EA_PROCESSOR_POWERPC_64
        #define EA_SYSTEM_BIG_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "Linux on PowerPC 64"
    #elif defined(__powerpc__)
        #define EA_PROCESSOR_POWERPC
        #define EA_PROCESSOR_POWERPC_32
        #define EA_SYSTEM_BIG_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "Linux on PowerPC"
    #else
        #error Unknown processor
        #error Unknown endianness
    #endif
    #if defined(__GNUC__)
        #define EA_ASM_STYLE_ATT
    #endif
    #define EA_PLATFORM_DESKTOP

// Win CE (Windows mobile)
#elif defined(EA_PLATFORM_WINCE) || defined(_WIN32_WCE)
    #undef  EA_PLATFORM_WINCE
    #define EA_PLATFORM_WINCE 1
    #define EA_PLATFORM_NAME "WinCE"
    #define EA_ASM_STYLE_INTEL
    #define EA_SYSTEM_LITTLE_ENDIAN
    #if defined(_M_ARM) // Also there is _M_ARMT
        #define CS_UNDEFINED_STRING
        #define EA_PLATFORM_DESCRIPTION "Windows CE on ARM"
    #elif defined(_M_IX86)
        #define EA_PROCESSOR_X86
        #define EA_PLATFORM_DESCRIPTION "Windows CE on X86"
    #else //Possibly other Windows CE variants
        #error Unknown processor
    #endif
    #define CS_UNDEFINED_STRING

// Windows
// _WIN32 is defined by the VC++, Intel and GCC compilers.
// _WIN64 is defined by the VC++, Intel and GCC compilers.
// __WIN32__ is defined by the Borland compiler.
// __INTEL__ is defined by the Metrowerks compiler.
// _M_IX86, _M_AMD64 and _M_IA64 are defined by the VC++, Intel, and Borland compilers.
// _X86_, _AMD64_, and _IA64_ are defined by the Metrowerks compiler.
// _M_ARM is defined by the VC++ compiler.
#elif (defined(EA_PLATFORM_WINDOWS) || (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || (defined(CS_UNDEFINED_STRING) && defined(_X86_)))) && !defined(CS_UNDEFINED_STRING)
    #undef  EA_PLATFORM_WINDOWS
    #define EA_PLATFORM_WINDOWS 1
    #define EA_PLATFORM_NAME "Windows"
    #ifdef _WIN64 // VC++ defines both _WIN32 and _WIN64 when compiling for Win64.
        #define EA_PLATFORM_WIN64
    #else
        #define EA_PLATFORM_WIN32
    #endif
    #if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
        #define EA_PROCESSOR_X86_64
        #define EA_SYSTEM_LITTLE_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "Windows on X86-64"
    #elif defined(_M_IX86) || defined(_X86_)
        #define EA_PROCESSOR_X86
        #define EA_SYSTEM_LITTLE_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "Windows on X86"
    #elif defined(_M_IA64) || defined(_IA64_)
        #define EA_PROCESSOR_IA64
        #define EA_SYSTEM_LITTLE_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "Windows on IA-64"
    #elif defined(_M_ARM)
        #define CS_UNDEFINED_STRING
        #define EA_SYSTEM_LITTLE_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "Windows CE on ARM"
    #else //Possibly other Windows CE variants
        #error Unknown processor
        #error Unknown endianness
    #endif
    #if defined(__GNUC__)
        #define EA_ASM_STYLE_ATT
    #elif defined(_MSC_VER) || defined(__BORLANDC__) || defined(__ICL)
        #define EA_ASM_STYLE_INTEL
    #endif
    #define EA_PLATFORM_DESKTOP
    #define EA_PLATFORM_MICROSOFT 1

// Sun (Solaris)
// __SUNPRO_CC is defined by the Sun compiler.
// __sun is defined by the GCC compiler.
// __i386 is defined by the Sun and GCC compilers.
// __sparc is defined by the Sun and GCC compilers.
#else
    #error Unknown platform
    #error Unknown processor
    #error Unknown endianness
#endif



// EA_PLATFORM_PTR_SIZE
// Platform pointer size; same as sizeof(void*).
// This is not the same as sizeof(int), as int is usually 32 bits on 
// even 64 bit platforms. 
//
// _WIN64 is defined by Win64 compilers, such as VC++.
// _M_IA64 is defined by VC++ and Intel compilers for IA64 processors.
// __LP64__ is defined by HP compilers for the LP64 standard.
// _LP64 is defined by the GCC and Sun compilers for the LP64 standard.
// __ia64__ is defined by the GCC compiler for IA64 processors.
// __arch64__ is defined by the Sparc compiler for 64 bit processors.
// __mips64__ is defined by the GCC compiler for MIPS processors.
// __powerpc64__ is defined by the GCC compiler for PowerPC processors.
// __64BIT__ is defined by the AIX compiler for 64 bit processors.
// __sizeof_ptr is defined by the ARM compiler (armcc, armcpp).
//
#ifndef EA_PLATFORM_PTR_SIZE
    #if defined(__WORDSIZE) // Defined by some variations of GCC.
        #define EA_PLATFORM_PTR_SIZE ((__WORDSIZE) / 8)
    #elif defined(_WIN64) || defined(__LP64__) || defined(_LP64) || defined(_M_IA64) || defined(__ia64__) || defined(__arch64__) || defined(__mips64__) || defined(__64BIT__) 
        #define EA_PLATFORM_PTR_SIZE 8
    #elif defined(__CC_ARM) && (__sizeof_ptr == 8)
        #define EA_PLATFORM_PTR_SIZE 8
    #else
        #define EA_PLATFORM_PTR_SIZE 4
    #endif
#endif



// EA_PLATFORM_WORD_SIZE
// This defines the size of a machine word. This will be the same as 
// the size of registers on the machine but not necessarily the same
// as the size of pointers on the machine. A number of 64 bit platforms
// have 64 bit registers but 32 bit pointers.
//
#ifndef EA_PLATFORM_WORD_SIZE
      #define EA_PLATFORM_WORD_SIZE EA_PLATFORM_PTR_SIZE
#endif



// Disabled until and unless deemed useful:
//
// Platform integer types
// These definitions allow us to define other things properly, such as 
// sized integer types. In order to bring some order to this chaos, 
// we follow a variation of the standard LP64 conventions defined at:
//    http://www.opengroup.org/public/tech/aspen/lp64_wp.htm 
//
// #if defined(EA_PLATFORM_LINUX) || defined(EA_PLATFORM_OSX)
//    #define EA_PLATFORM_ILP32_LL64         // int, long, ptr = 32 bits; long long = 64 bits.
// 
// #elif defined(EA_PLATFORM_SUN) || defined(EA_PLATFORM_SGI)
//    #if (EA_PLATFORM_WORD_SIZE == 32)
//       #define ILP32_LL64                  // int, long, ptr = 32 bits; long long = 64 bits.
//    #else // 64 bit platform
//       #define EA_PLATFORM_I32_LLLP64      // int = 32 bits; long, long long, ptr = 64 bits.
//    #endif
// 
// #elif defined(EA_PLATFORM_WINDOWS)
//    #if (EA_PLATFORM_WORD_SIZE == 32)
//       #define ILP32_LL64                  // int, long, ptr = 32 bits; long long = 64 bits.
//    #else // 64 bit platform
//       #if defined(__MWERKS__) || defined(__GNUC__)
//          #define EA_PLATFORM_I32_LLLP64   // int = 32 bits; long, long long, ptr = 64 bits.
//       #else // MSVC
//          #define EA_PLATFORM_IL32_LLP64   // int, long = 32 bits; long long, ptr = 64 bits.
//       #endif
//    #endif
// #endif


#endif // INCLUDED_eaplatform_H














