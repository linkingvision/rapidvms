
#include "framework.h"
#include "ck_socket_test.h"
#include "cppkit/ck_socket.h"
#include "cppkit/os/ck_time_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_socket_test);

#define TRY_N_TIMES(a,b)     \
{                            \
    int tries = 0;           \
    while(!a && (tries < b)) \
    {                        \
        ck_usleep(100000);   \
        tries++;             \
    }                        \
}

void ck_socket_test::setup()
{
}

void ck_socket_test::teardown()
{
}

void ck_socket_test::test_bind_ephemeral()
{
    {
        ck_socket socket;
        int port = 0;
        UT_ASSERT_NO_THROW( port = socket.bind_ephemeral() );
        UT_ASSERT( port >= 1024 && port <= 65535 );
    }
    {
        ck_socket socket;
        int port = 0;
        UT_ASSERT_NO_THROW( port = socket.bind_ephemeral("127.0.0.1") );
        UT_ASSERT( port >= 1024 && port <= 65535 );
    }
}

void ck_socket_test::test_connect()
{
    const char addrs[][32] = { "127.0.0.1", "localhost", "::1" };

    int port = UT_NEXT_PORT();

    try
    {
        for (int i=0; i<3; ++i)
        {
            thread t([&](){
                ck_socket server_sok;
                server_sok.bind( port, addrs[i] );
                server_sok.listen();
                auto connected = server_sok.accept();
                unsigned int val = 0;
                connected->recv(&val, 4);
                val+=1;
                connected->send(&val, 4);
            });
            t.detach();

            unsigned int val = 41;
            ck_socket client_sok;

            TRY_N_TIMES(client_sok.query_connect(addrs[i], port , 1),10);

            // Test get_peer_ip()
            UT_ASSERT_NO_THROW( client_sok.get_peer_ip() );
            UT_ASSERT( client_sok.get_peer_ip() != "0.0.0.0" );

            // Test get_local_ip()
            UT_ASSERT_NO_THROW( client_sok.get_local_ip() );
            UT_ASSERT( client_sok.get_local_ip() != "0.0.0.0" );

            client_sok.send( &val, 4 );
            client_sok.set_recv_timeout( 500 );
            client_sok.recv( &val, 4 );
            UT_ASSERT( val == 42 );
        }
    }
    catch( cppkit::ck_exception& )
    {
        UT_ASSERT( false && "TestConnect threw!" );
    }
}

void ck_socket_test::test_dual_protocol_server()
{
    int port = UT_NEXT_PORT();

    try
    {
        // Test IPv4 only
        {
            ck_socket::ck_socket_type server_socket_type = ck_socket::IPV4;

            unsigned int val = 41;

            thread t([&](){
                ck_socket server_sok(server_socket_type);
                server_sok.bind(port, ip4_addr_any);

                server_sok.listen();
                auto connected = server_sok.accept();

                unsigned int val = 0;
                connected->recv( &val, 4 );
                val += 1;
                connected->send( &val, 4 );
            });

            t.detach();

            ck_socket client_sok;
            TRY_N_TIMES(client_sok.query_connect("127.0.0.1", port, 1), 10);

            // IPv4
            client_sok.send( &val, 4 );
            client_sok.set_recv_timeout( 500 );
            client_sok.recv( &val, 4 );
            UT_ASSERT( val == 42 );

            // IPv6
            UT_ASSERT_THROWS( client_sok.connect( "::1", port ), ck_socket_exception );
        }

        // Test IPv6 only
        {
            ck_socket::ck_socket_type server_socket_type = ck_socket::IPV6;
            unsigned int val = 41;

            thread t([&](){
                ck_socket server_sok(server_socket_type);
                server_sok.bind(port, ip6_addr_any);

                server_sok.listen();
                auto connected = server_sok.accept();

                unsigned int val = 0;
                connected->recv( &val, 4 );
                val += 1;
                connected->send( &val, 4 );
            });

            t.detach();

            ck_socket client_sok;

            // IPv4
            TRY_N_TIMES(client_sok.query_connect("::1", port, 1), 10);
            client_sok.send( &val, 4 );
            client_sok.set_recv_timeout( 500 );
            client_sok.recv( &val, 4 );
            UT_ASSERT( val == 42 );

            // IPv6
            UT_ASSERT_THROWS( client_sok.connect( "127.0.0.1", port ), ck_socket_exception );
        }

        // Test both IPv4 and IPv6
        // [tdistler] Windows XP doesn't support IPV6_V6ONLY option... detect XP and skip if necessary
        bool ipv6_only_supported = true;
//#ifdef IS_WINDOWS
//        OSVERSIONINFO version;
//        memset(&version, 0, sizeof(OSVERSIONINFO));
//        version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
//        GetVersionEx(&version);
//        if (version.dwMajorVersion < 6)
//            ipv6_only_supported = false; // XP or earlier
//#endif
        if (ipv6_only_supported)
        {
            const char addrs[][32] = { "127.0.0.1", "localhost", "::1" };

            // Keep creating the same server... it should accept both IPv6 and IPv4 traffic
            for (int ii=0; ii<3; ++ii)
            {
                ck_socket::ck_socket_type server_socket_type = ck_socket::IPV6_WITH_IPV4;
                unsigned int val = 41;

                thread t([&](){
                    ck_socket server_sok(server_socket_type);
                    if( server_socket_type == ck_socket::IPV4 )
                        server_sok.bind(port, ip4_addr_any);
                    else server_sok.bind(port, ip6_addr_any);

                    server_sok.listen();
                    auto connected = server_sok.accept();

                    unsigned int val = 0;
                    connected->recv( &val, 4 );
                    val += 1;
                    connected->send( &val, 4 );
                });

                t.detach();

                ck_socket client_sok;
                TRY_N_TIMES(client_sok.query_connect(addrs[ii], port, 1), 10);
                client_sok.send( &val, 4 );
                client_sok.set_recv_timeout( 500 );
                client_sok.recv( &val, 4 );
                UT_ASSERT( val == 42 );
            }
        }
        else
        {
            printf("  SKIPPING IPv6 w/ IPv4 (Not supported in Windows XP)\n");
            fflush(stdout);
        }
    }
    catch( ck_exception& )
    {
        UT_ASSERT( false && "TestDualProtocolServer threw!" );
    }
}

