#ifndef XMonoClockTest_h
#define XMonoClockTest_h

#include "framework.h"

class XMonoClockTest : public test_fixture
{
public:
    TEST_SUITE(XMonoClockTest);
        TEST(XMonoClockTest::TestGetTime);
        TEST(XMonoClockTest::TestGetFrequency);
        TEST(XMonoClockTest::TestGetElapsedTime);
    TEST_SUITE_END();

    virtual ~XMonoClockTest() throw()
    {}

    void setup();
    void teardown();

protected:

    void TestGetTime();
    void TestGetFrequency();
    void TestGetElapsedTime();
};

#endif
