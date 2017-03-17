
#include "framework.h"
#include "ck_socket_address_test.h"

#include "cppkit/ck_socket.h"

using namespace std;
using namespace cppkit;

REGISTER_TEST_FIXTURE(ck_socket_address_test);

void ck_socket_address_test::setup()
{
}

void ck_socket_address_test::teardown()
{
}

void ck_socket_address_test::test_constructor()
{

    UT_ASSERT_NO_THROW( ck_socket_address(0) );
    UT_ASSERT_NO_THROW( ck_socket_address(-1) );
    UT_ASSERT_NO_THROW( ck_socket_address(65535) );
    UT_ASSERT_NO_THROW( ck_socket_address(0, ip6_addr_any) );
    UT_ASSERT_NO_THROW( ck_socket_address(-1, ip6_addr_any) );
    UT_ASSERT_NO_THROW( ck_socket_address(42, "127.0.0.1") );
    UT_ASSERT_NO_THROW( ck_socket_address(42, "localhost") );
    UT_ASSERT_NO_THROW( ck_socket_address(42, "::1") );
    UT_ASSERT_NO_THROW( ck_socket_address(42, "239.0.0.1") );
    UT_ASSERT_NO_THROW( ck_socket_address(42, "ff05::4242") );

    UT_ASSERT_NO_THROW( ck_socket_address(42, "::ffff:10.221.218.1") );
    UT_ASSERT_NO_THROW( ck_socket_address(42, "::") );
    {

       ck_socket_address adr(42, "[fc01:1:10:221:beae:c5ff:fe63:c26]");
       UT_ASSERT( !adr.is_ipv4() );
       UT_ASSERT( adr.is_ipv6() );
       UT_ASSERT( !adr.is_multicast() );
       UT_ASSERT( !adr.is_ipv4_mapped_to_ipv6() );

    }

    {

       ck_socket_address adr(42, "::");
       UT_ASSERT( !adr.is_ipv4() );
       UT_ASSERT( adr.is_ipv6() );
       UT_ASSERT( !adr.is_multicast() );
       UT_ASSERT( !adr.is_ipv4_mapped_to_ipv6() );

    }

    // Invalid addresses
    UT_ASSERT_THROWS( ck_socket_address(0, "255.255.255.256"), ck_exception );
    UT_ASSERT_THROWS( ck_socket_address(0, "::fffg"), ck_exception );
    UT_ASSERT_THROWS( ck_socket_address(0, "NoSuchAddress_75006DD5-D829-464a-8E8C-7AC4628ECDCE"), ck_exception );

    // sockaddr version
    {
        ck_socket_address addr(4242, "127.0.0.1");
        UT_ASSERT_NO_THROW( ck_socket_address(addr.get_sock_addr(), addr.sock_addr_size()) );
        ck_socket_address a4(addr.get_sock_addr(), addr.sock_addr_size());
        UT_ASSERT( a4.address_family() == AF_INET );
        UT_ASSERT( a4.port() == 4242 );
        UT_ASSERT( a4.address() == "127.0.0.1" );
    }

    {
        ck_socket_address addr(4242, "dead::beef");
        UT_ASSERT_NO_THROW( ck_socket_address(addr.get_sock_addr(), addr.sock_addr_size()) );
        ck_socket_address a6(addr.get_sock_addr(), addr.sock_addr_size());
        UT_ASSERT( a6.address_family() == AF_INET6 );
        UT_ASSERT( a6.port() == 4242 );
        UT_ASSERT( a6.address() == "dead::beef" );
    }

}

void ck_socket_address_test::test_get_set_port_num()
{
    {
        ck_socket_address addr(42);
        UT_ASSERT_NO_THROW( addr.port() );
        UT_ASSERT( addr.port() == 42 );
    }
    {
        ck_socket_address addr(-1);
        UT_ASSERT( addr.port() == -1 );
        UT_ASSERT_NO_THROW( addr.set_port_num(42) );
        UT_ASSERT( addr.port() == 42 );
    }
}

