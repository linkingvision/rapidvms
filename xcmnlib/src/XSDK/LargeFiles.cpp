
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/LargeFiles.h"

#ifdef IS_LINUX
#include <fcntl.h>
#endif

#ifdef IS_WINDOWS
#include <io.h>
#endif

namespace XSDK
{

#ifdef WIN32

X_API int x_stat( const XString& fileName, struct x_file_info* fileInfo )
{
    // XXX - Needs wide character conversion

    XString pathWithoutTrailingSlash = (fileName.EndsWith("\\")||fileName.EndsWith("/"))?fileName.substr(0,fileName.length()-1):fileName;
    int result = -1;
    struct __stat64 sfi;

    if( _wstat64( pathWithoutTrailingSlash.get_wide_string().data(), &sfi ) == 0 )
    {
        fileInfo->_fileSize = sfi.st_size;
        fileInfo->_fileType = (sfi.st_mode & _S_IFDIR) ? X_DIRECTORY : X_REGULAR;
        fileInfo->_accessTime = sfi.st_atime;

        // Note: On Win32, we currently hard code this to a reasonable value.
        fileInfo->_optimalBlockSize = 4096;

        result = 0;;
    }
    return result;
}

X_API int x_fseeko( FILE* file, int64_t offset, int whence )
{
    return _fseeki64( file, offset, whence );
}

X_API int64_t x_ftello( FILE* file )
{
    return _ftelli64( file );
}

X_API void x_fileflush( FILE* file )
{
    fflush( file );
}

X_API int x_filecommit( FILE* file )
{
    fflush(file);

    // Tell windows to commit all buffered file data to disk.
    int fd = _fileno(file);
    int err = _commit(fd);

    return err;
}

X_API int x_fallocate( FILE* file, int64_t size )
{
    return ( _chsize_s( fileno( file ), size ) == 0) ? 0 : -1;
}

#else

X_API int x_stat( const XString& fileName, struct x_file_info* fileInfo )
{
    struct stat sfi;

    if( stat( fileName.c_str(), &sfi ) == 0 )
    {
        fileInfo->_fileSize = sfi.st_size;
        fileInfo->_fileType = (sfi.st_mode & S_IFDIR) ? X_DIRECTORY : X_REGULAR;
        fileInfo->_accessTime = sfi.st_atime;
        fileInfo->_optimalBlockSize = sfi.st_blksize;

        return 0;
    }

    return -1;
}

X_API int x_fseeko( FILE* file, int64_t offset, int whence )
{
    return fseeko( file, offset, whence );
}

X_API int64_t x_ftello( FILE* file )
{
    return ftello( file );
}

X_API void x_fileflush( FILE* file )
{
    fflush( file );
}

X_API int x_filecommit( FILE* file )
{
    fflush(file);

    // sync all data associated with file stream to disk.
#ifndef __APPLE__
    int fd = fileno(file);
    fdatasync(fd);

    // advise kernel to dump cached data from memory.
    int err = posix_fadvise64(fd, 0, 0, POSIX_FADV_DONTNEED);

    return err;
#else
    return -1;
#endif
}

X_API int x_fallocate( FILE* file, int64_t size )
{
#ifndef __APPLE__
    return ( posix_fallocate64( fileno( file ), 0, size ) == 0 ) ? 0 : -1;
#else
    return -1;
#endif
}

#endif

X_API bool Exists( const XString& fileName )
{
    x_file_info fileInfo;
    return x_stat(fileName, &fileInfo) == 0;
}

XString TempDir()
{
#ifdef IS_LINUX
    const char* tmpDir = getenv("TMPDIR");
    if(tmpDir && Exists(tmpDir))
        return tmpDir;

    tmpDir = getenv("TEMP");
    if(tmpDir && Exists(tmpDir))
        return tmpDir;

    tmpDir = getenv("TMP");
    if(tmpDir && Exists(tmpDir))
        return tmpDir;

    tmpDir = "/tmp";
    if(Exists(tmpDir))
        return tmpDir;

    tmpDir = "/var/tmp";
    if(Exists(tmpDir))
        return tmpDir;

    tmpDir = "/usr/tmp";
    if(Exists(tmpDir))
        return tmpDir;

    return ".";
#else
    wchar_t buf[MAX_PATH];
    DWORD len = GetTempPathW(MAX_PATH, buf);
    return len != 0 ? XString(buf, len) : ".";
#endif
}

}
