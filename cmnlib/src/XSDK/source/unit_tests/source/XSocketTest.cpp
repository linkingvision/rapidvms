
#include "XSocketTest.h"

#include "XSDK/XSocket.h"
#include "XSDK/XGuard.h"
#include "XSDK/TimeUtils.h"

#ifndef WIN32
#include <unistd.h>
#endif

using namespace XSDK;
using namespace std;

REGISTER_TEST_FIXTURE(XSocketTest);

bool SendTimeoutHandler( void* opaque )
{
    XSocketTest* st = (XSocketTest*)opaque;

    UT_ASSERT( false );

    if( st )
    {
        if( st->_expectingTimeout == true )
        {
            st->_gotTimeout = true;

            return false;
        }
    }

    UT_ASSERT( true && "Unexpected socket timeout!" );

    return true;
}

bool RecvTimeoutHandler( void* opaque )
{
    return false;
}

void XSocketTest::setup()
{
    _serverIPAddr.clear();
}

void XSocketTest::teardown()
{
}

void* XSocketTest::EntryPoint()
{
    switch( _testNum )
    {
    case 0:
    {
        try
        {
            XSocket serverSok;

            serverSok.Bind( 5242, _serverIPAddr );
            serverSok.Listen();
            XRef<XSocket> connected = serverSok.Accept();

            unsigned int val = 0;

            connected->Recv( &val, 4 );

            val += 1;

            connected->Send( &val, 4 );
        }

        catch( XSDK::XException& ex )
        {
            printf("%s\n",ex.GetMsg());
            printf("%s     @    %d\n",ex.GetSrcFile(), ex.GetLineNum());
            UT_ASSERT( false && "Test threw!" );
        }
    }
    break;
    case 1:
    {
        try
        {
            XSocket serverSok;
            int sokId = serverSok.GetSokId();
            int rcvSize = 4096;
            setsockopt( sokId, SOL_SOCKET, SO_RCVBUF, (char*)&rcvSize, (int)sizeof(rcvSize) );

            serverSok.Bind( 6242, _serverIPAddr );
            serverSok.Listen();
            XRef<XSocket> connected = serverSok.Accept();

            x_usleep( 2000000 ); // should force a send timeout...

            unsigned char msg[262144];
            memset( msg, 0, 262144 );

            connected->Recv( msg, 262144 );

            unsigned int val = 0;
            for( int i = 0; i < 262144; i++ )
                val += msg[i];

            connected->Send( &val, 4 );
        }

        catch( XSDK::XException& ex )
        {
            printf("%s\n",ex.GetMsg());
            printf("%s     @    %d\n",ex.GetSrcFile(), ex.GetLineNum());
            UT_ASSERT( false && "Test threw!" );
        }
    }
    break;
    case 2:
        {
            unsigned int val = 0;
            int count = 0;
            timeval tv;
            x_gettimeofday(&tv);
            srand(tv.tv_sec);
            while ( count++ < 3 )
            {
                try
                {
                    XSocket serverSok;
                    _portBoundOn = 49352 + rand() % 20;
                    {
                        XGuard guard(_lock);
                        serverSok.Bind(_portBoundOn, _serverIPAddr);
                    }
                    serverSok.Listen();
                    {
                        // The client port scan causes a connect to happen, but
                        // that's not socket that will be used by the client to
                        // actually send data.
                        serverSok.Accept();
                    }
                    serverSok.Listen();
                    XRef<XSocket> connected = serverSok.Accept();
                    connected->Recv( &val, 4 );
                    val *= 3;
                    connected->Send( &val, 4 );

                    break;
                }
                catch( XSDK::XException& ex )
                {
                    printf("Exception: %s\n",ex.what());
                    if ( count == 3 )
                        UT_ASSERT(false && "Unable to find availible port");
                }
            }
        }
        break;
    case 3:
    {
        // Tests constructor that specifies XSocketType
        try
        {
            XSocket serverSok(_serverSocketType);
            if (_serverSocketType == XSocket::IPV4)
                serverSok.Bind( 5242, ip4_addr_any );
            else
                serverSok.Bind( 5242, ip6_addr_any );
            serverSok.Listen();
            XRef<XSocket> connected = serverSok.Accept();

            unsigned int val = 0;

            connected->Recv( &val, 4 );

            val += 1;

            connected->Send( &val, 4 );
        }

        catch( XSDK::XException& ex )
        {
            printf("%s\n",ex.GetMsg());
            printf("%s     @    %d\n",ex.GetSrcFile(), ex.GetLineNum());
            UT_ASSERT( false && "Test threw!" );
        }
    }
    break;
    default:
        break;
    };
    return &_val;
}
void XSocketTest::TestBindEphemeral()
{

    {
        XSocket socket;
        int port = 0;
        UT_ASSERT_NO_THROW( port = socket.BindEphemeral() );
        UT_ASSERT( port >= 1024 && port <= 65535 );
    }
    {
        XSocket socket;
        int port = 0;
        UT_ASSERT_NO_THROW( port = socket.BindEphemeral("127.0.0.1") );
        UT_ASSERT( port >= 1024 && port <= 65535 );
    }
}

