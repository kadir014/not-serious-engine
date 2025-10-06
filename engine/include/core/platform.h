/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

/**
 * @file core/platform.h
 * @brief Platform identification.
 * 
 * All macros in this header uses preprocessors to identify platform specs.
 * Architecture related ones are usually not defined in MSVC.
 */
#ifndef _NS_PLATFORM_H
#define _NS_PLATFORM_H


#define __NS_STR(x) #x
#define _NS_STR(x) __NS_STR(x)


/*
    Compiler identification

    NS_COMPILER -> Identified compiler enum.
    NS_COMPILER_VERSION_STR -> Identified compiler version strings.
*/

#define NS_COMPILER_GCC     0
#define NS_COMPILER_CLANG   1
#define NS_COMPILER_MSVC    2
#define NS_COMPILER_UNKNOWN 3

#if defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__)

    #define NS_COMPILER NS_COMPILER_GCC
    #define NS_COMPILER_VERSION_STR  \
            _NS_STR(__GNUC__) "."       \
            _NS_STR(__GNUC_MINOR__) "." \
            _NS_STR(__GNUC_PATCHLEVEL__)

#elif defined(__clang__)

    #define NS_COMPILER NS_COMPILER_CLANG
    #define NS_COMPILER_VERSION_STR   \
            _NS_STR(__clang_major_) "."  \
            _NS_STR(__clang_minor__) "." \
            _NS_STR(__clang_patchlevel__)

#elif defined(_MSC_VER) || defined(_MSC_FULL_VER) || defined(_MSVC_LANG)

    #define NS_COMPILER NS_COMPILER_MSVC
    #define NS_COMPILER_VERSION_STR _NS_STR(_MSC_VER)

#else

    #define NS_COMPILER NS_COMPILER_UNKNOWN
    #define NS_COMPILER_VERSION_STR ""

#endif


/*
    Platform identification

    NS_PLATFORM -> Identified runtime platform enum.
*/

#define NS_PLATFORM_WEB     4
#define NS_PLATFORM_WINDOWS 5
#define NS_PLATFORM_LINUX   6
#define NS_PLATFORM_MACOS   7
#define NS_PLATFORM_IOS     8
#define NS_PLATFORM_ANDROID 9
#define NS_PLATFORM_UNIX    10
#define NS_PLATFORM_UNKNOWN 11

#if defined(__EMSCRIPTEN__)

    #define NS_PLATFORM NS_PLATFORM_WEB

#elif defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)

    #define NS_PLATFORM NS_PLATFORM_WINDOWS
    
#elif defined(__linux__) && !defined(__ANDROID__)

    #define NS_PLATFORM NS_PLATFORM_LINUX

#elif defined(__APPLE__) || defined(__MACH__)

    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE

        #define NS_PLATFORM NS_PLATFORM_IOS

    #elif TARGET_OS_MAC

        #define NS_PLATFORM NS_PLATFORM_MACOS

    #endif

#elif defined(__ANDROID__)

    #define NS_PLATFORM NS_PLATFORM_ANDROID

#elif defined(__unix__) || defined(__unix)

    #define NS_PLATFORM NS_PLATFORM_UNIX

#else

    #define NS_PLATFORM NS_PLATFORM_UNKNOWN

#endif


/*
    Architecture identification

    NS_ARCH -> Identified architecture enum.
*/

#define NS_ARCH_X86_64  12
#define NS_ARCH_X86     13
#define NS_ARCH_ARM     14
#define NS_ARCH_WASM    15
#define NS_ARCH_UNKNOWN 16

#if defined(__x86_64) || \
    defined(__x86_64) || \
    defined(__amd64)  || \
    defined(__amd64__)

    #define NS_ARCH NS_ARCH_X86_64

#elif defined(i386)     || \
      defined(__i386)   || \
      defined(__i386__) || \
      defined(_X86_)    || \
      defined(__I86__)

    #define NS_ARCH NS_ARCH_X86

#elif defined(__arm__)     || \
      defined(__arm64)     || \
      defined(__aarch64__)

    #define NS_ARCH NS_ARCH_ARM

#elif NS_PLATFORM == NS_PLATFORM_WEB

    #define NS_ARCH NS_ARCH_WASM

#else

    #define NS_ARCH NS_ARCH_UNKNOWN

#endif


/**
 * @brief Get the compiler identification as string.
 * 
 * @return char *
 */
static inline const char *NS_COMPILER_as_string() {
    switch (NS_COMPILER) {
        case NS_COMPILER_GCC:
            return "GCC";

        case NS_COMPILER_CLANG:
            return "Clang";

        case NS_COMPILER_MSVC:
            return "MSVC";

        default:
            return "Unknown";
    }
}

/**
 * @brief Get the platform identification as string.
 * 
 * @return char *
 */
static inline const char *NS_PLATFORM_as_string() {
    switch (NS_PLATFORM) {
        case NS_PLATFORM_WEB:
            return "Web";

        case NS_PLATFORM_WINDOWS:
            return "Windows";

        case NS_PLATFORM_LINUX:
            return "Linux";

        case NS_PLATFORM_MACOS:
            return "MacOS";

        case NS_PLATFORM_IOS:
            return "iOS";

        case NS_PLATFORM_ANDROID:
            return "Android";

        case NS_PLATFORM_UNIX:
            return "Unix";

        default:
            return "Unknown";
    }
}

/**
 * @brief Get the architecture identification as string.
 * 
 * @return char *
 */
static inline const char *NS_ARCH_as_string() {
    switch (NS_ARCH) {
        case NS_ARCH_X86_64:
            return "x86_64";

        case NS_ARCH_X86:
            return "x86";

        case NS_ARCH_ARM:
            return "ARM";

        case NS_ARCH_WASM:
            return "WebAssembly";

        default:
            return "Unknown";
    }
}


#endif