void ck_socket_address_test::test_get_set_address()
{

    {
        // Test default address
        ck_socket_address addr(0);
        UT_ASSERT_NO_THROW( addr.address() );
        UT_ASSERT( addr.address() == ip4_addr_any );
    }
    {
        ck_socket_address addr(0, ip6_addr_any);
        UT_ASSERT( addr.address() == ip6_addr_any );
    }
    {
        ck_socket_address addr(42);
        UT_ASSERT_NO_THROW( addr.set_address("127.0.0.1") );
        UT_ASSERT( addr.address() == "127.0.0.1" );

        UT_ASSERT_NO_THROW( addr.set_address("localhost") );
        UT_ASSERT( addr.address() == "localhost" );

        UT_ASSERT_NO_THROW( addr.set_address("::1") );
        UT_ASSERT( addr.address() == "::1" );

        UT_ASSERT_NO_THROW( addr.set_address("239.0.0.1") );
        UT_ASSERT( addr.address() == "239.0.0.1" );

        UT_ASSERT_NO_THROW( addr.set_address("ff05::4242") );
        UT_ASSERT( addr.address() == "ff05::4242" );

        ck_string prev = addr.address();

        UT_ASSERT_THROWS( addr.set_address("NoSuchAddress_75006DD5-D829-464a-8E8C-7AC4628ECDCE"), ck_exception );
        UT_ASSERT( addr.address() == prev ); // Make sure the address didn't change b/c of error.

    }
}

void ck_socket_address_test::test_get_address_family()
{
    {
        ck_socket_address addr(0, ip4_addr_any);
        UT_ASSERT_NO_THROW( addr.address_family() );
        UT_ASSERT( addr.address_family() == AF_INET );

        addr.set_address( ip6_addr_any );
        UT_ASSERT( addr.address_family() == AF_INET6 );
    }
    {
        ck_socket_address addr(42);
        UT_ASSERT_NO_THROW( addr.set_address("127.0.0.1") );
        UT_ASSERT( addr.address_family() == AF_INET );

        UT_ASSERT_NO_THROW( addr.set_address("::1") );
        UT_ASSERT( addr.address_family() == AF_INET6 );

        UT_ASSERT_NO_THROW( addr.set_address("239.0.0.1") );
        UT_ASSERT( addr.address_family() == AF_INET );

        UT_ASSERT_NO_THROW( addr.set_address("ff05::4242") );
        UT_ASSERT( addr.address_family() == AF_INET6 );

        UT_ASSERT_NO_THROW( addr.set_address("0.0.0.0") );
        UT_ASSERT( addr.address_family() == AF_INET );

        UT_ASSERT_NO_THROW( addr.set_address("::") );
        UT_ASSERT( addr.address_family() == AF_INET6 );

        UT_ASSERT_THROWS( addr.set_address("NoSuchAddress_75006DD5-D829-464a-8E8C-7AC4628ECDCE"), ck_exception );
    }
}

void ck_socket_address_test::test_sock_addr_cast()
{
    {
        ck_socket_address addr(0xf0a5, "127.0.0.1");
        struct sockaddr* sa = addr.get_sock_addr();
        UT_ASSERT( sa->sa_family == AF_INET );
        UT_ASSERT( addr.sock_addr_size() == sizeof(struct sockaddr_in) );
        struct sockaddr_in* sin = (struct sockaddr_in*)sa;
        UT_ASSERT( sin->sin_port == htons(0xf0a5) );
        UT_ASSERT( sin->sin_addr.s_addr == htonl(0x7f000001) );
    }
    {
        ck_socket_address addr(0xf0a5, "ff05::dead:beef");
        struct sockaddr* sa = addr.get_sock_addr();
        UT_ASSERT( sa->sa_family == AF_INET6 );
        UT_ASSERT( addr.sock_addr_size() == sizeof(struct sockaddr_in6) );
        struct sockaddr_in6* sin = (struct sockaddr_in6*)sa;
        UT_ASSERT( sin->sin6_port == htons(0xf0a5) );
        UT_ASSERT( sin->sin6_addr.s6_addr[0] == 0xff );
        UT_ASSERT( sin->sin6_addr.s6_addr[1] == 0x05 );
        UT_ASSERT( sin->sin6_addr.s6_addr[2] == 0 );
        UT_ASSERT( sin->sin6_addr.s6_addr[3] == 0 );
        UT_ASSERT( sin->sin6_addr.s6_addr[4] == 0 );
        UT_ASSERT( sin->sin6_addr.s6_addr[5] == 0 );
        UT_ASSERT( sin->sin6_addr.s6_addr[6] == 0 );
        UT_ASSERT( sin->sin6_addr.s6_addr[7] == 0 );
        UT_ASSERT( sin->sin6_addr.s6_addr[8] == 0 );
        UT_ASSERT( sin->sin6_addr.s6_addr[9] == 0 );
        UT_ASSERT( sin->sin6_addr.s6_addr[10] == 0 );
        UT_ASSERT( sin->sin6_addr.s6_addr[11] == 0 );
        UT_ASSERT( sin->sin6_addr.s6_addr[12] == 0xde );
        UT_ASSERT( sin->sin6_addr.s6_addr[13] == 0xad );
        UT_ASSERT( sin->sin6_addr.s6_addr[14] == 0xbe );
        UT_ASSERT( sin->sin6_addr.s6_addr[15] == 0xef );
    }

}

