
#include "XDynamicLibraryTest.h"
#include "XSDK/XException.h"
#include <fstream>

#ifndef WIN32
// Needed for 'system' call
#include <cstdlib>
#endif

using namespace XSDK;
using namespace std;

REGISTER_TEST_FIXTURE(XDynamicLibraryTest);

#if defined WIN32
XSDK::XString VALID_DYNAMIC_LIB = "XSDK.dll";
XSDK::XString INVALID_DYNAMIC_LIB = "invalid.dll";
#else
XSDK::XString VALID_DYNAMIC_LIB = "libXSDK.so";
XSDK::XString INVALID_DYNAMIC_LIB = "invalid.so";
#endif

XSDK::XString VALID_FUNC_NAME = "XSDK_SYM";
XSDK::XString INVALID_FUNC_NAME = "Invalid";

typedef void (*Valid)();
typedef void (*Invalid)(OUT XSDK::XException** ppException);


void XDynamicLibraryTest::setup()
{
}

void XDynamicLibraryTest::teardown()
{
}


void XDynamicLibraryTest::TestLoad()
{
    XDynamicLibrary dl(VALID_DYNAMIC_LIB);

    UT_ASSERT_NO_THROW(dl.Load(VALID_DYNAMIC_LIB));

    UT_ASSERT_NO_THROW(dl.Unload());

    UT_ASSERT_THROWS(dl.Load(INVALID_DYNAMIC_LIB), XSDK::XException);
}

void XDynamicLibraryTest::TestResolveSymbol()
{
    XDynamicLibrary dl(VALID_DYNAMIC_LIB);
    Valid pValidFunc;

    UT_ASSERT_NO_THROW(dl.Load());
    UT_ASSERT_NO_THROW(pValidFunc = (Valid)dl.ResolveSymbol(VALID_FUNC_NAME));
    UT_ASSERT(pValidFunc != 0);
    Invalid pInvalidFunc = (Invalid)dl.ResolveSymbol(INVALID_FUNC_NAME);
    UT_ASSERT( pInvalidFunc == 0);

}
