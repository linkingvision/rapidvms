
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_OS_Sprintf_h
#define XSDK_OS_Sprintf_h

#include <cstdio>
#include <cctype>
#include "XSDK/XString.h"

#ifdef IS_WINDOWS
#include <stdarg.h>

#pragma warning(push)
#pragma warning(disable:4996)   // Disable warning about unsafe _vsnprintf

#endif

namespace XSDK
{

//sscanf
#ifdef IS_LINUX
/// Cross-platform version of printf.
///
/// Will take the Windows i32 and i64 type specifiers (e.g. %i64d, %i64u, %i64x, etc.).
int x_printf(const char* format...);
#elif defined(IS_WINDOWS)
#define x_printf printf
#else
    #error ">>Unknown OS!"
#endif

//snprintf
//This is a macro instead of a function thanks to its variadic argument.
#ifdef IS_WINDOWS
#define x_snprintf _snprintf
#else
#define x_snprintf snprintf
#endif

//vsnprintf
#ifdef IS_LINUX
/// Cross-platform version of vsnprintf.
///
/// Will take the Windows i32 and i64 type specifiers (e.g. %i64d, %i64u, %i64x, etc.).
int x_vsnprintf(char* str, size_t size, XSDK::XString format, va_list args);

#elif defined(IS_WINDOWS)
inline int x_vsnprintf(char* str, size_t size, XSDK::XString format, va_list args)
{
    return x_vsnprintf(str, size, format.c_str(), args);
}

inline int x_vsnprintf(char* str, size_t size, const char* format, va_list args)
{
    if(str == NULL || size == 0)
        return _vscprintf(format, args);
    else
        return _vsnprintf(str, size, format, args);
}
#else
    #error ">>Unknown OS!"
#endif

//sscanf
#ifdef IS_LINUX
/// Cross-platform version of sscanf.
///
/// Will take the Windows i32 and i64 type specifiers (e.g. %i64d, %i64u, %i64x, etc.).
int x_sscanf(const char* str, const char* format...);
#elif defined(IS_WINDOWS)
#define x_sscanf sscanf
#else
    #error ">>Unknown OS!"
#endif

}

#ifdef IS_WINDOWS
#ifndef va_copy
#define va_copy(dst, src) ((void)((dst) = (src)))
#endif
#endif

#ifdef IS_WINDOWS
#pragma warning(pop)
#endif

#endif

