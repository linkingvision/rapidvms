
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_ARGB24ToYUV420P_h
#define __AVKit_ARGB24ToYUV420P_h

#include "XSDK/Types.h"
#include "XSDK/XMemory.h"

namespace AVKit
{

class ARGB24ToYUV420P
{
public:
    X_API ARGB24ToYUV420P();

    X_API virtual ~ARGB24ToYUV420P() throw();

    X_API void Transform( uint8_t* src, size_t width, size_t height );
    X_API void Transform( XIRef<XSDK::XMemory> src, size_t width, size_t height );

    X_API size_t GetYUV420PSize() const;
    X_API void GetYUV420P( uint8_t* dest ) const;
    X_API XIRef<XSDK::XMemory> GetYUV420P() const;

private:
    ARGB24ToYUV420P( const ARGB24ToYUV420P& obj );
    ARGB24ToYUV420P& operator = ( const ARGB24ToYUV420P& );

    XIRef<XSDK::XMemory> _yuv420;
};

}

#endif
