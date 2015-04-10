
#include "h264_transcoder_test.h"
#include "ffkit/options.h"
#include "ffkit/av_demuxer.h"
#include "ffkit/h264_decoder.h"
#include "ffkit/h264_encoder.h"
#include "ffkit/av_muxer.h"
#include "ffkit/h264_transcoder.h"
#include "ffkit/locky.h"
#include "ffkit/options.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_path.h"

#include "gop.c"
#include "pic.c"

using namespace std;
using namespace cppkit;
using namespace ffkit;

REGISTER_TEST_FIXTURE(h264_transcoder_test);

void h264_transcoder_test::setup()
{
    locky::register_ffmpeg();

    struct codec_options options;

    options.gop_size = 15;
    options.bit_rate = 1424400; // size of gop * 8 == bit_rate
    options.width = 1280;
    options.height = 720;
    options.time_base_num = 1;
    options.time_base_den = 30;

    {
        shared_ptr<av_muxer> m = make_shared<av_muxer>( options,
                                                        "big.mp4",
                                                        av_muxer::OUTPUT_LOCATION_FILE );

        for( int i = 0; i < (NUM_FRAMES_IN_GOP * 10); i++ )
        {
            int index = i % NUM_FRAMES_IN_GOP;
            shared_ptr<av_packet> pkt = make_shared<av_packet>( gop[index].frame, gop[index].frameSize, false );
            m->write_video_packet( pkt, ((i % 15) == 0) ? true : false );
        }

        m->finalize_file();
    }
}

void h264_transcoder_test::teardown()
{
    locky::unregister_ffmpeg();

    remove( "big.mp4" );
}

void h264_transcoder_test::test_constructor()
{
    h264_transcoder transcoder( 1, 30, 1, 15, 1.0 );
}

void h264_transcoder_test::test_basic()
{
    av_demuxer dm( "big.mp4" );
    h264_decoder decoder( get_fast_h264_decoder_options() );
    h264_encoder encoder( get_fast_h264_encoder_options( 1000000, 640, 360, 15, 1, 15 ), false );
    av_muxer muxer( encoder.get_options(), "small.mp4", av_muxer::OUTPUT_LOCATION_FILE );
    muxer.set_extra_data( encoder.get_extra_data() );

    struct stream_statistics stats = av_demuxer::get_video_stream_statistics( "big.mp4" );

    h264_transcoder transcoder( stats.timeBaseNum.value(), stats.timeBaseDen.value(), 1, 15, 1.0 );

    decoder.set_output_width( 640 );
    decoder.set_output_height( 360 );

    while( !dm.end_of_file() )
    {
        if( transcoder.decode( dm, decoder ) )
        {
            transcoder.encode_yuv420p_and_mux( encoder, muxer, decoder.get() );
        }
    }

    muxer.finalize_file();

    UT_ASSERT( ck_path::exists( "small.mp4" ) );

    struct stream_statistics outputStats = av_demuxer::get_video_stream_statistics( "small.mp4" );

    UT_ASSERT( outputStats.frameRate.value() == 15 );
    UT_ASSERT( outputStats.gopSize.value() == 15 );
    UT_ASSERT( outputStats.numFrames.value() == 75 );

    remove( "small.mp4" );
}
