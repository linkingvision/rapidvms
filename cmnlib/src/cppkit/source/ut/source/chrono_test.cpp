
#include "chrono_test.h"

#include "cppkit/os/ck_time_utils.h"

#include <mutex>
#include <future>
#include <ctime>

using namespace std;
using namespace std::chrono;
using namespace cppkit;

REGISTER_TEST_FIXTURE(chrono_test);

void chrono_test::setup()
{
}

void chrono_test::teardown()
{
}

void chrono_test::test_time_point_construction()
{
    // system clock
    {
        time_t sysNow = time(0);
        system_clock::time_point now = system_clock::now();
        time_t chronoNow = system_clock::to_time_t(now);
        UT_ASSERT( (sysNow == chronoNow) || ((sysNow+1) == chronoNow) );
    }

    // steady clock
    {
        steady_clock::time_point before = steady_clock::now();
        ck_usleep(1000);
        steady_clock::time_point after = steady_clock::now();
        UT_ASSERT(duration_cast<microseconds>(after - before).count() > 1000);
    }

    // high resolution clock
    {
        high_resolution_clock::time_point before = high_resolution_clock::now();
        ck_usleep(1000);
        high_resolution_clock::time_point after = high_resolution_clock::now();
        UT_ASSERT(duration_cast<microseconds>(after - before).count() > 1000);
    }
}

void chrono_test::test_duration_addition()
{
    auto d = hours(1) + minutes(30);
    UT_ASSERT(duration_cast<seconds>(d).count() == 5400);
}

void chrono_test::test_duration_setup()
{
    nanoseconds numNanos(250);
    microseconds numMicros(250);
    milliseconds numMillis(250);
    seconds numSeconds(250);
    minutes numMinutes(250);
    hours numHours(250);
    UT_ASSERT( numHours > numMinutes );
    UT_ASSERT( numMinutes > numSeconds );
    UT_ASSERT( numSeconds > numMillis );
    UT_ASSERT( numMillis > numMicros );
    UT_ASSERT( numMicros > numNanos );
    UT_ASSERT( minutes(15001) > numHours );
}