void ck_socket_test::test_get_hostname()
{
    ck_string hostname = ck_socket::get_hostname();

    UT_ASSERT( !hostname.empty() );
}

void ck_socket_test::test_get_addresses_by_hostname()
{
    vector<ck_string> addresses = ck_socket::get_addresses_by_hostname( "localhost" );
    UT_ASSERT( !addresses.empty() );
}

void ck_socket_test::test_get_interface_addresses()
{
    auto interfaceAddresses = ck_socket::get_interface_addresses();

#ifndef IS_WINDOWS
    ck_string localHostAddress = interfaceAddresses["lo"].front();

    ck_string ipAddress;
    if(interfaceAddresses.find("eth0")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["eth0"].front();
    else if(interfaceAddresses.find("eth1")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["eth1"].front();
    else if(interfaceAddresses.find("p5p1")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["p5p1"].front();
    else if(interfaceAddresses.find("p2p1")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["p2p1"].front();
    else if(interfaceAddresses.find("wlan0")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["wlan0"].front();
    else if(interfaceAddresses.find("venet0:0")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["venet0:0"].front();
    else if(interfaceAddresses.find("venet0")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["venet0"].front();

    UT_ASSERT(localHostAddress == "127.0.0.1");
    UT_ASSERT(!ipAddress.empty());
#else

    // The bamboo test boxes don't always seem to give us "Local Area Connection"
    // and have at least 2 - 4.
    ck_string ipAddress;
    if(interfaceAddresses.find("Local Area Connection")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["Local Area Connection"].front();
    else if(interfaceAddresses.find("Local Area Connection 1")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["Local Area Connection 1"].front();
    else if(interfaceAddresses.find("Local Area Connection 2")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["Local Area Connection 2"].front();
    else if(interfaceAddresses.find("Local Area Connection 3")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["Local Area Connection 3"].front();
    else if(interfaceAddresses.find("Local Area Connection 4")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["Local Area Connection 4"].front();
    else if(interfaceAddresses.find("Local Area Connection 5")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["Local Area Connection 5"].front();
    else if(interfaceAddresses.find("Local Area Connection 6")!=interfaceAddresses.end())
        ipAddress = interfaceAddresses["Local Area Connection 6"].front();
	else if (interfaceAddresses.find("Local Area Connection* 15") != interfaceAddresses.end())
		ipAddress = interfaceAddresses["Local Area Connection* 15"].front();
	else CK_THROW(("interface name not found."));
    // TODO: Find out if we need this. Buildboxes do not have this interface.
    if(interfaceAddresses.find("Loopback Pseudo-Interface 1")!=interfaceAddresses.end())
    {
        ck_string localHostAddress = interfaceAddresses["Loopback Pseudo-Interface 1"].front();
        UT_ASSERT(localHostAddress == "127.0.0.1");
    }

    // The bamboo test boxes appear to have this one.
    if(interfaceAddresses.find("MS TCP Loopback interface")!=interfaceAddresses.end())
    {
        ck_string localHostAddress = interfaceAddresses["MS TCP Loopback interface"].front();
        UT_ASSERT(localHostAddress == "127.0.0.1");
    }

    UT_ASSERT(!ipAddress.empty());
#endif
}

