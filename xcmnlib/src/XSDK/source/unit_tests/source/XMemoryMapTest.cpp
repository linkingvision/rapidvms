
#include "XMemoryMapTest.h"
#include "XSDK/XMemoryMap.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XStopWatch.h"

using namespace std;
using namespace XSDK;

static const uint32_t FILE_SIZE = (1024*1024);

REGISTER_TEST_FIXTURE( XMemoryMapTest );

#ifdef WIN32
#define UNLINK(a) _unlink(a)
#else
#define UNLINK(a) unlink(a)
#endif

void XMemoryMapTest::setup()
{
    FILE* outFile = fopen( "data", "w+b" );

    uint8_t buffer[4096];

    uint32_t blocksToWrite = 256;
    while( blocksToWrite > 0 )
    {
        memset( buffer, (uint8_t)(blocksToWrite-1), 4096 );
        blocksToWrite -= fwrite( buffer, 4096, 1, outFile );
    }

    fclose( outFile );
}

void XMemoryMapTest::teardown()
{
    UNLINK( "data" );
}

void XMemoryMapTest::TestFileMapping()
{
    FILE* dataFile = fopen( "data", "r+b" );

    {
        XMemoryMap mm( fileno( dataFile ),
                       0,
                       FILE_SIZE,
                       XMemoryMap::XMM_PROT_READ | XMemoryMap::XMM_PROT_WRITE,
                       XMemoryMap::XMM_TYPE_FILE | XMemoryMap::XMM_SHARED );

        uint8_t* val = (uint8_t*)mm.Map();

        UT_ASSERT( val != NULL );

        uint8_t expectedValue = 255;

        for( int i = 0; i < 256; i++ )
        {
            for( int ii = 0; ii < 4096; ii++ )
            {
                UT_ASSERT( *val++ == expectedValue );
            }

            expectedValue--;
        }
    }

    fclose( dataFile );
}

void XMemoryMapTest::TestPersistToDisk()
{
    // this test maps the data file prepared in setup, and then sets all of its bytes to 0.
    // It then closes the mapping and the file.
    // It then re-opens the file and using fread() verifies that the file contains all 0 now.

    FILE* dataFile = fopen( "data", "r+b" );

    {
        XMemoryMap mm( fileno( dataFile ),
                       0,
                       FILE_SIZE,
                       XMemoryMap::XMM_PROT_READ | XMemoryMap::XMM_PROT_WRITE,
                       XMemoryMap::XMM_TYPE_FILE | XMemoryMap::XMM_SHARED );

        uint8_t* val = (uint8_t*)mm.Map();

        UT_ASSERT( val != NULL );

        for( unsigned int i = 0; i < FILE_SIZE; i++ )
            *val++ = 0;

    }

    fclose( dataFile );

    dataFile = fopen( "data", "r+b" );

    uint8_t buffer[4096];

    uint32_t blocksToRead = 256;

    while( blocksToRead > 0 )
    {
        memset( buffer, 1, 4096 );

        uint32_t blocksJustRead = fread( buffer, 4096, 1, dataFile );

        if( blocksJustRead > 0 )
        {
            blocksToRead -= blocksJustRead;

            for( uint32_t i = 0; i < 4096; i++ )
                UT_ASSERT( buffer[i] == 0 );
        }
    }

    fclose( dataFile );
}
