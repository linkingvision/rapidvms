
#include "ck_path_test.h"
#include "cppkit/ck_path.h"

#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_path_test);

#ifdef WIN32
#include <direct.h>
#define RMDIR _rmdir
#define UNLINK _unlink
#define MKDIR(a) _mkdir(a)
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define RMDIR rmdir
#define UNLINK unlink
#define MKDIR(a) mkdir(a,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif

void ck_path_test::setup()
{
#ifdef IS_WINDOWS
    _mkdir( "foo" );
    FILE* regularFile = fopen("foo\\bar","w");
    _mkdir( "foobar" );
    _wmkdir(L"上开花");
    FILE* unicodeFile = _wfopen(L"上开花\\Чернобыльской",L"w");
    _wmkdir(L"走上");
#else
    mkdir( "foo", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
    FILE* regularFile = fopen("foo/bar","w");
    mkdir( "foobar", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );

    mkdir("上开花", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
    FILE* unicodeFile = fopen("上开花/Чернобыльской","w");
    mkdir("走上", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
#endif

    unsigned char buf[128];
    memset( buf, 0, 128 );
    fwrite( buf, 1, 128, regularFile );
    fwrite( buf, 1, 128, unicodeFile );
    fclose( regularFile );
    fclose( unicodeFile );
}

void ck_path_test::teardown()
{
#ifdef IS_WINDOWS
    UNLINK( "foo\\bar" );
    _wunlink(L"上开花\\Чернобыльской");
    _wrmdir(L"上开花");
    _wrmdir(L"走上");
#else
    UNLINK( "foo/bar" );
    UNLINK("上开花/Чернобыльской");
    RMDIR("上开花");
    RMDIR("走上");
#endif
    RMDIR( "foo" );
    RMDIR( "foobar" );
}

void ck_path_test::test_constructor()
{
    ck_path path( "foo" );
    ck_path path2( L"上开花" );
}

void ck_path_test::test_open_path()
{
    {
        ck_path path( "foo" );

        ck_string fileName;
        bool ok = path.read_dir( fileName );

        UT_ASSERT( fileName == "bar" );
    }
    {
        ck_path path( L"上开花" );

        ck_string fileName;
        bool ok = path.read_dir( fileName );

        UT_ASSERT( fileName == L"Чернобыльской" );
    }
}

void ck_path_test::test_is_path()
{
    UT_ASSERT( ck_path::is_dir( "foo" ) == true );
    UT_ASSERT( ck_path::is_dir( L"上开花" ) == true );
#ifdef IS_WINDOWS
    UT_ASSERT( ck_path::is_dir( "foo\\bar" ) == false );
    UT_ASSERT( ck_path::is_dir( L"上开花\\Чернобыльской" ) == false );
#else
    UT_ASSERT( ck_path::is_dir( "foo/bar" ) == false );
    UT_ASSERT( ck_path::is_dir( L"上开花/Чернобыльской" ) == false );
#endif
}

void ck_path_test::test_is_reg()
{
    UT_ASSERT( ck_path::is_reg( "foo" ) == false );
    UT_ASSERT( ck_path::is_reg( L"上开花" ) == false );
#ifdef IS_WINDOWS
    UT_ASSERT( ck_path::is_reg( "foo\\bar" ) == true );
    UT_ASSERT( ck_path::is_reg( L"上开花\\Чернобыльской" ) == true );
#else
    UT_ASSERT( ck_path::is_reg( "foo/bar" ) == true );
    UT_ASSERT( ck_path::is_reg( L"上开花/Чернобыльской" ) == true );
#endif
}

void ck_path_test::test_exists()
{
    UT_ASSERT( ck_path::exists( "FileDoesNotExist.txt" ) == false );
    UT_ASSERT( ck_path::exists( "SomeFile.dat" ) == false );
#ifdef IS_WINDOWS
    UT_ASSERT( ck_path::exists( "blah\\foo" ) == false );
    UT_ASSERT( ck_path::exists( L"花\\ыльской" ) == false );
#else
    UT_ASSERT( ck_path::exists( "blah/foo" ) == false );
    UT_ASSERT( ck_path::exists( L"花/Черноской" ) == false );
#endif

    UT_ASSERT( ck_path::exists( "foo" ) == true );
    UT_ASSERT( ck_path::exists( L"上开花" ) == true );
#ifdef IS_WINDOWS
    UT_ASSERT( ck_path::exists( "foo\\bar" ) == true );
    UT_ASSERT( ck_path::exists( L"上开花\\Чернобыльской" ) == true );
#else
    UT_ASSERT( ck_path::exists( "foo/bar" ) == true );
    UT_ASSERT( ck_path::exists( L"上开花/Чернобыльской" ) == true );
#endif
}

void ck_path_test::test_iter()
{

    {
        ck_path path( "foo" );

        ck_string fileName;
        int i = 0;
        for( ; path.read_dir( fileName ); ++i );

        UT_ASSERT( i == 1 );
    }

    {
        ck_path path( "foo" );

        ck_string fileName;
        path.read_dir( fileName );

        UT_ASSERT( fileName == "bar" );
    }

    {
        ck_path path( L"上开花" );

        ck_string fileName;
        int i = 0;
        for( ; path.read_dir( fileName ); ++i );

        UT_ASSERT( i == 1 );
    }
    {
        ck_path path( L"上开花" );

        ck_string fileName;
        path.read_dir( fileName );

        UT_ASSERT( fileName == L"Чернобыльской" );
    }
}

void ck_path_test::test_file_size()
{
    MKDIR( "fs_root" );
    MKDIR( "fs_root" PATH_SLASH "a" );
    MKDIR( "fs_root" PATH_SLASH "b" );
    MKDIR( "fs_root" PATH_SLASH "c" );
    MKDIR( "fs_root" PATH_SLASH "d" );

    FILE* f = fopen( "fs_root" PATH_SLASH "reg", "wb" );
    uint8_t buf[1024];
    memset(buf, 0, sizeof(buf));
    fwrite( &buf, 1, 1024, f );
    fclose( f );

    f = fopen( "fs_root" PATH_SLASH "a" PATH_SLASH "reg", "wb" );
    fwrite( &buf, 1, 1024, f );
    fclose( f );

    UT_ASSERT( ck_path::file_size( "fs_root" ) == 2048 );

    UNLINK( "fs_root" PATH_SLASH "a" PATH_SLASH "reg" );
    UNLINK( "fs_root" PATH_SLASH "reg" );

    RMDIR( "fs_root" PATH_SLASH "a" );
    RMDIR( "fs_root" PATH_SLASH "b" );
    RMDIR( "fs_root" PATH_SLASH "c" );
    RMDIR( "fs_root" PATH_SLASH "d" );
    RMDIR( "fs_root" );
}
