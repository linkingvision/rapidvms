#ifndef URITest_h
#define URITest_h

#include "framework.h"

class URITest : public test_fixture
{
public:
    TEST_SUITE(URITest);
        TEST(URITest::TestDefaultConstructor);
        TEST(URITest::TestConstructor);
        TEST(URITest::TestCopyConstructor);
        TEST(URITest::TestConstructURI2Args);
        TEST(URITest::TestConstructURI3Args);
        TEST(URITest::TestAssignmentOperator);
        TEST(URITest::TestGetFullResourcePath);
        TEST(URITest::TestAddGetArg);
        TEST(URITest::TestClearArgs);
        TEST(URITest::TestSetFullRawURI);
        TEST(URITest::TestSetResource);
        TEST(URITest::TestSetResourcePath);
        TEST(URITest::TestSetGetArgs);
    TEST_SUITE_END();

    void setup() {}
    void teardown() {}

protected:

    void TestDefaultConstructor();
    void TestConstructor();
    void TestCopyConstructor();
    void TestConstructURI2Args();
    void TestConstructURI3Args();
    void TestAssignmentOperator();
    void TestGetFullResourcePath();
    void TestAddGetArg();
    void TestClearArgs();
    void TestSetFullRawURI();
    void TestSetResource();
    void TestSetResourcePath();
    void TestSetGetArgs();
};

#endif
