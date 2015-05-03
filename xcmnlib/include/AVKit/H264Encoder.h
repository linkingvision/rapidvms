
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_H264Encoder_h
#define __AVKit_H264Encoder_h

#include "AVKit/Options.h"
#include "AVKit/FrameTypes.h"
#include "AVKit/Encoder.h"
#include "XSDK/XMemory.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

namespace AVKit
{

const int H264_ENCODE_ATTEMPTS = 16;

class H264Encoder : public Encoder
{
public:

    X_API H264Encoder( const struct CodecOptions& options,
                       bool annexB = true,
                       int encodeAttempts = H264_ENCODE_ATTEMPTS );

    X_API virtual ~H264Encoder() throw();

    /// Encode the YUV420P image pointed to by pic into an H.264 frame, writing the output frame
    /// to the memory pointed to by output.
    X_API virtual size_t EncodeYUV420P( uint8_t* pic, uint8_t* output, size_t outputSize,
                                        FrameType type = FRAME_TYPE_AUTO_GOP );

    /// A convenience method that wraps the functionaliy provided above but takes and returns
    /// XMemory objects. (note: because this method allocates memory, it is not quite as efficient
    /// as the above method).
    X_API virtual XIRef<XSDK::XMemory> EncodeYUV420P( XIRef<XSDK::XMemory> pic,
                                                      FrameType type = FRAME_TYPE_AUTO_GOP );

    X_API virtual bool LastWasKey() const { return _lastWasKey; }

    X_API virtual struct CodecOptions GetOptions() const;

    X_API virtual XIRef<XSDK::XMemory> GetExtraData() const;

private:
    H264Encoder( const H264Encoder& obj );
    H264Encoder& operator = ( const H264Encoder& );

    AVCodec* _codec;
    AVCodecContext* _context;
    struct CodecOptions _options;
    uint8_t _gopSize;
    uint8_t _numTillKey;
    int _encodeAttempts;
    XSDK::XMemory _extraData;
    bool _lastWasKey;
    bool _annexB;
};

}

#endif
