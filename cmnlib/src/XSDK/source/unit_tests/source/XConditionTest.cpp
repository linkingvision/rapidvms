
#include "XConditionTest.h"
#include "XSDK/XGuard.h"
#include "XSDK/XMonoClock.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XTime.h"
#include <time.h>

using namespace XSDK;

REGISTER_TEST_FIXTURE(XConditionTest);

#define SECONDS_TO_WAIT 3

using namespace std;
void* XConditionTest::EntryPoint()
{
    switch( _testNum )
    {

    case 0:
        {
            XGuard guard( _lok );

            // Because conditions may wake spuriously, they must be wrapped in a loop...
            while( _waiting )
                _cond.Wait();

            _val = 42;
        }
        break;

    case 1:
        {
            XGuard guard( _lok );

            const time_t thenSeconds = time(0) + SECONDS_TO_WAIT;
            const time_t thenMicro = 0;
            const uint64_t waitUntil = XCondition::FutureMonotonicTime(thenSeconds, thenMicro);

            while( !_cond.WaitUntil(waitUntil) )
            {}
        }
        break;

    case 2:
        {
            XGuard guard( _lok );

            const uint64_t waitUntil = XCondition::FutureMonotonicTime(XDuration(SECONDS, SECONDS_TO_WAIT));

            while( !_cond.WaitUntil( waitUntil ) )
            {}
        }
        break;

    case 3:
        {
            XGuard guard( _lok );
            x_sleep(1);
            _val = 43;
           _cond.Signal();
        }
        break;

    default:
        break;

    };

    return &_val;
}

void XConditionTest::TestConditionSimple()
{
    _val = 0; // Initially, our _val is 0.
    _testNum = 0;

    _waiting = true; // XCondition variables may wake spuriousley, so they must always be
                     // associated with some underlying boolean conditional...

    this->Start(); // Start our worker thread...

    x_sleep( 1 ); // Give the worker thread time to acquire the lock and call Wait() on the condition...

    UT_ASSERT( _val == 0 ); // Verify that the worker is blocked in Wait()...

    _waiting = false; // Adjust our waiting boolean (so the next time the Wait() returns, it will
                      // not call Wait() again...

    _lok.Acquire(); // Begin the process of signaling our condition...

    _cond.Signal(); // Signal our condition...

    x_sleep( 1 ); // Proves that until we release our lock, the worker thread is still Wait()ing

    UT_ASSERT( _val == 0 );  // Until we call release, the thread is still in "Wait()"...

    _lok.Release(); // Finally, release our worker thread from it's Wait()...

    x_sleep( 1 ); // Give the worker a chance to ahjust _val...

    UT_ASSERT( _val == 42 ); // Verify that the worker changed _val...

	Join();
}

void XConditionTest::TestFutureMonoNegative()
{
    {
        const uint64_t before = XMonoClock::GetTime();
        const uint64_t result = XCondition::FutureMonotonicTime(XDuration::Zero());
        const uint64_t after = XMonoClock::GetTime();
        UT_ASSERT(before <= result && result <= after);
    }

    {
        const uint64_t before = XMonoClock::GetTime();
        const uint64_t result = XCondition::FutureMonotonicTime(XDuration(HNSECS, -1));
        const uint64_t after = XMonoClock::GetTime();
        UT_ASSERT(before <= result && result <= after);
    }

    {
        const uint64_t before = XMonoClock::GetTime();
        const uint64_t result = XCondition::FutureMonotonicTime(XDuration(MSECS, -1));
        const uint64_t after = XMonoClock::GetTime();
        UT_ASSERT(before <= result && result <= after);
    }
}

void XConditionTest::TestWaitUntil()
{
    {
        _testNum = 1;

        const XTime startTime = Clock::currTime();
        this->Start(); // Start our worker thread...
        this->Join();
        const XTime endTime = Clock::currTime();
        const XDuration diff = endTime - startTime;

        // The wait until seems to be frustratingly imprecise with sub-second times.
        try
        {
            UT_ASSERT( diff > XDuration(SECONDS, SECONDS_TO_WAIT) - XDuration(SECONDS, 1) );
            UT_ASSERT( diff < XDuration(SECONDS, SECONDS_TO_WAIT) + XDuration(MSECS, 250) );
        }
        catch(...)
        {
            // So that we can see how off it was and adjust it if we have to.
            printf("%s -> %s -> %I64d\n", startTime.ToUTC().ToISOExtString().c_str(),
                                          endTime.ToUTC().ToISOExtString().c_str(),
                                          diff.Total(HNSECS));
            throw;
        }
    }
    //The same test but using the FutureMonotonic which takes an XDuration.
    {
        _testNum = 2;

        const XTime startTime = Clock::currTime();
        this->Start(); // Start our worker thread...
        this->Join();
        const XTime endTime = Clock::currTime();
        const XDuration diff = endTime - startTime;
        // This overload tends to be more precise (for this test at least) based on how the time is calculated.
        UT_ASSERT( diff > XDuration(SECONDS, SECONDS_TO_WAIT) - XDuration(MSECS, 500) );
        UT_ASSERT( diff < XDuration(SECONDS, SECONDS_TO_WAIT) + XDuration(MSECS, 250) );
    }
}


void XConditionTest::TestWaitUntilReturnValue()
{
#if 0
     _testNum = 3;
     struct timeval now = { 0, 0 };
     struct timeval delta = { SECONDS_TO_WAIT, 0 };
     struct timeval sleepUntil = { 0, 0 };

     x_gettimeofday( &now );
     timeradd( &now, &delta, &sleepUntil );
     bool timedOut = false;

     while(!timedOut)
         timedOut = _cond.WaitUntil( XCondition::FutureMonotonicTime(sleepUntil.tv_sec, sleepUntil.tv_usec) );
     // has to be true
     UT_ASSERT(timedOut == true);

     x_gettimeofday( &now );
     timeradd( &now, &delta, &sleepUntil );

     this->Start();
     while(_val != 43)
         timedOut = _cond.WaitUntil( XCondition::FutureMonotonicTime(sleepUntil.tv_sec, sleepUntil.tv_usec) );
     UT_ASSERT(timedOut == false);
#endif
 }
