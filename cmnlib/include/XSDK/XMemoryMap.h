
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_XMemoryMap_h
#define _XSDK_XMemoryMap_h

#include "XSDK/XString.h"
#include "XSDK/XMemory.h"
#include "XSDK/XBaseObject.h"

namespace XSDK
{

class XMemoryMap : public XBaseObject
{
public:
    enum Flags
    {
        XMM_TYPE_FILE = 0x01,
        XMM_TYPE_ANON = 0x02,
        XMM_SHARED = 0x04,
        XMM_PRIVATE = 0x08,
        XMM_FIXED = 0x10
    };

    enum Protection
    {
        XMM_PROT_NONE = 0x00,
        XMM_PROT_READ = 0x01,
        XMM_PROT_WRITE = 0x02,
        XMM_PROT_EXEC = 0x04
    };

    enum Advice
    {
        XMM_ADVICE_NORMAL = 0x00,
        XMM_ADVICE_RANDOM = 0x01,
        XMM_ADVICE_SEQUENTIAL = 0x02,
        XMM_ADVICE_WILLNEED = 0x04,
        XMM_ADVICE_DONTNEED = 0x08
    };

    X_API XMemoryMap( int fd,
                      uint32_t offset,
                      uint32_t len,
                      uint32_t prot,
                      uint32_t flags );

    X_API virtual ~XMemoryMap() throw();

    X_API inline void* Map() const
    {
        return _mem;
    }

    X_API inline uint32_t Length() const
    {
        return _length;
    }

    X_API void Advise( void* addr, size_t length, int advice ) const;

    X_API void Flush( void* addr, size_t length, bool now );

private:
    XMemoryMap( const XMemoryMap& obj );
    XMemoryMap& operator = ( const XMemoryMap& obj );

#ifdef WIN32
    int _GetWinProtFlags( int flags ) const;
    int _GetWinAccessFlags( int flags ) const;
#else
    int _GetPosixProtFlags( int prot ) const;
    int _GetPosixAccessFlags( int flags ) const;
    int _GetPosixAdvice( int advice ) const;
#endif

#ifdef WIN32
    HANDLE _fileHandle;
    HANDLE _mapHandle;
#endif
    void* _mem;
    uint32_t _length;
};

}

#endif
