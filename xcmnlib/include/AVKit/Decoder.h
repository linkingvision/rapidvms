
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit__Decoder_h
#define __AVKit__Decoder_h

#include "XSDK/XMemory.h"
#include "XSDK/XIRef.h"

namespace AVKit
{

class Decoder
{
public:
    X_API virtual ~Decoder() throw() {}

    X_API virtual void Decode( uint8_t* frame, size_t frameSize ) = 0;
    X_API virtual void Decode( XIRef<XSDK::XMemory> frame ) = 0;

    X_API virtual uint16_t GetInputWidth() const = 0;
    X_API virtual uint16_t GetInputHeight() const = 0;

    X_API virtual void SetOutputWidth( uint16_t outputWidth ) = 0;
    X_API virtual uint16_t GetOutputWidth() const = 0;

    X_API virtual void SetOutputHeight( uint16_t outputHeight ) = 0;
    X_API virtual uint16_t GetOutputHeight() const = 0;

    X_API virtual size_t GetYUV420PSize() const = 0;
    X_API virtual void MakeYUV420P( uint8_t* dest ) = 0;
    X_API virtual XIRef<XSDK::XMemory> MakeYUV420P() = 0;
};

}

#endif
