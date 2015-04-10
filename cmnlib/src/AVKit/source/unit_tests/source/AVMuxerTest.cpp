
#include "AVMuxerTest.h"
#include "AVKit/Options.h"
#include "AVKit/AVMuxer.h"
#include "AVKit/H264Decoder.h"
#include "AVKit/H264Encoder.h"
#include "AVKit/JPEGEncoder.h"
#include "AVKit/Locky.h"
#include "AVKit/Options.h"
#include "XSDK/XRef.h"
#include "XSDK/XMemory.h"

#include "gop.c"
#include "pic.c"

using namespace std;
using namespace XSDK;
using namespace AVKit;

CPPUNIT_TEST_SUITE_REGISTRATION(AVMuxerTest);

void AVMuxerTest::setUp()
{
    av_register_all();
    Locky::RegisterFFMPEG();

    // pic_0 comes from the above included file pic.c
    _pic = new XMemory;
    memcpy( &_pic->Extend( pic_0_len ), pic_0, pic_0_len );
}

void AVMuxerTest::tearDown()
{
    Locky::UnregisterFFMPEG();
}

void AVMuxerTest::TestConstructor()
{
    printf("AVMuxerTest::TestConstructor()\n");
    fflush(stdout);

    CPPUNIT_ASSERT_NO_THROW( XRef<AVMuxer> c = new AVMuxer( GetFastH264EncoderOptions( 500000, 1280, 720, 15, 1, 15 ), "foo.mp4", AVMuxer::OUTPUT_LOCATION_BUFFER ) );
}

void AVMuxerTest::TestMP4()
{
    printf("AVMuxerTest::TestMP4()\n");
    fflush(stdout);

    XRef<H264Decoder> d = new H264Decoder( GetFastH264DecoderOptions() );
    d->SetOutputWidth( 640 );
    d->SetOutputHeight( 360 );
    XRef<H264Encoder> e = new H264Encoder( GetFastH264EncoderOptions( 250000, 640, 360, 15, 1, 15 ) );
    XRef<AVMuxer> c = new AVMuxer( e->GetOptions(),
                                   "foo.mp4",
                                   AVMuxer::OUTPUT_LOCATION_FILE );

    c->SetExtraData( e->GetExtraData() );

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        int index = i % NUM_FRAMES_IN_GOP;

        d->Decode( gop[index].frame,
                   gop[index].frameSize );

        c->WriteVideoFrame( e->EncodeYUV420P( d->MakeYUV420P() ), ((i % 15) == 0) ? true : false );
    }

    c->FinalizeFile();
}

void AVMuxerTest::TestReContainerize()
{
    printf("AVMuxerTest::TestReContainerize()\n");
    fflush(stdout);

    struct CodecOptions options;

    options.gop_size = 15;
    options.bit_rate = 1424400; // size of gop * 8 == bit_rate
    options.width = 1280;
    options.height = 720;
    options.time_base_num = 1;
    options.time_base_den = 30;

    XRef<AVMuxer> c = new AVMuxer( options,
                                   "bar.mp4",
                                   AVMuxer::OUTPUT_LOCATION_FILE );

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        int index = i % NUM_FRAMES_IN_GOP;

        c->WriteVideoFrame( gop[index].frame, gop[index].frameSize, ((i % 15) == 0) ? true : false );
    }

    c->FinalizeFile();
}
