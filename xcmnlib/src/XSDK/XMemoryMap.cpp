
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
#include "XSDK/Platform.h"
#include "XSDK/XMemoryMap.h"
#include "XSDK/OS.h"

#ifdef WIN32
#include <io.h>
#else
#include <sys/mman.h>
#endif

using namespace XSDK;

static const uint32_t MAX_MAPPING_LEN = 1048576000;

XMemoryMap::XMemoryMap( int fd,
                        uint32_t offset,
                        uint32_t len,
                        uint32_t prot,
                        uint32_t flags ) :
    XBaseObject(),
#ifdef WIN32
    _fileHandle( INVALID_HANDLE_VALUE ),
    _mapHandle( INVALID_HANDLE_VALUE ),
#endif
    _mem( NULL ),
    _length( len )
{
    if( fd <= 0 )
        X_THROW(( "Attempting to memory map a bad file descriptor." ));

    if( (len == 0) || (len > MAX_MAPPING_LEN) )
        X_THROW(( "Attempting to memory map more than 1gb is invalid." ));

    if( !(flags & XMM_TYPE_FILE) && !(flags & XMM_TYPE_ANON) )
        X_THROW(( "A mapping must be either a file mapping, or an anonymous mapping (neither was specified)." ));

    if( flags & XMM_FIXED )
        X_THROW(( "XMemoryMap does not support fixed mappings." ));

#ifdef WIN32
    int protFlags = _GetWinProtFlags( prot );
    int accessFlags = _GetWinAccessFlags( prot );

    if( fd != -1 )
        _fileHandle = (HANDLE)_get_osfhandle( fd );

    if( _fileHandle == INVALID_HANDLE_VALUE )
    {
        if( !(flags & XMM_TYPE_ANON) )
            X_THROW(( "An invalid fd was passed and this is not an anonymous mapping." ));
    }
    else
    {
        if( !DuplicateHandle( GetCurrentProcess(),
                              _fileHandle,
                              GetCurrentProcess(),
                              &_fileHandle,
                              0,
                              FALSE,
                              DUPLICATE_SAME_ACCESS ) )
            X_THROW(( "Unable to duplicate the provided fd file handle." ));

        _mapHandle = CreateFileMapping( _fileHandle, NULL, protFlags, 0, _length, NULL );
        if( _mapHandle == 0 )
            X_THROW(( "Unable to create file mapping: %s", GetLastErrorMsg().c_str() ));

        _mem = MapViewOfFile( _mapHandle, accessFlags, 0, offset, 0 );
        if( _mem == NULL )
            X_THROW(( "Unable to complete file mapping: %s", GetLastErrorMsg().c_str() ));
    }
#else
    _mem = mmap( NULL,
                 _length,
                 _GetPosixProtFlags( prot ),
                 _GetPosixAccessFlags( flags ),
                 fd,
                 offset );
#endif
}

XMemoryMap::~XMemoryMap() throw()
{
#ifdef WIN32
    UnmapViewOfFile( _mem );
    CloseHandle( _mapHandle );
    CloseHandle( _fileHandle );
#else
    munmap( _mem, _length );
#endif
}

void XMemoryMap::Advise( void* addr, size_t length, int advice ) const
{
#ifndef WIN32
    int posixAdvice = _GetPosixAdvice( advice );

    int err = madvise( addr, length, posixAdvice );

    if( err != 0 )
        X_THROW(( "Unable to apply memory mapping advice." ));
#endif
}

void XMemoryMap::Flush( void* addr, size_t length, bool now )
{
#ifndef WIN32
    int err = msync( addr, length, (now) ? MS_SYNC : MS_ASYNC );

    if( err != 0 )
        X_THROW(("Unable to sync memory mapped file."));
#else
    if( !FlushViewOfFile( addr, length ) )
        X_THROW(("Unable to sync memory mapped file."));

    if( now )
    {
        if( !FlushFileBuffers( _fileHandle ) )
            X_THROW(("Unable to flush file handle."));
    }
#endif
}

#ifdef WIN32

int XMemoryMap::_GetWinProtFlags( int flags ) const
{
    int prot = 0;

    if( flags & XMM_PROT_READ )
    {
        if( flags & XMM_PROT_WRITE )
            prot = (flags & XMM_PROT_EXEC) ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE;
        else prot = (flags & XMM_PROT_EXEC) ? PAGE_EXECUTE_READ : PAGE_READONLY;
    }
    else if( flags & XMM_PROT_WRITE )
        prot = (flags & XMM_PROT_EXEC) ? PAGE_EXECUTE_READ : PAGE_WRITECOPY;
    else if( flags & XMM_PROT_EXEC )
        prot = PAGE_EXECUTE_READ;

    return prot;
}

int XMemoryMap::_GetWinAccessFlags( int flags ) const
{
    int access = 0;

    if( flags & XMM_PROT_READ )
    {
        if( flags & XMM_PROT_WRITE )
            access = FILE_MAP_WRITE;
        else access = (flags & XMM_PROT_EXEC) ? FILE_MAP_EXECUTE : FILE_MAP_READ;
    }
    else if( flags & XMM_PROT_WRITE )
        access = FILE_MAP_COPY;
    else if( flags & XMM_PROT_EXEC )
        access = FILE_MAP_EXECUTE;

    return access;
}

#else

int XMemoryMap::_GetPosixProtFlags( int prot ) const
{
    int osProtFlags = 0;

    if( prot & XMM_PROT_READ )
        osProtFlags |= PROT_READ;
    if( prot & XMM_PROT_WRITE )
        osProtFlags |= PROT_WRITE;
    if( prot & XMM_PROT_EXEC )
        osProtFlags |= PROT_EXEC;

    return osProtFlags;
}

int XMemoryMap::_GetPosixAccessFlags( int flags ) const
{
    int osFlags = 0;

    if( flags & XMM_TYPE_FILE )
        osFlags |= MAP_FILE;
    if( flags & XMM_TYPE_ANON )
        osFlags |= MAP_ANONYMOUS;
    if( flags & XMM_SHARED )
        osFlags |= MAP_SHARED;
    if( flags & XMM_PRIVATE )
        osFlags |= MAP_PRIVATE;
    if( flags & XMM_FIXED )
        osFlags |= MAP_FIXED;

    return osFlags;
}

int XMemoryMap::_GetPosixAdvice( int advice ) const
{
    int posixAdvice = 0;

    if( advice & XMM_ADVICE_RANDOM )
        posixAdvice |= MADV_RANDOM;
    if( advice & XMM_ADVICE_SEQUENTIAL )
        posixAdvice |= MADV_SEQUENTIAL;
    if( advice & XMM_ADVICE_WILLNEED )
        posixAdvice |= MADV_WILLNEED;
    if( advice & XMM_ADVICE_DONTNEED )
        posixAdvice |= MADV_DONTNEED;

    return posixAdvice;
}

#endif