void XSocketTest::TestConnect()
{

    const char addrs[][32] = { "127.0.0.1", "localhost", "::1" };

    try
    {
        for (int ii=0; ii<3; ++ii)
        {

            _testNum = 0;
            _serverIPAddr = addrs[ii];
            unsigned int val = 41;
            XSocket clientSok;
            Start();
            x_usleep( 100000 );
            clientSok.Connect( _serverIPAddr, 5242 );

            // Test GetPeerIp()
            UT_ASSERT_NO_THROW( clientSok.GetPeerIp() );
            UT_ASSERT( clientSok.GetPeerIp() != "0.0.0.0" );

            // Test GetLocalIp()
            UT_ASSERT_NO_THROW( clientSok.GetLocalIp() );
            UT_ASSERT( clientSok.GetLocalIp() != "0.0.0.0" );

            clientSok.Send( &val, 4 );
            clientSok.SetRecvTimeout( 500 );
            clientSok.Recv( &val, 4 );
            UT_ASSERT( val == 42 );
            Join();
        }

    }

    catch( XSDK::XException& )
    {
        UT_ASSERT( false && "TestConnect threw!" );
    }
}

void XSocketTest::TestDualProtocolServer()
{
    try
    {
        // Test IPv4 only
        {

            _testNum = 3;
            _serverSocketType = XSocket::IPV4;
            unsigned int val = 41;
            XSocket clientSok;
            Start();
            x_sleep( 1 );

            // IPv4
            clientSok.Connect( "127.0.0.1", 5242 );
            clientSok.Send( &val, 4 );
            clientSok.SetRecvTimeout( 500 );
            clientSok.Recv( &val, 4 );
            UT_ASSERT( val == 42 );

            // IPv6
            UT_ASSERT_THROWS( clientSok.Connect( "::1", 5242 ), XSocketException );

            Join();
        }

        // Test IPv6 only
        {

            _testNum = 3;
            _serverSocketType = XSocket::IPV6;
            unsigned int val = 41;
            XSocket clientSok;
            Start();
            x_sleep( 1 );

            // IPv4
            clientSok.Connect( "::1", 5242 );
            clientSok.Send( &val, 4 );
            clientSok.SetRecvTimeout( 500 );
            clientSok.Recv( &val, 4 );
            UT_ASSERT( val == 42 );

            // IPv6
            UT_ASSERT_THROWS( clientSok.Connect( "127.0.0.1", 5242 ), XSocketException );

            Join();
        }

        // Test both IPv4 and IPv6
        // [tdistler] Windows XP doesn't support IPV6_V6ONLY option... detect XP and skip if necessary
        bool IPv6OnlySupported = true;
#ifdef IS_WINDOWS
        OSVERSIONINFO version;
        memset(&version, 0, sizeof(OSVERSIONINFO));
        version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        GetVersionEx(&version);
        if (version.dwMajorVersion < 6)
            IPv6OnlySupported = false; // XP or earlier
#endif
        if (IPv6OnlySupported)
        {
            const char addrs[][32] = { "127.0.0.1", "localhost", "::1" };

            // Keep creating the same server... it should accept both IPv6 and IPv4 traffic
            for (int ii=0; ii<3; ++ii)
            {

                _testNum = 3;
                _serverSocketType = XSocket::IPV6_WITH_IPV4;
                unsigned int val = 41;
                XSocket clientSok;
                Start();
                x_sleep( 1 );
                clientSok.Connect( addrs[ii], 5242 );
                clientSok.Send( &val, 4 );
                clientSok.SetRecvTimeout( 500 );
                clientSok.Recv( &val, 4 );
                UT_ASSERT( val == 42 );
                Join();
            }
        }
        else
        {
            printf("  SKIPPING IPv6 w/ IPv4 (Not supported in Windows XP)\n");
            fflush(stdout);
        }

    }
    catch( XSDK::XException& )
    {
        UT_ASSERT( false && "TestDualProtocolServer threw!" );
    }
}

