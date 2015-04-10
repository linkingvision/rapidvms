
#include "framework.h"

#include "cppkit/ck_string.h"

class ck_large_files_test : public test_fixture
{
public:

    TEST_SUITE(ck_large_files_test);
      TEST(ck_large_files_test::test_ascii_file);
      TEST(ck_large_files_test::test_unicode_file);
      TEST(ck_large_files_test::test_seek_tell);
      TEST(ck_large_files_test::test_pre_allocated_file);
    TEST_SUITE_END();

    virtual ~ck_large_files_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_ascii_file();
    void test_unicode_file();
    void test_seek_tell();
    void test_pre_allocated_file();
};
