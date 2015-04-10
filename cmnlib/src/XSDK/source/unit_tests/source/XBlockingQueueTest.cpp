#include "XBlockingQueueTest.h"
#include <cmath>
#include <ctime>
#include <vector>

using namespace XSDK;
using namespace std;

REGISTER_TEST_FIXTURE(XBlockingQueueTest);

void XBlockingQueueTest::TestCreateDestroy(void)
{
    XBlockingQueue<int> blockingQueueStack;
    XBlockingQueue<double> * blockingQueueHeap = new XBlockingQueue<double>;
    delete blockingQueueHeap;
}

void XBlockingQueueTest::TestPushPop(void)
{
    static const unsigned int iterations = 10;
    static const unsigned int volume = 4;

    srand(time(NULL));

    std::vector<int> randInts;
    std::vector<double> randDoubles;

    XBlockingQueue<int> blockingQueue1;
    XBlockingQueue<double> blockingQueue2;

    _testHelper1.SetBlockingQueue(blockingQueue1);
    _testHelper2.SetBlockingQueue(blockingQueue2);
    _testHelper3.SetBlockingQueue(blockingQueue2);

    for(unsigned int i = 0;i < iterations;++i)
    {
        for(unsigned j = 0;j < volume;++j)
        {
            randInts.push_back(rand() % 1000);
            randDoubles.push_back((rand() % 1000) * ((rand() % 1000) / 1000));
            _testHelper1.Post(TestCommand<int>(randInts[j],PUSH));
            _testHelper2.Post(TestCommand<double>(randDoubles[j],PUSH));
            _testHelper3.Post(TestCommand<double>(randDoubles[j],PUSH));
        }
        for(unsigned j = 0;j < volume;++j)
        {
            UT_ASSERT(blockingQueue1.Pop() == randInts[j]);
            UT_ASSERT(blockingQueue2.Pop() == randDoubles[j]);
            UT_ASSERT(blockingQueue2.Pop() == randDoubles[j]);
        }
        randInts.clear();
        randDoubles.clear();
    }

    UT_ASSERT(blockingQueue1.NonBlockingPeek() == false);
    UT_ASSERT(blockingQueue2.NonBlockingPeek() == false);

    for(unsigned int i = 0;i < iterations;++i)
    {
        for(unsigned j = 0;j < volume;++j)
        {
            randInts.push_back(rand() % 1000);
            randDoubles.push_back((rand() % 1000) * ((rand() % 1000) / 1000));
            blockingQueue1.Push(randInts[j]);
            blockingQueue2.Push(randDoubles[j]);
        }
        for(unsigned j = 0;j < volume;++j)
        {
            _testHelper1.Post(TestCommand<int>(randInts[j],POP));
            _testHelper2.Post(TestCommand<double>(randDoubles[j],POP));
        }
        randInts.clear();
        randDoubles.clear();
    }

    //Wait for the test helpers to finish
    _Wait(blockingQueue1,blockingQueue2);
}


void XBlockingQueueTest::TestNonBlockingPeek(void)
{
    static const unsigned int iterations = 1;
    static const unsigned int volume = 2;

    srand(time(NULL));

    std::vector<int> randInts;
    std::vector<double> randDoubles;

    XBlockingQueue<int> blockingQueue1;
    XBlockingQueue<double> blockingQueue2;

    _testHelper1.SetBlockingQueue(blockingQueue1);
    _testHelper2.SetBlockingQueue(blockingQueue2);

    UT_ASSERT(blockingQueue1.NonBlockingPeek() == false);
    UT_ASSERT(blockingQueue2.NonBlockingPeek() == false);

    for(unsigned int i = 0;i < iterations;++i)
    {
        for(unsigned j = 0;j < volume;++j)
        {
            randInts.push_back(rand() % 1000);
            randDoubles.push_back((rand() % 1000) * ((rand() % 1000) / 1000));
            _testHelper1.Post(TestCommand<int>(randInts[j],PUSH));
            _testHelper2.Post(TestCommand<double>(randDoubles[j],PUSH));
        }
        for(unsigned j = 0;j < volume;++j)
        {
            UT_ASSERT(blockingQueue1.BlockingPeek() == true);
            UT_ASSERT(randInts[j] == blockingQueue1.Pop());
            UT_ASSERT(blockingQueue2.BlockingPeek() == true);
            UT_ASSERT(randDoubles[j] == blockingQueue2.Pop());
        }
        randInts.clear();
        randDoubles.clear();
    }

    UT_ASSERT(blockingQueue1.NonBlockingPeek() == false);
    UT_ASSERT(blockingQueue2.NonBlockingPeek() == false);
}

