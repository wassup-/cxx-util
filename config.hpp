////////////////////////////////////////////////////////////
//
// typesafe-db
// Copyright (C) 2012-2013 Tom Knapen (tom@failproof.be)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#include "cxx_required.hpp"

#ifndef CXX_UTIL_CONFIG_HPP_
#define CXX_UTIL_CONFIG_HPP_

////////////////////////////////////////////////////////////
// Define the library version
////////////////////////////////////////////////////////////
#define CXXU_VERSION_MAJOR 0
#define CXXU_VERSION_MINOR 1

////////////////////////////////////////////////////////////
// Identify the operating system
////////////////////////////////////////////////////////////
#if (defined(_WIN32) || defined(__WIN32__))
  // Windows
  #define CXXU_SYSTEM_WINDOWS
  // Prevent MIN / MAX preprocessor macros
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
#elif (defined(linux) || defined(__linux))
  // Linux
  #define CXXU_SYSTEM_LINUX
#elif (defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh))
  // MacOS
  #define CXXU_SYSTEM_MACOS
#elif (defined(__FreeBSD__) || defined(__FreeBSD_kernel__))
  // FreeBSD
  #define CXXU_SYSTEM_FREEBSD
#else
  // Unsupported system
  #error This operating system is unsupported by typesafe-db
#endif

#if defined(__GNUC__)
  #if defined(__clang)
    #define CXXU_COMPILER_CLANG
  #elif defined(__INTEL_COMPILER)
    #define CXXU_COMPILER_INTEL
  #else
    #define CXXU_COMPILER_GCC
  #endif
#elif defined(__COMO__)
  #define CXXU_COMPILER_COMEAU
#elif defined(__llvm__)
  #define CXXU_COMPILER_LLVM
#elif defined(__MWERKS__)
  #define CXXU_COMPILER_METROWERKS
#elif defined(_MSC_VER)
  #define CXXU_COMPILER_MSVC
#elif defined(__MINGW32__)
  #define CXXU_COMPILER_MINGW
#endif

////////////////////////////////////////////////////////////
// Define a portable debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG)
  #define CXXU_DEBUG
#endif

#if defined(__GNUC__) && (__GNUC__ >= 4)
  #define CXXU_UNREACHABLE      __builtin_unreachable();
  #define CXXU_LIKELY(x)        __builtin_expect(!!(x), 1)
  #define CXXU_UNLIKELY(x)      __builtin_expect(!!(x), 0)
#else
  #define CXXU_UNREACHABLE
  #define CXXU_LIKELY(x)        (x)
  #define CXXU_UNLIKELY(x)      (x)
#endif

#if defined(CXXU_COMPILER_GCC)
  #define CXXU_DEPRECATED(fn, msg)  fn __attribute__((deprecated(msg)))
#elif defined(CXXU_COMPILER_MSVC)
  #define CXXU_DEPRECATED(fn, msg)  __declspec(deprecated(msg)) fn
#else
  #define CXXU_DEPRECATED(fn, msg)  fn
#endif

////////////////////////////////////////////////////////////
// Define helpers to create portable import / export macros for each module
////////////////////////////////////////////////////////////
#if !defined(CXXU_STATIC)
  #if defined(CXXU_SYSTEM_WINDOWS)
    // Windows compilers need specific (and different) keywords for export and import
    #define CXXU_API_EXPORT   __declspec(dllexport)
    #define CXXU_API_IMPORT   __declspec(dllimport)
    #define CXXU_API_LOCAL
  #else // Linux, FreeBSD, Mac OS X
    #if (__GNUC__ >= 4)
      // GCC 4 has special keywords for showing/hiding symbols,
      // the same keyword is used for both importing and exporting
      #define CXXU_API_EXPORT   __attribute__ ((__visibility__ ("default")))
      #define CXXU_API_IMPORT   __attribute__ ((__visibility__ ("default")))
      #define CXXU_API_LOCAL    __attribute__ ((__visibility__ ("hidden")))
    #else
      // GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
      #define CXXU_API_EXPORT
      #define CXXU_API_IMPORT
      #define CXXU_API_LOCAL
    #endif
  #endif
#else
  // Static build doesn't need import/export macros
  #define CXXU_API_EXPORT
  #define CXXU_API_IMPORT
  #define CXXU_API_LOCAL
#endif

#endif  // CXX_UTIL_CONFIG_HPP_
