
#include "ck_udp_sender_test.h"

#include "cppkit/ck_udp_sender.h"
#include "cppkit/ck_udp_receiver.h"
#include "cppkit/os/ck_time_utils.h"

#ifdef IS_LINUX
#include <unistd.h>
#endif

#include <thread>

using namespace cppkit;
using namespace std;

REGISTER_TEST_FIXTURE(ck_udp_sender_test);

void ck_udp_sender_test::setup()
{
}

void ck_udp_sender_test::teardown()
{
}

void ck_udp_sender_test::test_send()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for( int ii=0; ii<2; ++ii)
    {
        _recvAddress = ck_socket_address::get_address_family( addrs[ii] ) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        thread t([&](){

            shared_ptr<ck_memory> buffer = make_shared<ck_memory>();

            ck_udp_receiver server( 7474, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress );

            int destinationPort = 0;

            server.receive( destinationPort, buffer );

            ck_byte_ptr reader = buffer->map();
            _val = reader.consume<int>();
        });
        t.detach();

        ck_usleep( 500000 );

        shared_ptr<ck_memory> buffer = make_shared<ck_memory>();
        ck_byte_ptr writer = buffer->extend_data( 4 );
        writer.write<int>( 42 );

        ck_udp_sender client( addrs[ii], 7474 );

        client.send( buffer->map().get_ptr(), buffer->size_data() );

        ck_usleep( 250000 );

        UT_ASSERT( _val == 42 );
    }
}

void ck_udp_sender_test::test_aim()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for( int ii=0; ii<2; ++ii )
    {
        _recvAddress = ck_socket_address::get_address_family( addrs[ii]) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        thread t([&](){
            shared_ptr<ck_memory> buffer = make_shared<ck_memory>();

            ck_udp_receiver server( 7575, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress );

            int destinationPort = 0;

            server.receive( destinationPort, buffer );

            ck_byte_ptr reader = buffer->map();
            _val = reader.consume<int>();
        });
        t.detach();

        ck_usleep( 500000 );

        shared_ptr<ck_memory> buffer = make_shared<ck_memory>();
        ck_byte_ptr writer = buffer->extend_data( 4 );
        writer.write<int>( 43 );

        // Inititially, we're aimed at some random machine and port...
        ck_udp_sender client( "192.168.1.0", 1234 );

        // Then we re-aim...
        client.aim( addrs[ii], 7575 );

        client.send( buffer->map().get_ptr(), buffer->size_data() );

        ck_usleep( 250000 );

        UT_ASSERT( _val == 43 );
    }
}

void ck_udp_sender_test::test_get_set_send_buffer_size()
{
    {
        ck_udp_sender s("127.0.0.1", 0);
        UT_ASSERT_NO_THROW( s.get_send_buffer_size() );
        UT_ASSERT_NO_THROW( s.set_send_buffer_size(256 * 1024) );
    }
    {
        ck_udp_sender s("127.0.0.1", 0);
        size_t sizeOrig, sizeFinal;
        UT_ASSERT_NO_THROW( sizeOrig = s.get_send_buffer_size() );
        UT_ASSERT_NO_THROW( s.set_send_buffer_size( sizeOrig * 2 ) );
        UT_ASSERT_NO_THROW( sizeFinal = s.get_send_buffer_size() );
        UT_ASSERT( sizeFinal > sizeOrig );
    }
}
