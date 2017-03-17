
#include "XUDPReceiverTest.h"

#include "XSDK/XUDPSender.h"
#include "XSDK/XUDPReceiver.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XHash.h"
#include "XSDK/XSocket.h"

#ifndef WIN32
#include <unistd.h>
#endif

using namespace XSDK;
using namespace std;

REGISTER_TEST_FIXTURE(XUDPReceiverTest);

void XUDPReceiverTest::setup()
{
    _recvAddress.clear();
    XSocket::SocketStartup();
}

void XUDPReceiverTest::teardown()
{
}

void* XUDPReceiverTest::EntryPoint()
{
    switch(_testNum)
    {
        case 0:
        {
            try
            {
                XIRef<XMemory> buffer = new XMemory;

                XUDPReceiver server(8484, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress);

                int destinationPort = 0;

                server.Receive(destinationPort, buffer);

                const unsigned char* reader = buffer->Map();

                _val = *((int*)reader);
            }

            catch(XSDK::XException& ex)
            {
                printf("%s\n",ex.GetMsg());
                printf("%s     @    %d\n", ex.GetSrcFile(), ex.GetLineNum());
                UT_ASSERT(false && "Test threw!");
            }
        }
        break;
        case 1:
        {
            try
            {
                XIRef<XMemory> buffer = new XMemory;

                // Bind to a specific interface
                XUDPReceiver server(8686, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress);

                int destinationPort = 0;

                server.Receive(destinationPort, buffer);

                const unsigned char* reader = buffer->Map();

                _val = *((int*)reader);
            }

            catch(XSDK::XException& ex)
            {
                printf("%s\n",ex.GetMsg());
                printf("%s     @    %d\n", ex.GetSrcFile(), ex.GetLineNum());
                UT_ASSERT(false);
            }
        }
        break;
        case 2:
        {
            try
            {
                XIRef<XMemory> buffer = new XMemory;

                // Bind to a specific interface
                XIRef<XUDPReceiver> serverA = new XUDPReceiver(8787, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress);
                XIRef<XUDPReceiver> serverB = new XUDPReceiver(8888, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress);

                serverA->Associate(serverB);

                int destinationPort = 0;

                serverA->Receive(destinationPort, buffer);

                UT_ASSERT(destinationPort == 8787);

                const unsigned char* reader = buffer->Map();

                _val += *((int*)reader);

                serverA->Receive(destinationPort, buffer);

                reader = buffer->Map();

                _val += *((int*)reader);

                UT_ASSERT(destinationPort == 8888);
            }

            catch(XSDK::XException& ex)
            {
                printf("%s\n",ex.GetMsg());
                printf("%s     @    %d\n", ex.GetSrcFile(), ex.GetLineNum());
                UT_ASSERT(false);
            }
        }
        break;
        case 3:
        {
            try
            {
                XIRef<XMemory> buffer = new XMemory(1600);

                int destinationPort = 0;

                _receiver->Receive(destinationPort, buffer);

                _val = 1234;

            }

            catch(XSDK::XException& ex)
            {
                printf("%s\n",ex.GetMsg());
                printf("%s     @    %d\n", ex.GetSrcFile(), ex.GetLineNum());
                UT_ASSERT(false);
            }
        }
        break;
        case 4:
        {
            try
            {
                XIRef<XMemory> buffer = new XMemory;

                XUDPReceiver server(9090, DEFAULT_UDP_RECV_BUF_SIZE, _recvAddress);

                int destinationPort = 0;

                server.RawReceive(destinationPort, buffer);

                const unsigned char* reader = buffer->Map();

                _val = *((int*)reader);
            }

            catch(XSDK::XException& ex)
            {
                printf("%s\n",ex.GetMsg());
                printf("%s     @    %d\n", ex.GetSrcFile(), ex.GetLineNum());
                UT_ASSERT(false && "Test threw!");
            }
        }
        break;
        case 5:
        {
            try
            {
                XIRef<XMemory> buffer = new XMemory(1600);

                int destinationPort = 0;

                _receiver->RawReceive(destinationPort, buffer);

                _val = 4321;

            }

            catch(XSDK::XException& ex)
            {
                printf("%s\n",ex.GetMsg());
                printf("%s     @    %d\n", ex.GetSrcFile(), ex.GetLineNum());
                UT_ASSERT(false);
            }
        }
        break;
        default:
        break;
    };

    return &_val;
}

void XUDPReceiverTest::TestSend()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for(int ii=0; ii<2; ++ii)
    {
        printf("  %s\n", addrs[ii]);
        fflush(stdout);

        _testNum = 0;
        _recvAddress = XSocketAddress::GetAddressFamily(addrs[ii]) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        Start();

        x_sleep(2);

        XIRef<XMemory> buffer = new XMemory;
        buffer->Clear();
        unsigned char* writer = &buffer->Extend(4);
        *((int*)writer) = 42;

        XUDPSender client(addrs[ii], 8484);

        client.Send(buffer->Map(), buffer->GetDataSize());

        x_sleep(1);

        UT_ASSERT(_val == 42);

        Join();
    }
}

