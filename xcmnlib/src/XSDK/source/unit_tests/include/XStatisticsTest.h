
#ifndef _XStatisticsTest_H_
#define _XStatisticsTest_H_

#include "framework.h"

class XStatisticsTest : public test_fixture
{
public:
    TEST_SUITE(XStatisticsTest);
        TEST(XStatisticsTest::TestAverage);
        TEST(XStatisticsTest::TestMedian);
        TEST(XStatisticsTest::TestMaxSamples);
        TEST(XStatisticsTest::TestSampleTimeout);
    TEST_SUITE_END();

    virtual ~XStatisticsTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestAverage();
    void TestMedian();
    void TestMaxSamples();
    void TestSampleTimeout();
};

#endif

