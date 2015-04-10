
#include "framework.h"

class chrono_test : public test_fixture
{
public:

    TEST_SUITE(chrono_test);
      TEST(chrono_test::test_time_point_construction);
      TEST(chrono_test::test_duration_addition);
      TEST(chrono_test::test_duration_setup);
    TEST_SUITE_END();

    virtual ~chrono_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_time_point_construction();
    void test_duration_addition();
    void test_duration_setup();
};
