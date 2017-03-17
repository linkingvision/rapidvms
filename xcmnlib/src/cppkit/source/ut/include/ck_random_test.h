
#include "framework.h"

class ck_random_test : public test_fixture
{
public:
    TEST_SUITE(ck_random_test);
        TEST(ck_random_test::test_random_device);
        TEST(ck_random_test::test_mersenne_twister);
        TEST(ck_random_test::test_mersenne_twister_with_random_device_seed);
        TEST(ck_random_test::test_mersenne_twister_64);
        TEST(ck_random_test::test_mersenne_twister_with_floating_point_distribution);
    TEST_SUITE_END();

    virtual ~ck_random_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_random_device();
    void test_mersenne_twister();
    void test_mersenne_twister_with_random_device_seed();
    void test_mersenne_twister_64();
    void test_mersenne_twister_with_floating_point_distribution();
};
