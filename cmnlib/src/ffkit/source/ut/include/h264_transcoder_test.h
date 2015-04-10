
#include "framework.h"
#include <memory>
#include "cppkit/ck_memory.h"

class h264_transcoder_test : public test_fixture
{
public:
    TEST_SUITE(h264_transcoder_test);
        TEST(h264_transcoder_test::test_constructor);
        TEST(h264_transcoder_test::test_basic);
    TEST_SUITE_END();

    virtual ~h264_transcoder_test() throw() {}

    void setup();
    void teardown();

    void test_constructor();
    void test_basic();
};
