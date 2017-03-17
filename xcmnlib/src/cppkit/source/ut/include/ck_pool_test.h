
#include "framework.h"

class ck_pool_test : public test_fixture
{
public:
    TEST_SUITE(ck_pool_test);
        TEST(ck_pool_test::test_constructor);
        TEST(ck_pool_test::test_get);
        TEST(ck_pool_test::test_deleter_transfer);
        TEST(ck_pool_test::test_empty);
    TEST_SUITE_END();

    virtual ~ck_pool_test() throw() {}

    void setup() {}
    void teardown() {}

    void test_constructor();
    void test_get();
    void test_deleter_transfer();
    void test_empty();
};
