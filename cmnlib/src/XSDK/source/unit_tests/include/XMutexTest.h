#ifndef XMutexTest_h
#define XMutexTest_h

#include "framework.h"

#include "XSDK/XTaskBase.h"
#include "XSDK/XMutex.h"

class XMutexTest : public test_fixture, public XSDK::XTaskBase
{
public:
    TEST_SUITE(XMutexTest);
        TEST(XMutexTest::TestMutexSimple);
        TEST(XMutexTest::TestLockTwice);
        TEST(XMutexTest::TestUnlockWhileNotLocked);
        TEST(XMutexTest::TestLotsOfLocks);
    TEST_SUITE_END();

    virtual ~XMutexTest() throw()
    {}

    void setup();
    void teardown();

    virtual void* EntryPoint();

protected:

    void TestMutexSimple();
    void TestLockTwice();
    void TestUnlockWhileNotLocked();
    void TestLotsOfLocks();

private:

    int _testNum;
    int _val;
    XSDK::XMutex _lok;
};

#endif
