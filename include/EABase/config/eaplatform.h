/*
Copyright (C) 2009 Electronic Arts, Inc.  All rights reserved.

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
 *    EA_PLATFORM_PLAYSTATION2
 *    EA_PLATFORM_PLAYSTATION2_IOP
 *    EA_PLATFORM_PS3
 *    EA_PLATFORM_PS3_PPU
 *    EA_PLATFORM_PS3_SPU
 *    EA_PLATFORM_PSP
 *    EA_PLATFORM_REVOLUTION
 *    EA_PLATFORM_GAMECUBE
 *    EA_PLATFORM_DS
 *    EA_PLATFORM_XBOX
 *    EA_PLATFORM_XENON (a.k.a. XBox2)
 *    EA_PLATFORM_MAC
 *    EA_PLATFORM_OSX
 *    EA_PLATFORM_LINUX
 *    EA_PLATFORM_WINDOWS
 *    EA_PLATFORM_WIN32
 *    EA_PLATFORM_WIN64
 *    EA_PLATFORM_HPUX
 *    EA_PLATFORM_SUN
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
 * Todo: 
 *    Consider making abbreviated platform definitions in order to make
 *    this file be consistent with standard EA build configuration names.
 *    Thus, EA_PLATFORM_PLAYSTATION2 would become EA_PLATFORM_PS2.
 *    
 * Todo: 
 *    Consider making EA_PLATFORM_WINDOWS be EA_PLATFORM_PC in order to be
 *    consistent with current standard EA configuration names.
 *
 * Todo: 
 *    Consider removing support for SGI as a platform, as it is almost 
 *    certainly never to be needed again.
 * 
 * Todo: 
 *    There currently is some lack of precise clarity regarding the identification
 *    of platform vs. identification of operating system and identification of
 *    processor. The definitions provided here are perhaps not precise enough
 *    for all uses. For example, EA_PLATFORM_LINUX refers to an operating system
 *    and not to any particular hardware that it runs on. On the other hand, 
 *    EA_PLATFORM_PLAYSTATION2 refers to hardware but not a particular system
 *    that runs on it (PS2 in fact can run Linux in addition to games). Consider
 *    finding a way to resolve this clarity issue.   
 *---------------------------------------------------------------------------*/


#ifndef INCLUDED_eaplatform_H
#define INCLUDED_eaplatform_H


// PlayStation 2
// __R5900 is defined by the GCC compiler and is only defined for PS2 processors.
// __MWERKS__ and __MIPS__ are defined by the Metrowerks compiler.
#if defined(EA_PLATFORM_PLAYSTATION2) || defined(EA_PLATFORM_PS2) || defined(__R5900) || defined(__R5900__)
    #undef  EA_PLATFORM_PLAYSTATION2
    #define EA_PLATFORM_PLAYSTATION2 1
    #define EA_PLATFORM_NAME "PS2"
    #define EA_PROCESSOR_R5900
    #define EA_PROCESSOR_MIPS
    #define EA_SYSTEM_LITTLE_ENDIAN
    #define EA_PLATFORM_DESCRIPTION "PS2 on R5900"

// PlayStation 2 IOP (IO Processor)
// _R3000 and/or R3000 is defined by PS1 and PS2 IP Processor compilers.
// The GNU PSP compiler also predefines _R3000, so we check that __psp__ 
// is not also defined.
#elif defined(EA_PLATFORM_PLAYSTATION2_IOP) || ((defined(_R3000) || defined(R3000)) && !defined(__psp__))
    #undef  EA_PLATFORM_PLAYSTATION2_IOP
    #define EA_PLATFORM_PLAYSTATION2_IOP 1
    #define EA_PLATFORM_NAME "PS2 IOP"
    #define EA_PROCESSOR_R3000
    #define EA_PROCESSOR_MIPS
    #define EA_SYSTEM_LITTLE_ENDIAN
    #define EA_PLATFORM_DESCRIPTION "PS2 IOP on R3000"

