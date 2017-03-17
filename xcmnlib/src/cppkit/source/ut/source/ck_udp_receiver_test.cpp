
#include "ck_udp_receiver_test.h"

#include "cppkit/ck_udp_sender.h"
#include "cppkit/ck_udp_receiver.h"
#include "cppkit/os/ck_time_utils.h"
#include "cppkit/ck_socket.h"

#include <unordered_map>
#include <thread>

#ifdef IS_LINUX
#include <unistd.h>
#endif

using namespace cppkit;
using namespace std;

REGISTER_TEST_FIXTURE(ck_udp_receiver_test);

void ck_udp_receiver_test::setup()
{
    _recvAddress.clear();
    ck_socket::socket_startup();
}

void ck_udp_receiver_test::teardown()
{
}

void ck_udp_receiver_test::test_send()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for( int ii=0; ii<2; ++ii )
    {
        _recvAddress = ck_socket_address::get_address_family( addrs[ii] ) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        thread t([&](){
            shared_ptr<ck_memory> buffer = make_shared<ck_memory>();

            ck_udp_receiver server( 8484, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress );

            int destinationPort = 0;

            server.receive( destinationPort, buffer );

            ck_byte_ptr reader = buffer->map();

            _val = reader.consume<int>();
        });
        t.detach();

        ck_usleep( 500000 );

        shared_ptr<ck_memory> buffer = make_shared<ck_memory>();
        buffer->clear();

        ck_byte_ptr writer = buffer->extend_data( 4 );

        writer.write<int>(42);

        ck_udp_sender client( addrs[ii], 8484 );

        client.send( buffer->map().get_ptr(), buffer->size_data() );

        ck_usleep( 250000 );

        UT_ASSERT( _val == 42 );
    }
}

