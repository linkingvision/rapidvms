#ifndef ck_udp_receiver_test_h
#define ck_udp_receiver_test_h

#include "framework.h"

#ifdef WIN32
#include <winsock2.h>
#endif // WIN32

#include "cppkit/ck_string.h"
#include "cppkit/ck_udp_receiver.h"
#include "cppkit/ck_memory.h"
#include <memory>

class ck_udp_receiver_test : public test_fixture
{
public:
    TEST_SUITE(ck_udp_receiver_test);
        TEST(ck_udp_receiver_test::test_send);
        TEST(ck_udp_receiver_test::test_send_multicast);
        TEST(ck_udp_receiver_test::test_associated_send);
        TEST(ck_udp_receiver_test::test_get_set_recv_buffer_size);
        TEST(ck_udp_receiver_test::test_shutdown_close_while_blocked);
        TEST(ck_udp_receiver_test::test_raw_recv);
        TEST(ck_udp_receiver_test::test_shutdown_close_while_blocked_in_raw_recv);
    TEST_SUITE_END();

    virtual ~ck_udp_receiver_test() throw()
    {}

    void setup();
    void teardown();

protected:
    void test_send();
    void test_send_multicast();
    void test_associated_send();
    void test_get_set_recv_buffer_size();
    void test_shutdown_close_while_blocked();
    void test_raw_recv();
    void test_shutdown_close_while_blocked_in_raw_recv();

private:
    int _val;
    cppkit::ck_string _recvAddress;
    cppkit::ck_string _interfaceAddress;
    std::shared_ptr<cppkit::ck_udp_receiver> _receiver;

};

#endif