void ck_socket_address_test::test_comparisons()
{
    {
        // IPv4
        struct sockaddr_in s4, s4_diff;
        struct sockaddr_storage ss, ss_diff;
        memset(&s4, 0, sizeof(s4));
        memset(&s4_diff, 0, sizeof(s4_diff));
        memset(&ss, 0, sizeof(ss));
        memset(&ss_diff, 0, sizeof(ss_diff));

        s4.sin_family = AF_INET;
        s4.sin_port = htons( 4243 );
        s4.sin_addr.s_addr = 0x0ff05aa5; // Nice bit pattern = 165.90.240.15
        memcpy(&s4_diff, &s4, sizeof(s4));
        memcpy(&ss, &s4, sizeof(s4));

        s4_diff.sin_port += 1;
        memcpy(&ss_diff, &s4_diff, sizeof(s4_diff));

        ck_socket_address xsa(4243, "165.90.240.15");
        ck_socket_address xsa_same(xsa);
        ck_socket_address xsa_diff_port(4242, "165.90.240.15");
        ck_socket_address xsa_diff_addr(4243, "165.91.240.15");

        UT_ASSERT( xsa == (struct sockaddr*)&s4 );
        UT_ASSERT( xsa == ss );
        UT_ASSERT( xsa == xsa_same );
        UT_ASSERT( (xsa == (struct sockaddr*)&s4_diff) == false );
        UT_ASSERT( (xsa == ss_diff) == false );
        UT_ASSERT( (xsa == xsa_diff_port) == false );
        UT_ASSERT( (xsa == xsa_diff_addr) == false );

        UT_ASSERT( xsa != (struct sockaddr*)&s4_diff );
        UT_ASSERT( xsa != ss_diff );
        UT_ASSERT( xsa != xsa_diff_port );
        UT_ASSERT( xsa != xsa_diff_addr );
        UT_ASSERT( (xsa != (struct sockaddr*)&s4) == false );
        UT_ASSERT( (xsa != ss) == false );
        UT_ASSERT( (xsa != xsa_same) == false );
    }
    {
        // IPv6
        struct sockaddr_in6 s6, s6_diff;
        struct sockaddr_storage ss, ss_diff;
        memset(&s6, 0, sizeof(s6));
        memset(&s6_diff, 0, sizeof(s6_diff));
        memset(&ss, 0, sizeof(ss));
        memset(&ss_diff, 0, sizeof(ss_diff));

        s6.sin6_family = AF_INET6;
        s6.sin6_port = htons( 4243 );
        // fe80::0ff0:5aa5
        s6.sin6_addr.s6_addr[0] = 0xfe;
        s6.sin6_addr.s6_addr[1] = 0x80;
        s6.sin6_addr.s6_addr[12] = 0x0f;
        s6.sin6_addr.s6_addr[13] = 0xf0;
        s6.sin6_addr.s6_addr[14] = 0x5a;
        s6.sin6_addr.s6_addr[15] = 0xa5;
        memcpy(&s6_diff, &s6, sizeof(s6));
        memcpy(&ss, &s6, sizeof(s6));

        s6_diff.sin6_port += 1;
        memcpy(&ss_diff, &s6_diff, sizeof(s6_diff));

        ck_socket_address xsa(4243, "fe80::0ff0:5aa5");
        ck_socket_address xsa_same(xsa);
        ck_socket_address xsa_diff_port(4242, "fe80::0ff0:5aa5");
        ck_socket_address xsa_diff_addr(4243, "fe80::0ff0:59a5");

        UT_ASSERT( xsa == (struct sockaddr*)&s6 );
        UT_ASSERT( xsa == ss );
        UT_ASSERT( xsa == xsa_same );
        UT_ASSERT( (xsa == (struct sockaddr*)&s6_diff) == false );
        UT_ASSERT( (xsa == ss_diff) == false );
        UT_ASSERT( (xsa == xsa_diff_port) == false );
        UT_ASSERT( (xsa == xsa_diff_addr) == false );

        UT_ASSERT( xsa != (struct sockaddr*)&s6_diff );
        UT_ASSERT( xsa != ss_diff );
        UT_ASSERT( xsa != xsa_diff_port );
        UT_ASSERT( xsa != xsa_diff_addr );
        UT_ASSERT( (xsa != (struct sockaddr*)&s6) == false );
        UT_ASSERT( (xsa != ss) == false );
        UT_ASSERT( (xsa != xsa_same) == false );
    }

}

