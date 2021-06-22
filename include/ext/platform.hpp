/**
 * @file    platform.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.06.2021
 */

#ifndef EXT_PLATFORM_HPP
#define EXT_PLATFORM_HPP

// Detect compiler
#if defined(__clang__)
#   define EXT_COMPILER_CLANG
#   define EXT_COMPILER clang
#elif defined(__GNUC__) or defined(__GNUG__)
#   define EXT_COMPILER_GCC
#   define EXT_COMPILER GCC
#else
#   error Compiler is not supported
#endif

// Header-only
#ifdef EXT_HEADER_ONLY
#   define EXT_INLINE inline
#else
#   define EXT_INLINE
#endif

// Utility macros
#define EXT_STRINGIFY_(ARG) #ARG
#define EXT_STRINGIFY(ARG) EXT_STRINGIFY_(ARG)

#define EXT_SEPARATE_WITH_SPACE(ARG0, ARG1) ARG0 ARG1

#define EXT_COMPILER_SPECIFIC_PRAGMA_(ARG) _Pragma(EXT_STRINGIFY(EXT_SEPARATE_WITH_SPACE(EXT_COMPILER, ARG)))
#define EXT_COMPILER_SPECIFIC_PRAGMA(ARG) EXT_COMPILER_SPECIFIC_PRAGMA_(ARG)

#define EXT_DISABLE_WARNINGS_PUSH \
        EXT_COMPILER_SPECIFIC_PRAGMA(diagnostic push) \
        EXT_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wall") \
        EXT_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wpedantic") \
        EXT_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wextra") \
        EXT_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wconversion") \
        EXT_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Weffc++")

#define EXT_DISABLE_WARNINGS_POP \
        EXT_COMPILER_SPECIFIC_PRAGMA(diagnostic pop)

#endif // EXT_PLATFORM_HPP
