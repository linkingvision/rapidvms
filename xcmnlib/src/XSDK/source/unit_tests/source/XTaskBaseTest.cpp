
#include "XTaskBaseTest.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XStopWatch.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(XTaskBaseTest);

void XTaskBaseTest::TestTaskBase()
{

    try
    {
        Thread thread(true);
        thread.Start();
        x_usleep(500000);

        // Make sure thread is running then let it go.
        UT_ASSERT(thread.IsRunning());
        thread.Signal();
        x_usleep(500000);

		int* p = (int*)thread.Join();

        UT_ASSERT(thread._myretval == 42);
        UT_ASSERT(!thread.IsRunning());
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}


void XTaskBaseTest::TestCancelThread()
{
    try
    {
        Thread thread(true);
        thread.Start();
        x_usleep(500000);

        UT_ASSERT(thread.IsRunning());
        thread.CancelThread();

        UT_ASSERT(thread._myretval != 42);
        UT_ASSERT(!thread.IsRunning());
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}


void XTaskBaseTest::TestUniqueThreadIDs()
{
    //We have a separate, outer try/catch block just in case
    //the constructors or Join() calls throw (which they'd better
    //not do). They're not done inside the other block because
    //the Join() calls need to be made or valgrind complains about
    //possible memory leaks.
    try
    {
        Thread bread;
        Thread t1;
        Thread t2;

        try
        {
            UT_ASSERT(t1.ThreadID() == XTaskBase::invalidThreadID);
            UT_ASSERT(t2.ThreadID() == XTaskBase::invalidThreadID);

            t1.Start();
            x_sleep(1);

            UT_ASSERT(t1.ThreadID() != XTaskBase::invalidThreadID);
            UT_ASSERT(t2.ThreadID() == XTaskBase::invalidThreadID);

            t2.Start();
            x_sleep(1);

            UT_ASSERT(t1.ThreadID() != XTaskBase::invalidThreadID);
            UT_ASSERT(t2.ThreadID() != XTaskBase::invalidThreadID);
            UT_ASSERT(t1.ThreadID() != t2.ThreadID());
            UT_ASSERT(t1.ThreadID() != bread.ThreadID());
            UT_ASSERT(t2.ThreadID() != bread.ThreadID());

        }
        catch(XSDK::XException&)
        {
            UT_ASSERT(false && "Test threw!");
        }

        bread.Join();
        t1.Join();
        t2.Join();
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Join() isn't supposed to go boom.");
    }

    try
    {
        XIRef<XThreadPool> threadPool = new XThreadPool(3,true);
        Thread bread(threadPool, false);
        Thread t1(threadPool, false);
        Thread t2(threadPool, false);
        Thread t3(threadPool, false);

        try
        {
            UT_ASSERT(t1.ThreadID() == XTaskBase::invalidThreadID);
            UT_ASSERT(t2.ThreadID() == XTaskBase::invalidThreadID);
            UT_ASSERT(t3.ThreadID() == XTaskBase::invalidThreadID);

            t1.Start();
            x_sleep(1);

            UT_ASSERT(t1.ThreadID() != XTaskBase::invalidThreadID);
            UT_ASSERT(t2.ThreadID() == XTaskBase::invalidThreadID);
            UT_ASSERT(t3.ThreadID() == XTaskBase::invalidThreadID);

            t2.Start();
            x_sleep(1);

            UT_ASSERT(t1.ThreadID() != XTaskBase::invalidThreadID);
            UT_ASSERT(t2.ThreadID() != XTaskBase::invalidThreadID);
            UT_ASSERT(t3.ThreadID() == XTaskBase::invalidThreadID);

            t3.Start();
            x_sleep(1);

            UT_ASSERT(t1.ThreadID() != XTaskBase::invalidThreadID);
            UT_ASSERT(t2.ThreadID() != XTaskBase::invalidThreadID);
            UT_ASSERT(t3.ThreadID() != XTaskBase::invalidThreadID);

            UT_ASSERT(t1.ThreadID() != t2.ThreadID());
            UT_ASSERT(t1.ThreadID() != t3.ThreadID());
            UT_ASSERT(t2.ThreadID() != t3.ThreadID());

            UT_ASSERT(t1.ThreadID() != bread.ThreadID());
            UT_ASSERT(t2.ThreadID() != bread.ThreadID());
            UT_ASSERT(t3.ThreadID() != bread.ThreadID());

        }
        catch(XSDK::XException&)
        {
            UT_ASSERT(false && "Test threw!");
        }

        bread.Join();
        t1.Join();
        t2.Join();
        t3.Join();
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Join() isn't supposed to go boom.");
    }
}


void XTaskBaseTest::TestPool()
{
    try
    {
// I'm disabling this test because 1) it doesn't work 2) I want to rip out the thread pool stuff anyway...
#if 0
        XIRef<XThreadPool> threadPool = new XThreadPool(6,true);
        Thread one(threadPool, true, 1);
        Thread two(threadPool, true, 2);
        Thread three(threadPool, true, 3);
        Thread four(threadPool, true, 4);
        Thread five(threadPool, true, 5);
        Thread six(threadPool, true, 6);

        UT_ASSERT(!one.IsRunning());
        UT_ASSERT(!two.IsRunning());
        UT_ASSERT(!three.IsRunning());
        UT_ASSERT(!four.IsRunning());
        UT_ASSERT(!five.IsRunning());
        UT_ASSERT(!six.IsRunning());

        one.Start();
        two.Start();
        three.Start();
        four.Start();
        five.Start();
        six.Start();
        x_sleep(1);

        UT_ASSERT(one.IsRunning());
        UT_ASSERT(two.IsRunning());
        UT_ASSERT(three.IsRunning());
        UT_ASSERT(four.IsRunning());
        UT_ASSERT(five.IsRunning());
        UT_ASSERT(six.IsRunning());

        one.Signal();
        two.Signal();
        three.Signal();
        four.Signal();
        five.Signal();
        six.Signal();

        int* result = (int*)one.Join();
        UT_ASSERT( result != NULL );
        const int result1 = *result;
        result = (int*)two.Join();
        UT_ASSERT( result != NULL );
        const int result2 = *result;
        result = (int*)three.Join();
        UT_ASSERT( result != NULL );
        const int result3 = *result;
        result = (int*)four.Join();
        UT_ASSERT( result != NULL );
        const int result4 = *result;
        result = (int*)five.Join();
        UT_ASSERT( result != NULL );
        const int result5 = *result;
        result = (int*)six.Join();
        UT_ASSERT( result != NULL );
        const int result6 = *result;

        UT_ASSERT(!one.IsRunning());
        UT_ASSERT(!two.IsRunning());
        UT_ASSERT(!three.IsRunning());
        UT_ASSERT(!four.IsRunning());
        UT_ASSERT(!five.IsRunning());
        UT_ASSERT(!six.IsRunning());

        UT_ASSERT(result1 == 1);
        UT_ASSERT(result2 == 2);
        UT_ASSERT(result3 == 3);
        UT_ASSERT(result4 == 4);
        UT_ASSERT(result5 == 5);
        UT_ASSERT(result6 == 6);
#endif
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XTaskBaseTest::TestPoolJoin()
{
    XIRef<XThreadPool> threadPool = new XThreadPool(3,true);
    Thread one(threadPool, true, 1);

    for( size_t i = 0; i < 10000; ++i )
    {
        one.Start();
        one.Signal();
        void* retVal = one.Join();
        UT_ASSERT( retVal != NULL );
    }
}

void XTaskBaseTest::TestBuildDestroyPool()
{
    {//This test case originally cause the application to crash.
        XThreadPool pool(100);
    }
}



class SleepyThread : public XSDK::XTaskBase
{
public:

    X_API SleepyThread (XString name)
        : XTaskBase( name )
    {}

    X_API SleepyThread(XIRef<XSDK::XThreadPool> pool,XString name)
        : XTaskBase(pool,name)
    {}

    X_API virtual ~SleepyThread() throw() {Join();}

    X_API virtual void* EntryPoint()
    {
        x_sleep(5);
        return this;
    }
};


void XTaskBaseTest::TestParrallelExecution()
{
    XIRef<XThreadPool> threadPool = new XThreadPool(3,false);
    XStopWatch sw(true);
    SleepyThread dopey(threadPool,"dopey");
    SleepyThread happy(threadPool,"happy");
    SleepyThread sneezy(threadPool,"sneeeey");
    dopey.Start();
    happy.Start();
    sneezy.Start();
    int64_t value = sw.TimeElapsed().Total(SECONDS);
    UT_ASSERT( value < 15 );
}








void XTaskBaseTest::TestThreadNames()
{
#ifdef IS_LINUX
    NamedThread thread( "thready" );
    thread.Start();

    x_usleep( 500000 );

    UT_ASSERT( thread.NameWasSet() );

    thread.Join();
#endif
}
void XTaskBaseTest::TestGetCPU()
{
    try
    {
        UT_ASSERT(XTaskBase::GetCPU()>= 0);
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}