void ck_socket_address_test::test_static_get_address_family()
{
    {
        UT_ASSERT( ck_socket_address::get_address_family("127.0.0.1", 0) == AF_INET );
        UT_ASSERT( ck_socket_address::get_address_family("42.42.32.64", 0) == AF_INET );
        UT_ASSERT( ck_socket_address::get_address_family("::1", 0) == AF_INET6 );
        UT_ASSERT( ck_socket_address::get_address_family("ff02::8008:1e55", 0) == AF_INET6 );
    }
    {
        // Test for sockaddr return value
        struct sockaddr_storage addr;
        struct sockaddr_in* addr4;
        struct sockaddr_in6* addr6;

        UT_ASSERT( ck_socket_address::get_address_family("127.0.0.1", (struct sockaddr*)&addr) == AF_INET );
        UT_ASSERT( addr.ss_family == AF_INET );
        addr4 = (struct sockaddr_in*)&addr;
        UT_ASSERT( addr4->sin_addr.s_addr == htonl(0x7f000001) );

        UT_ASSERT( ck_socket_address::get_address_family("ff02::8008:1e55", (struct sockaddr*)&addr) == AF_INET6 );
        UT_ASSERT( addr.ss_family == AF_INET6 );
        addr6 = (struct sockaddr_in6*)&addr;
        UT_ASSERT( addr6->sin6_addr.s6_addr[0] == 0xff );
        UT_ASSERT( addr6->sin6_addr.s6_addr[1] == 0x02 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[2] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[3] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[4] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[5] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[6] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[7] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[8] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[9] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[10] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[11] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[12] == 0x80 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[13] == 0x08 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[14] == 0x1e );
        UT_ASSERT( addr6->sin6_addr.s6_addr[15] == 0x55 );
    }

}

void ck_socket_address_test::test_static_get_sock_addr_size()
{
    UT_ASSERT( ck_socket_address::sock_addr_size( AF_INET ) == sizeof(struct sockaddr_in) );
    UT_ASSERT( ck_socket_address::sock_addr_size( AF_INET6 ) == sizeof(struct sockaddr_in6) );

}

void ck_socket_address_test::test_static_address_to_string()
{
    {
        ck_socket_address addr(0, "42.43.44.45");
        UT_ASSERT( ck_socket_address::address_to_string(addr.get_sock_addr(), addr.sock_addr_size()) == "42.43.44.45" );
    }
    {
        ck_socket_address addr(0, "fe08:110::a5f0:f0a5");
        UT_ASSERT( ck_socket_address::address_to_string(addr.get_sock_addr(), addr.sock_addr_size()).contains("fe08:110::a5f0:f0a5") );
    }

}

void ck_socket_address_test::test_static_string_to_address()
{
    {
        struct sockaddr_storage addr;
        struct sockaddr_in* addr4;
        struct sockaddr_in6* addr6;

        UT_ASSERT_NO_THROW( ck_socket_address::string_to_address("127.0.0.1", (struct sockaddr*)&addr, sizeof(addr)) );
        UT_ASSERT( addr.ss_family == AF_INET );
        addr4 = (struct sockaddr_in*)&addr;
        UT_ASSERT( addr4->sin_addr.s_addr == htonl(0x7f000001) );

        UT_ASSERT_NO_THROW( ck_socket_address::string_to_address("ff02::8008:1e55", (struct sockaddr*)&addr, sizeof(addr)) );
        UT_ASSERT( addr.ss_family == AF_INET6 );
        addr6 = (struct sockaddr_in6*)&addr;
        UT_ASSERT( addr6->sin6_addr.s6_addr[0] == 0xff );
        UT_ASSERT( addr6->sin6_addr.s6_addr[1] == 0x02 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[2] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[3] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[4] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[5] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[6] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[7] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[8] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[9] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[10] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[11] == 0 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[12] == 0x80 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[13] == 0x08 );
        UT_ASSERT( addr6->sin6_addr.s6_addr[14] == 0x1e );
        UT_ASSERT( addr6->sin6_addr.s6_addr[15] == 0x55 );
    }

}

