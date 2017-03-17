
#include "AVDeMuxerTest.h"
#include "AVKit/AVMuxer.h"
#include "AVKit/AVDeMuxer.h"
#include "AVKit/H264Decoder.h"
#include "AVKit/Locky.h"
#include "AVKit/Options.h"
#include "XSDK/XRef.h"
#include "XSDK/XMemory.h"
#include "XSDK/LargeFiles.h"

#include "gop.c"

using namespace std;
using namespace XSDK;
using namespace AVKit;

CPPUNIT_TEST_SUITE_REGISTRATION(AVDeMuxerTest);

void AVDeMuxerTest::setUp()
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
                                       "AVDeMuxerTest.mp4",
                                       AVMuxer::OUTPUT_LOCATION_FILE );

        for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
        {
            int index = i % NUM_FRAMES_IN_GOP;
            m->WriteVideoFrame( gop[index].frame, gop[index].frameSize, ((i % 15) == 0) ? true : false );
        }

        m->FinalizeFile();
    }

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

void AVDeMuxerTest::tearDown()
{
    Locky::UnregisterFFMPEG();

    remove( "big.mp4" );
    remove( "AVDeMuxerTest.mp4" );
}

void AVDeMuxerTest::TestConstructor()
{
    printf("AVDeMuxerTest::TestConstructor()\n");
    fflush(stdout);

    XRef<AVDeMuxer> deMuxer = new AVDeMuxer( "AVDeMuxerTest.mp4" );
}

void AVDeMuxerTest::TestExamineFile()
{
    printf("AVDeMuxerTest::TestTexamineFile()\n");
    fflush(stdout);

    XRef<AVDeMuxer> deMuxer = new AVDeMuxer( "AVDeMuxerTest.mp4" );

    int videoStreamIndex = deMuxer->GetVideoStreamIndex();

    int index = 0;
    int i = 0;
    int streamIndex = 0;
    while( deMuxer->ReadFrame( streamIndex ) )
    {
        CPPUNIT_ASSERT( streamIndex == videoStreamIndex );

        int index = i % NUM_FRAMES_IN_GOP;

        bool keyness = ((i % NUM_FRAMES_IN_GOP) == 0) ? true : false;

        CPPUNIT_ASSERT( deMuxer->GetFrameSize() == gop[index].frameSize );
        CPPUNIT_ASSERT( deMuxer->IsKey() == keyness );

        XIRef<XMemory> frame = deMuxer->GetFrame();

        CPPUNIT_ASSERT( frame->GetDataSize() == gop[index].frameSize );

        // XXX Note: The contents of the frame cannot be compared for equality because FFMPEG stores the frames in
        // annexb format (with start codes).

        i++;
    }

    CPPUNIT_ASSERT( deMuxer->EndOfFile() );
}

void AVDeMuxerTest::TestFileFromMemory()
{
    printf("AVDeMuxerTest::TestFileFromMemory()\n");
    fflush(stdout);

    XRef<AVDeMuxer> deMuxer = new AVDeMuxer( AVDeMuxer::LoadFile( "AVDeMuxerTest.mp4" ) );

    int videoStreamIndex = deMuxer->GetVideoStreamIndex();

    int index = 0;
    int i = 0;
    int streamIndex = 0;
    while( deMuxer->ReadFrame( streamIndex ) )
    {
        CPPUNIT_ASSERT( streamIndex == videoStreamIndex );

        int index = i % NUM_FRAMES_IN_GOP;

        bool keyness = ((i % NUM_FRAMES_IN_GOP) == 0) ? true : false;

        CPPUNIT_ASSERT( deMuxer->GetFrameSize() == gop[index].frameSize );
        CPPUNIT_ASSERT( deMuxer->IsKey() == keyness );

        XIRef<XMemory> frame = deMuxer->GetFrame();

        CPPUNIT_ASSERT( frame->GetDataSize() == gop[index].frameSize );

        // XXX Note: The contents of the frame cannot be compared for equality because FFMPEG stores the frames in
        // annexb format (with start codes).

        i++;
    }

    CPPUNIT_ASSERT( deMuxer->EndOfFile() );
}

void AVDeMuxerTest::TestGetContainerStatistics()
{
    printf("AVDeMuxerTest::TestGetContainerStatistics()\n");
    fflush(stdout);

    struct StreamStatistics stats = AVDeMuxer::GetVideoStreamStatistics( "big.mp4" );

    CPPUNIT_ASSERT( !stats.averageBitRate.IsNull() );
    // Is the averageBitRate in the right ballpark?
    CPPUNIT_ASSERT( (stats.averageBitRate.Value() > 2500000) && (stats.averageBitRate.Value() < 3000000) );

    CPPUNIT_ASSERT( !stats.frameRate.IsNull() );
    CPPUNIT_ASSERT( stats.frameRate.Value() == 30 );

    CPPUNIT_ASSERT( !stats.gopSize.IsNull() );
    CPPUNIT_ASSERT( stats.gopSize.Value() == 15 );
}
