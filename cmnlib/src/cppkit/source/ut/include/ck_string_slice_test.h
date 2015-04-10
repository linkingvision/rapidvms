
#ifndef string_slice_test_h
#define string_slice_test_h

#include "framework.h"

class ck_string_slice_test : public test_fixture
{
public:
    TEST_SUITE(ck_string_slice_test);
        TEST(ck_string_slice_test::test_constructor);
        TEST(ck_string_slice_test::test_constructor_unicode);
        TEST(ck_string_slice_test::test_copy_constructor);
        TEST(ck_string_slice_test::test_copy_constructor_unicode);
        TEST(ck_string_slice_test::test_assignment_operator);
        TEST(ck_string_slice_test::test_assignment_operator_unicode);
        TEST(ck_string_slice_test::test_equality_operator);
        TEST(ck_string_slice_test::test_inequality_operator);
        TEST(ck_string_slice_test::test_pop_x);
        TEST(ck_string_slice_test::test_iterators);
        TEST(ck_string_slice_test::test_starts_with);
        TEST(ck_string_slice_test::test_starts_with_unicode_version);
        TEST(ck_string_slice_test::test_ends_with);
        TEST(ck_string_slice_test::test_ends_with_unicode_version);
        TEST(ck_string_slice_test::test_to_string);
        TEST(ck_string_slice_test::test_slice);
        TEST(ck_string_slice_test::test_find);
        TEST(ck_string_slice_test::test_find_until);
    TEST_SUITE_END();

    virtual ~ck_string_slice_test() throw()
    {}

    void setup() {}
    void teardown() {}

protected:
    void test_constructor();
    void test_constructor_unicode();
    void test_copy_constructor();
    void test_copy_constructor_unicode();
    void test_assignment_operator();
    void test_assignment_operator_unicode();
    void test_equality_operator();
    void test_inequality_operator();
    void test_pop_x();
    void test_iterators();
    void test_starts_with();
    void test_starts_with_unicode_version();
    void test_ends_with();
    void test_ends_with_unicode_version();
    void test_to_string();
    void test_slice();
    void test_find();
    void test_find_until();
};

#endif

