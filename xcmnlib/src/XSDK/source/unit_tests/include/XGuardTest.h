
#ifndef __XGuardTest_h
#define __XGuardTest_h

#include "framework.h"

#include "XSDK/XMutex.h"
#include "XSDK/XTaskBase.h"

class XGuardTest : public test_fixture, XSDK::XTaskBase
{
public:
    TEST_SUITE_INIT(XGuardTest)
            _lock(),
            _testNum(0),
            _val(0)
    TEST_SUITE_BEGIN()
        TEST(XGuardTest::TestNormal);
        TEST(XGuardTest::TestManual);
        TEST(XGuardTest::TestGeneralCleaner);
        TEST(XGuardTest::TestDefaultCleaner);
        TEST(XGuardTest::TestClearCleaner);
        TEST(XGuardTest::TestSetCleaner);
    TEST_SUITE_END()

    virtual ~XGuardTest() throw()
    {
    }

    void* EntryPoint();

    void setup();
    void teardown();

protected:
    void TestGeneralCleaner();
    void TestDefaultCleaner();
    void TestSetCleaner();
    void TestClearCleaner();
    void TestNormal();
    void TestManual();

    XSDK::XMutex _lock;
    int _testNum;
    int _val;
};

#endif

