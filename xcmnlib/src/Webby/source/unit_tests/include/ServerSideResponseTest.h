#ifndef _WEBBY_SERVERSIDE_RESPONSE_TEST_H_
#define _WEBBY_SERVERSIDE_RESPONSE_TEST_H_

#include "framework.h"
#include "XSDK/XTaskBase.h"

class ResponseTest : public test_fixture, public XSDK::TaskBase
{
public:

    TEST_SUITE(ResponseTest);
        TEST(ResponseTest::TestDefaultConstructor);
        TEST(ResponseTest::TestCopyConstructor);
        TEST(ResponseTest::TestAssignmentOperator);
        TEST(ResponseTest::TestWriteResponse);
        TEST(ResponseTest::TestSuccess);
        TEST(ResponseTest::TestFailure);
        TEST(ResponseTest::TestNVPairs);
        TEST(ResponseTest::TestSetBody);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:

    virtual void* EntryPoint();

    void TestDefaultConstructor();
    void TestCopyConstructor();
    void TestAssignmentOperator();
    void TestWriteResponse();
    void TestSuccess();
    void TestFailure();
    void TestNVPairs();
    void TestAddNVPairs();
    void TestEndNVPairs();
    void TestSetBody();

 private:

    int _testNum;
};

#endif
