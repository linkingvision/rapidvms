
#include "framework.h"
#include "ck_random_test.h"
#include "cppkit/ck_types.h"
#include <random>

using namespace std;

REGISTER_TEST_FIXTURE(ck_random_test);

void ck_random_test::setup()
{
}

void ck_random_test::teardown()
{
}

/// Note: we're not actually really doing any testing here. The point here is concise example code that
/// exercises a new feature of C++11.

void ck_random_test::test_random_device()
{
    // std::random_device is a uniformly-distributed integer random number generator that produces
    // non-deterministic random numbers. Note that std::random_device may be implemented in terms of a
    // pseudo-random number engine if a non-deterministic source (e.g. a hardware device) is not
    // available to the implementation.

    random_device rd; // creating RNGs might be slow. random_device() is particularly slow.

    int val = rd(); // random_device uses the OS's RNG and might block (behaves like reading from
                    // /dev/random on linux.
}

void ck_random_test::test_mersenne_twister()
{
    // declare a mersenne twister rng with seed 1234
    mt19937 mersenne_twister_rng( 1234 );

    // we want a uniform distribution of values between 0 and 99 inclusive.
    uniform_int_distribution<int> uid( 0, 99 ); // Note: both values are inclusive.

    int val = uid( mersenne_twister_rng );
}

void ck_random_test::test_mersenne_twister_with_random_device_seed()
{
    random_device rd;

    // declare a mersenne twister rng with seed from the OS's entropy pool
    mt19937 mersenne_twister_rng( rd() );

    // we want a uniform distribution of values between 0 and 99 inclusive.
    uniform_int_distribution<int> uid( 0, 99 ); // Note: both values are inclusive.

    int val = uid( mersenne_twister_rng );
}

void ck_random_test::test_mersenne_twister_64()
{
    // declare a mersenne twister rng with seed 1234
    mt19937_64 mersenne_twister_64_rng( 1234 );

    // we want a uniform distribution of values between 0 and 99 inclusive.
    uniform_int_distribution<uint64_t> uid( 0, 99 ); // Note: both values are inclusive.

    uint64_t val = uid( mersenne_twister_64_rng );
}

void ck_random_test::test_mersenne_twister_with_floating_point_distribution()
{
    // declare a mersenne twister rng with seed 1234
    mt19937 mersenne_twister_rng( 1234 );

    // we want a uniform distribution of values between 0 and 99 inclusive.
    uniform_real_distribution<> urd( 0, 1 ); // Note: both values are inclusive.

    float fval = (float)urd( mersenne_twister_rng );
    double dval = urd( mersenne_twister_rng );
}
