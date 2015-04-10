
#include "framework.h"

class ck_byte_ptr_test : public test_fixture
{
public:

    TEST_SUITE(ck_byte_ptr_test);
        TEST(ck_byte_ptr_test::test_constructor);
        TEST(ck_byte_ptr_test::test_get_set_ptr);
        TEST(ck_byte_ptr_test::test_inc_dec);
        TEST(ck_byte_ptr_test::test_pointer);
        TEST(ck_byte_ptr_test::test_add_sub);
        TEST(ck_byte_ptr_test::test_comparisons);
        TEST(ck_byte_ptr_test::test_casting);
        TEST(ck_byte_ptr_test::test_offset);
        TEST(ck_byte_ptr_test::test_seek);
        TEST(ck_byte_ptr_test::test_read_write);
        TEST(ck_byte_ptr_test::test_bulk_write);
    TEST_SUITE_END();

    virtual ~ck_byte_ptr_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_constructor();
    void test_get_set_ptr();
    void test_inc_dec();
    void test_pointer();
    void test_add_sub();
    void test_comparisons();
    void test_casting();
    void test_offset();
    void test_seek();
    void test_read_write();
    void test_bulk_write();
};
