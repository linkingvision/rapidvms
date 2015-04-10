
#include "XUDPSenderTest.h"

#include "XSDK/XUDPSender.h"
#include "XSDK/XUDPReceiver.h"
#include "XSDK/TimeUtils.h"

#ifndef WIN32
#include <unistd.h>
#endif

using namespace XSDK;
using namespace std;

REGISTER_TEST_FIXTURE(XUDPSenderTest);

void XUDPSenderTest::setup()
{
}

void XUDPSenderTest::teardown()
{
}

void* XUDPSenderTest::EntryPoint()
{
    switch( _testNum )
    {
    case 0:
    {
        try
        {
            XIRef<XMemory> buffer = new XMemory;

            XUDPReceiver server( 7474, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress );

            int destinationPort = 0;

            server.Receive( destinationPort, buffer );

            const unsigned char* reader = buffer->Map();

            _val = *((int*)reader);

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
        XIRef<XMemory> buffer = new XMemory;

        XUDPReceiver server( 7575, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress );

        int destinationPort = 0;

        server.Receive( destinationPort, buffer );

        const unsigned char* reader = buffer->Map();

        _val = *((int*)reader);
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

void XUDPSenderTest::TestSend()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for( int ii=0; ii<2; ++ii)
    {
        _testNum = 0;
        _recvAddress = XSocketAddress::GetAddressFamily(addrs[ii]) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        Start();

        x_sleep( 1 );

        XIRef<XMemory> buffer = new XMemory;
        buffer->Clear();
        unsigned char* writer = &buffer->Extend( 4 );
        *((int*)writer) = 42;

        XUDPSender client( addrs[ii], 7474 );

        client.Send( buffer->Map(), buffer->GetDataSize() );

        x_sleep( 1 );

        UT_ASSERT( _val == 42 );

        Join();
    }
}

void XUDPSenderTest::TestAim()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for( int ii=0; ii<2; ++ii)
    {
        _testNum =  1;
        _recvAddress = XSocketAddress::GetAddressFamily(addrs[ii]) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        Start();

        x_sleep( 1 );

        XIRef<XMemory> buffer = new XMemory;
        buffer->Clear();
        unsigned char* writer = &buffer->Extend( 4 );
        *((int*)writer) = 43;

        // Inititially, we're aimed at some random machine and port...
        XUDPSender client( "192.168.1.0", 1234 );

        // Then we re-aim...
        client.Aim( addrs[ii], 7575 );

        client.Send( buffer->Map(), buffer->GetDataSize() );

        x_sleep( 1 );

        UT_ASSERT( _val == 43 );

        Join();
    }
}

void XUDPSenderTest::TestGetSetSendBufferSize()
{
    {
        XUDPSender s("127.0.0.1", 0);
        UT_ASSERT_NO_THROW( s.GetSendBufferSize() );
        UT_ASSERT_NO_THROW( s.SetSendBufferSize(256 * 1024) );
    }
    {
        XUDPSender s("127.0.0.1", 0);
        size_t sizeOrig, sizeFinal;
        UT_ASSERT_NO_THROW( sizeOrig = s.GetSendBufferSize() );
        UT_ASSERT_NO_THROW( s.SetSendBufferSize(sizeOrig * 2) );
        UT_ASSERT_NO_THROW( sizeFinal = s.GetSendBufferSize() );
        UT_ASSERT( sizeFinal > sizeOrig );
    }
}
