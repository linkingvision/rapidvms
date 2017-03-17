#ifndef XTaskBaseTest_h
#define XTaskBaseTest_h

#include "framework.h"

#include "XSDK/XTaskBase.h"
#include "XSDK/XCondition.h"
#include "XSDK/XGuard.h"
#include "XSDK/XString.h"

class XTaskBaseTest : public test_fixture
{
public:
    TEST_SUITE(XTaskBaseTest);
        TEST(XTaskBaseTest::TestParrallelExecution);
        TEST(XTaskBaseTest::TestTaskBase);
        TEST(XTaskBaseTest::TestCancelThread);
        TEST(XTaskBaseTest::TestPoolJoin);
        TEST(XTaskBaseTest::TestUniqueThreadIDs);
        TEST(XTaskBaseTest::TestBuildDestroyPool);
        TEST(XTaskBaseTest::TestPool);
        TEST(XTaskBaseTest::TestThreadNames);
        TEST(XTaskBaseTest::TestGetCPU);
    TEST_SUITE_END();

    void setup() {}
    void teardown() {}

protected:

    void TestTaskBase();
    void TestCancelThread();
    void TestUniqueThreadIDs();
    void TestParrallelExecution();
    void TestPool();
    void TestBuildDestroyPool();
    void TestPoolJoin();
    void TestThreadNames();
    void TestGetCPU();

private:
};

class Thread : public XSDK::XTaskBase
{
    friend class XTaskBaseTest;

public:

    Thread(bool wait = false, int result = 42)
        : XTaskBase( "mighty_thread" ),
          _wait(wait),
          _myretval(0),
          _result(result),
          _signaled(false),
          _lock(),
          _cond(_lock)
    {}

    Thread(XIRef<XSDK::XThreadPool> pool, bool wait = false, int result = 42)
        : XTaskBase(pool),
          _wait(wait),
          _myretval(0),
          _result(result),
          _signaled(false),
          _lock(),
          _cond(_lock)
    {}

    void* EntryPoint()
    {
        UT_ASSERT(CurrThread() == this);

        // Wait for test to be ready to continue
        if( _wait )
        {
            XSDK::XGuard lock(_lock);
            while(!_signaled)
                _cond.Wait();
            _signaled = false;
        }

        _myretval = _result;

        return &_myretval;
    }

    void Signal()
    {
        XSDK::XGuard lock(_lock);
        _signaled = true;
        _cond.Signal();
    }

private:

    bool _wait;
    int  _myretval;
    int  _result;

    bool _signaled;
    XSDK::XMutex _lock;
    XSDK::XCondition _cond;
};

class NamedThread : public XSDK::XTaskBase
{
    friend class XTaskBaseTest;

public:

    NamedThread( const XSDK::XString& threadName )
        : XTaskBase( threadName ),
        _name( threadName ),
        _nameWasSet( false )
    {}

    void* EntryPoint()
    {
#ifdef IS_LINUX
        char nameBuffer[32];
        memset( nameBuffer, 0, 32 );

        prctl(PR_GET_NAME, (unsigned long)nameBuffer, 0, 0, 0);

        XSDK::XString fetchedName = nameBuffer;

        if( fetchedName == _name )
            _nameWasSet = true;
        else _nameWasSet = false;
#endif
        return NULL;
    }

    bool NameWasSet()
    {
        return _nameWasSet;
    }

private:
    XSDK::XString _name;
    bool _nameWasSet;
};

#endif
