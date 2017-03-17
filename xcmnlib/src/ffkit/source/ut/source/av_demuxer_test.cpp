
#include "av_demuxer_test.h"

#include "ffkit/av_muxer.h"
#include "ffkit/av_demuxer.h"
#include "ffkit/h264_decoder.h"
#include "ffkit/locky.h"
#include "ffkit/options.h"
#include "cppkit/ck_memory.h"
#include "cppkit/os/ck_large_files.h"

#include "gop.c"

using namespace std;
using namespace cppkit;
using namespace ffkit;

REGISTER_TEST_FIXTURE(av_demuxer_test);

void av_demuxer_test::setup()
{
//    av_register_all();
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
                                                        "AVDeMuxerTest.mp4",
                                                        av_muxer::OUTPUT_LOCATION_FILE );

        for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
        {
            int index = i % NUM_FRAMES_IN_GOP;
            shared_ptr<av_packet> pkt = make_shared<av_packet>( gop[index].frame, gop[index].frameSize, false );
            m->write_video_packet( pkt, ((i % 15) == 0) ? true : false );
        }

        m->finalize_file();
    }

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

void av_demuxer_test::teardown()
{
    locky::unregister_ffmpeg();

    remove( "big.mp4" );
    remove( "AVDeMuxerTest.mp4" );
}

void av_demuxer_test::test_constructor()
{
    shared_ptr<av_demuxer> deMuxer = make_shared<av_demuxer>( "AVDeMuxerTest.mp4" );
}

void av_demuxer_test::test_examine_file()
{
    shared_ptr<av_demuxer> deMuxer = make_shared<av_demuxer>( "AVDeMuxerTest.mp4" );

    int videoStreamIndex = deMuxer->get_video_stream_index();

    int index = 0;
    int i = 0;
    int streamIndex = 0;
    while( deMuxer->read_frame( streamIndex ) )
    {
        UT_ASSERT( streamIndex == videoStreamIndex );

        int index = i % NUM_FRAMES_IN_GOP;

        bool keyness = ((i % NUM_FRAMES_IN_GOP) == 0) ? true : false;

        UT_ASSERT( deMuxer->is_key() == keyness );

        shared_ptr<av_packet> frame = deMuxer->get();

        UT_ASSERT( frame->get_data_size() == gop[index].frameSize );

        // XXX Note: The contents of the frame cannot be compared for equality because FFMPEG stores the frames in
        // annexb format (with start codes).

        i++;
    }

    UT_ASSERT( deMuxer->end_of_file() );
}

void av_demuxer_test::test_file_from_memory()
{
    shared_ptr<av_demuxer> deMuxer = make_shared<av_demuxer>( av_demuxer::load_file( "AVDeMuxerTest.mp4" ) );

    int videoStreamIndex = deMuxer->get_video_stream_index();

    int index = 0;
    int i = 0;
    int streamIndex = 0;
    while( deMuxer->read_frame( streamIndex ) )
    {
        UT_ASSERT( streamIndex == videoStreamIndex );

        int index = i % NUM_FRAMES_IN_GOP;

        bool keyness = ((i % NUM_FRAMES_IN_GOP) == 0) ? true : false;

        UT_ASSERT( deMuxer->is_key() == keyness );

        shared_ptr<av_packet> frame = deMuxer->get();

        UT_ASSERT( frame->get_data_size() == gop[index].frameSize );

        // XXX Note: The contents of the frame cannot be compared for equality because FFMPEG stores the frames in
        // annexb format (with start codes).

        i++;
    }

    UT_ASSERT( deMuxer->end_of_file() );
}

void av_demuxer_test::test_get_container_statistics()
{
    struct stream_statistics stats = av_demuxer::get_video_stream_statistics( "big.mp4" );

    UT_ASSERT( !stats.averageBitRate.is_null() );
    // Is the averageBitRate in the right ballpark?
    UT_ASSERT( (stats.averageBitRate.value() > 2500000) && (stats.averageBitRate.value() < 3000000) );

    UT_ASSERT( !stats.frameRate.is_null() );
    UT_ASSERT( stats.frameRate.value() == 30 );

    UT_ASSERT( !stats.gopSize.is_null() );
    UT_ASSERT( stats.gopSize.value() == 15 );
}
