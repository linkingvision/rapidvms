
#include "framework.h"
#include "ck_actor_test.h"
#include "cppkit/ck_actor.h"

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_actor_test);

/// adder_actor is an actor that add's the int's in the pair sent to it and returns
/// the result...

class adder_actor : public ck_actor<std::pair<int,int>,int>
{
public:
    virtual int process( const std::pair<int,int>& cmd ) override
    {
        return cmd.first + cmd.second;
    }
};

void ck_actor_test::setup()
{
}

void ck_actor_test::teardown()
{
}

void ck_actor_test::test_default_constructor()
{
    adder_actor a;
}

void ck_actor_test::test_start_stop()
{
    adder_actor a;
    a.start();
    UT_ASSERT( a.started() == true );
    a.stop();
    UT_ASSERT( a.started() == false );
}

void ck_actor_test::test_adder()
{
    adder_actor a;
    a.start();
    future<int> result = a.post( make_pair<int,int>( 21, 21 ) );
    result.wait();
    UT_ASSERT( result.get() == 42 );
    a.stop();
}
