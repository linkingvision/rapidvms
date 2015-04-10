#ifndef XTimedCommandQueueTest_h
#define XTimedCommandQueueTest_h

#include "framework.h"
#include "XSDK/XGuard.h"
#include "XSDK/XCondition.h"
#include "XSDK/XTimedCommandQueue.h"

extern int _globalInt;
extern XSDK::XMutex _processLock;
extern XSDK::XCondition _processCond;

class SimpleTimedCommandQueue : public XSDK::XTimedCommandQueue<int>
{
 public:
    SimpleTimedCommandQueue();

    virtual ~SimpleTimedCommandQueue() throw();

    virtual bool Process( int type );

    static void TimerCallback( void* context, int id );
};

class XTimedCommandQueueTest : public test_fixture
{
public:
    TEST_SUITE(XTimedCommandQueueTest);
        TEST(XTimedCommandQueueTest::TestCreateDestroy);
        TEST(XTimedCommandQueueTest::TestRoundTripCommand);
        TEST(XTimedCommandQueueTest::TestLots);
        TEST(XTimedCommandQueueTest::TestOneSecondTimer);
        TEST(XTimedCommandQueueTest::TestMultipleTimers);
        TEST(XTimedCommandQueueTest::TestCancelTimer);
        TEST(XTimedCommandQueueTest::TestRepeatingTimer);
        TEST(XTimedCommandQueueTest::TestUltimate);
        TEST(XTimedCommandQueueTest::TestAccessContext);
    TEST_SUITE_END();

    virtual ~XTimedCommandQueueTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestCreateDestroy();
    void TestRoundTripCommand();
    void TestLots();

    void TestOneSecondTimer();
    void TestMultipleTimers();
    void TestCancelTimer();
    void TestRepeatingTimer();

    void TestAccessContext();

    void TestUltimate();

    void TestCancelWhileTimerCallback();

private:

    SimpleTimedCommandQueue _q;

};


#endif
