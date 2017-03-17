
#include "H264DecoderTest.h"
#include "AVKit/H264Decoder.h"
#include "AVKit/JPEGEncoder.h"
#include "AVKit/Options.h"
#include "AVKit/Locky.h"
#include "XSDK/XRef.h"
#include "XSDK/XMemory.h"

extern "C"
{
#include "libavformat/avformat.h"
};

#include "gop.c"

using namespace std;
using namespace XSDK;
using namespace AVKit;

CPPUNIT_TEST_SUITE_REGISTRATION(H264DecoderTest);

void H264DecoderTest::setUp()
{
    av_register_all();
    Locky::RegisterFFMPEG();
}

void H264DecoderTest::tearDown()
{
    Locky::UnregisterFFMPEG();
}

void H264DecoderTest::TestConstructor()
{
    printf("H264DecoderTest::TestConstructor()\n");
    fflush(stdout);

    CPPUNIT_ASSERT_NO_THROW( XRef<H264Decoder> d = new H264Decoder( GetFastH264DecoderOptions() ) );
}

void H264DecoderTest::TestInputDimensions()
{
    printf("H264DecoderTest::TestInputDimensions()\n");
    fflush(stdout);

    XRef<H264Decoder> decoder = new H264Decoder( GetFastH264DecoderOptions() );

    decoder->Decode( gop[0].frame, gop[0].frameSize );

    CPPUNIT_ASSERT( decoder->GetInputWidth() == 1280 );
    CPPUNIT_ASSERT( decoder->GetInputHeight() == 720 );
}

void H264DecoderTest::TestOutputDimensions()
{
    printf("H264DecoderTest::TestOutputDimensions()\n");
    fflush(stdout);

    XRef<H264Decoder> decoder = new H264Decoder( GetFastH264DecoderOptions() );

    decoder->Decode( gop[0].frame, gop[0].frameSize );

    decoder->SetOutputWidth( 640 );
    decoder->SetOutputHeight( 360 );

    size_t picSize = decoder->GetYUV420PSize();

    CPPUNIT_ASSERT( picSize == 345600 );

    XIRef<XMemory> pic;
    CPPUNIT_ASSERT_NO_THROW( pic = decoder->MakeYUV420P() );

    CPPUNIT_ASSERT( pic->GetDataSize() == 345600 );
}

void H264DecoderTest::TestDecodeGOP()
{
    printf("H264DecoderTest::TestDecodeGOP()\n");
    fflush(stdout);

    XRef<H264Decoder> decoder = new H264Decoder( GetFastH264DecoderOptions() );

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        CPPUNIT_ASSERT_NO_THROW( decoder->Decode( gop[i].frame, gop[i].frameSize ) );

        XIRef<XMemory> pic;
        CPPUNIT_ASSERT_NO_THROW( pic = decoder->MakeYUV420P() );

        CPPUNIT_ASSERT( pic->GetDataSize() == 1382400 );
    }
}
