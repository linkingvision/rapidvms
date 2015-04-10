
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_H264Decoder_h
#define __AVKit_H264Decoder_h

#include "AVKit/Options.h"
#include "AVKit/AVDeMuxer.h"
#include "AVKit/Decoder.h"

#include "XSDK/Types.h"
#include "XSDK/XMemory.h"

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

namespace AVKit
{

const int H264_DECODE_ATTEMPTS = 16;

class H264Decoder : public Decoder
{
public:
    X_API H264Decoder( const struct CodecOptions& options, int decodeAttempts = H264_DECODE_ATTEMPTS );
    X_API H264Decoder( AVDeMuxer& deMuxer, const struct CodecOptions& options, int decodeAttempts = H264_DECODE_ATTEMPTS );
    X_API virtual ~H264Decoder() throw();

    X_API virtual void Decode( uint8_t* frame, size_t frameSize );
    X_API virtual void Decode( XIRef<XSDK::XMemory> frame );

    X_API virtual uint16_t GetInputWidth() const;
    X_API virtual uint16_t GetInputHeight() const;

    X_API virtual void SetOutputWidth( uint16_t outputWidth );
    X_API virtual uint16_t GetOutputWidth() const;

    X_API virtual void SetOutputHeight( uint16_t outputHeight );
    X_API virtual uint16_t GetOutputHeight() const;

    X_API virtual size_t GetYUV420PSize() const;
    X_API virtual void MakeYUV420P( uint8_t* dest );
    X_API virtual XIRef<XSDK::XMemory> MakeYUV420P();

private:
    H264Decoder( const H264Decoder& obj );
    H264Decoder& operator = ( const H264Decoder& );

    void _DestroyScaler();

    AVCodec* _codec;
    AVCodecContext* _context;
    struct CodecOptions _options;
    AVFrame* _frame;
    SwsContext* _scaler;
    uint16_t _outputWidth;
    uint16_t _outputHeight;

    int _decodeAttempts;
};

}

#endif
