#include "XPathTest.h"
#include "XSDK/XPath.h"

#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XPathTest);

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

void XPathTest::setup()
{
#ifdef WIN32
    _mkdir( "foo" );
    FILE* regularFile = fopen("foo\\bar","w");
    _mkdir( "foobar" );
    _wmkdir(L"树上开花");
    FILE* unicodeFile = _wfopen(L"树上开花\\Чернобыльской",L"w");
    _wmkdir(L"走为上");
#else
    mkdir( "foo", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
    FILE* regularFile = fopen("foo/bar","w");
    mkdir( "foobar", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );

    mkdir("树上开花", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
    FILE* unicodeFile = fopen("树上开花/Чернобыльской","w");
    mkdir("走为上", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
#endif

    unsigned char buf[128];
    memset( buf, 0, 128 );
    fwrite( buf, 1, 128, regularFile );
    fwrite( buf, 1, 128, unicodeFile );
    fclose( regularFile );
    fclose( unicodeFile );
}

void XPathTest::teardown()
{
#ifdef WIN32
    UNLINK( "foo\\bar" );
    _wunlink(L"树上开花\\Чернобыльской");
    _wrmdir(L"树上开花");
    _wrmdir(L"走为上");
#else
    UNLINK( "foo/bar" );
    UNLINK("树上开花/Чернобыльской");
    RMDIR("树上开花");
    RMDIR("走为上");
#endif
    RMDIR( "foo" );
    RMDIR( "foobar" );
}

void XPathTest::TestConstructor()
{
    XPath path( "foo" );
    XPath path2( L"树上开花" );
}

void XPathTest::TestCopyConstructor()
{
    XPath path( "foo" );
    XPath path2( path );

    XPath path3( L"树上开花" );
    XPath path4( path3 );
}

void XPathTest::TestAssignmentOperator()
{
    {
        XPath path( "foo" );
        XPath path2( "foobar" );
        path2 = path;

        UT_ASSERT( path2.GetElemCount() == path.GetElemCount() );
    }
    {
        XPath path( L"走为上" );
        XPath path2( L"走为上" );
        path2 = path;

        UT_ASSERT( path2.GetElemCount() == path.GetElemCount() );
    }
}

void XPathTest::TestOpenPath()
{
    {
        XPath path( "foo" );

        path.IterStart();

        UT_ASSERT( *path.IterData() == "bar" );
    }
    {
        XPath path( L"树上开花" );

        path.IterStart();

        UT_ASSERT( *path.IterData() == L"Чернобыльской" );
    }
}

void XPathTest::TestIsPath()
{
    UT_ASSERT( XPath::IsDir( "foo" ) == true );
    UT_ASSERT( XPath::IsDir( L"树上开花" ) == true );
#ifdef WIN32
    UT_ASSERT( XPath::IsDir( "foo\\bar" ) == false );
    UT_ASSERT( XPath::IsDir( L"树上开花\\Чернобыльской" ) == false );
#else
    UT_ASSERT( XPath::IsDir( "foo/bar" ) == false );
    UT_ASSERT( XPath::IsDir( L"树上开花/Чернобыльской" ) == false );
#endif
}

void XPathTest::TestIsReg()
{
    UT_ASSERT( XPath::IsReg( "foo" ) == false );
    UT_ASSERT( XPath::IsReg( L"树上开花" ) == false );
#ifdef WIN32
    UT_ASSERT( XPath::IsReg( "foo\\bar" ) == true );
    UT_ASSERT( XPath::IsReg( L"树上开花\\Чернобыльской" ) == true );
#else
    UT_ASSERT( XPath::IsReg( "foo/bar" ) == true );
    UT_ASSERT( XPath::IsReg( L"树上开花/Чернобыльской" ) == true );
#endif
}

void XPathTest::TestExists()
{
    UT_ASSERT( XPath::Exists( "FileDoesNotExist.txt" ) == false );
    UT_ASSERT( XPath::Exists( "SomeFile.dat" ) == false );
#ifdef WIN32
    UT_ASSERT( XPath::Exists( "blah\\foo" ) == false );
    UT_ASSERT( XPath::Exists( L"树花\\ыльской" ) == false );
#else
    UT_ASSERT( XPath::Exists( "blah/foo" ) == false );
    UT_ASSERT( XPath::Exists( L"树花/Черноской" ) == false );
#endif

    UT_ASSERT( XPath::Exists( "foo" ) == true );
    UT_ASSERT( XPath::Exists( L"树上开花" ) == true );
#ifdef WIN32
    UT_ASSERT( XPath::Exists( "foo\\bar" ) == true );
    UT_ASSERT( XPath::Exists( L"树上开花\\Чернобыльской" ) == true );
#else
    UT_ASSERT( XPath::Exists( "foo/bar" ) == true );
    UT_ASSERT( XPath::Exists( L"树上开花/Чернобыльской" ) == true );
#endif
}

void XPathTest::TestClear()
{
    XPath path( "foo" );

    path.Clear();

    UT_ASSERT( path.GetElemCount() == 0 );
}

void XPathTest::TestIter()
{
    {
        XPath path( "foo" );

        int i = 0;
        for ( path.IterStart();path.IterValid(); ++i,path.Iterate());

        UT_ASSERT( i == 1 );
        path.IterStart();
        UT_ASSERT( *path.IterData() == "bar" );
    }
    {
        XPath path( L"树上开花" );

        int i = 0;
        for ( path.IterStart();path.IterValid(); ++i,path.Iterate());

        UT_ASSERT( i == 1 );
        path.IterStart();
        UT_ASSERT( *path.IterData() == L"Чернобыльской" );
    }
}

void XPathTest::TestFileSize()
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

    UT_ASSERT( XPath::FileSize( "fs_root" ) == 2048 );

    UNLINK( "fs_root" PATH_SLASH "a" PATH_SLASH "reg" );
    UNLINK( "fs_root" PATH_SLASH "reg" );

    RMDIR( "fs_root" PATH_SLASH "a" );
    RMDIR( "fs_root" PATH_SLASH "b" );
    RMDIR( "fs_root" PATH_SLASH "c" );
    RMDIR( "fs_root" PATH_SLASH "d" );
    RMDIR( "fs_root" );
}

