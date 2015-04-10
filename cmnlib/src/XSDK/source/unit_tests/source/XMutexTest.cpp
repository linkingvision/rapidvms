
#include "XMutexTest.h"
#include "XSDK/TimeUtils.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(XMutexTest);

void XMutexTest::setup()
{
}

void XMutexTest::teardown()
{
}

// XXX - READ THIS
//
// Many of these tests require spawning a thread. Since their can be only 1 EntryPoint() method,
// _testNum is used to control what actual work is done when the thread starts.

void* XMutexTest::EntryPoint()
{
    switch( _testNum )
    {
    case 0:
    {
        _lok.Acquire();

        x_sleep( 2 );

        _val = 42;

        _lok.Release();
    }
    break;
    case 1:
    {
        _lok.Acquire();

        _val = 42;

        _lok.Release();
    }
    break;
    default:
        break;
    };

    return &_val;
}

void XMutexTest::TestMutexSimple()
{
    // Initially, _val is 0.
    // We then Start our thread and go to x_sleep for 1 second (plenty of time for the thread to start).
    // While the main thread x_sleeps, our worker wakes up and grabs our lock and then goes to x_sleep.
    // The main thread then wakes up and attempts to grab our lock... (which blocks the main thread)...
    // With the lock held, the worker thread then modifies _val to be == to 42 and then unlocks the mutex.
    // Finally, the main thread acquires the lock and then checks that the value == 42.

    try
    {
        _val = 0;
        _testNum = 0; // See comment regarding _testNum in EntryPoint() method.

        Start();

        x_sleep( 1 );

        _lok.Acquire();

        UT_ASSERT( _val == 42 );

        _lok.Release();

        int* retVal = (int*)Join();
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT( false && "Test threw!" );
    }
}

void XMutexTest::TestLockTwice()
{
    // Initially, _val is 0.
    // We then immediately lock our mutex twice.
    // Next, we start our worker thread and go to x_sleep.
    // While the main thread is x_sleeping, the worker thread attempts to acquire our lock, which blocks it.
    // We then call Release once on our mutex and x_sleep for 1 second. The worker should STILL be blocked, which
    // we verify by checking that _val still equals 0.
    // We then call Release one more time and x_sleep for 1 second. The worker is then able to acquire the lock,
    // and immediately set _val == 42.
    // When the main thread wakes up it then asserts that _val == 42.

    try
    {
        _val = 0;
        _testNum = 1; // See comment regarding _testNum in EntryPoint() method.

        _lok.Acquire();
        _lok.Acquire();

        Start();

        x_sleep( 1 );

        _lok.Release();

        x_sleep( 1 );

        UT_ASSERT( _val == 0 );

        _lok.Release();

        x_sleep( 1 );

        UT_ASSERT( _val == 42 );

        Join();
    }

    catch(XSDK::XException&)
    {
        UT_ASSERT( false && "Test threw!" );
    }
}

void XMutexTest::TestUnlockWhileNotLocked()
{
    // Should throw, since _lok is not locked.
    UT_ASSERT_THROWS( _lok.Release(), XException);
}

void XMutexTest::TestLotsOfLocks()
{
        _val = 0;
        _testNum = 1; // See comment regarding _testNum in EntryPoint() method.

        _lok.Acquire();

        Start();
        UT_ASSERT( _val == 0 );
        x_sleep(1);
        UT_ASSERT( _val == 0 );

        for( int i = 0; i < 999; i++ )
            _lok.Acquire();

        // At this point, we've acquire the lock 1000 times total...

        UT_ASSERT( _val == 0 );

        for( int i = 0; i < 999; i++ )
            _lok.Release();

        // At this point, we've released the lock 999 times...

        UT_ASSERT( _val == 0 );

        _lok.Release(); // final release...

        x_sleep( 1 ); // give worker time to acquire _lok and modify _val...

        UT_ASSERT( _val == 42 );

        Join();
}
