
#include "framework.h"

class hyperhttp_client_server_test : public test_fixture
{
public:
    TEST_SUITE(hyperhttp_client_server_test);
        TEST(hyperhttp_client_server_test::test_get);
        TEST(hyperhttp_client_server_test::test_post);
        TEST(hyperhttp_client_server_test::test_chunked_response_lambda);
        TEST(hyperhttp_client_server_test::test_chunked_response_bind);
        TEST(hyperhttp_client_server_test::test_multipart_response_lambda);
        TEST(hyperhttp_client_server_test::test_post_with_vars);
    TEST_SUITE_END();

    virtual ~hyperhttp_client_server_test() throw() {}

    void setup() {}
    void teardown() {}

    void test_get();
    void test_post();
    void test_chunked_response_lambda();
    void test_chunked_response_bind();
    void test_multipart_response_lambda();
    void test_post_with_vars();
};
