
#ifndef _XSDK_OBJECT_TEST_H_
#define _XSDK_OBJECT_TEST_H_

#include "framework.h"

//=============================================================================
// This class represents a set of tests called a test suite. Each test is
// contained in a seperate member function within the object.
//=============================================================================
class XBaseObjectTest : public test_fixture
{
public:
    //=========================================================================
    // These macros are provided by CppUnit to automatically create the test
    // suite and register each test.
    //=========================================================================
    TEST_SUITE(XBaseObjectTest);
        TEST(XBaseObjectTest::TestConstruction);
        TEST(XBaseObjectTest::TestDestruction);
        TEST(XBaseObjectTest::TestSetGetObjectName);
        TEST(XBaseObjectTest::TestReferenceCounting);
        TEST(XBaseObjectTest::TestDoubleDeleteMPF1195);
        TEST(XBaseObjectTest::TestQueryInterface);
        TEST(XBaseObjectTest::TestGetClassId);
    TEST_SUITE_END();

    //=========================================================================
    // This method is called by the test fixture before any tests are run.
    //=========================================================================
    void setup();

    //=========================================================================
    // This method is called by the test fixture after all tests have
    // completed.
    //=========================================================================
    void teardown();

 protected:

    void TestConstruction();
    void TestDestruction();
    void TestSetGetObjectName();
    void TestReferenceCounting();
    void TestDoubleDeleteMPF1195();
    void TestQueryInterface();

    void TestGetClassId();

};

#endif // _XSDK_OBJECT_TEST_H_

