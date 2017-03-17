
#include "framework.h"

class ck_nullable_test : public test_fixture
{
public:
    TEST_SUITE(ck_nullable_test);
        TEST(ck_nullable_test::test_bool);
        TEST(ck_nullable_test::test_int);
        TEST(ck_nullable_test::test_ck_string);
        TEST(ck_nullable_test::test_shared_ptr);
    TEST_SUITE_END();

    virtual ~ck_nullable_test() throw() {}

    void setup() {}
    void teardown() {}

    void test_bool();
    void test_int();
    void test_ck_string();
    void test_shared_ptr();
};
