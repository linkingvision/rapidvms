
#include "XSocketAddressTest.h"

#include "XSDK/XSocketAddress.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XSocketAddressTest);


void XSocketAddressTest::TestConstructor()
{
    UT_ASSERT_NO_THROW( XSocketAddress(0) );
    UT_ASSERT_NO_THROW( XSocketAddress(-1) );
    UT_ASSERT_NO_THROW( XSocketAddress(65535) );
    UT_ASSERT_NO_THROW( XSocketAddress(0, ip6_addr_any) );
    UT_ASSERT_NO_THROW( XSocketAddress(-1, ip6_addr_any) );
    UT_ASSERT_NO_THROW( XSocketAddress(42, "127.0.0.1") );
    UT_ASSERT_NO_THROW( XSocketAddress(42, "localhost") );
    UT_ASSERT_NO_THROW( XSocketAddress(42, "::1") );
    UT_ASSERT_NO_THROW( XSocketAddress(42, "239.0.0.1") );
    UT_ASSERT_NO_THROW( XSocketAddress(42, "ff05::4242") );

    UT_ASSERT_NO_THROW( XSocketAddress(42, "::ffff:10.221.218.1") );
    UT_ASSERT_NO_THROW( XSocketAddress(42, "::") );
    {

       XSocketAddress adr(42, "[fc01:1:10:221:beae:c5ff:fe63:c26]");
       UT_ASSERT( !adr.IsIPv4() );
       UT_ASSERT( adr.IsIPv6() );
       UT_ASSERT( !adr.IsMulticast() );
       UT_ASSERT( !adr.IsIPv4MappedToIPv6() );

    }

    {

       XSocketAddress adr(42, "::");
       UT_ASSERT( !adr.IsIPv4() );
       UT_ASSERT( adr.IsIPv6() );
       UT_ASSERT( !adr.IsMulticast() );
       UT_ASSERT( !adr.IsIPv4MappedToIPv6() );

    }

    // Invalid addresses
    UT_ASSERT_THROWS( XSocketAddress(0, "255.255.255.256"), XException );
    UT_ASSERT_THROWS( XSocketAddress(0, "::fffg"), XException );
    UT_ASSERT_THROWS( XSocketAddress(0, "NoSuchAddress_75006DD5-D829-464a-8E8C-7AC4628ECDCE"), XException );

    // sockaddr version
    {
        XSocketAddress addr(4242, "127.0.0.1");
        UT_ASSERT_NO_THROW( XSocketAddress(addr.GetSockAddr(), addr.SockAddrSize()) );
        XSocketAddress a4(addr.GetSockAddr(), addr.SockAddrSize());
        UT_ASSERT( a4.AddressFamily() == AF_INET );
        UT_ASSERT( a4.Port() == 4242 );
        UT_ASSERT( a4.Address() == "127.0.0.1" );
    }
    {
        XSocketAddress addr(4242, "dead::beef");
        UT_ASSERT_NO_THROW( XSocketAddress(addr.GetSockAddr(), addr.SockAddrSize()) );
        XSocketAddress a6(addr.GetSockAddr(), addr.SockAddrSize());
        UT_ASSERT( a6.AddressFamily() == AF_INET6 );
        UT_ASSERT( a6.Port() == 4242 );
        UT_ASSERT( a6.Address() == "dead::beef" );
    }
}

void XSocketAddressTest::TestGetSetPortNum()
{
    {
        XSocketAddress addr(42);
        UT_ASSERT_NO_THROW( addr.Port() );
        UT_ASSERT( addr.Port() == 42 );
    }
    {
        XSocketAddress addr(-1);
        UT_ASSERT( addr.Port() == -1 );
        UT_ASSERT_NO_THROW( addr.SetPortNum(42) );
        UT_ASSERT( addr.Port() == 42 );
    }
}

