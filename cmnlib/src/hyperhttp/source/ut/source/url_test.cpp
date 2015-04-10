
#include "url_test.h"
#include "hyperhttp/url.h"
#include <map>

using namespace std;
using namespace cppkit;
using namespace hyperhttp;

REGISTER_TEST_FIXTURE(url_test);

void url_test::test_construct()
{
    {
        url u;
    }

    {
        uri uari;
        uari.set_full_raw_uri( "/foo/bar?id=1234&width=1280&height=720" );

        ck_nullable<int> port = 10010;
        url u( "http", "127.0.0.1", port, uari );

        UT_ASSERT( u.get_full_raw_url() == "http://127.0.0.1:10010/foo/bar?height=720&id=1234&width=1280" );

        UT_ASSERT( u.get_protocol() == "http" );
        UT_ASSERT( u.get_ip() == "127.0.0.1" );
        UT_ASSERT( u.get_port_num().value() == 10010 );

        u.clear_get_args();

        UT_ASSERT( u.get_full_raw_url() == "http://127.0.0.1:10010/foo/bar" );
    }
}

void url_test::test_setters()
{
    url u;
    u.set_protocol( "http" );
    u.set_ip( "127.0.0.1" );
    u.set_port_num( 10010 );
    u.set_uri( uri::construct_uri( "/foo/bar/baz", "thing" ) );
    u.add_get_arg( "id", "1234" );
    u.add_get_arg( "width", "1280" );
    u.add_get_arg( "height", "720" );

    UT_ASSERT( u.get_full_raw_url().starts_with( "http://127.0.0.1:10010/foo/bar/baz/thing?" ) );
    UT_ASSERT( u.get_full_raw_url().contains( "id=1234" ) );
    UT_ASSERT( u.get_full_raw_url().contains( "width=1280" ) );
    UT_ASSERT( u.get_full_raw_url().contains( "height=720" ) );
}
