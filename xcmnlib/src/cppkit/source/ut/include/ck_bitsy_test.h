
#include "framework.h"

class ck_bitsy_test : public test_fixture
{
public:
    TEST_SUITE(ck_bitsy_test);
        TEST(ck_bitsy_test::test_constructor);
        TEST(ck_bitsy_test::test_set_buffer);
        TEST(ck_bitsy_test::test_reset);
        TEST(ck_bitsy_test::test_get_bits);
        TEST(ck_bitsy_test::test_get_exact_bits);
    TEST_SUITE_END();

    virtual ~ck_bitsy_test() throw() {}

    void setup() {}
    void teardown() {}

    void test_constructor();
    void test_set_buffer();
    void test_reset();
    void test_get_bits();
    void test_get_exact_bits();
};
