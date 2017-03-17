
#include "framework.h"

class url_test : public test_fixture
{
public:
    TEST_SUITE(url_test);
        TEST(url_test::test_construct);
        TEST(url_test::test_setters);
    TEST_SUITE_END();

    virtual ~url_test() throw() {}

    void setup() {}
    void teardown() {}

    void test_construct();
    void test_setters();
};
