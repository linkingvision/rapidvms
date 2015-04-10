
#include "XMD5Test.h"
#include "XSDK/XException.h"
#include "XSDK/XString.h"
#include "XSDK/XMD5.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(XMD5Test);

void XMD5Test::setup()
{
}

void XMD5Test::teardown()
{
}

void XMD5Test::TestHashBasic()
{
    XMD5 md5;
    XString msg = "Beneath this mask there is an idea, Mr. Creedy, and ideas are bulletproof.";
    md5.Update( (uint8_t*)msg.c_str(), msg.length() );
    md5.Finalize();
    XString output = md5.GetAsString();
    UT_ASSERT( output == "68cc4c2cbf04714ffd2b4306376410b8" );
}
