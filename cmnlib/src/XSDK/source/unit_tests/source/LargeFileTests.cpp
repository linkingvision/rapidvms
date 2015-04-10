#include "XSDK/LargeFiles.h"
#include "LargeFileTests.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(LargeFileTests);

static const XString ASCII_FILE = "LargeFileTests_ASCII_File.txt";
static const int ASCII_FILE_LEN = strlen( ASCII_FILE.c_str() );
static const XString UNICODE_FILE = L"Masta_カメラ_2010-12-21_1652.txt";
static const XString PRE_ALLOCATED_FILE = "LargeFileTests_PreAllocatedFile.txt";

void LargeFileTests::setup()
{
    FILE* f = x_fopen(ASCII_FILE,WRITE_MODE);
    fwrite(ASCII_FILE.c_str(),ASCII_FILE.length(),1,f);
    fclose(f);
    f = x_fopen(UNICODE_FILE,WRITE_MODE);
    fwrite(UNICODE_FILE.get_wide_string().data(),UNICODE_FILE.get_wide_string().length(),1,f);
    fclose(f);
    f = x_fopen( PRE_ALLOCATED_FILE, "w+b" );
    fwrite(UNICODE_FILE.get_wide_string().data(),UNICODE_FILE.get_wide_string().length(),1,f);
    fclose(f);
}

void LargeFileTests::teardown()
{
#ifdef WIN32
    if ( !DeleteFile(UNICODE_FILE.get_wide_string().data()) )
        printf("Failed to delete unicode file\n");
    if ( !DeleteFile(ASCII_FILE.get_wide_string().data()) )
        printf("Failed to delete ascii file\n");
    if ( !DeleteFile(PRE_ALLOCATED_FILE.get_wide_string().data()) )
        printf("Failed to delete pre allocated file\n");
#else
    if ( remove(ASCII_FILE.c_str()) != 0)
        printf("Failed to delete ascii file\n");
    if (remove(UNICODE_FILE.c_str()) != 0)
        printf("Failed to delete unicode file\n");
    if (remove(PRE_ALLOCATED_FILE.c_str()) != 0)
        printf("Failed to delete pre allocated file\n");
#endif
}


void LargeFileTests::TestAsciiFile()
{
    static const XString ASCII_FILE_false = "LargeFileTests_ASCII_File2.txt";
    x_file_info info;
    UT_ASSERT( x_stat(ASCII_FILE,&info) == 0 );

    UT_ASSERT( x_stat(ASCII_FILE_false,&info) == -1);
}


void LargeFileTests::TestUnicodeFile()
{
    static const XString UNICODE_FILE_false = L"Masta_カメララ_2010-12-21_1652.txt";
    x_file_info info;
    UT_ASSERT( x_stat(UNICODE_FILE,&info) == 0 );
    UT_ASSERT( x_stat(UNICODE_FILE_false,&info) == -1 );
}

void LargeFileTests::TestSeekTell()
{
    FILE* f = fopen( ASCII_FILE.c_str(), "r" );
    x_fseeko( f, 0, SEEK_END );
    int64_t pos = x_ftello( f );
    UT_ASSERT( pos == ASCII_FILE_LEN );
    fclose( f );
}

void LargeFileTests::TestPreAllocatedFile()
{
    FILE* f = x_fopen( PRE_ALLOCATED_FILE, "r+b" );

    x_fallocate( f, (1024*1024) );

    fclose( f );

    x_file_info info;

    x_stat(PRE_ALLOCATED_FILE,&info);

    UT_ASSERT( info._fileSize == (1024*1024) );
}