bool ScanOpPortTester(XString ip, int port)
{
    XSocket clientSok;
    clientSok.Connect(ip,port);
    unsigned int val = 1;
    clientSok.Send( &val, 4 );
    int time = 10000;
    if ( clientSok.WaitRecv(time) )
        return false;
    clientSok.Recv( &val, 4 );
    return ( val == 3 );
}
void XSocketTest::TestScan()
{

    const char addrs[][32] = { "127.0.0.1", "::1" };

    try
    {
        for (int ii=0; ii<2; ++ii)
        {

            _testNum = 2;
            _serverIPAddr = addrs[ii];
            unsigned int val = 667;

            XSocket clientSok;
            Start();
            x_usleep( 200000 );
            XGuard guard(_lock);
            int port = XSocket::ScanPortRange(ScanOpPortTester,_serverIPAddr,_portBoundOn-10,_portBoundOn+10);
            clientSok.Connect(_serverIPAddr,port);
            clientSok.Send( &val, 4 );
            clientSok.SetRecvTimeout( 5000 );
            clientSok.Recv( &val, 4 );
            UT_ASSERT( val == 2001 );
            Join();
        }
    }

    catch( XSDK::XException& )
    {
        UT_ASSERT( false && "TestConnect threw!" );
    }

}

void XSocketTest::TestSendTimeout()
{

    const char addrs[][32] = { "127.0.0.1", "::1" };

    try
    {
        for (int ii=0; ii<2; ++ii)
        {

            _testNum = 1;
            _serverIPAddr = addrs[ii];

            XSocket clientSok;

            Start();

            x_usleep( 100000 );

            clientSok.SetSendTimeout( 10 );
            clientSok.AttachSendTimeoutHandler( SendTimeoutHandler, this );

            const int sokId = clientSok.GetSokId();
            const int sndSize = 4096;
            setsockopt( sokId, SOL_SOCKET, SO_SNDBUF, (char*)&sndSize, (int)sizeof(sndSize) );

            unsigned char msg[262144];
            memset( msg, 1, 262144 );

            clientSok.Connect( _serverIPAddr, 6242 );

            _gotTimeout = false;
            _expectingTimeout = true;

            clientSok.Send( &msg, 262144 );

            _expectingTimeout = false;
            UT_ASSERT( _gotTimeout == true );

            unsigned int sum = 0;

            clientSok.Recv( &sum, 4 );

            Join();

            UT_ASSERT( sum == 262144 );
        }
    }

    catch( XSDK::XException& ex )
    {
        printf("%s\n",ex.GetMsg());
        printf("%s     @    %d\n",ex.GetSrcFile(), ex.GetLineNum());
        UT_ASSERT( false && "Test threw!" );
    }
}

