
#include "h264_decoder_test.h"
#include "ffkit/h264_decoder.h"
#include "ffkit/jpeg_encoder.h"
#include "ffkit/options.h"
#include "ffkit/locky.h"
#include "cppkit/ck_memory.h"

extern "C"
{
#include "libavformat/avformat.h"
};

#include "gop.c"

using namespace std;
using namespace cppkit;
using namespace ffkit;

REGISTER_TEST_FIXTURE(h264_decoder_test);

void h264_decoder_test::setup()
{
    locky::register_ffmpeg();
}

void h264_decoder_test::teardown()
{
    locky::unregister_ffmpeg();
}

void h264_decoder_test::test_constructor()
{
    UT_ASSERT_NO_THROW( shared_ptr<h264_decoder> d = make_shared<h264_decoder>( get_fast_h264_decoder_options() ) );
}

void h264_decoder_test::test_input_dimensions()
{
    shared_ptr<h264_decoder> decoder = make_shared<h264_decoder>( get_fast_h264_decoder_options() );
    shared_ptr<av_packet> pkt = make_shared<av_packet>( gop[0].frame, gop[0].frameSize, false );
    decoder->decode( pkt );

    UT_ASSERT( decoder->get_input_width() == 1280 );
    UT_ASSERT( decoder->get_input_height() == 720 );
}

void h264_decoder_test::test_output_dimensions()
{
    shared_ptr<h264_decoder> decoder = make_shared<h264_decoder>( get_fast_h264_decoder_options() );
    shared_ptr<av_packet> pkt = make_shared<av_packet>( gop[0].frame, gop[0].frameSize, false );
    decoder->decode( pkt );

    decoder->set_output_width( 640 );
    decoder->set_output_height( 360 );

    shared_ptr<av_packet> pic;
    UT_ASSERT_NO_THROW( pic = decoder->get() );
    UT_ASSERT( pic->get_data_size() == 345600 );
}

void h264_decoder_test::test_decode_gop()
{
    shared_ptr<h264_decoder> decoder = make_shared<h264_decoder>( get_fast_h264_decoder_options() );

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        shared_ptr<av_packet> pkt = make_shared<av_packet>( gop[i].frame, gop[i].frameSize, false );
        UT_ASSERT_NO_THROW( decoder->decode( pkt ) );

        shared_ptr<av_packet> pic;
        UT_ASSERT_NO_THROW( pic = decoder->get() );

        UT_ASSERT( pic->get_data_size() == 1382400 );
    }
}
