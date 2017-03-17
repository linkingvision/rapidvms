
#include "framework.h"
#include <memory>
#include "cppkit/ck_memory.h"

class h264_decoder_test : public test_fixture
{
public:
    TEST_SUITE(h264_decoder_test);
        TEST(h264_decoder_test::test_constructor);
        TEST(h264_decoder_test::test_input_dimensions);
        TEST(h264_decoder_test::test_output_dimensions);
        TEST(h264_decoder_test::test_decode_gop);
    TEST_SUITE_END();

    virtual ~h264_decoder_test() throw() {}

    void setup();
    void teardown();

    void test_constructor();
    void test_input_dimensions();
    void test_output_dimensions();
    void test_decode_gop();
};
