
#include "ck_timer_test.h"

#include "cppkit/ck_timer.h"
#include "cppkit/os/ck_time_utils.h"

using namespace std;
using namespace cppkit;


REGISTER_TEST_FIXTURE(ck_timer_test);

void ck_timer_test::test_repeating()
{
    int counter = 0;
    ck_timer t( 250, [&](){ counter++; } );
    t.start();
    ck_sleep( 1 );
    t.stop();
    UT_ASSERT( counter >= 2 );
}
