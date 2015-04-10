#ifndef XDynamicLibraryTest_h
#define XDynamicLibraryTest_h

#include "framework.h"

#include "XSDK/XDynamicLibrary.h"

class XDynamicLibraryTest : public test_fixture
{
public:
    TEST_SUITE(XDynamicLibraryTest);
        TEST(XDynamicLibraryTest::TestLoad);
        TEST(XDynamicLibraryTest::TestResolveSymbol);
    TEST_SUITE_END();

    virtual ~XDynamicLibraryTest() throw()
    {}

    void setup();
    void teardown();

protected:

    void TestLoad();
    void TestResolveSymbol();

};

#endif
