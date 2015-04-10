
#include "XTimerTest.h"
#include "XSDK/OS.h"

#define TIMER_DELAY 0.100   // in seconds
#define ALLOWED_ERROR 0.050

using namespace XSDK;


REGISTER_TEST_FIXTURE(XSimpleTimerTest);

void XSimpleTimerTest::TestNormal()
{
    XSimpleTimer timer(XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

}


void XSimpleTimerTest::TestLongWait()
{
    XSimpleTimer timer(XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 3300));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 50));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 155));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());
}


void XSimpleTimerTest::TestStartLater()
{
    XTime startTime = Clock::currTime(XUTC::Instance()) + XDuration(MSECS, 400);
    XSimpleTimer timer(startTime, XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 200));
    UT_ASSERT(timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

}


void XSimpleTimerTest::TestNormalNoRepeat()
{
    XSimpleTimer timer(XDuration(MSECS, 500), false);
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

}


void XSimpleTimerTest::TestLongWaitNoRepeat()
{
    XSimpleTimer timer(XDuration(MSECS, 500), false);
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

    x_sleep(XDuration(MSECS, 3300));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 105));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 105));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

}


void XSimpleTimerTest::TestStartLaterNoRepeat()
{
    XTime startTime = Clock::currTime(XUTC::Instance()) + XDuration(MSECS, 400);
    XSimpleTimer timer(startTime, XDuration(MSECS, 500), false);
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 200));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(!timer.Repeating());
}


void XSimpleTimerTest::TestStartNormal()
{
    XSimpleTimer timer;
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration::Zero());
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

    x_sleep(XDuration(MSECS, 200));
    UT_ASSERT(!timer.Poll());

    timer.Start(XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());
}


void XSimpleTimerTest::TestStartLongWait()
{
    XSimpleTimer timer;
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration::Zero());
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

    x_sleep(XDuration(MSECS, 200));
    UT_ASSERT(!timer.Poll());

    timer.Start(XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 3300));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 105));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 105));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());
}


void XSimpleTimerTest::TestStartStartLater()
{
    XSimpleTimer timer;
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration::Zero());
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

    x_sleep(XDuration(MSECS, 200));
    UT_ASSERT(!timer.Poll());

    XTime startTime = Clock::currTime(XUTC::Instance()) + XDuration(MSECS, 400);
    timer.Start(startTime, XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 200));
    UT_ASSERT(timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());
}


void XSimpleTimerTest::TestStartNormalNoRepeat()
{
    XSimpleTimer timer;
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration::Zero());
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

    x_sleep(XDuration(MSECS, 200));
    UT_ASSERT(!timer.Poll());

    timer.Start(XDuration(MSECS, 500), false);
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(!timer.Repeating());
}


void XSimpleTimerTest::TestStartLongWaitNoRepeat()
{
    XSimpleTimer timer;
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration::Zero());
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

    x_sleep(XDuration(MSECS, 200));
    UT_ASSERT(!timer.Poll());

    timer.Start(XDuration(MSECS, 500), false);
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

    x_sleep(XDuration(MSECS, 3300));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 105));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 105));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(!timer.Repeating());
}


void XSimpleTimerTest::TestStartStartLaterNoRepeat()
{
    XSimpleTimer timer;
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration::Zero());
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

    x_sleep(XDuration(MSECS, 200));
    UT_ASSERT(!timer.Poll());

    XTime startTime = Clock::currTime(XUTC::Instance()) + XDuration(MSECS, 400);
    timer.Start(startTime, XDuration(MSECS, 500), false);
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(!timer.Repeating());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 100));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 101));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 200));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(!timer.Repeating());
}


void XSimpleTimerTest::TestStop()
{
    XSimpleTimer timer(XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());

    timer.Stop();
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(timer.Repeating());
}


void XSimpleTimerTest::TestStopDelay()
{
    XSimpleTimer timer(XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 3300));
    timer.Stop();
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(timer.Repeating());
}


void XSimpleTimerTest::TestRestart()
{
    XSimpleTimer timer(XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());

    timer.Stop();
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(!timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    timer.Start();
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());
}


