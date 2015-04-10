
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_YUV420PToARGB24_h
#define __AVKit_YUV420PToARGB24_h

#include "XSDK/Types.h"
#include "XSDK/XMemory.h"

namespace AVKit
{

class YUV420PToARGB24
{
public:
    X_API YUV420PToARGB24();

    X_API virtual ~YUV420PToARGB24() throw();

    X_API void Transform( uint8_t* src, size_t width, size_t height );
    X_API void Transform( XIRef<XSDK::XMemory> src, size_t width, size_t height );

    X_API size_t GetARGB24Size() const;
    X_API void GetARGB24( uint8_t* dest ) const;
    X_API XIRef<XSDK::XMemory> GetARGB24() const;

private:
    YUV420PToARGB24( const YUV420PToARGB24& obj );
    YUV420PToARGB24& operator = ( const YUV420PToARGB24& );

    XIRef<XSDK::XMemory> _rgb24;
};

}

#endif
