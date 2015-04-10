
#include "SoapArgsTest.h"
#include "XSDK/XString.h"
#include "Webby/SoapArgs.h"

using namespace std;
using namespace XSDK;
using namespace WEBBY;
using namespace XSDK;

REGISTER_TEST_FIXTURE(SoapArgsTest);

void SoapArgsTest::setup()
{
}

void SoapArgsTest::teardown()
{
}

void SoapArgsTest::TestConstructor()
{
    SoapArgs args;
}

void SoapArgsTest::TestArgSimple()
{
    SoapArgs args;
    args.AddArg( "foo", "42" );

    XString xml = args.ToXML();

    UT_ASSERT( xml == "<foo>42</foo>" );

}

void SoapArgsTest::TestArgComplex()
{
    SoapArgs args;
    args.AddArg( "foo.bar.a", "42" );
    args.AddArg( "foo.bar.b", "32" );
    args.AddArg( "foo.foobar.a", "22" );
    args.AddArg( "foo.foobar.b", "12" );

    XString xml = args.ToXML();

    UT_ASSERT( xml == "<foo><bar><a>42</a><b>32</b></bar><foobar><a>22</a><b>12</b></foobar></foo>" );
}

void SoapArgsTest::TestMultipleTops()
{
    SoapArgs args;
    args.AddArg( "foo.bar.a", "42" );
    args.AddArg( "foo.bar.b", "32" );
    args.AddArg( "foo.foobar.a", "22" );
    args.AddArg( "foo.foobar.b", "12" );
    args.AddArg( "foobar", "42" );

    XString xml = args.ToXML();

    UT_ASSERT( xml ==
"<foo><bar><a>42</a><b>32</b></bar><foobar><a>22</a><b>12</b></foobar></foo><foobar>42</foobar>" );
}

void SoapArgsTest::TestAttributes()
{
    SoapArgs args;
    args.AddArg( "foo.bar.a", "42" );
    args.AddAttribute("foo.bar.a", "height", "45");
    args.AddAttribute("foo.bar.a", "width", "57");
    args.AddArg( "foo.bar.b", "32" );
    args.AddAttribute("foo.foobar.a", "depth", "none");
    args.AddArg( "foo.foobar.a", "22" );
    args.AddArg( "foo.foobar.b", "12" );
    args.AddArg( "foobar", "42" );

    XString xml = args.ToXML();

    UT_ASSERT( xml == "<foo><bar><a height=\"45\" width=\"57\">42</a><b>32</b></bar><foobar><a depth=\"none\">22</a><b>12</b></foobar></foo><foobar>42</foobar>" );
}

