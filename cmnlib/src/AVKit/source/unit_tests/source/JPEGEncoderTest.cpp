
#include "JPEGEncoderTest.h"
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

CPPUNIT_TEST_SUITE_REGISTRATION(JPEGEncoderTest);

void JPEGEncoderTest::setUp()
{
    av_register_all();
    Locky::RegisterFFMPEG();

    // pic_0 comes from the above included file pic.c
    _pic = new XMemory;
    memcpy( &_pic->Extend( pic_0_len ), pic_0, pic_0_len );
}

void JPEGEncoderTest::tearDown()
{
    Locky::UnregisterFFMPEG();
}

void JPEGEncoderTest::TestConstructor()
{
    printf("JPEGEncoderTest::TestConstructor()\n");
    fflush(stdout);

    CPPUNIT_ASSERT_NO_THROW( XRef<JPEGEncoder> e = new JPEGEncoder( GetJPEGOptions( 1280, 720 ) ) );
}

void JPEGEncoderTest::TestEncode()
{
    printf("JPEGEncoderTest::TestEncode()\n");
    fflush(stdout);

    XRef<JPEGEncoder> e;
    CPPUNIT_ASSERT_NO_THROW( e = new JPEGEncoder( GetJPEGOptions( 1280, 720 ) ) );

    XIRef<XMemory> jpeg = e->EncodeYUV420P( _pic );

    CPPUNIT_ASSERT( jpeg->GetDataSize() > 0 );

#if 0 // To verify that jpeg contains a valid jpeg, enable this block of code.
    JPEGEncoder::WriteJPEGFile( "out.jpg", jpeg );
#endif
}
