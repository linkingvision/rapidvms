
#include "XLoggerTest.h"

#include "XSDK/Errors.h"
#include "XSDK/XString.h"
#include "XSDK/XTaskBase.h"
#include "XSDK/TimeUtils.h"

using namespace std;
using namespace XSDK;

REGISTER_TEST_FIXTURE(XLoggerTest);


void LogWithVarArgs(const LOG_LEVEL level, const int line, const char* file, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    XLog::Write("",level, line, file, format, args);
    va_end(args);
}


class TestTask : public XTaskBase
{
public:

    void* EntryPoint()
    {
        //Test with macros.
        X_LOG_CRITICAL("It's critical.");
        X_LOG_ERROR("It's an error.");
        X_LOG_WARNING("It's a warning.");
        X_LOG_NOTICE("It's a notice.");
        X_LOG_INFO("It's informational.");
        X_LOG_TRACE("It's trace info.");
        X_LOG_DEBUG("It's debug info.");

        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown",
                                     X_THROW(("Something bad happened.")),
                                     XSDK::XException);
        UT_ASSERT_THROWS_MESSAGE("expected exception not thrown",
                                     X_THROW_CRITICAL(("Something _really_ bad happened.")),
                                     XSDK::XException);

        //Test without macros.
        XLog::Write("",LOGLEVEL_ERROR, "message");
        XLog::Write("",LOGLEVEL_ERROR, "message %s", "here");
        XLog::Write("",LOGLEVEL_ERROR, __LINE__, __FILE__, "message");

        const XString strThatCompilerDoesntWantMeToUseDirectlyWithoutComplaining = "here";

        XLog::Write("",LOGLEVEL_ERROR, __LINE__, __FILE__, "message %s", strThatCompilerDoesntWantMeToUseDirectlyWithoutComplaining.c_str());
        LogWithVarArgs(LOGLEVEL_ERROR, __LINE__, __FILE__, "message %s", "here");

        return NULL;
    }
};


void XLoggerTest::setup()
{
}

void XLoggerTest::TestThrottledLogging()
{
   long test = XLog::GetThrottleLogMessageCount("FOO");
   UT_ASSERT ( test == 0 );
   for ( int i = 0; i < 11; ++i )
   {
      X_LOG_INFO_THROTTLED(10000,"FOO","FOO MSG(%s:%d)","Fubar!",i);
      test = XLog::GetThrottleLogMessageCount("FOO");
      UT_ASSERT ( test == i%10 );
      x_sleep(1);
   }
   for ( int i = 0; i < 11; ++i )
   {
      X_LOG_INFO_THROTTLED(0,"BAR","BAR MSG(%s:%d)","Fubar!",i);
      test = XLog::GetThrottleLogMessageCount("BAR");
      UT_ASSERT ( test == 0 );
   }
   X_LOG_INFO_THROTTLED(0,"FOO","FOO MSG(%s)","Fubar2!");
   for ( int i = 0; i < 11; ++i )
   {
      X_LOG_INFO_THROTTLED(10000,"FOO","FOO MSG(%s:%d)","Fubar2!",i);
      test = XLog::GetThrottleLogMessageCount("FOO");
      if ( i < 10 )
         UT_ASSERT ( test == i+1 );
      else UT_ASSERT ( test == 0 );
      x_sleep(1);
   }
   for ( int i = 0; i < 100; ++i )
   {
      X_LOG_INFO_THROTTLED(1000,"BAR","BAR MSG(%s:%d)","Fubar!",i);
      test = XLog::GetThrottleLogMessageCount("BAR");
      if ( i == 0 )
         UT_ASSERT ( test == i );
   }
}


void XLoggerTest::teardown()
{
}


