
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// cppkit - http://www.cppkit.org
/// Copyright (c) 2013, Tony Di Croce
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted
/// provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
///    the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
///    and the following disclaimer in the documentation and/or other materials provided with the
///    distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
/// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
/// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// The views and conclusions contained in the software and documentation are those of the authors and
/// should not be interpreted as representing official policies, either expressed or implied, of the cppkit
/// Project.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#include "cppkit/os/ck_large_files.h"
#include "cppkit/os/ck_platform.h"
#include "cppkit/ck_memory_map.h"

#ifdef IS_WINDOWS
    #include <io.h>
#else
    #include <sys/mman.h>
#endif

using namespace cppkit;

static const uint32_t MAX_MAPPING_LEN = 1048576000;

ck_memory_map::ck_memory_map( int fd,
                              uint32_t offset,
                              uint32_t len,
                              uint32_t prot,
                              uint32_t flags ) :
#ifdef IS_WINDOWS
    _fileHandle( INVALID_HANDLE_VALUE ),
    _mapHandle( INVALID_HANDLE_VALUE ),
#endif
    _mem( NULL ),
    _length( len )
{
    if( fd <= 0 )
        CK_THROW(( "Attempting to memory map a bad file descriptor." ));

    if( (len == 0) || (len > MAX_MAPPING_LEN) )
        CK_THROW(( "Attempting to memory map more than 1gb is invalid." ));

    if( !(flags & MM_TYPE_FILE) && !(flags & MM_TYPE_ANON) )
        CK_THROW(( "A mapping must be either a file mapping, or an anonymous mapping (neither was specified)." ));

    if( flags & MM_FIXED )
        CK_THROW(( "ck_memory_map does not support fixed mappings." ));

#ifdef IS_WINDOWS
    int protFlags = _get_win_prot_flags( prot );
    int accessFlags = _get_win_access_flags( prot );

    if( fd != -1 )
        _fileHandle = (HANDLE)_get_osfhandle( fd );

    if( _fileHandle == INVALID_HANDLE_VALUE )
    {
        if( !(flags & MM_TYPE_ANON) )
            CK_THROW(( "An invalid fd was passed and this is not an anonymous mapping." ));
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
            CK_THROW(( "Unable to duplicate the provided fd file handle." ));

        _mapHandle = CreateFileMapping( _fileHandle, NULL, protFlags, 0, _length, NULL );
        if( _mapHandle == 0 )
            CK_THROW(( "Unable to create file mapping." ));

        _mem = MapViewOfFile( _mapHandle, accessFlags, 0, offset, 0 );
        if( _mem == NULL )
            CK_THROW(( "Unable to complete file mapping." ));
    }
#else
    _mem = mmap( NULL,
                 _length,
                 _get_posix_prot_flags( prot ),
                 _get_posix_access_flags( flags ),
                 fd,
                 offset );
#endif
}

ck_memory_map::~ck_memory_map() throw()
{
#ifdef IS_WINDOWS
    UnmapViewOfFile( _mem );
    CloseHandle( _mapHandle );
    CloseHandle( _fileHandle );
#else
    munmap( _mem, _length );
#endif
}

void ck_memory_map::advise( void* addr, size_t length, int advice ) const
{
#ifdef IS_LINUX
    int posixAdvice = _get_posix_advice( advice );

    int err = madvise( addr, length, posixAdvice );

    if( err != 0 )
        CK_THROW(( "Unable to apply memory mapping advice." ));
#endif
}

#ifdef IS_WINDOWS

int ck_memory_map::_get_win_prot_flags( int flags ) const
{
    int prot = PAGE_NOACCESS;

    if( flags & MM_PROT_READ )
    {
        if( flags & MM_PROT_WRITE )
            prot = (flags & MM_PROT_EXEC) ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE;
        else prot = (flags & MM_PROT_EXEC) ? PAGE_EXECUTE_READ : PAGE_READONLY;
    }
    else if( flags & MM_PROT_WRITE )
        prot = (flags & MM_PROT_EXEC) ? PAGE_EXECUTE_READ : PAGE_WRITECOPY;
    else if( flags & MM_PROT_EXEC )
        prot = PAGE_EXECUTE_READ;

    return prot;
}

int ck_memory_map::_get_win_access_flags( int flags ) const
{
    int access = 0;

    if( flags & MM_PROT_READ )
    {
        if( flags & MM_PROT_WRITE )
            access = FILE_MAP_WRITE;
        else access = (flags & MM_PROT_EXEC) ? FILE_MAP_EXECUTE : FILE_MAP_READ;
    }
    else if( flags & MM_PROT_WRITE )
        access = FILE_MAP_COPY;
    else if( flags & MM_PROT_EXEC )
        access = FILE_MAP_EXECUTE;

    return access;
}

#else

int ck_memory_map::_get_posix_prot_flags( int prot ) const
{
    int osProtFlags = 0;

    if( prot & MM_PROT_READ )
        osProtFlags |= PROT_READ;
    if( prot & MM_PROT_WRITE )
        osProtFlags |= PROT_WRITE;
    if( prot & MM_PROT_EXEC )
        osProtFlags |= PROT_EXEC;

    return osProtFlags;
}

int ck_memory_map::_get_posix_access_flags( int flags ) const
{
    int osFlags = 0;

    if( flags & MM_TYPE_FILE )
        osFlags |= MAP_FILE;
    if( flags & MM_TYPE_ANON )
        osFlags |= MAP_ANONYMOUS;
    if( flags & MM_SHARED )
        osFlags |= MAP_SHARED;
    if( flags & MM_PRIVATE )
        osFlags |= MAP_PRIVATE;
    if( flags & MM_FIXED )
        osFlags |= MAP_FIXED;

    return osFlags;
}

int ck_memory_map::_get_posix_advice( int advice ) const
{
    int posixAdvice = 0;

    if( advice & MM_ADVICE_RANDOM )
        posixAdvice |= MADV_RANDOM;
    if( advice & MM_ADVICE_SEQUENTIAL )
        posixAdvice |= MADV_SEQUENTIAL;
    if( advice & MM_ADVICE_WILLNEED )
        posixAdvice |= MADV_WILLNEED;
    if( advice & MM_ADVICE_DONTNEED )
        posixAdvice |= MADV_DONTNEED;

    return posixAdvice;
}

#endif
