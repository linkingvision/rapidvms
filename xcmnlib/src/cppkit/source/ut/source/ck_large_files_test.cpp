
#include "cppkit/os/ck_large_files.h"
#include "cppkit/os/ck_files.h"

#include "ck_large_files_test.h"

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_large_files_test);

static const ck_string ASCII_FILE = "LargeFileTests_ASCII_File.txt";
static const int ASCII_FILE_LEN = strlen( ASCII_FILE.c_str() );
static const ck_string UNICODE_FILE = L"Masta_カメラ_2010-12-21_1652.txt";
static const ck_string PRE_ALLOCATED_FILE = "LargeFileTests_PreAllocatedFile.txt";

void ck_large_files_test::setup()
{
    FILE* f = ck_fopen(ASCII_FILE,WRITE_MODE);
    fwrite(ASCII_FILE.c_str(),ASCII_FILE.length(),1,f);
    fclose(f);
    f = ck_fopen(UNICODE_FILE,WRITE_MODE);
    fwrite(UNICODE_FILE.get_wide_string().data(),UNICODE_FILE.get_wide_string().length(),1,f);
    fclose(f);
    f = ck_fopen( PRE_ALLOCATED_FILE, "w+b" );
    fwrite(UNICODE_FILE.get_wide_string().data(),UNICODE_FILE.get_wide_string().length(),1,f);
    fclose(f);
}

void ck_large_files_test::teardown()
{
#ifdef IS_WINDOWS
    if ( !DeleteFile(UNICODE_FILE.get_wide_string().data()) )
        printf("Failed to delete unicode file\n");
    if ( !DeleteFile(ASCII_FILE.get_wide_string().data()) )
        printf("Failed to delete ascii file\n");
    if ( !DeleteFile(PRE_ALLOCATED_FILE.get_wide_string().data()) )
        printf("Failed to delete pre allocated file\n");
#endif
#ifdef IS_LINUX
    if ( remove(ASCII_FILE.c_str()) != 0)
        printf("Failed to delete ascii file\n");
    if (remove(UNICODE_FILE.c_str()) != 0)
        printf("Failed to delete unicode file\n");
    if (remove(PRE_ALLOCATED_FILE.c_str()) != 0)
        printf("Failed to delete pre allocated file\n");
#endif
}

void ck_large_files_test::test_ascii_file()
{
    static const ck_string ASCII_FILE_false = "LargeFileTests_ASCII_File2.txt";
    ck_file_info info;
    UT_ASSERT( ck_stat(ASCII_FILE,&info) == 0 );
    UT_ASSERT( ck_stat(ASCII_FILE_false,&info) == -1);
}

void ck_large_files_test::test_unicode_file()
{
    static const ck_string UNICODE_FILE_false = L"Masta_カメララ_2010-12-21_1652.txt";
    ck_file_info info;
    UT_ASSERT( ck_stat(UNICODE_FILE,&info) == 0 );
    UT_ASSERT( ck_stat(UNICODE_FILE_false,&info) == -1 );
}

void ck_large_files_test::test_seek_tell()
{
    FILE* f = fopen( ASCII_FILE.c_str(), "r" );
    ck_fseeko( f, 0, SEEK_END );
    int64_t pos = ck_ftello( f );
    UT_ASSERT( pos == ASCII_FILE_LEN );
    fclose( f );
}

void ck_large_files_test::test_pre_allocated_file()
{
    FILE* f = ck_fopen( PRE_ALLOCATED_FILE, "r+b" );
    ck_fallocate( f, (1024*1024) );
    fclose( f );
    ck_file_info info;
    ck_stat(PRE_ALLOCATED_FILE,&info);
    UT_ASSERT( info.file_size == (1024*1024) );
}
