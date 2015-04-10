
#include "framework.h"

class ck_command_queue_test : public test_fixture
{
public:
    TEST_SUITE(ck_command_queue_test);
        TEST(ck_command_queue_test::test_default_constructor);
        TEST(ck_command_queue_test::test_start_stop);
        TEST(ck_command_queue_test::test_full_queue);
    TEST_SUITE_END();

    virtual ~ck_command_queue_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_default_constructor();
    void test_start_stop();
    void test_full_queue();
};
