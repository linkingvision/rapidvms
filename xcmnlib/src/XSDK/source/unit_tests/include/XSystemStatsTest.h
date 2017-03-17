
#ifndef _X_SYSTEM_STATS_TEST_H_
#define _X_SYSTEM_STATS_TEST_H_

#include "framework.h"
#include "XSDK/XIRef.h"
#include "XSDK/XIWeakRef.h"
#include "XSDK/XTaskBase.h"
#include "XSDK/XSystemStats.h"
#include <list>

class XSystemStatsTest : public test_fixture
{
public:
    TEST_SUITE_INIT(XSystemStatsTest)
        _numberOfProcessors(0)
    TEST_SUITE_BEGIN()
        TEST(XSystemStatsTest::TestGetNumberOfProcessors);
        TEST(XSystemStatsTest::TestGetCurrentProcessCPULoad);
        TEST(XSystemStatsTest::TestGetSystemMemory);
        TEST(XSystemStatsTest::TestGetDiskUsage);
        TEST(XSystemStatsTest::TestGetProcessName);
        TEST(XSystemStatsTest::TestGetSystemInformation);
    TEST_SUITE_END()

    class LoadACPU : public XSDK::XTaskBase
    {
    public:
        LoadACPU(void) : XSDK::XTaskBase(),_isRunning(true)
        {
        }
        virtual ~LoadACPU(void) throw()
        {
        }
        virtual void * EntryPoint()
        {
            while(_isRunning);
            return 0;
        }
        void Stop(void)
        {
            _isRunning = false;
        }
    private:
        volatile bool _isRunning;
    };

    virtual ~XSystemStatsTest(void) throw()
    {
    }

    void setup();
    void teardown();

protected:
    void TestGetNumberOfProcessors(void);
    void TestGetCurrentProcessCPULoad(void);
    void TestGetSystemMemory(void);
    void TestGetDiskUsage(void);
    void TestGetProcessName(void);
    void TestGetSystemInformation(void);

private:
    int _numberOfProcessors;
};

#endif
