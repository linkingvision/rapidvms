#ifndef ck_udp_sender_test_h
#define ck_udp_sender_test_h

#include "framework.h"

#ifdef IS_WINDOWS
#include <winsock2.h>
#endif

#include "cppkit/ck_udp_sender.h"

class ck_udp_sender_test : public test_fixture
{
public:
    TEST_SUITE(ck_udp_sender_test);
        TEST(ck_udp_sender_test::test_send);
        TEST(ck_udp_sender_test::test_aim);
        TEST(ck_udp_sender_test::test_get_set_send_buffer_size);
    TEST_SUITE_END();

    virtual ~ck_udp_sender_test() throw()
    {}

    void setup();
    void teardown();

protected:

    void test_send();
    void test_aim();
    void test_get_set_send_buffer_size();

private:
    int _val;
    cppkit::ck_string _recvAddress;
};

#endif
