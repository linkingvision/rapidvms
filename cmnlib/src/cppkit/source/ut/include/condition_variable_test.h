
#include "framework.h"

class condition_variable_test : public test_fixture
{
public:

    TEST_SUITE(condition_variable_test);
      TEST(condition_variable_test::test_simple);
      TEST(condition_variable_test::test_wait_until);
      TEST(condition_variable_test::test_wait_for);
    TEST_SUITE_END();

    virtual ~condition_variable_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_simple();
    void test_wait_until();
    void test_wait_for();
};
