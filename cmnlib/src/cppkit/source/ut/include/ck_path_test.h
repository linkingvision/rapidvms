#ifndef ck_path_test_h
#define ck_path_test_h

#include "framework.h"

class ck_path_test : public test_fixture
{
public:
    TEST_SUITE(ck_path_test);
        TEST(ck_path_test::test_constructor);
        TEST(ck_path_test::test_open_path);
        TEST(ck_path_test::test_is_path);
        TEST(ck_path_test::test_is_reg);
        TEST(ck_path_test::test_exists);
        TEST(ck_path_test::test_iter);
        TEST(ck_path_test::test_file_size);
    TEST_SUITE_END();

    virtual ~ck_path_test() throw()
    {}

    void setup();
    void teardown();

protected:

    void test_constructor();
    void test_open_path();
    void test_is_path();
    void test_is_reg();
    void test_exists();
    void test_iter();
    void test_file_size();

private:
};

#endif
