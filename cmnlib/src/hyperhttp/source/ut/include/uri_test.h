
#include "framework.h"

class uri_test : public test_fixture
{
public:
    TEST_SUITE(uri_test);
        TEST(uri_test::test_construct_uri);
        TEST(uri_test::test_set_full_raw_uri);
    TEST_SUITE_END();

    virtual ~uri_test() throw() {}

    void setup() {}
    void teardown() {}

    void test_construct_uri();

    void test_set_full_raw_uri();
};