void XSocketAddressTest::TestGetSetAddress()
{
    {
        // Test default address
        XSocketAddress addr(0);
        UT_ASSERT_NO_THROW( addr.Address() );
        UT_ASSERT( addr.Address() == ip4_addr_any );
    }
    {
        XSocketAddress addr(0, ip6_addr_any);
        UT_ASSERT( addr.Address() == ip6_addr_any );
    }
    {
        XSocketAddress addr(42);
        UT_ASSERT_NO_THROW( addr.SetAddress("127.0.0.1") );
        UT_ASSERT( addr.Address() == "127.0.0.1" );

        UT_ASSERT_NO_THROW( addr.SetAddress("localhost") );
        UT_ASSERT( addr.Address() == "localhost" );

        UT_ASSERT_NO_THROW( addr.SetAddress("::1") );
        UT_ASSERT( addr.Address() == "::1" );

        UT_ASSERT_NO_THROW( addr.SetAddress("239.0.0.1") );
        UT_ASSERT( addr.Address() == "239.0.0.1" );

        UT_ASSERT_NO_THROW( addr.SetAddress("ff05::4242") );
        UT_ASSERT( addr.Address() == "ff05::4242" );

        XString prev = addr.Address();
        UT_ASSERT_THROWS( addr.SetAddress("NoSuchAddress_75006DD5-D829-464a-8E8C-7AC4628ECDCE"), XException );
        UT_ASSERT( addr.Address() == prev ); // Make sure the address didn't change b/c of error.
    }
}

void XSocketAddressTest::TestGetAddressFamily()
{
    {
        XSocketAddress addr(0, ip4_addr_any);
        UT_ASSERT_NO_THROW( addr.AddressFamily() );
        UT_ASSERT( addr.AddressFamily() == AF_INET );

        addr.SetAddress( ip6_addr_any );
        UT_ASSERT( addr.AddressFamily() == AF_INET6 );
    }
    {
        XSocketAddress addr(42);
        UT_ASSERT_NO_THROW( addr.SetAddress("127.0.0.1") );
        UT_ASSERT( addr.AddressFamily() == AF_INET );

        UT_ASSERT_NO_THROW( addr.SetAddress("::1") );
        UT_ASSERT( addr.AddressFamily() == AF_INET6 );

        UT_ASSERT_NO_THROW( addr.SetAddress("239.0.0.1") );
        UT_ASSERT( addr.AddressFamily() == AF_INET );

        UT_ASSERT_NO_THROW( addr.SetAddress("ff05::4242") );
        UT_ASSERT( addr.AddressFamily() == AF_INET6 );

        UT_ASSERT_NO_THROW( addr.SetAddress("0.0.0.0") );
        UT_ASSERT( addr.AddressFamily() == AF_INET );

        UT_ASSERT_NO_THROW( addr.SetAddress("::") );
        UT_ASSERT( addr.AddressFamily() == AF_INET6 );

        UT_ASSERT_THROWS( addr.SetAddress("NoSuchAddress_75006DD5-D829-464a-8E8C-7AC4628ECDCE"), XException );
    }
}

