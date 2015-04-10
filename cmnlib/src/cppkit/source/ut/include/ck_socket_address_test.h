
#include "framework.h"

#include "cppkit/ck_string.h"

class ck_socket_address_test : public test_fixture
{
public:

    TEST_SUITE(ck_socket_address_test);
      TEST(ck_socket_address_test::test_constructor);
      TEST(ck_socket_address_test::test_get_set_port_num);
      TEST(ck_socket_address_test::test_get_set_address);
      TEST(ck_socket_address_test::test_get_address_family);
      TEST(ck_socket_address_test::test_sock_addr_cast);
      TEST(ck_socket_address_test::test_comparisons);
      TEST(ck_socket_address_test::test_static_get_address_family);
      TEST(ck_socket_address_test::test_static_get_sock_addr_size);
      TEST(ck_socket_address_test::test_static_address_to_string);
      TEST(ck_socket_address_test::test_static_string_to_address);
      TEST(ck_socket_address_test::test_isolate_address);
      TEST(ck_socket_address_test::test_address_type_methods);
      TEST(ck_socket_address_test::test_is_wildcard_address);
      TEST(ck_socket_address_test::test_is_hostname);
    TEST_SUITE_END();

    virtual ~ck_socket_address_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_constructor();
    void test_get_set_port_num();
    void test_get_set_address();
    void test_get_address_family();
    void test_sock_addr_cast();
    void test_comparisons();
    void test_static_get_address_family();
    void test_static_get_sock_addr_size();
    void test_static_address_to_string();
    void test_static_string_to_address();
    void test_isolate_address();
    void test_address_type_methods();
    void test_is_wildcard_address();
    void test_is_hostname();
};
