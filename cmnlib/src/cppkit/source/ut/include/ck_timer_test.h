
#include "framework.h"

class ck_timer_test : public test_fixture
{
public:
    TEST_SUITE(ck_timer_test);
        TEST(ck_timer_test::test_repeating);
    TEST_SUITE_END();

    virtual ~ck_timer_test() throw() {}

    void setup() {}
    void teardown() {}

    void test_repeating();
};
