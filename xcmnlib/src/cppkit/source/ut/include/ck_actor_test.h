
#include "framework.h"

class ck_actor_test : public test_fixture
{
public:
    TEST_SUITE(ck_actor_test);
        TEST(ck_actor_test::test_default_constructor);
        TEST(ck_actor_test::test_start_stop);
        TEST(ck_actor_test::test_adder);
    TEST_SUITE_END();

    virtual ~ck_actor_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_default_constructor();
    void test_start_stop();
    void test_adder();
};
