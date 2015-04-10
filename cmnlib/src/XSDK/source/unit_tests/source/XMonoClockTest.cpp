
#include "XMonoClockTest.h"

#include <cstdlib>
#include <cmath>

#include "XSDK/OS/Sprintf.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XMonoClock.h"

#define TEST_ITERATIONS 10000
#define USEC_DURATION 0.742f  // 42 usecs
#define USEC_TEST_VARIANCE 0.01f // 1 usec (for math errors)

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XMonoClockTest);

void XMonoClockTest::setup()
{
}

void XMonoClockTest::teardown()
{
}

void XMonoClockTest::TestGetTime()
{
    uint64_t time1=0, time2=0;

    UT_ASSERT_NO_THROW( time1 = XMonoClock::GetTime() );
    UT_ASSERT( time1 != 0 );
    x_sleep(1);
    UT_ASSERT_NO_THROW( time2 = XMonoClock::GetTime() );
    UT_ASSERT( time2 != 0 );
    UT_ASSERT( time1 != time2 );

    // Test many calls to GetTime() and make sure it never ticks backwards.
    uint64_t now, last;
    UT_ASSERT_NO_THROW( last = XMonoClock::GetTime() );
    uint64_t max = (uint64_t)-1;
    for( int ii=0; ii<TEST_ITERATIONS; ii++ )
    {
        x_usleep( 10 );
        UT_ASSERT_NO_THROW( now = XMonoClock::GetTime() );

        // If the clock has ticked backwards, then the difference between 2
        // readings will be huge (because negative numbers go hugely positive
        // when using unsigned variables). Otherwise, it is reasonable to
        // assume the difference will be less than half the max.
        UT_ASSERT( (now - last) < (max /2) );
        last = now;
    }
}

void XMonoClockTest::TestGetFrequency()
{
    uint64_t freq = 0;
    UT_ASSERT_NO_THROW( freq = XMonoClock::GetFrequency() );
    UT_ASSERT( freq != 0 );

    // Call a bunch of times just to be sure.
    for( int ii=0; ii<TEST_ITERATIONS; ii++ )
    {
        freq = 0;
        UT_ASSERT_NO_THROW( freq = XMonoClock::GetFrequency() );
        UT_ASSERT( freq != 0 );
    }
}

void XMonoClockTest::TestGetElapsedTime()
{
    // Use GetFrequency() to simulate an exact 1 second duration.
    uint64_t second = XMonoClock::GetFrequency();
    double duration = 0.0;
    UT_ASSERT_NO_THROW( duration = XMonoClock::GetElapsedTime( 0, second ) );
    UT_ASSERT( duration == 1.0 );

    // Test with a random start time.
    srand( time(0) );
    uint64_t start = (uint64_t)rand();
    x_printf( "\t- Ticks-per-second: %I64u\n", second );
    x_printf( "\t- Random start time: %I64u\n", start );
    fflush(stdout);

    duration = 0.0;
    UT_ASSERT_NO_THROW( duration = XMonoClock::GetElapsedTime( start, start + second ) );
    UT_ASSERT( duration == 1.0 );

    // Simulate a small, microsecond duration. NOTE: Due to math percision errors, we will
    // never get an exact match, so we test for an answer within an allowed range.
    uint64_t usecs = (uint64_t)( (float)second * USEC_DURATION );
    duration = 0.0;
    UT_ASSERT_NO_THROW( duration = XMonoClock::GetElapsedTime( start, start + usecs ) );

    UT_ASSERT( USEC_DURATION+USEC_TEST_VARIANCE > duration && duration > USEC_DURATION-USEC_TEST_VARIANCE );

    // Test the case where the counter has wrapped back to 0 and end time
    // is less than the start time.
    uint64_t max = (uint64_t)-1;
    uint64_t wrapped = max + second;
    duration = 0.0;
    UT_ASSERT_NO_THROW( duration = XMonoClock::GetElapsedTime( max, wrapped ) );
    UT_ASSERT( duration == 1.0 );
}
