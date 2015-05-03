
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_OS_LargeFiles_h
#define XSDK_OS_LargeFiles_h

// Large file support on Windows & Linux requires these definitions BEFORE
// inclusion of stdio.h.

#define _LARGE_FILE_API
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "XSDK/XSharedLib.h"
#include "XSDK/Types.h"
#include "XSDK/XString.h"

#ifdef IS_LINUX
#include <unistd.h>
#endif

namespace XSDK
{

enum x_file_type
{
    X_REGULAR,
    X_DIRECTORY
};

struct x_file_info
{
    int64_t _fileSize;
    uint64_t _accessTime;
    x_file_type _fileType;
    uint32_t _optimalBlockSize;
};

X_API int x_stat( const XString& fileName, struct x_file_info* fileInfo );

X_API int x_fseeko( FILE* file, int64_t offset, int whence );

X_API int64_t x_ftello( FILE* file );

X_API void x_fileflush( FILE* file );

X_API int x_filecommit( FILE* file );

X_API int x_fallocate( FILE* file, int64_t size );

/// Returns whether the given file exists.
X_API bool Exists( const XString& fileName );

/// Returns a directory for putting temporary files.
X_API XString TempDir();

}

#endif