// PlayStation 3 PPU (Primary Processing Unit)
#elif defined(EA_PLATFORM_PS3_PPU) || defined(EA_PLATFORM_PS3) || defined(__PU__) || defined(__PPU__)
    #undef  EA_PLATFORM_PS3_PPU
    #define EA_PLATFORM_PS3_PPU 1
    #undef  EA_PLATFORM_PS3
    #define EA_PLATFORM_PS3 1
    #define EA_PLATFORM_NAME "PS3"
    #define EA_PROCESSOR_POWERPC
    #define EA_SYSTEM_BIG_ENDIAN
    #define EA_PLATFORM_DESCRIPTION "PS3 on PowerPC"

// PlayStation 3 SPU (Synergistic Processing Unit)
#elif defined(EA_PLATFORM_PS3_SPU) || defined(__SPU__)
    #undef  EA_PLATFORM_PS3_SPU
    #define EA_PLATFORM_PS3_SPU 1
    #define EA_PLATFORM_NAME "PS3 SPU"
    #define EA_PROCESSOR_SPU
    #define EA_SYSTEM_BIG_ENDIAN
    #define EA_PLATFORM_DESCRIPTION "PS3 SPU on SPU"

// PlayStation Portable (a.k.a. PSP)
// __psp__ is defined by the GCC compiler.
// _R4000 is defined by the SN compiler.
#elif defined(EA_PLATFORM_PSP) || (defined(__GNUC__) && defined(__psp__)) || (defined(__SNC__) && defined(_R4000))
    #undef  EA_PLATFORM_PSP
    #define EA_PLATFORM_PSP 1
    #define EA_PLATFORM_NAME "PSP"
    #define EA_PROCESSOR_MIPS32
    #define EA_PROCESSOR_MIPS
    #define EA_SYSTEM_LITTLE_ENDIAN
    #define EA_PLATFORM_DESCRIPTION "PSP on MIPS32"

// PlayStation 1
// _R3000 and/or R3000 is defined by PS1 and PS2 IP Processor compilers.
// Disabled, as this currently conflicts with EA_PLATFORM_PLAYSTATION2_IOP detection.
// #if defined(EA_PLATFORM_PLAYSTATION1) || (defined(_R3000) || defined(R3000))
//     #undef  EA_PLATFORM_PLAYSTATION1
//     #define EA_PLATFORM_PLAYSTATION1 1
//     #define EA_PLATFORM_NAME "PS1"
//     #define EA_PROCESSOR_R3000
//     #define EA_PROCESSOR_MIPS
//     #define EA_SYSTEM_LITTLE_ENDIAN
//     #define EA_PLATFORM_DESCRIPTION "PS1 on R3000"

// Revolution (Nintendo Wii)
// There is no way to automatically detect the Wii uniquely from GameCube until 
// CodeWarrior 2.4 for Wii. For earlier compiler versions you may need to manually
// define EA_PLATFORM_REVOLUTION in your project settings.
#elif defined(EA_PLATFORM_REVOLUTION) || defined(__PPCBROADWAY__)
    #undef  EA_PLATFORM_REVOLUTION
    #define EA_PLATFORM_REVOLUTION 1
    #define EA_PLATFORM_NAME "Revolution"
    #define EA_PROCESSOR_POWERPC_GEKKO
    #define EA_PROCESSOR_POWERPC
    #define EA_SYSTEM_BIG_ENDIAN
    #define EA_PLATFORM_DESCRIPTION "Revolution on PowerPC"

// GameCube
// __SN__ and __PPC__ are defined by the SN compiler.
// __MWERKS__ and __PPCGEKKO__ are defined by the Metrowerks compiler.
#elif defined(EA_PLATFORM_GAMECUBE) || ((defined(__SN__) && defined(__PPC__)) || (defined(__MWERKS__) && defined(__PPCGEKKO__)))
    #undef  EA_PLATFORM_GAMECUBE
    #define EA_PLATFORM_GAMECUBE 1
    #define EA_PLATFORM_NAME "GameCube"
    #define EA_PROCESSOR_POWERPC_GEKKO
    #define EA_PROCESSOR_POWERPC
    #define EA_SYSTEM_BIG_ENDIAN
    #define EA_PLATFORM_DESCRIPTION "GameCube on PowerPC"

