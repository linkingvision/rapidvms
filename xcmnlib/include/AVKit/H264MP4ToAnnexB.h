
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_H264MP4ToAnnexB_h
#define __AVKit_H264MP4ToAnnexB_h

#include "AVKit/Options.h"
#include "AVKit/AVDeMuxer.h"

#include "XSDK/Types.h"
#include "XSDK/XMemory.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

namespace AVKit
{

class H264MP4ToAnnexB
{
public:
    X_API H264MP4ToAnnexB( AVDeMuxer& deMuxer );

    X_API virtual ~H264MP4ToAnnexB() throw();

    X_API void Transform( uint8_t* src, size_t srcSize, bool keyFrame );
    X_API void Transform( XIRef<XSDK::XMemory> src, bool keyFrame );

    X_API size_t GetAnnexBSize() const;
    X_API void GetAnnexB( uint8_t* dest ) const;
    X_API XIRef<XSDK::XMemory> GetAnnexB() const;

private:
    H264MP4ToAnnexB( const H264MP4ToAnnexB& obj );
    H264MP4ToAnnexB& operator = ( const H264MP4ToAnnexB& );

    void _FreeFilteredPacket();

    AVBitStreamFilterContext* _bsfc;
    AVCodecContext* _codec;
    AVPacket _filteredPacket;
};

}

#endif