void XLoggerTest::TestLogger()
{
    //Two try blocks to ensure that Join() is always called and doesn't throw
    //outside of the function (not that it should ever be throwing, but let's
    //play it safe).
    try
    {
        TestTask testTask;

        try
        {
            //Test with macros.
            X_LOG_CRITICAL("It's critical.");
            X_LOG_ERROR("It's an error.");
            X_LOG_WARNING("It's a warning.");
            X_LOG_NOTICE("It's a notice.");
            X_LOG_INFO("It's informational.");
            X_LOG_TRACE("It's trace info.");
            X_LOG_DEBUG("It's debug info.");

            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown",
                                         X_THROW(("Something bad happened.")),
                                         XSDK::XException);
            UT_ASSERT_THROWS_MESSAGE("expected exception not thrown",
                                         X_THROW_CRITICAL(("Something _really_ bad happened.")),
                                         XSDK::XException);

            //Test without macros.
            XLog::Write("",LOGLEVEL_ERROR, "message");
            XLog::Write("",LOGLEVEL_ERROR, "message %s", "here");
            XLog::Write("",LOGLEVEL_ERROR, __LINE__, __FILE__, "message");

            const XString strThatCompilerDoesntWantMeToUseDirectlyWithoutComplaining = "here";

            XLog::Write("",LOGLEVEL_ERROR, __LINE__, __FILE__, "message %s", strThatCompilerDoesntWantMeToUseDirectlyWithoutComplaining.c_str());
            LogWithVarArgs(LOGLEVEL_ERROR, __LINE__, __FILE__, "message %s", "here");

            //Test the Logging when running within task/thread.
            testTask.Start();

            x_sleep(1);
        }
        catch(XSDK::XException&)
        {
            UT_ASSERT(false && "Test threw!");
        }

        testTask.Join();
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}


void XLoggerTest::TestDefaults()
{
    try
    {
        UT_ASSERT(XLog::LogEnabled());

#ifndef NDEBUG
        UT_ASSERT(XLog::GetLogFlags() & kLogFileLine);
        UT_ASSERT(XLog::GetLogFlags() & kLogThreadID);
        UT_ASSERT(XLog::GetLogFlags() & kLogTimestamp);
        UT_ASSERT(XLog::GetLogLevel() == LOGLEVEL_DEBUG);
#else
        UT_ASSERT(!(XLog::GetLogFlags() & kLogFileLine));
        UT_ASSERT(!(XLog::GetLogFlags() & kLogThreadID));
        UT_ASSERT(!(XLog::GetLogFlags() & kLogTimestamp));
        UT_ASSERT(XLog::GetLogLevel() == LOGLEVEL_WARNING);
#endif

        UT_ASSERT(!XLog::PrintToStdOutEnabled());
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XLoggerTest::TestLogLevelToString()
{
    {
        UT_ASSERT( XLog::LogLevelToString(LOGLEVEL_TRACE)    == "trace" );
        UT_ASSERT( XLog::LogLevelToString(LOGLEVEL_DEBUG)    == "debug" );
        UT_ASSERT( XLog::LogLevelToString(LOGLEVEL_INFO)     == "info" );
        UT_ASSERT( XLog::LogLevelToString(LOGLEVEL_NOTICE)   == "notice" );
        UT_ASSERT( XLog::LogLevelToString(LOGLEVEL_WARNING)  == "warning" );
        UT_ASSERT( XLog::LogLevelToString(LOGLEVEL_ERROR)    == "error" );
        UT_ASSERT( XLog::LogLevelToString(LOGLEVEL_CRITICAL) == "critical" );
        UT_ASSERT_THROWS( XLog::LogLevelToString((LOG_LEVEL)9999), XException );
    }
    {
        UT_ASSERT( XLog::StringToLogLevel("trace") == LOGLEVEL_TRACE );
        UT_ASSERT( XLog::StringToLogLevel("debug") == LOGLEVEL_DEBUG );
        UT_ASSERT( XLog::StringToLogLevel("info") == LOGLEVEL_INFO );
        UT_ASSERT( XLog::StringToLogLevel("notice") == LOGLEVEL_NOTICE );
        UT_ASSERT( XLog::StringToLogLevel("warning") == LOGLEVEL_WARNING );
        UT_ASSERT( XLog::StringToLogLevel("error") == LOGLEVEL_ERROR );
        UT_ASSERT( XLog::StringToLogLevel("critical") == LOGLEVEL_CRITICAL );
        UT_ASSERT_THROWS( XLog::StringToLogLevel("ah snap!"), XException );
    }
}

void XLoggerTest::TestBacktrace()
{
    try
    {
        XLog::LogBacktrace();
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XLoggerTest::TestPercentString()
{
    UT_ASSERT_NO_THROW(X_LOG_INFO("This has some problems %s", "This is my %d argument"));
}