void ck_udp_receiver_test::test_send_multicast()
{
    unordered_map<string, vector<ck_string>> interfaceAddresses = ck_socket::get_interface_addresses( AF_INET );

    ck_string ipAddress;

#ifdef IS_WINDOWS
    // The bamboo test boxes don't always seem to give us "Local Area Connection"
    // and have at least 2 - 4.
    if( interfaceAddresses.find( "Local Area Connection") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["Local Area Connection"].front();
    else if( interfaceAddresses.find("Local Area Connection 1") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["Local Area Connection 1"].front();
    else if( interfaceAddresses.find("Local Area Connection 2") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["Local Area Connection 2"].front();
    else if( interfaceAddresses.find("Local Area Connection 3") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["Local Area Connection 3"].front();
    else if( interfaceAddresses.find("Local Area Connection 4") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["Local Area Connection 4"].front();
    else if( interfaceAddresses.find("Local Area Connection 5") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["Local Area Connection 5"].front();
    else if( interfaceAddresses.find("Local Area Connection 6") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["Local Area Connection 6"].front();
    else if( interfaceAddresses.find("Local Area Connection* 15") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["Local Area Connection* 15"].front();
    else CK_THROW(( "Unknown interface name." ));
#else
    if( interfaceAddresses.find("eth0") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["eth0"].front();
    else if( interfaceAddresses.find("p5p1") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["p5p1"].front();
    else if( interfaceAddresses.find("p2p1") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["p2p1"].front();
    else if( interfaceAddresses.find("wlan0") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["wlan0"].front();
    else if( interfaceAddresses.find("em0") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["em0"].front();
    else if( interfaceAddresses.find("em1") != interfaceAddresses.end() )
        ipAddress = interfaceAddresses["em1"].front();
    else if( interfaceAddresses.find("venet0:0")!=interfaceAddresses.end() )
        ipAddress = interfaceAddresses["venet0:0"].front();
    else if( interfaceAddresses.find("venet0")!=interfaceAddresses.end() )
        ipAddress = interfaceAddresses["venet0"].front();
    else CK_THROW(( "Unknown interface name." ));
#endif

    vector<ck_string> addressParts = ipAddress.split( "." );

    ck_string localMulticastAddress = ck_string::format( "239.%s.%s.%s",
                                                         addressParts[1].c_str(),
                                                         addressParts[2].c_str(),
                                                         addressParts[3].c_str() );

    ck_string addrs[] = { localMulticastAddress };

    _recvAddress = addrs[0];
    _val = 0;

    thread t([&](){
        shared_ptr<ck_memory> buffer = make_shared<ck_memory>();

        // Bind to a specific interface
        ck_udp_receiver server( 8686, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress );

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

    ck_udp_sender client( addrs[0], 8686 );

    client.send( buffer->map().get_ptr(), buffer->size_data() );

    ck_usleep( 250000 );

    UT_ASSERT( _val == 42 );
}

void ck_udp_receiver_test::test_associated_send()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for( int ii=0; ii<2; ++ii )
    {
        _recvAddress = ck_socket_address::get_address_family( addrs[ii] ) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        thread t([&](){
            shared_ptr<ck_memory> buffer = make_shared<ck_memory>();

            // Bind to a specific interface
            shared_ptr<ck_udp_receiver> serverA = make_shared<ck_udp_receiver>( 8787, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress );
            shared_ptr<ck_udp_receiver> serverB = make_shared<ck_udp_receiver>( 8888, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress );

            serverA->associate( serverB );

            int destinationPort = 0;

            serverA->receive( destinationPort, buffer );

            UT_ASSERT( destinationPort == 8787 );

            ck_byte_ptr reader = buffer->map();

            _val += reader.consume<int>();

            serverA->receive( destinationPort, buffer );

            reader = buffer->map();

            _val += reader.consume<int>();

            UT_ASSERT(destinationPort == 8888);
        });
        t.detach();

        ck_usleep(500000);

        shared_ptr<ck_memory> buffer = make_shared<ck_memory>();
        ck_byte_ptr writer = buffer->extend_data( 4 );
        writer.write<int>( 42 );

        ck_udp_sender clientA( addrs[ii], 8787 );

        clientA.send( buffer->map().get_ptr(), buffer->size_data() );

        ck_udp_sender clientB( addrs[ii], 8888 );

        clientB.send( buffer->map().get_ptr(), buffer->size_data() );

        ck_usleep(100000);

        UT_ASSERT( _val == 84 );
    }
}

void ck_udp_receiver_test::test_get_set_recv_buffer_size()
{
    {
        ck_udp_receiver r;
        UT_ASSERT_NO_THROW( r.get_recv_buffer_size() );
        UT_ASSERT_NO_THROW( r.set_recv_buffer_size(256 * 1024) );
    }
    {
        ck_udp_receiver r;
        size_t sizeOrig, sizeFinal;
        UT_ASSERT_NO_THROW( sizeOrig = r.get_recv_buffer_size() );
        UT_ASSERT_NO_THROW( r.set_recv_buffer_size( sizeOrig * 2 ) );
        UT_ASSERT_NO_THROW( sizeFinal = r.get_recv_buffer_size() );
        UT_ASSERT( sizeFinal > sizeOrig );
    }
}

void ck_udp_receiver_test::test_shutdown_close_while_blocked()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for( int ii=0; ii<2; ++ii )
    {
        _recvAddress = ck_socket_address::get_address_family( addrs[ii] ) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        _receiver = make_shared<ck_udp_receiver>( 8989, DEFAULT_UDP_RECV_BUF_SIZE, addrs[ii] );

        thread t([&](){
            shared_ptr<ck_memory> buffer = make_shared<ck_memory>( 1600 );

            int destinationPort = 0;

            _receiver->receive( destinationPort, buffer );

            _val = 1234;
        });
        t.detach();

        ck_usleep(1000000);

        _receiver->shutdown();
        _receiver->close();

        ck_usleep( 100000 );

        UT_ASSERT( _val == 1234 );
    }
}

void ck_udp_receiver_test::test_raw_recv()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for( int ii=0; ii<2; ++ii )
    {
        _recvAddress = ck_socket_address::get_address_family( addrs[ii] ) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        thread t([&](){
            shared_ptr<ck_memory> buffer = make_shared<ck_memory>();

            ck_udp_receiver server( 9090, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress );

            int destinationPort = 0;

            server.raw_receive( destinationPort, buffer );

            ck_byte_ptr reader = buffer->map();

            _val = reader.consume<int>();
        });
        t.detach();

        ck_usleep( 500000 );

        shared_ptr<ck_memory> buffer = make_shared<ck_memory>();
        ck_byte_ptr writer = buffer->extend_data( 4 );
        writer.write<int>( 24 );

        ck_udp_sender client( addrs[ii], 9090 );

        client.send( buffer->map().get_ptr(), buffer->size_data() );

        ck_usleep(100000);

        UT_ASSERT( _val == 24 );
    }
}

void ck_udp_receiver_test::test_shutdown_close_while_blocked_in_raw_recv()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for( int ii=0; ii<2; ++ii )
    {
        _recvAddress = ck_socket_address::get_address_family( addrs[ii] ) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        _receiver = make_shared<ck_udp_receiver>( 9191, DEFAULT_UDP_RECV_BUF_SIZE, addrs[ii] );

        thread t([&](){
            shared_ptr<ck_memory> buffer = make_shared<ck_memory>( 1600 );

            int destinationPort = 0;

            _receiver->raw_receive( destinationPort, buffer );

            _val = 4321;
        });
        t.detach();

        ck_usleep( 500000 );

        _receiver->close();

        ck_usleep( 1000000 );

        UT_ASSERT( _val == 4321 );
    }
}
