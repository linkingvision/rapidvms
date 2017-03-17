#ifndef XUDPSenderTest_h
#define XUDPSenderTest_h

#include "framework.h"

#ifdef WIN32
#include <winsock2.h>
#endif // WIN32

#include "XSDK/XTaskBase.h"
#include "XSDK/XUDPSender.h"

class XUDPSenderTest : public test_fixture, public XSDK::XTaskBase
{
public:
    TEST_SUITE(XUDPSenderTest);
        TEST(XUDPSenderTest::TestSend);
        TEST(XUDPSenderTest::TestAim);
        TEST(XUDPSenderTest::TestGetSetSendBufferSize);
    TEST_SUITE_END();

    virtual ~XUDPSenderTest() throw()
    {}

    void setup();
    void teardown();

    virtual void* EntryPoint();

protected:

    void TestSend();
    void TestAim();
    void TestGetSetSendBufferSize();

private:

    int _testNum;
    int _val;
    XSDK::XString _recvAddress;
};

#endif
