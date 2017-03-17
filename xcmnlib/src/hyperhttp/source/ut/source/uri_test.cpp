
#include "uri_test.h"
#include "hyperhttp/uri.h"
#include <map>

using namespace std;
using namespace cppkit;
using namespace hyperhttp;

REGISTER_TEST_FIXTURE(uri_test);

void uri_test::test_construct_uri()
{
    map<string,ck_string> args;
    args.insert( make_pair( "data_source_id", "df268f87-05e1-4901-b632-a20f02a048a9" ) );
    args.insert( make_pair( "width", "1280" ) );
    args.insert( make_pair( "height", "720" ) );

    uri u = uri::construct_uri( "/foo/bar/baz", "thing", args );

    UT_ASSERT( u.get_full_raw_uri() == "/foo/bar/baz/thing?data_source_id=df268f87-05e1-4901-b632-a20f02a048a9&height=720&width=1280" );
}

void uri_test::test_set_full_raw_uri()
{
    uri u;
    u.set_full_raw_uri( "/foo/bar?id=1234&width=1280&height=720" );

    UT_ASSERT( u.get_resource() == "bar" );
    UT_ASSERT( u.get_resource_path() == "/foo/" );

    map<string,ck_string> args = u.get_get_args();

    auto found = args.find( "width" );
    UT_ASSERT( found != args.end() );

    UT_ASSERT( (*found).first == "width" );
    UT_ASSERT( (*found).second == "1280" );

    found = args.find( "height" );
    UT_ASSERT( found != args.end() );

    UT_ASSERT( (*found).first == "height" );
    UT_ASSERT( (*found).second == "720" );
}
