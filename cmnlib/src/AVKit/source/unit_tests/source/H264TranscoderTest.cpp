
#include "H264TranscoderTest.h"
#include "AVKit/Options.h"
#include "AVKit/AVDeMuxer.h"
#include "AVKit/H264Decoder.h"
#include "AVKit/H264Encoder.h"
#include "AVKit/AVMuxer.h"
#include "AVKit/H264Transcoder.h"
#include "AVKit/Locky.h"
#include "AVKit/Options.h"
#include "XSDK/XRef.h"
#include "XSDK/XMemory.h"
#include "XSDK/XPath.h"

#include "gop.c"
#include "pic.c"

using namespace std;
using namespace XSDK;
using namespace AVKit;

CPPUNIT_TEST_SUITE_REGISTRATION(H264TranscoderTest);

void H264TranscoderTest::setUp()
{
    av_register_all();
    Locky::RegisterFFMPEG();

    struct CodecOptions options;

    options.gop_size = 15;
    options.bit_rate = 1424400; // size of gop * 8 == bit_rate
    options.width = 1280;
    options.height = 720;
    options.time_base_num = 1;
    options.time_base_den = 30;

    {
        XRef<AVMuxer> m = new AVMuxer( options,
                                       "big.mp4",
                                       AVMuxer::OUTPUT_LOCATION_FILE );

        for( int i = 0; i < (NUM_FRAMES_IN_GOP * 10); i++ )
        {
            int index = i % NUM_FRAMES_IN_GOP;
            m->WriteVideoFrame( gop[index].frame, gop[index].frameSize, ((i % 15) == 0) ? true : false );
        }

        m->FinalizeFile();
    }
}

void H264TranscoderTest::tearDown()
{
    Locky::UnregisterFFMPEG();

    remove( "big.mp4" );
}

void H264TranscoderTest::TestConstructor()
{
    printf("H264TranscoderTest::TestConstructor()\n");
    fflush(stdout);

    H264Transcoder transcoder( 1, 30, 1, 15, 1.0 );
}

void H264TranscoderTest::TestBasic()
{
    printf("H264TranscoderTest::TestBasic()\n");
    fflush(stdout);

    AVDeMuxer dm( "big.mp4" );
    H264Decoder decoder( GetFastH264DecoderOptions() );
    H264Encoder encoder( GetFastH264EncoderOptions( 1000000, 640, 360, 15, 1, 15 ), false );
    AVMuxer muxer( encoder.GetOptions(), "small.mp4", AVMuxer::OUTPUT_LOCATION_FILE );
    muxer.SetExtraData( encoder.GetExtraData() );

    struct StreamStatistics stats = AVDeMuxer::GetVideoStreamStatistics( "big.mp4" );

    H264Transcoder transcoder( stats.timeBaseNum.Value(), stats.timeBaseDen.Value(), 1, 15, 1.0 );

    decoder.SetOutputWidth( 640 );
    decoder.SetOutputHeight( 360 );

    while( !dm.EndOfFile() )
    {
        if( transcoder.Decode( dm, decoder ) )
        {
            transcoder.EncodeYUV420PAndMux( encoder, muxer, decoder.MakeYUV420P() );
        }
    }

    muxer.FinalizeFile();

    CPPUNIT_ASSERT( XPath::Exists( "small.mp4" ) );

    struct StreamStatistics outputStats = AVDeMuxer::GetVideoStreamStatistics( "small.mp4" );

    CPPUNIT_ASSERT( outputStats.frameRate.Value() == 15 );
    CPPUNIT_ASSERT( outputStats.gopSize.Value() == 15 );
    CPPUNIT_ASSERT( outputStats.numFrames.Value() == 75 );

    remove( "small.mp4" );
}

