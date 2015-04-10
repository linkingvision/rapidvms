
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_OS_H_
#define _XSDK_OS_H_

#include "XSDK/Platform.h"
#include "XSDK/OS/StackTrace.h"
#include "XSDK/OS/STL.h"
#include "XSDK/OS/Sprintf.h"
#include "XSDK/OS/Files.h"
#include "XSDK/Errors.h"
#include "XSDK/XString.h"
#include <string.h>


#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))

// Aligned memory alocations
#ifdef IS_WINDOWS

/// Allocates a buffer that's aligned to a power of 2.
#define X_NEW_ALIGNED_BUFFER( POINTER, SIZE, ALIGNMENT ) \
X_MACRO_BEGIN \
    POINTER = (uint8_t*)_aligned_malloc( SIZE, ALIGNMENT ); \
    if( POINTER == 0 ) \
        X_THROW_CRITICAL(("Failed to allocate an aligned buffer.")); \
X_MACRO_END

/// Frees an aligned buffer.
#define X_DELETE_ALIGNED_BUFFER( POINTER ) \
X_MACRO_BEGIN \
     _aligned_free( POINTER ); \
X_MACRO_END

#define FULL_MEM_BARRIER MemoryBarrier

#else // not IS_WINDOWS

/// @brief Allocates a buffer that's aligned to a power of 2 and a multiple of sizeof(void*).
#define X_NEW_ALIGNED_BUFFER( POINTER, SIZE, ALIGNMENT ) \
X_MACRO_BEGIN \
    size_t __alignment = (ALIGNMENT); \
    if (__alignment & (sizeof(void*) - 1)) \
        __alignment = (__alignment & ~(sizeof(void*) - 1)) + sizeof(void*); \
    size_t __size = (SIZE); \
    if (__size & (sizeof(void*) - 1)) \
        __size = (__size & ~(sizeof(void*) - 1)) + sizeof(void*); \
    if( posix_memalign( (void**)&(POINTER), __alignment, __size ) != 0 ) \
        X_THROW_CRITICAL(("Failed to allocate an aligned buffer.")); \
X_MACRO_END

/// Frees an aligned buffer.
#define X_DELETE_ALIGNED_BUFFER( POINTER ) \
X_MACRO_BEGIN \
     free( POINTER ); \
X_MACRO_END

#define FULL_MEM_BARRIER __sync_synchronize

#endif //IS_WINDOWS

namespace XSDK
{

    /// @brief Returns a string describing the last system error that occurred.
    /// The error is taken from 'errno' on Linux, or GetLastError() on Windows.
    X_API XString GetLastErrorMsg();

    /// @brief Returns a string describing the specified error code.
    X_API XString GetErrorMsg(int error);

}

#endif /* _XSDK_OS_H_ */