// Nintendo DS
// __MWERKS__ are defined by the Metrowerks compiler.
#elif defined(EA_PLATFORM_DS) || (defined(__MWERKS__) && defined(SDK_ARM9))
    #undef  EA_PLATFORM_DS
    #define EA_PLATFORM_DS 1
    #define EA_PLATFORM_NAME "NDS"
    #define EA_PROCESSOR_ARM7
    #define EA_PROCESSOR_ARM9
    #define EA_SYSTEM_BIG_ENDIAN // This is configurable on the arm processors, but big-endian for this platform.
    #define EA_PLATFORM_DESCRIPTION "NDS on ARM"

// XBox
// _XBOX is defined by the VC++ project, not the compiler. There is no way
// to tell if the compiler is compiling for XBox unless _XBOX is #defined 
// in the project files or otherwise. _M_IX86 is the VC++ way of detecting 
// an x86 target, which would mean XBox and not Xenon (a.k.a. XBox2).
#elif defined(EA_PLATFORM_XBOX) || (defined(_XBOX) && defined(_M_IX86))
    #undef  EA_PLATFORM_XBOX
    #define EA_PLATFORM_XBOX 1
    #define EA_PLATFORM_NAME "XBox"
    #define EA_PROCESSOR_X86
    #define EA_SYSTEM_LITTLE_ENDIAN
    #define EA_PLATFORM_DESCRIPTION "XBox on X86"
    #if defined(_MSC_VER) || defined(__ICL)
       #define EA_ASM_STYLE_INTEL
    #endif

// Xenon (XBox 360)
// The Xenon compiler doesn't define anything in particular to indicate that the 
// target is the Xenon platform. The Xenon SDK, however, expects that XBOX and
// _XBOX are #defined, so the project build file must make sure these are defined.
// Since the Xenon compiler in fact defines _M_PPC, we can use this information 
// to infer that Xenon is the target if neither _XENON nor _XBOX2 are specifically 
// defined by the project build file.
#elif defined(EA_PLATFORM_XENON) || defined(_XENON) || defined(_XBOX2) || ((defined(_XBOX) || defined(XBOX)) && defined(_M_PPC))
    #undef  EA_PLATFORM_XENON
    #define EA_PLATFORM_XENON 1
    #define EA_PLATFORM_NAME "Xenon"
    #define EA_PROCESSOR_POWERPC
    #define EA_SYSTEM_BIG_ENDIAN
    #define EA_PLATFORM_DESCRIPTION "Xenon on PowerPC"
    #if defined(_MSC_VER) || defined(__ICL)
       #define EA_ASM_STYLE_INTEL
    #endif

// Macintosh OS (non-OSX)
// TARGET_OS_MAC is defined by the Metrowerks and MacOS AppleC compilers.
// __dest_os is defined by the Metrowerks compiler.
// TARGET_CPU_PPC is defined by the Metrowerks and AppleC compilers.
// powerc and __powerc are defined by the Metrowerks and GCC compilers.
// __m68k__ is defined by the GCC compiler.
#elif defined(EA_PLATFORM_MAC) || (defined(TARGET_OS_MAC) && !defined(__MACH__)) || (defined(__MSL__) && (__dest_os == __mac_os))
    #undef  EA_PLATFORM_MAC
    #define EA_PLATFORM_MAC 1
    #define EA_PLATFORM_NAME "MAC"
    #if defined(TARGET_CPU_PPC) || defined (powerc) || defined (__powerc)
        #define EA_PROCESSOR_POWERPC
        #define EA_SYSTEM_BIG_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "Macintosh on PowerPC"
    #elif defined(TARGET_CPU_68K) || defined(m68k) || defined(m68000) || defined(__m68k__)
        #define EA_PROCESSOR_68000
        #define EA_SYSTEM_BIG_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "Macintosh on 68000"
    #else
        #error Unknown processor
        #error Unknown endianness
    #endif
    #if defined(__GNUC__)
        #define EA_ASM_STYLE_ATT
    #else
        #define EA_ASM_STYLE_MOTOROLA
    #endif

