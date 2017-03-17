
#include "framework.h"

class av_demuxer_test : public test_fixture
{
public:
    TEST_SUITE(av_demuxer_test);
        TEST(av_demuxer_test::test_constructor);
        TEST(av_demuxer_test::test_examine_file);
        TEST(av_demuxer_test::test_file_from_memory);
        TEST(av_demuxer_test::test_get_container_statistics);
    TEST_SUITE_END();

    virtual ~av_demuxer_test() throw() {}

    void setup();
    void teardown();

    void test_constructor();
    void test_examine_file();
    void test_file_from_memory();
    void test_get_container_statistics();
};
