
#ifndef ck_variant_test_h
#define ck_variant_test_h

#include "framework.h"

class ck_variant_test : public test_fixture
{
public:
    TEST_SUITE(ck_variant_test);
        TEST(ck_variant_test::test_default_and_copy_constructor);
        TEST(ck_variant_test::test_conversions);
        TEST(ck_variant_test::test_assignment_operators);
        TEST(ck_variant_test::test_cast_operators);
        TEST(ck_variant_test::test_clear);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:
    void test_default_and_copy_constructor();
    void test_conversions();
    void test_assignment_operators();
    void test_cast_operators();
    void test_clear();
};

#endif