void ck_socket_address_test::test_isolate_address()
{
    UT_ASSERT_NO_THROW( ck_socket_address::isolate_address("127.0.0.1") );
    UT_ASSERT_NO_THROW( ck_socket_address::isolate_address("f782::3259:aad9") );
    UT_ASSERT_NO_THROW( ck_socket_address::isolate_address("[f782::3259:aad9]:4242") );

    UT_ASSERT( ck_socket_address::isolate_address("127.0.0.1") == "127.0.0.1" );
    UT_ASSERT( ck_socket_address::isolate_address("f782::3259:aad9") == "f782::3259:aad9" );
    UT_ASSERT( ck_socket_address::isolate_address("[f782::3259:aad9]:4242") == "f782::3259:aad9" );

}

void ck_socket_address_test::test_address_type_methods()
{
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "127.0.0.1").is_ipv4() );
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "0.0.0.0").is_ipv4() );
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "::1").is_ipv4() );
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "127.0.0.1").is_ipv6() );
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "::1").is_ipv6() );
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "::").is_ipv6() );
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "127.0.0.1").is_multicast() );
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "239.0.0.1").is_multicast() );
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "::1").is_multicast() );
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "ff02::f000").is_multicast() );
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "127.0.0.1").is_ipv4_mapped_to_ipv6() );
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "::1").is_ipv4_mapped_to_ipv6() );
    UT_ASSERT_NO_THROW( ck_socket_address(4242, "::ffff:127.0.0.1").is_ipv4_mapped_to_ipv6() );
    {
        ck_string addr;
        UT_ASSERT_NO_THROW( ck_socket_address(4242, "127.0.0.1").is_ipv4_mapped_to_ipv6(&addr) );
        UT_ASSERT_NO_THROW( ck_socket_address(4242, "::1").is_ipv4_mapped_to_ipv6(&addr) );
        UT_ASSERT_NO_THROW( ck_socket_address(4242, "::ffff:127.0.0.1").is_ipv4_mapped_to_ipv6(&addr) );
    }

    UT_ASSERT( ck_socket_address(4242, "127.0.0.1").is_ipv4() == true  );
    UT_ASSERT( ck_socket_address(4242, "0.0.0.0").is_ipv4() == true  );
    UT_ASSERT( ck_socket_address(4242, "::1").is_ipv4() == false );
    UT_ASSERT( ck_socket_address(4242, "127.0.0.1").is_ipv6() == false );
    UT_ASSERT( ck_socket_address(4242, "::1").is_ipv6() == true  );
    UT_ASSERT( ck_socket_address(4242, "::").is_ipv6() == true  );
    UT_ASSERT( ck_socket_address(4242, "127.0.0.1").is_multicast() == false );
    UT_ASSERT( ck_socket_address(4242, "0.0.0.0").is_multicast() == false );
    UT_ASSERT( ck_socket_address(4242, "239.0.0.1").is_multicast() == true );
    UT_ASSERT( ck_socket_address(4242, "::1").is_multicast() == false );
    UT_ASSERT( ck_socket_address(4242, "::").is_multicast() == false );
    UT_ASSERT( ck_socket_address(4242, "ff02::f000").is_multicast() == true );
    UT_ASSERT( ck_socket_address(4242, "127.0.0.1").is_ipv4_mapped_to_ipv6()  == false);
    UT_ASSERT( ck_socket_address(4242, "::1").is_ipv4_mapped_to_ipv6() == false );
    UT_ASSERT( ck_socket_address(4242, "::ffff:127.0.0.1").is_ipv4_mapped_to_ipv6() == true );
    {
        ck_string addr;
        UT_ASSERT( ck_socket_address(4242, "127.0.0.1").is_ipv4_mapped_to_ipv6(&addr) == false );
        UT_ASSERT( ck_socket_address(4242, "::1").is_ipv4_mapped_to_ipv6(&addr) == false );
        UT_ASSERT( ck_socket_address(4242, "::ffff:127.0.0.1").is_ipv4_mapped_to_ipv6(&addr) == true );
        UT_ASSERT( addr == "127.0.0.1" );
    }

    // STATIC versions
    UT_ASSERT_NO_THROW( ck_socket_address::is_ipv4("127.0.0.1") );
    UT_ASSERT_NO_THROW( ck_socket_address::is_ipv4("::1") );
    UT_ASSERT_NO_THROW( ck_socket_address::is_ipv6("127.0.0.1") );
    UT_ASSERT_NO_THROW( ck_socket_address::is_ipv6("::1") );
    UT_ASSERT_NO_THROW( ck_socket_address::is_multicast("127.0.0.1") );
    UT_ASSERT_NO_THROW( ck_socket_address::is_multicast("239.0.0.1") );
    UT_ASSERT_NO_THROW( ck_socket_address::is_multicast("::1") );
    UT_ASSERT_NO_THROW( ck_socket_address::is_multicast("ff02::f000") );
    UT_ASSERT_NO_THROW( ck_socket_address::is_ipv4_mapped_to_ipv6("127.0.0.1") );
    UT_ASSERT_NO_THROW( ck_socket_address::is_ipv4_mapped_to_ipv6("::1") );
    UT_ASSERT_NO_THROW( ck_socket_address::is_ipv4_mapped_to_ipv6("::ffff:127.0.0.1") );
    {
        ck_string addr;
        UT_ASSERT_NO_THROW( ck_socket_address::is_ipv4_mapped_to_ipv6("127.0.0.1", &addr) );
        UT_ASSERT_NO_THROW( ck_socket_address::is_ipv4_mapped_to_ipv6("::1", &addr) );
        UT_ASSERT_NO_THROW( ck_socket_address::is_ipv4_mapped_to_ipv6("::ffff:127.0.0.1", &addr) );
    }

    UT_ASSERT( ck_socket_address::is_ipv4("127.0.0.1") == true );
    UT_ASSERT( ck_socket_address::is_ipv4("::1") == false );
    UT_ASSERT( ck_socket_address::is_ipv6("127.0.0.1") == false );
    UT_ASSERT( ck_socket_address::is_ipv6("::1") == true );
    UT_ASSERT( ck_socket_address::is_multicast("127.0.0.1") == false );
    UT_ASSERT( ck_socket_address::is_multicast("239.0.0.1") == true );
    UT_ASSERT( ck_socket_address::is_multicast("::1") == false );
    UT_ASSERT( ck_socket_address::is_multicast("ff02::f000") == true );
    UT_ASSERT( ck_socket_address::is_ipv4_mapped_to_ipv6("127.0.0.1") == false );
    UT_ASSERT( ck_socket_address::is_ipv4_mapped_to_ipv6("::1") == false );
    UT_ASSERT( ck_socket_address::is_ipv4_mapped_to_ipv6("::ffff:127.0.0.1") == true );
    {
        ck_string addr;
        UT_ASSERT( ck_socket_address::is_ipv4_mapped_to_ipv6("127.0.0.1", &addr) == false );
        UT_ASSERT( ck_socket_address::is_ipv4_mapped_to_ipv6("::1", &addr) == false );
        UT_ASSERT( ck_socket_address::is_ipv4_mapped_to_ipv6("::ffff:127.0.0.1", &addr) == true );
        UT_ASSERT( addr == "127.0.0.1" );
    }

    // sockaddr versions
    UT_ASSERT( ck_socket_address::is_ipv4( ck_socket_address(0, "127.0.0.1").get_sock_addr(), sizeof(sockaddr_in) ) == true );
    UT_ASSERT( ck_socket_address::is_ipv4( ck_socket_address(0, "::1").get_sock_addr(), sizeof(sockaddr_in6) ) == false );
    UT_ASSERT( ck_socket_address::is_ipv6( ck_socket_address(0, "127.0.0.1").get_sock_addr(), sizeof(sockaddr_in) ) == false );
    UT_ASSERT( ck_socket_address::is_ipv6( ck_socket_address(0, "::1").get_sock_addr(), sizeof(sockaddr_in6) ) == true );
    UT_ASSERT( ck_socket_address::is_multicast( ck_socket_address(0, "127.0.0.1").get_sock_addr(), sizeof(sockaddr_in) ) == false );
    UT_ASSERT( ck_socket_address::is_multicast( ck_socket_address(0, "239.0.0.1").get_sock_addr(), sizeof(sockaddr_in) ) == true );
    UT_ASSERT( ck_socket_address::is_multicast( ck_socket_address(0, "::1").get_sock_addr(), sizeof(sockaddr_in6) ) == false );
    UT_ASSERT( ck_socket_address::is_multicast( ck_socket_address(0, "ff02::f000").get_sock_addr(), sizeof(sockaddr_in6) ) == true );
    UT_ASSERT( ck_socket_address::is_ipv4_mapped_to_ipv6( ck_socket_address(0, "127.0.0.1").get_sock_addr(), sizeof(sockaddr_in) ) == false );
    UT_ASSERT( ck_socket_address::is_ipv4_mapped_to_ipv6( ck_socket_address(0, "::1").get_sock_addr(), sizeof(sockaddr_in6) ) == false );
    UT_ASSERT( ck_socket_address::is_ipv4_mapped_to_ipv6( ck_socket_address(0, "::ffff:127.0.0.1").get_sock_addr(), sizeof(sockaddr_in6) ) == true );
    {
        ck_string addr;
        UT_ASSERT( ck_socket_address::is_ipv4_mapped_to_ipv6( ck_socket_address(0, "127.0.0.1").get_sock_addr(), sizeof(sockaddr_in), &addr ) == false );
        UT_ASSERT( ck_socket_address::is_ipv4_mapped_to_ipv6( ck_socket_address(0, "::1").get_sock_addr(), sizeof(sockaddr_in6), &addr ) == false );
        UT_ASSERT( ck_socket_address::is_ipv4_mapped_to_ipv6( ck_socket_address(0, "::ffff:127.0.0.1").get_sock_addr(), sizeof(sockaddr_in6), &addr ) == true );
        UT_ASSERT( addr == "127.0.0.1" );
    }

}