void XSocketAddressTest::TestSockAddrCast()
{
    {
        XSocketAddress addr(0xf0a5, "127.0.0.1");
        struct sockaddr* sa = addr.GetSockAddr();
        UT_ASSERT( sa->sa_family == AF_INET );
        UT_ASSERT( addr.SockAddrSize() == sizeof(struct sockaddr_in) );
        struct sockaddr_in* sin = (struct sockaddr_in*)sa;
        UT_ASSERT( sin->sin_port == htons(0xf0a5) );
        UT_ASSERT( sin->sin_addr.s_addr == htonl(0x7f000001) );
    }
    {
        XSocketAddress addr(0xf0a5, "ff05::dead:beef");
        struct sockaddr* sa = addr.GetSockAddr();
        UT_ASSERT( sa->sa_family == AF_INET6 );
        UT_ASSERT( addr.SockAddrSize() == sizeof(struct sockaddr_in6) );
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

void XSocketAddressTest::TestComparisons()
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

        XSocketAddress xsa(4243, "165.90.240.15");
        XSocketAddress xsa_same(xsa);
        XSocketAddress xsa_diff_port(4242, "165.90.240.15");
        XSocketAddress xsa_diff_addr(4243, "165.91.240.15");

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

        XSocketAddress xsa(4243, "fe80::0ff0:5aa5");
        XSocketAddress xsa_same(xsa);
        XSocketAddress xsa_diff_port(4242, "fe80::0ff0:5aa5");
        XSocketAddress xsa_diff_addr(4243, "fe80::0ff0:59a5");

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

void XSocketAddressTest::TestStaticGetAddressFamily()
{
    {
        UT_ASSERT( XSocketAddress::GetAddressFamily("127.0.0.1", 0) == AF_INET );
        UT_ASSERT( XSocketAddress::GetAddressFamily("42.42.32.64", 0) == AF_INET );
        UT_ASSERT( XSocketAddress::GetAddressFamily("::1", 0) == AF_INET6 );
        UT_ASSERT( XSocketAddress::GetAddressFamily("ff02::8008:1e55", 0) == AF_INET6 );
    }
    {
        // Test for sockaddr return value
        struct sockaddr_storage addr;
        struct sockaddr_in* addr4;
        struct sockaddr_in6* addr6;

        UT_ASSERT( XSocketAddress::GetAddressFamily("127.0.0.1", (struct sockaddr*)&addr) == AF_INET );
        UT_ASSERT( addr.ss_family == AF_INET );
        addr4 = (struct sockaddr_in*)&addr;
        UT_ASSERT( addr4->sin_addr.s_addr == htonl(0x7f000001) );

        UT_ASSERT( XSocketAddress::GetAddressFamily("ff02::8008:1e55", (struct sockaddr*)&addr) == AF_INET6 );
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

void XSocketAddressTest::TestStaticGetSockAddrSize()
{
    UT_ASSERT( XSocketAddress::SockAddrSize( AF_INET ) == sizeof(struct sockaddr_in) );
    UT_ASSERT( XSocketAddress::SockAddrSize( AF_INET6 ) == sizeof(struct sockaddr_in6) );
}

void XSocketAddressTest::TestStaticAddressToString()
{
    {
        XSocketAddress addr(0, "42.43.44.45");
        UT_ASSERT( XSocketAddress::AddressToString(addr.GetSockAddr(), addr.SockAddrSize()) == "42.43.44.45" );
    }
    {
        XSocketAddress addr(0, "fe08:110::a5f0:f0a5");
        UT_ASSERT( XSocketAddress::AddressToString(addr.GetSockAddr(), addr.SockAddrSize()).Contains("fe08:110::a5f0:f0a5") );
    }
}

void XSocketAddressTest::TestStaticStringToAddress()
{
    {
        struct sockaddr_storage addr;
        struct sockaddr_in* addr4;
        struct sockaddr_in6* addr6;

        UT_ASSERT_NO_THROW( XSocketAddress::StringToAddress("127.0.0.1", (struct sockaddr*)&addr, sizeof(addr)) );
        UT_ASSERT( addr.ss_family == AF_INET );
        addr4 = (struct sockaddr_in*)&addr;
        UT_ASSERT( addr4->sin_addr.s_addr == htonl(0x7f000001) );

        UT_ASSERT_NO_THROW( XSocketAddress::StringToAddress("ff02::8008:1e55", (struct sockaddr*)&addr, sizeof(addr)) );
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

void XSocketAddressTest::TestIsolateAddress()
{
    UT_ASSERT_NO_THROW( XSocketAddress::IsolateAddress("127.0.0.1") );
    UT_ASSERT_NO_THROW( XSocketAddress::IsolateAddress("f782::3259:aad9") );
    UT_ASSERT_NO_THROW( XSocketAddress::IsolateAddress("[f782::3259:aad9]:4242") );

    UT_ASSERT( XSocketAddress::IsolateAddress("127.0.0.1") == "127.0.0.1" );
    UT_ASSERT( XSocketAddress::IsolateAddress("f782::3259:aad9") == "f782::3259:aad9" );
    UT_ASSERT( XSocketAddress::IsolateAddress("[f782::3259:aad9]:4242") == "f782::3259:aad9" );
}

void XSocketAddressTest::TestAddressTypeMethods()
{
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "127.0.0.1").IsIPv4() );
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "0.0.0.0").IsIPv4() );
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "::1").IsIPv4() );
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "127.0.0.1").IsIPv6() );
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "::1").IsIPv6() );
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "::").IsIPv6() );
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "127.0.0.1").IsMulticast() );
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "239.0.0.1").IsMulticast() );
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "::1").IsMulticast() );
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "ff02::f000").IsMulticast() );
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "127.0.0.1").IsIPv4MappedToIPv6() );
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "::1").IsIPv4MappedToIPv6() );
    UT_ASSERT_NO_THROW( XSocketAddress(4242, "::ffff:127.0.0.1").IsIPv4MappedToIPv6() );
    {
        XString addr;
        UT_ASSERT_NO_THROW( XSocketAddress(4242, "127.0.0.1").IsIPv4MappedToIPv6(&addr) );
        UT_ASSERT_NO_THROW( XSocketAddress(4242, "::1").IsIPv4MappedToIPv6(&addr) );
        UT_ASSERT_NO_THROW( XSocketAddress(4242, "::ffff:127.0.0.1").IsIPv4MappedToIPv6(&addr) );
    }

    UT_ASSERT( XSocketAddress(4242, "127.0.0.1").IsIPv4() == true  );
    UT_ASSERT( XSocketAddress(4242, "0.0.0.0").IsIPv4() == true  );
    UT_ASSERT( XSocketAddress(4242, "::1").IsIPv4() == false );
    UT_ASSERT( XSocketAddress(4242, "127.0.0.1").IsIPv6() == false );
    UT_ASSERT( XSocketAddress(4242, "::1").IsIPv6() == true  );
    UT_ASSERT( XSocketAddress(4242, "::").IsIPv6() == true  );
    UT_ASSERT( XSocketAddress(4242, "127.0.0.1").IsMulticast() == false );
    UT_ASSERT( XSocketAddress(4242, "0.0.0.0").IsMulticast() == false );
    UT_ASSERT( XSocketAddress(4242, "239.0.0.1").IsMulticast() == true );
    UT_ASSERT( XSocketAddress(4242, "::1").IsMulticast() == false );
    UT_ASSERT( XSocketAddress(4242, "::").IsMulticast() == false );
    UT_ASSERT( XSocketAddress(4242, "ff02::f000").IsMulticast() == true );
    UT_ASSERT( XSocketAddress(4242, "127.0.0.1").IsIPv4MappedToIPv6()  == false);
    UT_ASSERT( XSocketAddress(4242, "::1").IsIPv4MappedToIPv6() == false );
    UT_ASSERT( XSocketAddress(4242, "::ffff:127.0.0.1").IsIPv4MappedToIPv6() == true );
    {
        XString addr;
        UT_ASSERT( XSocketAddress(4242, "127.0.0.1").IsIPv4MappedToIPv6(&addr) == false );
        UT_ASSERT( XSocketAddress(4242, "::1").IsIPv4MappedToIPv6(&addr) == false );
        UT_ASSERT( XSocketAddress(4242, "::ffff:127.0.0.1").IsIPv4MappedToIPv6(&addr) == true );
        UT_ASSERT( addr == "127.0.0.1" );
    }

    // STATIC versions
    UT_ASSERT_NO_THROW( XSocketAddress::IsIPv4("127.0.0.1") );
    UT_ASSERT_NO_THROW( XSocketAddress::IsIPv4("::1") );
    UT_ASSERT_NO_THROW( XSocketAddress::IsIPv6("127.0.0.1") );
    UT_ASSERT_NO_THROW( XSocketAddress::IsIPv6("::1") );
    UT_ASSERT_NO_THROW( XSocketAddress::IsMulticast("127.0.0.1") );
    UT_ASSERT_NO_THROW( XSocketAddress::IsMulticast("239.0.0.1") );
    UT_ASSERT_NO_THROW( XSocketAddress::IsMulticast("::1") );
    UT_ASSERT_NO_THROW( XSocketAddress::IsMulticast("ff02::f000") );
    UT_ASSERT_NO_THROW( XSocketAddress::IsIPv4MappedToIPv6("127.0.0.1") );
    UT_ASSERT_NO_THROW( XSocketAddress::IsIPv4MappedToIPv6("::1") );
    UT_ASSERT_NO_THROW( XSocketAddress::IsIPv4MappedToIPv6("::ffff:127.0.0.1") );
    {
        XString addr;
        UT_ASSERT_NO_THROW( XSocketAddress::IsIPv4MappedToIPv6("127.0.0.1", &addr) );
        UT_ASSERT_NO_THROW( XSocketAddress::IsIPv4MappedToIPv6("::1", &addr) );
        UT_ASSERT_NO_THROW( XSocketAddress::IsIPv4MappedToIPv6("::ffff:127.0.0.1", &addr) );
    }

    UT_ASSERT( XSocketAddress::IsIPv4("127.0.0.1") == true );
    UT_ASSERT( XSocketAddress::IsIPv4("::1") == false );
    UT_ASSERT( XSocketAddress::IsIPv6("127.0.0.1") == false );
    UT_ASSERT( XSocketAddress::IsIPv6("::1") == true );
    UT_ASSERT( XSocketAddress::IsMulticast("127.0.0.1") == false );
    UT_ASSERT( XSocketAddress::IsMulticast("239.0.0.1") == true );
    UT_ASSERT( XSocketAddress::IsMulticast("::1") == false );
    UT_ASSERT( XSocketAddress::IsMulticast("ff02::f000") == true );
    UT_ASSERT( XSocketAddress::IsIPv4MappedToIPv6("127.0.0.1") == false );
    UT_ASSERT( XSocketAddress::IsIPv4MappedToIPv6("::1") == false );
    UT_ASSERT( XSocketAddress::IsIPv4MappedToIPv6("::ffff:127.0.0.1") == true );
    {
        XString addr;
        UT_ASSERT( XSocketAddress::IsIPv4MappedToIPv6("127.0.0.1", &addr) == false );
        UT_ASSERT( XSocketAddress::IsIPv4MappedToIPv6("::1", &addr) == false );
        UT_ASSERT( XSocketAddress::IsIPv4MappedToIPv6("::ffff:127.0.0.1", &addr) == true );
        UT_ASSERT( addr == "127.0.0.1" );
    }

    // sockaddr versions
    UT_ASSERT( XSocketAddress::IsIPv4( XSocketAddress(0, "127.0.0.1").GetSockAddr(), sizeof(sockaddr_in) ) == true );
    UT_ASSERT( XSocketAddress::IsIPv4( XSocketAddress(0, "::1").GetSockAddr(), sizeof(sockaddr_in6) ) == false );
    UT_ASSERT( XSocketAddress::IsIPv6( XSocketAddress(0, "127.0.0.1").GetSockAddr(), sizeof(sockaddr_in) ) == false );
    UT_ASSERT( XSocketAddress::IsIPv6( XSocketAddress(0, "::1").GetSockAddr(), sizeof(sockaddr_in6) ) == true );
    UT_ASSERT( XSocketAddress::IsMulticast( XSocketAddress(0, "127.0.0.1").GetSockAddr(), sizeof(sockaddr_in) ) == false );
    UT_ASSERT( XSocketAddress::IsMulticast( XSocketAddress(0, "239.0.0.1").GetSockAddr(), sizeof(sockaddr_in) ) == true );
    UT_ASSERT( XSocketAddress::IsMulticast( XSocketAddress(0, "::1").GetSockAddr(), sizeof(sockaddr_in6) ) == false );
    UT_ASSERT( XSocketAddress::IsMulticast( XSocketAddress(0, "ff02::f000").GetSockAddr(), sizeof(sockaddr_in6) ) == true );
    UT_ASSERT( XSocketAddress::IsIPv4MappedToIPv6( XSocketAddress(0, "127.0.0.1").GetSockAddr(), sizeof(sockaddr_in) ) == false );
    UT_ASSERT( XSocketAddress::IsIPv4MappedToIPv6( XSocketAddress(0, "::1").GetSockAddr(), sizeof(sockaddr_in6) ) == false );
    UT_ASSERT( XSocketAddress::IsIPv4MappedToIPv6( XSocketAddress(0, "::ffff:127.0.0.1").GetSockAddr(), sizeof(sockaddr_in6) ) == true );
    {
        XString addr;
        UT_ASSERT( XSocketAddress::IsIPv4MappedToIPv6( XSocketAddress(0, "127.0.0.1").GetSockAddr(), sizeof(sockaddr_in), &addr ) == false );
        UT_ASSERT( XSocketAddress::IsIPv4MappedToIPv6( XSocketAddress(0, "::1").GetSockAddr(), sizeof(sockaddr_in6), &addr ) == false );
        UT_ASSERT( XSocketAddress::IsIPv4MappedToIPv6( XSocketAddress(0, "::ffff:127.0.0.1").GetSockAddr(), sizeof(sockaddr_in6), &addr ) == true );
        UT_ASSERT( addr == "127.0.0.1" );
    }
}

