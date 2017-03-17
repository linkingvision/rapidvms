#ifndef XConditionTest_h
#define XConditionTest_h

#include "framework.h"

#include "XSDK/XTaskBase.h"
#include "XSDK/XMutex.h"
#include "XSDK/XCondition.h"

class XConditionTest : public test_fixture, public XSDK::XTaskBase
{
public:
    TEST_SUITE_INIT(XConditionTest)
        _testNum(0),
        _val(0),
        _waiting(false),
        _lok(),
        _cond(_lok)
    TEST_SUITE_BEGIN()
        TEST(XConditionTest::TestConditionSimple);
        TEST(XConditionTest::TestFutureMonoNegative);
        TEST(XConditionTest::TestWaitUntil);
        TEST(XConditionTest::TestWaitUntilReturnValue);
    TEST_SUITE_END()

    virtual ~XConditionTest() throw()
    {}

    void setup()
    {
        _testNum = 0;
        _val = 0;
        _waiting = false;
    }

    void teardown() {}

    virtual void* EntryPoint();

protected:

    void TestConditionSimple();
    void TestFutureMonoNegative();
    void TestWaitUntil();
    void TestWaitUntilReturnValue();

private:

    int _testNum;
    int _val;
    bool _waiting;
    XSDK::XMutex _lok;
    XSDK::XCondition _cond;
};

#endif
