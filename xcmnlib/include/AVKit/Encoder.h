
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_Encoder_h
#define __AVKit_Encoder_h

#include "AVKit/FrameTypes.h"
#include "XSDK/XMemory.h"

namespace AVKit
{

class Encoder
{
public:
    X_API virtual ~Encoder() throw() {}

    X_API virtual size_t EncodeYUV420P( uint8_t* pic, uint8_t* output, size_t outputSize,
                                        FrameType type = FRAME_TYPE_AUTO_GOP ) = 0;

    X_API virtual XIRef<XSDK::XMemory> EncodeYUV420P( XIRef<XSDK::XMemory> pic,
                                                      FrameType type = FRAME_TYPE_AUTO_GOP ) = 0;

    X_API virtual bool LastWasKey() const = 0;

    X_API virtual struct CodecOptions GetOptions() const = 0;

    X_API virtual XIRef<XSDK::XMemory> GetExtraData() const = 0;
};

}

#endif
