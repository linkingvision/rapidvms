
#include "ffkit/ffoptions.h"

using namespace cppkit;

namespace ffkit
{

struct codec_options get_fast_h264_decoder_options()
{
    struct codec_options options;

    options.thread_count = 1;
    options.tune = "zerolatency";

    return options;
}

struct codec_options get_fast_decoder_options()
{
    struct codec_options options;

    options.thread_count = 1;
    options.tune = "zerolatency";

    return options;
}


struct codec_options get_fast_h264_encoder_options( int bitRate, int picWidth, int picHeight, int gopSize, int timeBaseNum, int timeBaseDen )
{
    struct codec_options options;

    options.gop_size = gopSize;
    options.bit_rate = bitRate;
    options.width = picWidth;
    options.height = picHeight;
    options.time_base_num = timeBaseNum;
    options.time_base_den = timeBaseDen;
    options.me_method = 0;
    options.me_subpel_quality = 4;
    options.delay = 0;
    options.thread_count = 6;
    options.refs = 3;
    options.rc_buffer_size = 0;
    options.profile = "baseline";
    options.preset = "ultrafast";
    options.tune = "zerolatency";

    return options;
}

struct codec_options get_hls_h264_encoder_options( int bitRate, int picWidth, int picHeight, int gopSize, int timeBaseNum, int timeBaseDen )
{
    struct codec_options options;

    options.gop_size = gopSize;
    options.bit_rate = bitRate;
    options.width = picWidth;
    options.height = picHeight;
    options.time_base_num = timeBaseNum;
    options.time_base_den = timeBaseDen;
    options.delay = 0;
    options.thread_count = 6;
    options.preset = "superfast";
    options.tune = "zerolatency";
    options.x264opts = ck_string::format("no-scenecut:vbv-maxrate=%d:vbv-bufsize=%d:me=hex:subme=7", bitRate / 1000, bitRate / 1000);

    return options;
}

struct codec_options get_transcode_export_h264_encoder_options( int bitRate, int picWidth, int picHeight, int gopSize, int timeBaseNum, int timeBaseDen )
{
    struct codec_options options;

    options.gop_size = gopSize;
    options.bit_rate = bitRate;
    options.width = picWidth;
    options.height = picHeight;
    options.time_base_num = timeBaseNum;
    options.time_base_den = timeBaseDen;
    options.delay = 0;
    options.thread_count = 6;
    options.tune = "zerolatency";

    return options;
}

struct codec_options get_jpeg_options( int picWidth,
                                       int picHeight,
                                       int bitRate,
                                       int qmin,
                                       int qmax )
{
    struct codec_options options;

    options.bit_rate = bitRate;
    options.width = picWidth;
    options.height = picHeight;
    options.qmin = qmin;
    options.qmax = qmax;

    return options;
}

}
