
#ifndef _BLOCKING_QUEUE_TEST_H_
#define _BLOCKING_QUEUE_TEST_H_

#include "framework.h"
#include "XSDK/XBlockingQueue.h"
#include "XSDK/XTaskBase.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XCommandQueue.h"

enum COMMAND
{
    PUSH,
    POP,
    BLOCKING_PEEK_POP,
    CANCEL
};

template<class T>
struct TestCommand
{
    TestCommand(void) :
        val(),cmd()
    {
    }
    TestCommand(T aval,COMMAND acmd) :
        val(aval),cmd(acmd)
    {
    }

    T val;
    COMMAND cmd;
};

template<class T>
class TestHelper : public XSDK::XCommandQueue<TestCommand<T> >
{
public:
    TestHelper(void) :
        XSDK::XCommandQueue<TestCommand<T> >(),
        _blockingQueue(0)
    {
    }
    void SetBlockingQueue(XSDK::XBlockingQueue<T> & blockingQueue)
    {
        _blockingQueue = &blockingQueue;
    }
    bool Process(TestCommand<T> elem)
    {
        switch(elem.cmd)
        {
        case PUSH:
        {
            _blockingQueue->Push(elem.val);
            break;
        }
        case POP:
        {
            if(_blockingQueue->NonBlockingPeek())
            {
                UT_ASSERT(elem.val == _blockingQueue->Pop());
            }
        }
        break;
        case BLOCKING_PEEK_POP:
        {
            UT_ASSERT(_blockingQueue->BlockingPeek() == true);
            UT_ASSERT(elem.val == _blockingQueue->Pop());
        }
        break;
        case CANCEL:
        {
            XSDK::x_sleep((unsigned int)elem.val);
            _blockingQueue->Cancel();
        }
        break;
        default:
            UT_ASSERT(false);
        };

        return true;
    }
private:
    XSDK::XBlockingQueue<T> * _blockingQueue;
};

class XBlockingQueueTest : public test_fixture
{
public:
    TEST_SUITE(XBlockingQueueTest);
        TEST(XBlockingQueueTest::TestCreateDestroy);
        TEST(XBlockingQueueTest::TestPushPop);
        TEST(XBlockingQueueTest::TestNonBlockingPeek);
        TEST(XBlockingQueueTest::TestBlockingPeek);
        TEST(XBlockingQueueTest::TestCancel);
        TEST(XBlockingQueueTest::BreakIt);
    TEST_SUITE_END();

    virtual ~XBlockingQueueTest(void) throw()
    {
    }

    void setup(void)
    {
        _testHelper1.Startup();
        _testHelper2.Startup();
        _testHelper3.Startup();
    }

    void teardown(void)
    {
        _testHelper1.Shutdown();
        _testHelper2.Shutdown();
        _testHelper3.Shutdown();
    }

protected:
    void TestCreateDestroy(void);
    void TestPushPop(void);
    void TestNonBlockingPeek(void);
    void TestBlockingPeek(void);
    void TestCancel(void);
    void BreakIt(void);
private:
    TestHelper<int> _testHelper1;
    TestHelper<double> _testHelper2;
    TestHelper<double> _testHelper3;

    template<class T,class S>
    void _Wait(T & bq1,S & bq2)
    {
        //Make sure the test helpers are finished
        while(bq1.NonBlockingPeek() ||
                bq2.NonBlockingPeek())
            XSDK::x_usleep(100);
    }
};

#endif
