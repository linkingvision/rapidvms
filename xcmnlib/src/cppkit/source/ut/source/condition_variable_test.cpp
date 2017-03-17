
#include "condition_variable_test.h"

#include "cppkit/os/ck_time_utils.h"

#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;
using namespace std::chrono;
using namespace cppkit;

REGISTER_TEST_FIXTURE(condition_variable_test);

void condition_variable_test::setup()
{
}

void condition_variable_test::teardown()
{
}

void condition_variable_test::test_simple()
{
    int val = 0;
    bool ready = false;

    recursive_mutex cvLok;
    condition_variable_any cv;

    thread t([&](){
        unique_lock<recursive_mutex> g(cvLok);
        cv.wait(g, [&](){return ready;}); // return true when external condition is true.
        val=42;
    });

    ck_usleep(1000);

    UT_ASSERT(val == 0);

    {
        unique_lock<recursive_mutex> g(cvLok);
        ready = true;
    }
    cv.notify_one();

    t.join();

    UT_ASSERT(val == 42);
}

void condition_variable_test::test_wait_until()
{
    recursive_mutex cvLok;
    condition_variable_any cv;
    bool ready = false;

    system_clock::time_point start_time = system_clock::now();

    thread t([&](){
        unique_lock<recursive_mutex> g(cvLok);

        cv.wait_until(g, system_clock::now()+milliseconds(100),
                      [&](){return ready;}); // return true when external condition is true.
    });

    t.join();

    system_clock::time_point end_time = system_clock::now();

    UT_ASSERT( duration_cast<milliseconds>(end_time-start_time).count() >= milliseconds(100).count() );
}

void condition_variable_test::test_wait_for()
{
    recursive_mutex cvLok;
    condition_variable_any cv;
    bool ready = false;

    system_clock::time_point start_time = system_clock::now();

    thread t([&](){
        unique_lock<recursive_mutex> g(cvLok);
        cv.wait_for(g, milliseconds(100),
                    [&](){return ready;}); // return true when external condition is true.
    });

    t.join();

    system_clock::time_point end_time = system_clock::now();

    UT_ASSERT( duration_cast<milliseconds>(end_time-start_time).count() >= milliseconds(100).count() );
}