// Macintosh OSX
// TARGET_OS_MAC is defined by the Metrowerks and older AppleC compilers.
// __i386__ and __intel__ are defined by the GCC compiler.
// __dest_os is defined by the Metrowerks compiler.
// __MACH__ is defined by the Metrowerks and GCC compilers.
// TARGET_CPU_PPC is defined by the Metrowerks and AppleC compilers.
// powerc and __powerc are defined by the Metrowerks and GCC compilers.
#elif defined(EA_PLATFORM_OSX) || (defined(TARGET_OS_MAC) || defined(__MACH__)) || (defined(__MSL__) && (__dest_os == __mac_os_x))
    #undef  EA_PLATFORM_OSX
    #define EA_PLATFORM_OSX 1
    #define EA_PLATFORM_UNIX 1
    #define EA_PLATFORM_NAME "OSX"
    #if defined(__i386__) || defined(__intel__)
        #define EA_PROCESSOR_X86
        #define EA_SYSTEM_LITTLE_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "OSX on x86"
    #else
        #define EA_PROCESSOR_POWERPC
        #define EA_SYSTEM_BIG_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "OSX on PowerPC"
    #endif
    #if defined(__GNUC__)
        #define EA_ASM_STYLE_ATT
    #else
        #define EA_ASM_STYLE_MOTOROLA
    #endif

// Linux
// __linux and __linux__ are defined by the GCC and Borland compiler.
// __i386__ and __intel__ are defined by the GCC compiler.
// __i386__ is defined by the Metrowerks compiler.
// _M_IX86 is defined by the Borland compiler.
// __sparc__ is defined by the GCC compiler.
// __powerpc__ is defined by the GCC compiler.
#elif defined(EA_PLATFORM_LINUX) || (defined(__linux) || defined(__linux__))
    #undef  EA_PLATFORM_LINUX
    #define EA_PLATFORM_LINUX 1
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
    #elif defined(__powerpc__)
        #define EA_PROCESSOR_POWERPC
        #define EA_SYSTEM_BIG_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "Linux on PowerPC"
    #else
        #error Unknown processor
        #error Unknown endianness
    #endif
    #if defined(__GNUC__)
        #define EA_ASM_STYLE_ATT
    #endif

// Windows
// _WIN32 is defined by the VC++, Intel and GCC compilers.
// _WIN64 is defined by the VC++, Intel and GCC compilers.
// __WIN32__ is defined by the Borland compiler.
// __INTEL__ is defined by the Metrowerks compiler.
// _M_IX86, _M_AMD64 and _M_IA64 are defined by the VC++, Intel, and Borland compilers.
// _X86_, _AMD64_, and _IA64_ are defined by the Metrowerks compiler.
// _M_ARM is defined by the VC++ compiler.
#elif (defined(EA_PLATFORM_WINDOWS) || (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || (defined(__MWERKS__) && defined(_X86_)))) && !defined(_XBOX)
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
        #define EA_PROCESSOR_ARM
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

// HP-UX
// __DECC_VER is defined by the DEC compiler.
// __hpux is defined by the GCC compiler.
// __alpha is defined by the DEC compiler.
// __hppa is defined by the DEC and GCC compilers.
// __ia64 is defined by the DEC and GCC compilers.
#elif defined(EA_PLATFORM_HPUX) || (defined(__DECC_VER) || defined(__DECCXX_VER) || defined(__hpux))
    #undef  EA_PLATFORM_HPUX
    #define EA_PLATFORM_HPUX 1
    #define EA_PLATFORM_UNIX 1
    #define EA_PLATFORM_NAME "HP-UX"
    #if defined(__alpha)
        #define EA_PROCESSOR_ALPHA
        #define EA_SYSTEM_BIG_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "HP-UX on ALPHA"
    #elif defined(__hppa)
        #define EA_PROCESSOR_PARISC
        #define EA_SYSTEM_BIG_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "HP-UX on PA-RISC"
    #elif defined(__ia64)
        #define EA_PROCESSOR_IA64
        #define EA_SYSTEM_BIG_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "HP-UX on IA64"
    #else
        #error Unknown processor
        #error Unknown endianness
    #endif

