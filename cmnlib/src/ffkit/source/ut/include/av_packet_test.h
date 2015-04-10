
#include "framework.h"
#include <memory>
#include "cppkit/ck_memory.h"

class av_packet_test : public test_fixture
{
public:
    TEST_SUITE(av_packet_test);
        TEST(av_packet_test::test_size_ctor_owning);
        TEST(av_packet_test::test_foreign_buffer_ctor_owning);
        TEST(av_packet_test::test_foreign_buffer_ctor_non_owning);
        TEST(av_packet_test::test_copy_ctor_owning);
        TEST(av_packet_test::test_copy_ctor_non_owning);
        TEST(av_packet_test::test_move_ctor);
        TEST(av_packet_test::test_copy_assignment);
        TEST(av_packet_test::test_move_assignment);
    TEST_SUITE_END();

    virtual ~av_packet_test() throw() {}

    void setup();
    void teardown();

    void test_size_ctor_owning();
    void test_foreign_buffer_ctor_owning();
    void test_foreign_buffer_ctor_non_owning();
    void test_copy_ctor_owning();
    void test_copy_ctor_non_owning();
    void test_move_ctor();
    void test_copy_assignment();
    void test_move_assignment();
};
