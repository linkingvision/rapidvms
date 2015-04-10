#ifndef XSSLSocketTest_h
#define XSSLSocketTest_h

#include "framework.h"
#include "XSDK/XTaskBase.h"
#include "XSDK/XMutex.h"

class XSSLSocketTest : public test_fixture, public XSDK::XTaskBase
{
public:
    TEST_SUITE(XSSLSocketTest);
        TEST(XSSLSocketTest::TestConnect);
        TEST(XSSLSocketTest::TestNonDefaultRecvBufferSize);
        TEST(XSSLSocketTest::TestDestructor);
        TEST(XSSLSocketTest::TestCloseWarmSocket);
        TEST(XSSLSocketTest::TestTwoConnects);
        TEST(XSSLSocketTest::TestBind);
        TEST(XSSLSocketTest::TestAccept);
        TEST(XSSLSocketTest::TestEncryptedPrivateKey);
        TEST(XSSLSocketTest::TestShutdown);
        TEST(XSSLSocketTest::TestVerify);
        TEST(XSSLSocketTest::TestRecvTimeout);
        TEST(XSSLSocketTest::TestXSocketAssign);
    TEST_SUITE_END();

    virtual ~XSSLSocketTest() throw()
    {}

    void setup();
    void teardown();

    virtual void* EntryPoint();

protected:
    void TestConnect();
    void TestNonDefaultRecvBufferSize();
    void TestDestructor();
    void TestCloseWarmSocket();
    void TestTwoConnects();
    void TestBind();
    void TestAccept();
    void TestEncryptedPrivateKey();
    void TestShutdown();
    void TestVerify();
    void TestRecvTimeout();
    void TestXSocketAssign();

private:
    bool RecvTimeoutCB()
    {
        _val = 1;
        return _recvTimeoutAnswer;
    }

    static bool _RecvTimeoutCB( void* opaque )
    {
        if( opaque )
            return ((XSSLSocketTest*)opaque)->RecvTimeoutCB();

        return false;
    }

    XSDK::XMutex _lock;
    int _testNum;
    XSDK::XString _serverIPAddr;
    int _val;

    bool _recvTimeoutAnswer;
};

#endif
