#ifndef XTimerTest_h
#define XTimerTest_h

#include "framework.h"
#include "XSDK/XTimer.h"
#include "XSDK/TimeUtils.h"
#include <list>


class XSimpleTimerTest : public test_fixture
{
public:
    TEST_SUITE(XSimpleTimerTest);
        TEST(XSimpleTimerTest::TestNormal);
        TEST(XSimpleTimerTest::TestLongWait);
        TEST(XSimpleTimerTest::TestStartLater);
        TEST(XSimpleTimerTest::TestNormalNoRepeat);
        TEST(XSimpleTimerTest::TestLongWaitNoRepeat);
        TEST(XSimpleTimerTest::TestStartLaterNoRepeat);
        TEST(XSimpleTimerTest::TestStartNormal);
        TEST(XSimpleTimerTest::TestStartLongWait);
        TEST(XSimpleTimerTest::TestStartStartLater);
        TEST(XSimpleTimerTest::TestStartNormalNoRepeat);
        TEST(XSimpleTimerTest::TestStartLongWaitNoRepeat);
        TEST(XSimpleTimerTest::TestStartStartLaterNoRepeat);
        TEST(XSimpleTimerTest::TestStop);
        TEST(XSimpleTimerTest::TestStopDelay);
        TEST(XSimpleTimerTest::TestRestart);
        TEST(XSimpleTimerTest::TestReset);
    TEST_SUITE_END();

    virtual ~XSimpleTimerTest() throw() {}

protected:

    void TestNormal();
    void TestLongWait();
    void TestStartLater();
    void TestNormalNoRepeat();
    void TestLongWaitNoRepeat();
    void TestStartLaterNoRepeat();
    void TestStartNormal();
    void TestStartLongWait();
    void TestStartStartLater();
    void TestStartNormalNoRepeat();
    void TestStartLongWaitNoRepeat();
    void TestStartStartLaterNoRepeat();
    void TestStop();
    void TestStopDelay();
    void TestRestart();
    void TestReset();
};


class XTimerTest : public test_fixture, public XSDK::XTaskBase
{
public:
    TEST_SUITE(XTimerTest);
        TEST(XTimerTest::TestThreadedTimer);
        TEST(XTimerTest::TestNonThreadedTimer);
    TEST_SUITE_END();

    virtual ~XTimerTest() throw()
    {}

    void setup();
    void teardown();

    // The thread entry point.
    virtual void* EntryPoint();

    // Timer callback function.
    static void cb( void* data );

protected:

    void TestThreadedTimer();
    void TestNonThreadedTimer();

    std::list<struct timeval> _deltas;
    int _fireCount;
    struct timeval _prevTime;
    XSDK::XTimer* _pTimer;

    double _CalcAverage()
    {
        double result = 0.0;

        for(std::list<struct timeval>::iterator iter = _deltas.begin(),
                                                end  = _deltas.end();
            iter != end;
            ++iter)
        {
            result += (double)iter->tv_sec + (double)(iter->tv_usec / 1000000.0);
        }

        return (result / (double)_fireCount);
    }

};

#endif