void XSocketAddressTest::TestIsWildcardAddress()
{

    UT_ASSERT_NO_THROW( XSocketAddress(0, "127.0.0.1").IsWildcardAddress() );
    UT_ASSERT_NO_THROW( XSocketAddress(0, "0.0.0.0").IsWildcardAddress() );
    UT_ASSERT_NO_THROW( XSocketAddress(0, "f782::3259:aad9").IsWildcardAddress() );
    UT_ASSERT_NO_THROW( XSocketAddress(0, "::").IsWildcardAddress() );
    UT_ASSERT_NO_THROW( XSocketAddress(0, "[f782::3259:aad9]:4242").IsWildcardAddress() );
    UT_ASSERT_NO_THROW( XSocketAddress(0, "[::]:4242").IsWildcardAddress() );

    UT_ASSERT( XSocketAddress(0, "127.0.0.1").IsWildcardAddress() == false );
    UT_ASSERT( XSocketAddress(0, "0.0.0.0").IsWildcardAddress() == true );
    UT_ASSERT( XSocketAddress(0, "f782::3259:aad9").IsWildcardAddress() == false );
    UT_ASSERT( XSocketAddress(0, "::").IsWildcardAddress() == true );
    UT_ASSERT( XSocketAddress(0, "[f782::3259:aad9]:4242").IsWildcardAddress() == false );
    UT_ASSERT( XSocketAddress(0, "[::]:4242").IsWildcardAddress() == true );
}

