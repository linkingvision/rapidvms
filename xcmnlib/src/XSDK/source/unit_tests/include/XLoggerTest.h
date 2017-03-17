#ifndef _LOGGERTEST_H_
#define _LOGGERTEST_H_

#include "framework.h"

#include "XSDK/XLogger.h"

class XLoggerTest : public test_fixture
{
public:
    TEST_SUITE(XLoggerTest);
        TEST(XLoggerTest::TestLogger);
        TEST(XLoggerTest::TestDefaults);
        TEST(XLoggerTest::TestLogLevelToString);
        TEST(XLoggerTest::TestBacktrace);
        TEST(XLoggerTest::TestPercentString);
        TEST(XLoggerTest::TestThrottledLogging);
    TEST_SUITE_END();

    virtual ~XLoggerTest() throw()
    {}

    void setup();
    void teardown();

protected:
    void TestThrottledLogging();
    void TestLogger();
    void TestDefaults();
    void TestLogLevelToString();
    void TestBacktrace();
    void TestPercentString();
};

#endif
