
#include "framework.h"

class ck_memory_test : public test_fixture
{
public:
    TEST_SUITE(ck_memory_test);
        TEST(ck_memory_test::test_default_constructor);
        TEST(ck_memory_test::test_sized_constructor);
        TEST(ck_memory_test::test_copy_constructor);
        TEST(ck_memory_test::test_assignment_operator);
        TEST(ck_memory_test::test_map);
        TEST(ck_memory_test::test_size);
        TEST(ck_memory_test::test_resize);
        TEST(ck_memory_test::test_extend_data);
        TEST(ck_memory_test::test_set_data_size);
        TEST(ck_memory_test::test_map_data);
        TEST(ck_memory_test::test_size_data);
    TEST_SUITE_END();

    virtual ~ck_memory_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_default_constructor();
    void test_sized_constructor();
    void test_copy_constructor();
    void test_assignment_operator();
    void test_map();
    void test_size();
    void test_resize();
    void test_extend_data();
    void test_set_data_size();
    void test_map_data();
    void test_size_data();
};