void XSocketTest::TestRecvTimeout()
{
}

void XSocketTest::TestGetHostname()
{
    XString hostname = XSocket::GetHostname();

    UT_ASSERT( !hostname.empty() );
}

void XSocketTest::TestGetAddressesByHostname()
{
    vector<XString> addresses = XSocket::GetAddressesByHostname( "localhost" );
    UT_ASSERT( !addresses.empty() );
}
//NOTE: I manually tested multiple addresses on a single interface by assigning
//multiple ipv6 addresses to my machine and running this test.
//I can't think of a non-error-prone way to do this automatically...
void XSocketTest::TestGetInterfaceAddresses()
{
#if 0
    XHash<std::vector<XString> > interfaceAddresses = XSocket::GetInterfaceAddresses();
#ifndef WIN32
    XString localHostAddress = interfaceAddresses.Find("lo")->front();

    XString ipAddress;
    if(interfaceAddresses.Find("eth0"))
        ipAddress = interfaceAddresses.Find("eth0")->front();
    else if(interfaceAddresses.Find("eth1"))
        ipAddress = interfaceAddresses.Find("eth1")->front();
    else if(interfaceAddresses.Find("p5p1"))
        ipAddress = interfaceAddresses.Find("p5p1")->front();
    else if(interfaceAddresses.Find("p2p1"))
        ipAddress = interfaceAddresses.Find("p2p1")->front();
    else if(interfaceAddresses.Find("wlan0"))
        ipAddress = interfaceAddresses.Find("wlan0")->front();
    else if(interfaceAddresses.Find("em0"))
        ipAddress = interfaceAddresses.Find("em0")->front();
    else if(interfaceAddresses.Find("em1"))
        ipAddress = interfaceAddresses.Find("em1")->front();

    UT_ASSERT(localHostAddress == "127.0.0.1");
    UT_ASSERT(!ipAddress.empty());
#else

    // The bamboo test boxes don't always seem to give us "Local Area Connection"
    // and have at least 2 - 4.
    XString ipAddress;
    if(interfaceAddresses.Find("Local Area Connection"))
        ipAddress = interfaceAddresses.Find("Local Area Connection")->front();
    else if(interfaceAddresses.Find("Local Area Connection 1"))
        ipAddress = interfaceAddresses.Find("Local Area Connection 1")->front();
    else if(interfaceAddresses.Find("Local Area Connection 2"))
        ipAddress = interfaceAddresses.Find("Local Area Connection 2")->front();
    else if(interfaceAddresses.Find("Local Area Connection 3"))
        ipAddress = interfaceAddresses.Find("Local Area Connection 3")->front();
    else if(interfaceAddresses.Find("Local Area Connection 4"))
        ipAddress = interfaceAddresses.Find("Local Area Connection 4")->front();
    else if(interfaceAddresses.Find("Local Area Connection 5"))
        ipAddress = interfaceAddresses.Find("Local Area Connection 5")->front();
    else if(interfaceAddresses.Find("Local Area Connection 6"))
        ipAddress = interfaceAddresses.Find("Local Area Connection 6")->front();

    // TODO: Find out if we need this. Buildboxes do not have this interface.
    if(interfaceAddresses.Find("Loopback Pseudo-Interface 1"))
    {
        XString localHostAddress = interfaceAddresses.Find("Loopback Pseudo-Interface 1")->front();
        UT_ASSERT(localHostAddress == "127.0.0.1");
    }

    // The bamboo test boxes appear to have this one.
    if(interfaceAddresses.Find("MS TCP Loopback interface"))
    {
        XString localHostAddress = interfaceAddresses.Find("MS TCP Loopback interface")->front();
        UT_ASSERT(localHostAddress == "127.0.0.1");
    }

    UT_ASSERT(!ipAddress.empty());
#endif
#endif
}
