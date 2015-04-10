#ifndef XSocketTest_h
#define XSocketTest_h

#include "framework.h"

#ifdef WIN32
#include <winsock2.h>
#endif // WIN32

#include "XSDK/XTaskBase.h"
#include "XSDK/XSocket.h"
#include "XSDK/XMutex.h"

class XSocketTest : public test_fixture, public XSDK::XTaskBase
{
public:
    TEST_SUITE(XSocketTest);
        TEST(XSocketTest::TestConnect);
        TEST(XSocketTest::TestDualProtocolServer);
        //TEST(TestSendTimeout);
        TEST(XSocketTest::TestGetHostname);
        TEST(XSocketTest::TestGetAddressesByHostname);
        TEST(XSocketTest::TestGetInterfaceAddresses);
        TEST(XSocketTest::TestBindEphemeral);
        TEST(XSocketTest::TestScan);
    TEST_SUITE_END();

    virtual ~XSocketTest() throw()
    {}

    void setup();
    void teardown();

    virtual void* EntryPoint();

    bool _expectingTimeout;
    bool _gotTimeout;

protected:
    void TestRecvTimeout();
    void TestScan();
    void TestBindEphemeral();
    void TestConnect();
    void TestDualProtocolServer();
    void TestSendTimeout();
    void TestGetHostname();
    void TestGetAddressesByHostname();
    void TestGetInterfaceAddresses();

private:
    XSDK::XMutex _lock;
    int _portBoundOn;
    int _testNum;
    XSDK::XString _serverIPAddr;
    XSDK::XSocket::XSocketType _serverSocketType;
    int _val;
};

#endif