//Test what happens when start is called on a running timer.
void XSimpleTimerTest::TestReset()
{
    XSimpleTimer timer(XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    timer.Start(XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    timer.Start(XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());

    timer.Start(XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    timer.Start(XDuration(MSECS, 500));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(!timer.Poll());

    x_sleep(XDuration(MSECS, 300));
    UT_ASSERT(timer.Poll());
    UT_ASSERT(timer.IntervalLen() == XDuration(MSECS, 500));
    UT_ASSERT(timer.IsRunning());
    UT_ASSERT(timer.Repeating());
}


REGISTER_TEST_FIXTURE(XTimerTest);

void XTimerTest::setup()
{
    _deltas.clear();
    _prevTime.tv_sec = 0;
    _prevTime.tv_usec = 0;
    _fireCount = 0;
    _pTimer = new XSDK::XTimer;
}

void XTimerTest::teardown()
{
    if( _pTimer )
        delete _pTimer;
    _pTimer = 0;
}

// Thread function
void* XTimerTest::EntryPoint()
{
    // This tests using out own thread to implement the timer.
    UT_ASSERT_NO_THROW( _pTimer->DoTimerLoop( (unsigned int)(TIMER_DELAY * 1000.0), true, cb, this ) );
    return 0;
}

// Timer callback
void XTimerTest::cb( void* data )
{
    XTimerTest* ctx = (XTimerTest*)data;

    // Keep track of the time interval between timer calls.
    struct timeval now = {0,0};
    x_gettimeofday( &now );
    struct timeval delta = {0,0};
    timersub( &now, &ctx->_prevTime, &delta );
    ctx->_deltas.push_back( delta );
    timercopy( &ctx->_prevTime, &now );
    ctx->_fireCount++;
}

void XTimerTest::TestThreadedTimer()
{
    // Periodic time
    x_gettimeofday( &_prevTime );
    UT_ASSERT_NO_THROW( _pTimer->StartTimer( (unsigned int)(TIMER_DELAY * 1000.0), false, cb, this ) );
    x_sleep(1);
    UT_ASSERT_NO_THROW( _pTimer->StopTimer() );
    double avg = _CalcAverage();
    UT_ASSERT( (TIMER_DELAY-ALLOWED_ERROR) < avg && avg < (TIMER_DELAY+ALLOWED_ERROR) );

    // Test timer reuse
    x_gettimeofday( &_prevTime );
    UT_ASSERT_NO_THROW( _pTimer->StartTimer( (unsigned int)(TIMER_DELAY * 1000.0), false, cb, this ) );
    x_sleep(1);
    UT_ASSERT_NO_THROW( _pTimer->StopTimer() );
    avg = _CalcAverage();
    UT_ASSERT( (TIMER_DELAY-ALLOWED_ERROR) < avg && avg < (TIMER_DELAY+ALLOWED_ERROR) );

    // Test calling StartTimer() and StopTimer() twice.
    x_gettimeofday( &_prevTime );
    UT_ASSERT_NO_THROW( _pTimer->StartTimer( (unsigned int)(TIMER_DELAY * 1000.0), false, cb, this ) );
    UT_ASSERT_NO_THROW( _pTimer->StartTimer( (unsigned int)(TIMER_DELAY * 1000.0), false, cb, this ) );
    x_sleep(1);
    UT_ASSERT_NO_THROW( _pTimer->StopTimer() );
    UT_ASSERT_NO_THROW( _pTimer->StopTimer() );
    avg = _CalcAverage();
    UT_ASSERT( (TIMER_DELAY-ALLOWED_ERROR) < avg && avg < (TIMER_DELAY+ALLOWED_ERROR) );

    // Test one-shot timer.
    _fireCount = 0;
    x_gettimeofday( &_prevTime );
    UT_ASSERT_NO_THROW( _pTimer->StartTimer( (unsigned int)(TIMER_DELAY * 1000.0), true, cb, this ) );
    x_usleep( (unsigned int)(TIMER_DELAY * 1000000.0 * 3.0) );
    UT_ASSERT( _fireCount == 1 );
}

void XTimerTest::TestNonThreadedTimer()
{
    // Tests using our own thread to implement the Timer.
    this->_fireCount = 0;
    this->Start();

    x_sleep( 1 );

    _pTimer->StopTimer(); // Should cause the timer loop to return and the thread to exit.

    x_usleep( 500000 );

    UT_ASSERT( !this->IsRunning() );
    UT_ASSERT( this->_fireCount > 0 );
}