// Sun (Solaris)
// __SUNPRO_CC is defined by the Sun compiler.
// __sun is defined by the GCC compiler.
// __i386 is defined by the Sun and GCC compilers.
// __sparc is defined by the Sun and GCC compilers.
#elif defined(EA_PLATFORM_SUN) || (defined(__SUNPRO_CC) || defined(__sun))
    #undef  EA_PLATFORM_SUN
    #define EA_PLATFORM_SUN 1
    #define EA_PLATFORM_UNIX 1
    #define EA_PLATFORM_NAME "SUN"
    #if defined(__i386)
        #define EA_PROCESSOR_X86
        #define EA_SYSTEM_LITTLE_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "SUN on x86"
    #elif defined(__sparc)
        #define EA_PROCESSOR_SPARC
        #define EA_SYSTEM_BIG_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "SUN on Sparc"
    #else
        #error Unknown processor
        #error Unknown endianness
    #endif


// SGI (IRIX OS)
// __sgi is defined by the SGI and GCC compilers.
// __mips is defined by the SGI and GCC compilers.
#elif defined(EA_PLATFORM_SGI) || defined(__sgi)
    #undef  EA_PLATFORM_SGI
    #define EA_PLATFORM_SGI 1
    #define EA_PLATFORM_UNIX 1
    #define EA_PLATFORM_NAME "SGI"
    #if defined(__mips)
        #define EA_PROCESSOR_MIPS
        #define EA_SYSTEM_BIG_ENDIAN
        #define EA_PLATFORM_DESCRIPTION "SGI on MIPS"
    #else
        #error Unknown processor
        #error Unknown endianness
    #endif

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
//
#ifndef EA_PLATFORM_PTR_SIZE
   #if defined(__WORDSIZE) // Defined by some variations of GCC.
      #define EA_PLATFORM_PTR_SIZE ((__WORDSIZE) / 8)
   #elif defined(_WIN64) || defined(__LP64__) ||defined(_LP64) || defined(_M_IA64) || defined(__ia64__) || defined(__arch64__) || defined(__mips64__) || defined(__64BIT__) 
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
   #if defined(EA_PLATFORM_XENON) || defined(EA_PLATFORM_PS3) || defined(EA_PLATFORM_PLAYSTATION2)
      #define EA_PLATFORM_WORD_SIZE 8
   #else
      #define EA_PLATFORM_WORD_SIZE EA_PLATFORM_PTR_SIZE
   #endif
#endif



// Disabled until and unless deemed useful:
//
// Platform integer types
// These definitions allow us to define other things properly, such as 
// sized integer types. In order to bring some order to this chaos, 
// we follow a variation of the standard LP64 conventions defined at:
//    http://www.opengroup.org/public/tech/aspen/lp64_wp.htm 
//
// #if defined(EA_PLATFORM_GAMECUBE) || defined(EA_PLATFORM_LINUX) || defined(EA_PLATFORM_PLAYSTATION2_IOP) || defined(EA_PLATFORM_OSX) || defined(EA_PLATFORM_XBOX) || defined(EA_PLATFORM_XENON)
//    #define EA_PLATFORM_ILP32_LL64         // int, long, ptr = 32 bits; long long = 64 bits.
// 
// #elif defined(EA_PLATFORM_PLAYSTATION2)
//    #define EA_PLATFORM_IP32_L64_LL128     // int, ptr = 32 bits; long = 64 bits; long long = 128 bits.
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














