#ifndef XStopWatchTest_h
#define XStopWatchTest_h

#include "framework.h"

class XStopWatchTest : public test_fixture
{
public:
    TEST_SUITE(XStopWatchTest);
        TEST(XStopWatchTest::TestTheBasics);
        TEST(XStopWatchTest::TestAssignment);
        TEST(XStopWatchTest::TestAccumulation);
        TEST(XStopWatchTest::TestEdgeCases);
        TEST(XStopWatchTest::TestRestart);
    TEST_SUITE_END();

    virtual ~XStopWatchTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestTheBasics();
    void TestAssignment();
    void TestAccumulation();
    void TestEdgeCases();
    void TestRestart();
};

#endif
