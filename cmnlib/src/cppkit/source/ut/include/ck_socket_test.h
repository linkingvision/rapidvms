
#include "framework.h"

#include "cppkit/ck_string.h"

class ck_socket_test : public test_fixture
{
public:

    TEST_SUITE(ck_socket_test);
      TEST(ck_socket_test::test_bind_ephemeral);
      TEST(ck_socket_test::test_connect);
      TEST(ck_socket_test::test_dual_protocol_server);
      TEST(ck_socket_test::test_get_hostname);
      TEST(ck_socket_test::test_get_addresses_by_hostname);
      TEST(ck_socket_test::test_get_interface_addresses);
    TEST_SUITE_END();

    virtual ~ck_socket_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_bind_ephemeral();
    void test_connect();
    void test_dual_protocol_server();
    void test_get_hostname();
    void test_get_addresses_by_hostname();
    void test_get_interface_addresses();
};