void ck_socket_address_test::test_is_wildcard_address()
{
    UT_ASSERT_NO_THROW( ck_socket_address(0, "127.0.0.1").is_wildcard_address() );
    UT_ASSERT_NO_THROW( ck_socket_address(0, "0.0.0.0").is_wildcard_address() );
    UT_ASSERT_NO_THROW( ck_socket_address(0, "f782::3259:aad9").is_wildcard_address() );
    UT_ASSERT_NO_THROW( ck_socket_address(0, "::").is_wildcard_address() );
    UT_ASSERT_NO_THROW( ck_socket_address(0, "[f782::3259:aad9]:4242").is_wildcard_address() );
    UT_ASSERT_NO_THROW( ck_socket_address(0, "[::]:4242").is_wildcard_address() );

    UT_ASSERT( ck_socket_address(0, "127.0.0.1").is_wildcard_address() == false );
    UT_ASSERT( ck_socket_address(0, "0.0.0.0").is_wildcard_address() == true );
    UT_ASSERT( ck_socket_address(0, "f782::3259:aad9").is_wildcard_address() == false );
    UT_ASSERT( ck_socket_address(0, "::").is_wildcard_address() == true );
    UT_ASSERT( ck_socket_address(0, "[f782::3259:aad9]:4242").is_wildcard_address() == false );
    UT_ASSERT( ck_socket_address(0, "[::]:4242").is_wildcard_address() == true );

}

