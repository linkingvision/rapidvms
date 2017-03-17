
#include "framework.h"
#include <memory>
#include "ffkit/av_packet.h"

class jpeg_encoder_test : public test_fixture
{
public:
    TEST_SUITE(jpeg_encoder_test);
        TEST(jpeg_encoder_test::test_constructor);
        TEST(jpeg_encoder_test::test_encode);
    TEST_SUITE_END();

    virtual ~jpeg_encoder_test() throw() {}

    void setup();
    void teardown();

    void test_constructor();
    void test_encode();
private:
    std::shared_ptr<ffkit::av_packet> _pic;
};