void XBlockingQueueTest::TestBlockingPeek(void)
{
    static const unsigned int iterations = 15;
    static const unsigned int volume = 2;

    srand(time(NULL));

    std::vector<int> randInts;
    std::vector<double> randDoubles;

    XBlockingQueue<int> blockingQueue1;
    XBlockingQueue<double> blockingQueue2;

    _testHelper1.SetBlockingQueue(blockingQueue1);
    _testHelper2.SetBlockingQueue(blockingQueue2);

    UT_ASSERT(blockingQueue1.NonBlockingPeek() == false);
    UT_ASSERT(blockingQueue2.NonBlockingPeek() == false);

    for(unsigned int i = 0;i < iterations;++i)
    {
        for(unsigned j = 0;j < volume;++j)
        {
            randInts.push_back(rand() % 1000);
            randDoubles.push_back((rand() % 1000) * ((rand() % 1000) / 1000));
            blockingQueue1.Push(randInts[j]);
            blockingQueue2.Push(randDoubles[j]);
        }
        for(unsigned j = 0;j < volume;++j)
        {
            _testHelper1.Post(TestCommand<int>(randInts[j],BLOCKING_PEEK_POP));
            _testHelper2.Post(TestCommand<double>(randDoubles[j],BLOCKING_PEEK_POP));
        }
        randInts.clear();
        randDoubles.clear();
    }

    //Wait for the test helpers to finish
    _Wait(blockingQueue1,blockingQueue2);
}

void XBlockingQueueTest::TestCancel(void)
{
    static const unsigned int iterations = 1;

    srand(time(NULL));

    XBlockingQueue<int> blockingQueue1;
    XBlockingQueue<double> blockingQueue2;

    _testHelper1.SetBlockingQueue(blockingQueue1);
    _testHelper2.SetBlockingQueue(blockingQueue2);

    UT_ASSERT(blockingQueue1.NonBlockingPeek() == false);
    UT_ASSERT(blockingQueue2.NonBlockingPeek() == false);

    for(unsigned i = 0;i < iterations;++i)
    {
        _testHelper1.Post(TestCommand<int>(1,CANCEL));
        _testHelper2.Post(TestCommand<double>(2,CANCEL));

        UT_ASSERT(blockingQueue1.BlockingPeek() == false);
        UT_ASSERT(blockingQueue2.BlockingPeek() == false);
    }
}

void XBlockingQueueTest::BreakIt(void)
{
    static const unsigned int iterations = 1000;
    static const unsigned int volume = 10;

    srand(time(NULL));

    std::vector<int> randInts;
    std::vector<double> randDoubles;

    XBlockingQueue<int> blockingQueue1;
    XBlockingQueue<double> blockingQueue2;

    _testHelper1.SetBlockingQueue(blockingQueue1);
    _testHelper2.SetBlockingQueue(blockingQueue2);
    _testHelper3.SetBlockingQueue(blockingQueue2);

    for(unsigned int i = 0;i < iterations;++i)
    {
        for(unsigned j = 0;j < volume;++j)
        {
            randInts.push_back(rand() % 1000);
            randDoubles.push_back((rand() % 1000) * ((rand() % 1000) / 1000));
            _testHelper1.Post(TestCommand<int>(randInts[j],PUSH));
            _testHelper2.Post(TestCommand<double>(randDoubles[j],PUSH));
            _testHelper3.Post(TestCommand<double>(randDoubles[j],PUSH));
        }
        for(unsigned j = 0;j < volume;++j)
        {
            UT_ASSERT(blockingQueue1.Pop() == randInts[j]);
            UT_ASSERT(blockingQueue2.Pop() == randDoubles[j]);
            UT_ASSERT(blockingQueue2.Pop() == randDoubles[j]);
        }
        randInts.clear();
        randDoubles.clear();
    }

    UT_ASSERT(blockingQueue1.NonBlockingPeek() == false);
    UT_ASSERT(blockingQueue2.NonBlockingPeek() == false);

    for(unsigned int i = 0;i < iterations;++i)
    {
        for(unsigned j = 0;j < volume;++j)
        {
            randInts.push_back(rand() % 1000);
            randDoubles.push_back((rand() % 1000) * ((rand() % 1000) / 1000));
            blockingQueue1.Push(randInts[j]);
            blockingQueue2.Push(randDoubles[j]);
        }
        for(unsigned j = 0;j < volume;++j)
        {
            _testHelper1.Post(TestCommand<int>(randInts[j],POP));
            _testHelper2.Post(TestCommand<double>(randDoubles[j],POP));
        }
        randInts.clear();
        randDoubles.clear();
    }

    //Wait for the test helpers to finish
    _Wait(blockingQueue1,blockingQueue2);
}