void ck_socket_address_test::test_is_hostname()
{
    UT_ASSERT(  ck_socket_address::is_hostname("localhost") );
    UT_ASSERT( !ck_socket_address::is_hostname("127.0.0.1") );
    UT_ASSERT( !ck_socket_address::is_hostname("10.221.218.75") );
    UT_ASSERT(  ck_socket_address::is_hostname("www.google.com") );
    UT_ASSERT(  ck_socket_address::is_hostname("google.com") );
    UT_ASSERT(  ck_socket_address::is_hostname("com") );

    UT_ASSERT( !ck_socket_address::is_hostname("www.go ogle.com") );
    UT_ASSERT( !ck_socket_address::is_hostname("www.go\nogle.com") );
    UT_ASSERT( !ck_socket_address::is_hostname("www.go\rogle.com") );

    UT_ASSERT( !ck_socket_address::is_hostname("") );
    UT_ASSERT( !ck_socket_address::is_hostname(".") );
    UT_ASSERT( !ck_socket_address::is_hostname("..") );
    UT_ASSERT( !ck_socket_address::is_hostname("...") );
    UT_ASSERT( !ck_socket_address::is_hostname(". . .") );

    UT_ASSERT(  ck_socket_address::is_hostname("a") );
    UT_ASSERT(  ck_socket_address::is_hostname("a.") );
    UT_ASSERT(  ck_socket_address::is_hostname("a.b") );
    UT_ASSERT(  ck_socket_address::is_hostname("a.a.") );
    UT_ASSERT(  ck_socket_address::is_hostname("a.a.a") );
    UT_ASSERT(  ck_socket_address::is_hostname("a.a.a.") );
    UT_ASSERT(  ck_socket_address::is_hostname("a.a.a.a") );
    UT_ASSERT(  ck_socket_address::is_hostname("a.a.a.a.") );
    UT_ASSERT(  ck_socket_address::is_hostname("a.a.a.a.a") );
    UT_ASSERT( !ck_socket_address::is_hostname("a.a.a.a.a ") );
    UT_ASSERT( !ck_socket_address::is_hostname(" a.a.a.a.a") );
    UT_ASSERT( !ck_socket_address::is_hostname("a.a.a .a.a") );

    UT_ASSERT( !ck_socket_address::is_hostname(".1") );
    UT_ASSERT( !ck_socket_address::is_hostname("1") );
    UT_ASSERT( !ck_socket_address::is_hostname("1.") );
    UT_ASSERT( !ck_socket_address::is_hostname("1.1") );
    UT_ASSERT( !ck_socket_address::is_hostname("1.1.") );
    UT_ASSERT( !ck_socket_address::is_hostname("1.1.1") );
    UT_ASSERT( !ck_socket_address::is_hostname("1.1.1.") );
    UT_ASSERT( !ck_socket_address::is_hostname("1.1.1.1") );
    UT_ASSERT( !ck_socket_address::is_hostname("1.1.1.1.") );
    UT_ASSERT( !ck_socket_address::is_hostname("1.1.1.1.1") );
    UT_ASSERT( !ck_socket_address::is_hostname("1.1.1.1.1 ") );
    UT_ASSERT( !ck_socket_address::is_hostname(" 1.1.1.1.1") );
    UT_ASSERT( !ck_socket_address::is_hostname("1.1. 1.1.1") );

    UT_ASSERT( !ck_socket_address::is_hostname(".a") );
    UT_ASSERT(  ck_socket_address::is_hostname("1.a") );
    UT_ASSERT(  ck_socket_address::is_hostname("1.a.") );
    UT_ASSERT(  ck_socket_address::is_hostname("1.1.a") );
    UT_ASSERT(  ck_socket_address::is_hostname("1.1.a.") );
    UT_ASSERT(  ck_socket_address::is_hostname("1.1.1.a") );
    UT_ASSERT(  ck_socket_address::is_hostname("1.1.1.a.") );
    UT_ASSERT(  ck_socket_address::is_hostname("1.1.1.1.a") );
    UT_ASSERT( !ck_socket_address::is_hostname("1.1.1.1.a ") );
    UT_ASSERT( !ck_socket_address::is_hostname(" 1.1.1.1.a") );
    UT_ASSERT( !ck_socket_address::is_hostname("1.1. 1.1.a") );

    UT_ASSERT( !ck_socket_address::is_hostname("-") );
    UT_ASSERT( !ck_socket_address::is_hostname(".-") );
    UT_ASSERT( !ck_socket_address::is_hostname("-.") );
    UT_ASSERT( !ck_socket_address::is_hostname(".-.") );
    UT_ASSERT( !ck_socket_address::is_hostname("a-") );
    UT_ASSERT( !ck_socket_address::is_hostname("-a") );
    UT_ASSERT(  ck_socket_address::is_hostname("a-a") );
    UT_ASSERT(  ck_socket_address::is_hostname("a-1") );
    UT_ASSERT( !ck_socket_address::is_hostname("1-a") );

    UT_ASSERT( !ck_socket_address::is_hostname("-.hello") );
    UT_ASSERT( !ck_socket_address::is_hostname(".-.hello") );
    UT_ASSERT( !ck_socket_address::is_hostname("-.hello") );
    UT_ASSERT( !ck_socket_address::is_hostname(".-.hello") );
    UT_ASSERT( !ck_socket_address::is_hostname("a-.hello") );
    UT_ASSERT( !ck_socket_address::is_hostname("-a.hello") );
    UT_ASSERT(  ck_socket_address::is_hostname("a-a.hello") );
    UT_ASSERT(  ck_socket_address::is_hostname("a-1.hello") );
    UT_ASSERT(  ck_socket_address::is_hostname("1-a.hello") );

    UT_ASSERT( !ck_socket_address::is_hostname("hello.-") );
    UT_ASSERT( !ck_socket_address::is_hostname("hello.-.") );
    UT_ASSERT( !ck_socket_address::is_hostname("hello.-.") );
    UT_ASSERT( !ck_socket_address::is_hostname("hello.a-") );
    UT_ASSERT( !ck_socket_address::is_hostname("hello.-a") );
    UT_ASSERT(  ck_socket_address::is_hostname("hello.a-a") );
    UT_ASSERT(  ck_socket_address::is_hostname("hello.a-1") );
    UT_ASSERT( !ck_socket_address::is_hostname("hello.1-a") );

}
