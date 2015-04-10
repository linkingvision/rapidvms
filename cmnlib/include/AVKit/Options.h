
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_Options_h
#define __AVKit_Options_h

#include "XSDK/XNullable.h"
#include "XSDK/XString.h"

namespace AVKit
{

struct CodecOptions
{
    XSDK::XNullable<int> gop_size;
    XSDK::XNullable<int> bit_rate;
    XSDK::XNullable<int> width;
    XSDK::XNullable<int> height;
    XSDK::XNullable<int> time_base_num;
    XSDK::XNullable<int> time_base_den;
    XSDK::XNullable<int> initial_qp;
    XSDK::XNullable<int> me_method;
    XSDK::XNullable<int> me_subpel_quality;
    XSDK::XNullable<int> delay;
    XSDK::XNullable<int> thread_count;
    XSDK::XNullable<int> refs;
    XSDK::XNullable<int> qmin;
    XSDK::XNullable<int> qmax;
    XSDK::XNullable<int> max_qdiff;
    XSDK::XNullable<int> rc_buffer_size;
    XSDK::XNullable<XSDK::XString> profile;
    XSDK::XNullable<XSDK::XString> preset;
    XSDK::XNullable<XSDK::XString> tune;
    XSDK::XNullable<XSDK::XString> x264opts;
    XSDK::XNullable<XSDK::XString> device_path;
    // From here on these options do not correspond to AVCodecContext fields.

    XSDK::XNullable<bool> jpeg_source; // Enables YUVJ420P scaler context output. Set this to true if you're going to
                                       // use this decoder for making thumbnails.
};

X_API struct CodecOptions GetFastH264DecoderOptions( const XSDK::XString& devicePath = "" );

X_API struct CodecOptions GetFastH264EncoderOptions( int bitRate, int picWidth, int picHeight, int gopSize, int timeBaseNum, int timeBaseDen, const XSDK::XString& devicePath = "" );

X_API struct CodecOptions GetHLSH264EncoderOptions( int bitRate, int picWidth, int picHeight, int gopSize, int timeBaseNum, int timeBaseDen, const XSDK::XString& devicePath = "", int initialQP = -1 );

X_API struct CodecOptions GetTranscodeExportH264EncoderOptions( int bitRate, int picWidth, int picHeight, int gopSize, int timeBaseNum, int timeBaseDen, const XSDK::XString& devicePath = "" );

X_API const int DEFAULT_JPEG_BIT_RATE = 100000;
X_API const int DEFAULT_JPEG_QMIN = 8;
X_API const int DEFAULT_JPEG_QMAX = 8;

X_API struct CodecOptions GetJPEGOptions( int picWidth,
                                          int picHeight,
                                          int bitRate = DEFAULT_JPEG_BIT_RATE,
                                          int qmin = DEFAULT_JPEG_QMIN,
                                          int qmax = DEFAULT_JPEG_QMAX );

}

#endif
