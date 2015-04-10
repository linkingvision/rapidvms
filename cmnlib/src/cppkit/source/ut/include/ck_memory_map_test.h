#ifndef _ck_memory_map_test_H_
#define _ck_memory_map_test_H_

#include "framework.h"

class ck_memory_map_test : public test_fixture
{
public:
    TEST_SUITE(ck_memory_map_test);
        TEST(ck_memory_map_test::test_file_mapping);
        TEST(ck_memory_map_test::test_persist_to_disk);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:

    void test_file_mapping();
    void test_persist_to_disk();
};

#endif