void XUDPReceiverTest::TestSendMulticast()
{
#if 0
    XHash<std::vector<XString> > interfaceAddresses = XSocket::GetInterfaceAddresses(AF_INET);

    XString ipAddress;

#ifdef WIN32
    // The bamboo test boxes don't always seem to give us "Local Area Connection"
    // and have at least 2 - 4.
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
#else
    if(interfaceAddresses.Find("eth0"))
        ipAddress = interfaceAddresses["eth0"].front();
    else if(interfaceAddresses.Find("p5p1"))
        ipAddress = interfaceAddresses["p5p1"].front();
    else if(interfaceAddresses.Find("p2p1"))
        ipAddress = interfaceAddresses["p2p1"].front();
    else if(interfaceAddresses.Find("wlan0"))
        ipAddress = interfaceAddresses["wlan0"].front();
    else if(interfaceAddresses.Find("em0"))
        ipAddress = interfaceAddresses["em0"].front();
    else if(interfaceAddresses.Find("em1"))
        ipAddress = interfaceAddresses["em1"].front();
    else X_THROW(("Unknown interface name."));
#endif

    vector<XString> addressParts;
    ipAddress.Split(".", addressParts);

    XString localMulticastAddress = XString::Format("239.%s.%s.%s", addressParts[1].c_str(), addressParts[2].c_str(), addressParts[3].c_str());

    XString addrs[] = { localMulticastAddress };

    printf("  %s\n", addrs[0].c_str());
    fflush(stdout);

    _testNum = 1;
    _recvAddress = addrs[0];
    _val = 0;

    Start();

    x_sleep(1);

    XIRef<XMemory> buffer = new XMemory;
    buffer->Clear();
    unsigned char* writer = &buffer->Extend(4);
    *((int*)writer) = 42;

    XUDPSender client(addrs[0], 8686);

    client.Send(buffer->Map(), buffer->GetDataSize());

    x_sleep(1);

    UT_ASSERT(_val == 42);

    Join();

#endif
}

void XUDPReceiverTest::TestAssociatedSend()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for(int ii=0; ii<2; ++ii)
    {
        printf("  %s\n", addrs[ii]);
        fflush(stdout);

        _testNum = 2;
        _recvAddress = XSocketAddress::GetAddressFamily(addrs[ii]) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        Start();

        x_usleep(1000000);

        XIRef<XMemory> buffer = new XMemory;
        buffer->Clear();
        unsigned char* writer = &buffer->Extend(4);
        *((int*)writer) = 42;

        XUDPSender clientA(addrs[ii], 8787);

        clientA.Send(buffer->Map(), buffer->GetDataSize());

        XUDPSender clientB(addrs[ii], 8888);

        clientB.Send(buffer->Map(), buffer->GetDataSize());

        x_usleep(1000000);

        UT_ASSERT(_val == 84);

        Join();
    }
}

void XUDPReceiverTest::TestGetSetRecvBufferSize()
{
    {
        XUDPReceiver r;
        UT_ASSERT_NO_THROW(r.GetRecvBufferSize());
        UT_ASSERT_NO_THROW(r.SetRecvBufferSize(256 * 1024));
    }
    {
        XUDPReceiver r;
        size_t sizeOrig, sizeFinal;
        UT_ASSERT_NO_THROW(sizeOrig = r.GetRecvBufferSize());
        UT_ASSERT_NO_THROW(r.SetRecvBufferSize(sizeOrig * 2));
        UT_ASSERT_NO_THROW(sizeFinal = r.GetRecvBufferSize());
        UT_ASSERT(sizeFinal > sizeOrig);
    }
}

void XUDPReceiverTest::TestShutdownCloseWhileBlocked()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for(int ii=0; ii<2; ++ii)
    {
        printf("  %s\n", addrs[ii]);
        fflush(stdout);

        _testNum = 3;
        _recvAddress = XSocketAddress::GetAddressFamily(addrs[ii]) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        _receiver = new XUDPReceiver(8989, DEFAULT_UDP_RECV_BUF_SIZE, addrs[ii]);

        Start();

        x_sleep(1);

        _receiver->Shutdown();

        _receiver->Close();

        Join();

        UT_ASSERT(_val == 1234);
    }
}

void XUDPReceiverTest::TestRawRecv()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for(int ii=0; ii<2; ++ii)
    {
        printf("  %s\n", addrs[ii]);
        fflush(stdout);

        _testNum = 4;
        _recvAddress = XSocketAddress::GetAddressFamily(addrs[ii]) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        Start();

        x_sleep(2);

        XIRef<XMemory> buffer = new XMemory;
        buffer->Clear();
        unsigned char* writer = &buffer->Extend(4);
        *((int*)writer) = 24;

        XUDPSender client(addrs[ii], 9090);

        client.Send(buffer->Map(), buffer->GetDataSize());

        x_sleep(1);

        UT_ASSERT(_val == 24);

        Join();
    }
}

void XUDPReceiverTest::TestShutdownCloseWhileBlockedInRawRecv()
{
    const char addrs[][32] = { "127.0.0.1", "::1" };

    for(int ii=0; ii<2; ++ii)
    {
        printf("  %s\n", addrs[ii]);
        fflush(stdout);

        _testNum = 5;
        _recvAddress = XSocketAddress::GetAddressFamily(addrs[ii]) == AF_INET ? ip4_addr_any : ip6_addr_any;
        _val = 0;

        _receiver = new XUDPReceiver(9191, DEFAULT_UDP_RECV_BUF_SIZE, addrs[ii]);

        Start();

        x_sleep(1);

        _receiver->Close();

        Join();

        UT_ASSERT(_val == 4321);
    }
}
