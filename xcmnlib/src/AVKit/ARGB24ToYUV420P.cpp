
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "AVKit/ARGB24ToYUV420P.h"

#include "XSDK/XException.h"
#include "XSDK/XSocket.h"

using namespace AVKit;
using namespace XSDK;

ARGB24ToYUV420P::ARGB24ToYUV420P() :
    _yuv420()
{
}

ARGB24ToYUV420P::~ARGB24ToYUV420P() throw()
{
}

void ARGB24ToYUV420P::Transform( uint8_t* src, size_t width, size_t height )
{
    _yuv420 = new XMemory;

    uint8_t* yuv = &_yuv420->Extend( width * height * 1.5 );

    /// Cairo ARGB24 buffers are stored "native endian" as 8 bit unsigned integer quantities in the order
    /// ARGB.

    size_t imageSize = width * height;
    size_t upos = imageSize;
    size_t vpos = upos + upos / 4;
    size_t i = 0;

    for( size_t line = 0; line < height; ++line )
    {
        if( !(line % 2) )
        {
            for( size_t x = 0; x < width; x += 2 )
            {
                // Here I read a 4 byte quantity and on some architectures, swap it. Why swap? Well, cairo buffers
                // are native endian... Which means they will be different on little endian than on big endian. By
                // using htonl(), I can achieve portable code.
                uint32_t word = *((uint32_t*)&src[4 * i]);
                uint32_t pixel = htonl( word );
                uint8_t* rgba = (uint8_t*)&pixel;

                uint8_t r = *(rgba + 1);
                uint8_t g = *(rgba + 2);
                uint8_t b = *(rgba + 3);

                yuv[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;

                yuv[upos++] = ((-38*r + -74*g + 112*b) >> 8) + 128;
                yuv[vpos++] = ((112*r + -94*g + -18*b) >> 8) + 128;

                word = *((uint32_t*)&src[4 * i]);
                pixel = htonl( word );
                rgba = (uint8_t*)&pixel;

                r = *(rgba + 1);
                g = *(rgba + 2);
                b = *(rgba + 3);

                yuv[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        }
        else
        {
            for( size_t x = 0; x < width; x += 1 )
            {
                uint32_t word = *((uint32_t*)&src[4 * i]);
                uint32_t pixel = htonl( word );
                uint8_t* rgba = (uint8_t*)&pixel;

                uint8_t r = *(rgba + 1);
                uint8_t g = *(rgba + 2);
                uint8_t b = *(rgba + 3);

                yuv[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        }
    }
}

void ARGB24ToYUV420P::Transform( XIRef<XMemory> src, size_t width, size_t height )
{
    Transform( src->Map(), width, height );
}

size_t ARGB24ToYUV420P::GetYUV420PSize() const
{
    return _yuv420->GetDataSize();
}

void ARGB24ToYUV420P::GetYUV420P( uint8_t* dest ) const
{
    memcpy( dest, _yuv420->Map(), _yuv420->GetDataSize() );
}

XIRef<XSDK::XMemory> ARGB24ToYUV420P::GetYUV420P() const
{
    return _yuv420->Clone();
}
