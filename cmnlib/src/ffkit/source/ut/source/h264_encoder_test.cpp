
#include "h264_encoder_test.h"
#include "ffkit/h264_encoder.h"
#include "ffkit/h264_decoder.h"
#include "ffkit/jpeg_encoder.h"
#include "ffkit/locky.h"
#include "ffkit/options.h"
#include "ffkit/frame_types.h"

extern "C"
{
#include "libavformat/avformat.h"
};

#include "gop.c"
#include "pic.c"

using namespace std;
using namespace cppkit;
using namespace ffkit;

REGISTER_TEST_FIXTURE(h264_encoder_test);

void h264_encoder_test::setup()
{
    //av_register_all();
    locky::register_ffmpeg();

    // pic_0 comes from the above included file pic.c
    _pic = make_shared<av_packet>( pic_0, pic_0_len );
}

void h264_encoder_test::teardown()
{
    locky::unregister_ffmpeg();
}

void h264_encoder_test::test_constructor()
{
    UT_ASSERT_NO_THROW( shared_ptr<h264_encoder> e = make_shared<h264_encoder>( get_fast_h264_encoder_options( 500000, 1280, 720, 15, 1, 15 ) ) );
}

void h264_encoder_test::test_encode_key()
{
    shared_ptr<h264_encoder> e;
    UT_ASSERT_NO_THROW( e = make_shared<h264_encoder>( get_fast_h264_encoder_options( 500000, 1280, 720, 15, 1, 15 ) ) );

    UT_ASSERT_NO_THROW( e->encode_yuv420p( _pic, FRAME_TYPE_KEY ) );

    shared_ptr<av_packet> output;
    UT_ASSERT_NO_THROW( output = e->get() );

    UT_ASSERT( output->get_data_size() > 0 );
}

void h264_encoder_test::test_encode_gop()
{
    shared_ptr<h264_decoder> d;
    UT_ASSERT_NO_THROW( d = make_shared<h264_decoder>( get_fast_h264_decoder_options() ) );

    d->set_output_width( 640 );
    d->set_output_height( 360 );

    shared_ptr<h264_encoder> e;
    UT_ASSERT_NO_THROW( e = make_shared<h264_encoder>( get_fast_h264_encoder_options( 500000, 640, 360, 15, 1, 5 ) ) );

    list<shared_ptr<av_packet> > outputFrames;

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        int index = i % NUM_FRAMES_IN_GOP;
        shared_ptr<av_packet> pkt = make_shared<av_packet>( gop[index].frame, gop[index].frameSize, false );
        d->decode( pkt );
        e->encode_yuv420p( d->get() );
        outputFrames.push_back( e->get() );
    }

    UT_ASSERT_NO_THROW( d = make_shared<h264_decoder>( get_fast_h264_decoder_options() ) );

    d->set_output_width( 640 );
    d->set_output_height( 360 );

    list<shared_ptr<av_packet> >::iterator i;
    for( i = outputFrames.begin(); i != outputFrames.end(); i++ )
    {
        shared_ptr<av_packet> frame = *i;
        d->decode( frame );

        shared_ptr<av_packet> pic;
        UT_ASSERT_NO_THROW( pic = d->get() );

#if 0
        shared_ptr<jpeg_encoder> e = make_shared<jpeg_encoder>( get_jpeg_options( 640, 360 ) );

        jpeg_encoder::write_jpeg_file( "out2.jpg",
                                       e->encode_yuv420p( pic ) );
#endif
    }
}