void XSocketAddressTest::TestIsHostname()
{

    UT_ASSERT(  XSocketAddress::IsHostname("localhost") );
    UT_ASSERT( !XSocketAddress::IsHostname("127.0.0.1") );
    UT_ASSERT( !XSocketAddress::IsHostname("10.221.218.75") );
    UT_ASSERT(  XSocketAddress::IsHostname("www.google.com") );
    UT_ASSERT(  XSocketAddress::IsHostname("google.com") );
    UT_ASSERT(  XSocketAddress::IsHostname("com") );

    UT_ASSERT( !XSocketAddress::IsHostname("www.go ogle.com") );
    UT_ASSERT( !XSocketAddress::IsHostname("www.go\nogle.com") );
    UT_ASSERT( !XSocketAddress::IsHostname("www.go\rogle.com") );

    UT_ASSERT( !XSocketAddress::IsHostname("") );
    UT_ASSERT( !XSocketAddress::IsHostname(".") );
    UT_ASSERT( !XSocketAddress::IsHostname("..") );
    UT_ASSERT( !XSocketAddress::IsHostname("...") );
    UT_ASSERT( !XSocketAddress::IsHostname(". . .") );

    UT_ASSERT(  XSocketAddress::IsHostname("a") );
    UT_ASSERT(  XSocketAddress::IsHostname("a.") );
    UT_ASSERT(  XSocketAddress::IsHostname("a.b") );
    UT_ASSERT(  XSocketAddress::IsHostname("a.a.") );
    UT_ASSERT(  XSocketAddress::IsHostname("a.a.a") );
    UT_ASSERT(  XSocketAddress::IsHostname("a.a.a.") );
    UT_ASSERT(  XSocketAddress::IsHostname("a.a.a.a") );
    UT_ASSERT(  XSocketAddress::IsHostname("a.a.a.a.") );
    UT_ASSERT(  XSocketAddress::IsHostname("a.a.a.a.a") );
    UT_ASSERT( !XSocketAddress::IsHostname("a.a.a.a.a ") );
    UT_ASSERT( !XSocketAddress::IsHostname(" a.a.a.a.a") );
    UT_ASSERT( !XSocketAddress::IsHostname("a.a.a .a.a") );

    UT_ASSERT( !XSocketAddress::IsHostname(".1") );
    UT_ASSERT( !XSocketAddress::IsHostname("1") );
    UT_ASSERT( !XSocketAddress::IsHostname("1.") );
    UT_ASSERT( !XSocketAddress::IsHostname("1.1") );
    UT_ASSERT( !XSocketAddress::IsHostname("1.1.") );
    UT_ASSERT( !XSocketAddress::IsHostname("1.1.1") );
    UT_ASSERT( !XSocketAddress::IsHostname("1.1.1.") );
    UT_ASSERT( !XSocketAddress::IsHostname("1.1.1.1") );
    UT_ASSERT( !XSocketAddress::IsHostname("1.1.1.1.") );
    UT_ASSERT( !XSocketAddress::IsHostname("1.1.1.1.1") );
    UT_ASSERT( !XSocketAddress::IsHostname("1.1.1.1.1 ") );
    UT_ASSERT( !XSocketAddress::IsHostname(" 1.1.1.1.1") );
    UT_ASSERT( !XSocketAddress::IsHostname("1.1. 1.1.1") );

    UT_ASSERT( !XSocketAddress::IsHostname(".a") );
    UT_ASSERT(  XSocketAddress::IsHostname("1.a") );
    UT_ASSERT(  XSocketAddress::IsHostname("1.a.") );
    UT_ASSERT(  XSocketAddress::IsHostname("1.1.a") );
    UT_ASSERT(  XSocketAddress::IsHostname("1.1.a.") );
    UT_ASSERT(  XSocketAddress::IsHostname("1.1.1.a") );
    UT_ASSERT(  XSocketAddress::IsHostname("1.1.1.a.") );
    UT_ASSERT(  XSocketAddress::IsHostname("1.1.1.1.a") );
    UT_ASSERT( !XSocketAddress::IsHostname("1.1.1.1.a ") );
    UT_ASSERT( !XSocketAddress::IsHostname(" 1.1.1.1.a") );
    UT_ASSERT( !XSocketAddress::IsHostname("1.1. 1.1.a") );

    UT_ASSERT( !XSocketAddress::IsHostname("-") );
    UT_ASSERT( !XSocketAddress::IsHostname(".-") );
    UT_ASSERT( !XSocketAddress::IsHostname("-.") );
    UT_ASSERT( !XSocketAddress::IsHostname(".-.") );
    UT_ASSERT( !XSocketAddress::IsHostname("a-") );
    UT_ASSERT( !XSocketAddress::IsHostname("-a") );
    UT_ASSERT(  XSocketAddress::IsHostname("a-a") );
    UT_ASSERT(  XSocketAddress::IsHostname("a-1") );
    UT_ASSERT( !XSocketAddress::IsHostname("1-a") );

    UT_ASSERT( !XSocketAddress::IsHostname("-.hello") );
    UT_ASSERT( !XSocketAddress::IsHostname(".-.hello") );
    UT_ASSERT( !XSocketAddress::IsHostname("-.hello") );
    UT_ASSERT( !XSocketAddress::IsHostname(".-.hello") );
    UT_ASSERT( !XSocketAddress::IsHostname("a-.hello") );
    UT_ASSERT( !XSocketAddress::IsHostname("-a.hello") );
    UT_ASSERT(  XSocketAddress::IsHostname("a-a.hello") );
    UT_ASSERT(  XSocketAddress::IsHostname("a-1.hello") );
    UT_ASSERT(  XSocketAddress::IsHostname("1-a.hello") );

    UT_ASSERT( !XSocketAddress::IsHostname("hello.-") );
    UT_ASSERT( !XSocketAddress::IsHostname("hello.-.") );
    UT_ASSERT( !XSocketAddress::IsHostname("hello.-.") );
    UT_ASSERT( !XSocketAddress::IsHostname("hello.a-") );
    UT_ASSERT( !XSocketAddress::IsHostname("hello.-a") );
    UT_ASSERT(  XSocketAddress::IsHostname("hello.a-a") );
    UT_ASSERT(  XSocketAddress::IsHostname("hello.a-1") );
    UT_ASSERT( !XSocketAddress::IsHostname("hello.1-a") );
}
