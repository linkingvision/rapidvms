
#include "framework.h"

class ck_statistics_test : public test_fixture
{
public:
    TEST_SUITE(ck_statistics_test);
        TEST(ck_statistics_test::test_constructor);
        TEST(ck_statistics_test::test_average);
        TEST(ck_statistics_test::test_median);
        TEST(ck_statistics_test::test_max_samples);
        TEST(ck_statistics_test::test_sample_timeout);
    TEST_SUITE_END();

    virtual ~ck_statistics_test() throw() {}

    void setup() {}
    void teardown() {}

    void test_constructor();
    void test_average();
    void test_median();
    void test_max_samples();
    void test_sample_timeout();
};
