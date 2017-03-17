#ifndef XUDPReceiverTest_h
#define XUDPReceiverTest_h

#include "framework.h"

#ifdef WIN32
#include <winsock2.h>
#endif // WIN32

#include "XSDK/XTaskBase.h"
#include "XSDK/XString.h"
#include "XSDK/XUDPReceiver.h"

class XUDPReceiverTest : public test_fixture, public XSDK::XTaskBase
{
public:
    TEST_SUITE(XUDPReceiverTest);
        TEST(XUDPReceiverTest::TestSend);
        TEST(XUDPReceiverTest::TestSendMulticast);
        TEST(XUDPReceiverTest::TestAssociatedSend);
        TEST(XUDPReceiverTest::TestGetSetRecvBufferSize);
        TEST(XUDPReceiverTest::TestShutdownCloseWhileBlocked);
        TEST(XUDPReceiverTest::TestRawRecv);
        TEST(XUDPReceiverTest::TestShutdownCloseWhileBlockedInRawRecv);
    TEST_SUITE_END();

    virtual ~XUDPReceiverTest() throw()
    {}

    void setup();
    void teardown();

    virtual void* EntryPoint();

protected:

    void TestSend();
    void TestSendMulticast();
    void TestAssociatedSend();
    void TestGetSetRecvBufferSize();
    void TestShutdownCloseWhileBlocked();
    void TestRawRecv();
    void TestShutdownCloseWhileBlockedInRawRecv();

private:

    int _testNum;
    int _val;
    XSDK::XString _recvAddress;
    XSDK::XString _interfaceAddress;
    XIRef<XSDK::XUDPReceiver> _receiver;

};

#endif
