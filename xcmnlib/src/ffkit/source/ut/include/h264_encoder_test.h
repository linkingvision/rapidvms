
#include "framework.h"
#include <memory>
#include "ffkit/av_packet.h"

class h264_encoder_test : public test_fixture
{
public:
    TEST_SUITE(h264_encoder_test);
        TEST(h264_encoder_test::test_constructor);
        TEST(h264_encoder_test::test_encode_key);
        TEST(h264_encoder_test::test_encode_gop);
    TEST_SUITE_END();

    virtual ~h264_encoder_test() throw() {}

    void setup();
    void teardown();

    void test_constructor();
    void test_encode_key();
    void test_encode_gop();

private:
    std::shared_ptr<ffkit::av_packet> _pic;
};
