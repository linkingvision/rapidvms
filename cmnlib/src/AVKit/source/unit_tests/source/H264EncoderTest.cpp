
#include "H264EncoderTest.h"
#include "AVKit/H264Encoder.h"
#include "AVKit/H264Decoder.h"
#include "AVKit/JPEGEncoder.h"
#include "AVKit/Locky.h"
#include "AVKit/Options.h"
#include "XSDK/XRef.h"
#include "XSDK/XMemory.h"

extern "C"
{
#include "libavformat/avformat.h"
};

#include "gop.c"
#include "pic.c"

using namespace std;
using namespace XSDK;
using namespace AVKit;

CPPUNIT_TEST_SUITE_REGISTRATION(H264EncoderTest);

void H264EncoderTest::setUp()
{
    av_register_all();
    Locky::RegisterFFMPEG();

    // pic_0 comes from the above included file pic.c
    _pic = new XMemory;
    memcpy( &_pic->Extend( pic_0_len ), pic_0, pic_0_len );
}

void H264EncoderTest::tearDown()
{
    Locky::UnregisterFFMPEG();
}

void H264EncoderTest::TestConstructor()
{
    printf("H264EncoderTest::TestConstructor()\n");
    fflush(stdout);

    CPPUNIT_ASSERT_NO_THROW( XRef<H264Encoder> e = new H264Encoder( GetFastH264EncoderOptions( 500000, 1280, 720, 15, 1, 15 ) ) );
}

void H264EncoderTest::TestEncodeKey()
{
    printf("H264EncoderTest::TestEncodeKey()\n");
    fflush(stdout);

    XRef<H264Encoder> e;
    CPPUNIT_ASSERT_NO_THROW( e = new H264Encoder( GetFastH264EncoderOptions( 500000, 1280, 720, 15, 1, 15 ) ) );

    XIRef<XMemory> output;
    CPPUNIT_ASSERT_NO_THROW( output = e->EncodeYUV420P( _pic, H264Encoder::FRAME_TYPE_KEY ) );

    CPPUNIT_ASSERT( output->GetDataSize() > 0 );
}

void H264EncoderTest::TestEncodeGOP()
{
    printf("H264EncoderTest::TestEncodeGOP()\n");
    fflush(stdout);

    XRef<H264Decoder> d;
    CPPUNIT_ASSERT_NO_THROW( d = new H264Decoder( GetFastH264DecoderOptions() ) );

    d->SetOutputWidth( 640 );
    d->SetOutputHeight( 360 );

    XRef<H264Encoder> e;
    CPPUNIT_ASSERT_NO_THROW( e = new H264Encoder( GetFastH264EncoderOptions( 500000, 640, 360, 15, 1, 5 ) ) );

    list<XIRef<XMemory> > outputFrames;

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        int index = i % NUM_FRAMES_IN_GOP;
        d->Decode( gop[index].frame, gop[index].frameSize );

        outputFrames.push_back( e->EncodeYUV420P( d->MakeYUV420P() ) );
    }

    CPPUNIT_ASSERT_NO_THROW( d = new H264Decoder( GetFastH264DecoderOptions() ) );

    d->SetOutputWidth( 640 );
    d->SetOutputHeight( 360 );

    list<XIRef<XMemory> >::iterator i;
    for( i = outputFrames.begin(); i != outputFrames.end(); i++ )
    {
        XIRef<XMemory> frame = *i;
        d->Decode( frame->Map(), frame->GetDataSize() );

        XIRef<XMemory> pic = new XMemory;
        CPPUNIT_ASSERT_NO_THROW( pic = d->MakeYUV420P() );

#if 0
        XRef<JPEGEncoder> e = new JPEGEncoder( GetJPEGOptions( 640, 360 ) );

        JPEGEncoder::WriteJPEGFile( "out2.jpg",
                                    e->EncodeYUV420P( pic ) );
#endif
    }
}
