
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

#ifndef cppkit_memory_map_h
#define cppkit_memory_map_h

#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_byte_ptr.h"

namespace cppkit
{

class ck_memory_map
{
public:
    enum Flags
    {
        MM_TYPE_FILE = 0x01,
        MM_TYPE_ANON = 0x02,
        MM_SHARED = 0x04,
        MM_PRIVATE = 0x08,
        MM_FIXED = 0x10
    };

    enum Protection
    {
        MM_PROT_NONE = 0x00,
        MM_PROT_READ = 0x01,
        MM_PROT_WRITE = 0x02,
        MM_PROT_EXEC = 0x04
    };

    enum Advice
    {
        MM_ADVICE_NORMAL = 0x00,
        MM_ADVICE_RANDOM = 0x01,
        MM_ADVICE_SEQUENTIAL = 0x02,
        MM_ADVICE_WILLNEED = 0x04,
        MM_ADVICE_DONTNEED = 0x08
    };

    CK_API ck_memory_map() = delete;
    CK_API ck_memory_map( const ck_memory_map& obj ) = delete;
    CK_API ck_memory_map( int fd,
                          uint32_t offset,
                          uint32_t len,
                          uint32_t prot,
                          uint32_t flags );

    CK_API virtual ~ck_memory_map() throw();

    CK_API ck_memory_map& operator = ( const ck_memory_map& ) = delete;

    CK_API inline ck_byte_ptr map() const
    {
        return ck_byte_ptr( _mem, _length );
    }

    CK_API inline uint32_t size() const
    {
        return _length;
    }

    CK_API void advise( void* addr, size_t length, int advice ) const;

private:

#ifdef IS_WINDOWS
    int _get_win_prot_flags( int flags ) const;
    int _get_win_access_flags( int flags ) const;
#else
    int _get_posix_prot_flags( int prot ) const;
    int _get_posix_access_flags( int flags ) const;
    int _get_posix_advice( int advice ) const;
#endif

#ifdef IS_WINDOWS
    HANDLE _fileHandle;
    HANDLE _mapHandle;
#endif
    void* _mem;
    uint32_t _length;
};

}

#endif
