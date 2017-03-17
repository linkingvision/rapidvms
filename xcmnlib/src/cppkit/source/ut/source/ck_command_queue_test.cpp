
#include "framework.h"
#include "ck_command_queue_test.h"
#include "cppkit/ck_command_queue.h"
#include "cppkit/os/ck_time_utils.h"
#include <thread>

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_command_queue_test);

void ck_command_queue_test::setup()
{
}

void ck_command_queue_test::teardown()
{
}

void ck_command_queue_test::test_default_constructor()
{
    command_queue<int> q;
}

void ck_command_queue_test::test_start_stop()
{
    command_queue<int> q;
    q.start();
    UT_ASSERT( q.started() == true );
    q.stop();
    UT_ASSERT( q.started() == false );
}

void ck_command_queue_test::test_full_queue()
{
    // Initially, this test worked the other way around... The worker thread would call q.wait() and q.pop() and sum
    // the total. I had a totally persisten valgrind "conditional jump or move depends upon uninitialized value" error.
    // I discovered that the error went away if the same code was simply no performed on another thread. I began to
    // suspect that the problem might be related to the thread that created the condition NOT being the one who called
    // wait... Because simply inverting it (having the worker thread call post) "fixes" the problem.
    command_queue<int> q;
    q.start();

    UT_ASSERT( q.started() == true );

    thread t([&q](){
        for( int i = 1; i <= 1000; i++ )
            q.post( i );
    });

    t.detach();

    int total = 0;

    while( total < 500500 )
    {
        q.wait();
        total += q.pop();
    }

    q.stop();

    UT_ASSERT( q.started() == false );

    UT_ASSERT( total == 500500 );
}
