
#include "recursive_mutex_test.h"

#include "cppkit/os/ck_time_utils.h"

#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(recursive_mutex_test);

void recursive_mutex_test::setup()
{
}

void recursive_mutex_test::teardown()
{
}

void recursive_mutex_test::test_simple()
{
    // On async thread:
    //    grab lock
    //    update val
    //    unlock lock
    //
    // On main:
    //    grab lock
    //    assert value set by thread
    //    release lock
    //
    // We use a condition_variable to signal from the async thread to the main thread that the value has
    // been updated.

    recursive_mutex cvLok;
    condition_variable_any cv;
    bool work_done=false;

    recursive_mutex lok;
    int val = 0;

    thread t([&](){
        lok.lock();
        val = 42;
        lok.unlock();

        {
            unique_lock<recursive_mutex> g(cvLok);
            work_done=true;
        }
        cv.notify_one();
    });

    {
        unique_lock<recursive_mutex> g(cvLok);
        cv.wait(g,[&](){return work_done;});
    }

    lok.lock();

    UT_ASSERT( val == 42 );

    lok.unlock();

    t.join();
}

void recursive_mutex_test::test_lock_twice()
{
    // This test utilizes a very short sleep (about 10ms) to let work be done on another thread. I don't
    // love this, but was unable to think of an alternative for this test. Open to suggestions!

    recursive_mutex lok;
    int val = 0;

    lok.lock();
    lok.lock();

    thread t([&](){
        lok.lock();
        val = 42;
        lok.unlock();
    });

    t.detach();

    ck_usleep(100000); // we sleep here long enough for above future to start.

    lok.unlock();

    UT_ASSERT(val == 0); // We locked it twice, so it should still be 0.

    lok.unlock(); // future should now run.

    ck_usleep(100000);

    UT_ASSERT(val == 42);
}

void recursive_mutex_test::test_lots_of_locks()
{
    // This test utilizes a very short sleep (about 1ms) to let work be done on another thread. I don't
    // love this, but was unable to think of an alternative for this test. Open to suggestions!

    recursive_mutex lok;
    int val = 0;

    lok.lock();

    thread t([&](){
        lok.lock();
        val = 42;
        lok.unlock();
    });

    t.detach();

    for( int i = 0; i < 999; i++ )
        lok.lock();

    // At this point, we've acquire the lock 1000 times total...

    UT_ASSERT( val == 0 );

    for( int i = 0; i < 999; i++ )
        lok.unlock();

    // At this point, we've released the lock 999 times...

    UT_ASSERT( val == 0 );

    lok.unlock(); // final release...

    ck_usleep( 100000 ); // give worker time to acquire lok and modify val...

    UT_ASSERT( val == 42 );

//    t.join();
}
