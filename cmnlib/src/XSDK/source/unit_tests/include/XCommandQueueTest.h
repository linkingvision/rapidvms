#ifndef XCommandQueueTest_h
#define XCommandQueueTest_h

#include "framework.h"

#include "XSDK/XCommandQueue.h"

class SimpleCommandQueue : public XSDK::XCommandQueue<int>
{
 public:
    SimpleCommandQueue();
    virtual ~SimpleCommandQueue() throw();

    virtual bool Process( int type );
};

class PriorityCommandQueue : public XSDK::XCommandQueue<int>
{
 public:
    PriorityCommandQueue();
    virtual ~PriorityCommandQueue() throw();

    virtual bool Process( int type );

    virtual int Compare( const int& a, const int& b );
};

class XCommandQueueTest : public test_fixture
{
public:
    TEST_SUITE(XCommandQueueTest);
        TEST(XCommandQueueTest::TestCreateDestroy);
        TEST(XCommandQueueTest::TestRoundTripCommand);
        TEST(XCommandQueueTest::TestLots);
        TEST(XCommandQueueTest::TestPriorityCommandQueue);
    TEST_SUITE_END();

    virtual ~XCommandQueueTest() throw()
    {}

    void setup() {}
    void teardown() {}


protected:

    void TestCreateDestroy();
    void TestRoundTripCommand();
    void TestLots();

    void TestPriorityCommandQueue();

private:

    SimpleCommandQueue _q;
    PriorityCommandQueue _pq;

};

#endif
