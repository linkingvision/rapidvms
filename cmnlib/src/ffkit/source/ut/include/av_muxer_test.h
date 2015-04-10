
#include "framework.h"
#include <memory>
#include "cppkit/ck_memory.h"
class av_muxer_test : public test_fixture
{
public:
    TEST_SUITE(av_muxer_test);
        TEST(av_muxer_test::test_constructor);
        TEST(av_muxer_test::test_mp4);
        TEST(av_muxer_test::test_recontainerize);
    TEST_SUITE_END();

    virtual ~av_muxer_test() throw() {}

    void setup();
    void teardown();

    void test_constructor();
    void test_mp4();
    void test_recontainerize();
private:
    std::shared_ptr<cppkit::ck_memory> _pic;
};
