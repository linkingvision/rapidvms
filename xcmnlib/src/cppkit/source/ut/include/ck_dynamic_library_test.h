#ifndef ck_dynamic_library_test_h
#define ck_dynamic_library_test_h

#include "framework.h"

#include "cppkit/ck_dynamic_library.h"

class ck_dynamic_library_test : public test_fixture
{
public:
    TEST_SUITE(ck_dynamic_library_test);
        TEST(ck_dynamic_library_test::test_load);
        TEST(ck_dynamic_library_test::test_resolve_symbol);
    TEST_SUITE_END();

    virtual ~ck_dynamic_library_test() throw()
    {}

    void setup();
    void teardown();

protected:
    void test_load();
    void test_resolve_symbol();
};

#endif
