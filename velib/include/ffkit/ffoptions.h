
#ifndef __ffkit_options_h
#define __ffkit_options_h

#include "cppkit/ck_nullable.h"
#include "cppkit/ck_string.h"

namespace ffkit
{

struct codec_options
{
    cppkit::ck_nullable<int> gop_size;
    cppkit::ck_nullable<int> bit_rate;
    cppkit::ck_nullable<int> width;
    cppkit::ck_nullable<int> height;
    cppkit::ck_nullable<int> time_base_num;
    cppkit::ck_nullable<int> time_base_den;
    cppkit::ck_nullable<int> me_method;
    cppkit::ck_nullable<int> me_subpel_quality;
    cppkit::ck_nullable<int> delay;
    cppkit::ck_nullable<int> thread_count;
    cppkit::ck_nullable<int> refs;
    cppkit::ck_nullable<int> qmin;
    cppkit::ck_nullable<int> qmax;
    cppkit::ck_nullable<int> max_qdiff;
    cppkit::ck_nullable<int> rc_buffer_size;
    cppkit::ck_nullable<cppkit::ck_string> profile;
    cppkit::ck_nullable<cppkit::ck_string> preset;
    cppkit::ck_nullable<cppkit::ck_string> tune;
    cppkit::ck_nullable<cppkit::ck_string> x264opts;

    // From here on these options do not correspond to AVCodecContext fields.

    cppkit::ck_nullable<bool> jpeg_source; // Enables YUVJ420P scaler context output. Set this to true if you're going to
                                           // use this decoder for making thumbnails.
};

struct codec_options get_fast_h264_decoder_options();

struct codec_options get_fast_h264_encoder_options( int bitRate, int picWidth, int picHeight, int gopSize, int timeBaseNum, int timeBaseDen );

struct codec_options get_hls_h264_encoder_options( int bitRate, int picWidth, int picHeight, int gopSize, int timeBaseNum, int timeBaseDen );

struct codec_options get_transcode_export_h264_encoder_options( int bitRate, int picWidth, int picHeight, int gopSize, int timeBaseNum, int timeBaseDen );

const int DEFAULT_JPEG_BIT_RATE = 100000;
const int DEFAULT_JPEG_QMIN = 8;
const int DEFAULT_JPEG_QMAX = 8;

struct codec_options get_jpeg_options( int picWidth,
                                       int picHeight,
                                       int bitRate = DEFAULT_JPEG_BIT_RATE,
                                       int qmin = DEFAULT_JPEG_QMIN,
                                       int qmax = DEFAULT_JPEG_QMAX );

}

#endif
