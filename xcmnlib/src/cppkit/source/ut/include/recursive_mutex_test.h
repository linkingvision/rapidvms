
#include "framework.h"

class recursive_mutex_test : public test_fixture
{
public:

    TEST_SUITE(recursive_mutex_test);
      TEST(recursive_mutex_test::test_simple);
      TEST(recursive_mutex_test::test_lock_twice);
      TEST(recursive_mutex_test::test_lots_of_locks);
    TEST_SUITE_END();

    virtual ~recursive_mutex_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_simple();
    void test_lock_twice();
    void test_lots_of_locks();
};
