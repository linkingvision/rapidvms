
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_LOGGER_H_
#define _XSDK_LOGGER_H_

#include <fstream>
#include <memory>
#include <cstdarg>
#include <vector>

//We're using std::string here instead of XSDK::XString because XSDK::XString
//includes this file, so we'd have a circular dependency if we tried to use
//XSDK::XString here.
#include <string>

#include "XSDK/Types.h"
#include "XSDK/XSharedLib.h"
#include "XSDK/XMutex.h"



/**
 * @def X_LGR_DFLT_MOD_ID
 * this macro is used to set a module for logging so that filtering can be applied.
 * to leverage this add the following two preprocessor directives to your CPP file, and then
 * change the define directive to have the module name you want.
 *
 * #undef X_LGR_DFLT_MOD_ID
 * #define X_LGR_DFLT_MOD_ID "YourModuleName"
 */
#define X_LGR_DFLT_MOD_ID "XSDK:DefaultModule"

namespace XSDK
{
    /// \brief Defines the different logging levels. The values are _not_ the
    ///        same as those of syslog as our levels differ slightly (we don't
    ///        have EMERGENCY or ALERT, and syslog doesn't have TRACE).
    enum LOG_LEVEL
    {
        LOGLEVEL_CRITICAL = 0,  ///< Should be corrected immediately, but indicates failure in a primary system.
        LOGLEVEL_ERROR    = 1,  ///< Non-urgent failures - these should be relayed to developers.
        LOGLEVEL_WARNING  = 2,  ///< Not an error, but indication that an error will occur if action is not taken, e.g. file system 85% full.
        LOGLEVEL_NOTICE   = 3,  ///< Events that are unusual but not error conditions - might be summarized in an email to developers to spot potential problems.
        LOGLEVEL_INFO     = 4,  ///< Normal operational messages - may be harvested for reporting, measuring throughput, etc.
        LOGLEVEL_TRACE    = 5,  ///< Provides Tracebility of methods being entered or exited - may be harvested for reporting, measuring throughput, etc.
        LOGLEVEL_DEBUG    = 6,  ///< Info useful to developers for debugging the application, not useful during operations.
    };

    const char kLogCriticalPrefix[] = "[CRITICAL]";
    const char kLogErrorPrefix[]    = "[ERROR]";
    const char kLogWarningPrefix[]  = "[WARNING]";
    const char kLogNoticePrefix[]   = "[NOTICE]";
    const char kLogInfoPrefix[]     = "[INFO]";
    const char kLogTracePrefix[]    = "[TRACE]";
    const char kLogDebugPrefix[]    = "[DEBUG]";

    // Flags for controlling what information is included in log messages.
    const uint32_t kLogFileLine   = 0x00000001;
    const uint32_t kLogThreadID   = 0x00000002;
    const uint32_t kLogTimestamp  = 0x00000004;


    // Simple printf-style macros for logging messages at different levels.
    // Note that they are written the way that they're written so that a log
    // command will only take up one line and X_LOG_*(blah); will count as a
    // single statement, and you're not forced to use braces with an if
    // statement in order to have an else after it.
    #define X_LOG_CRITICAL(format, ...) X_MACRO_BEGIN if(XSDK::XLog::_ShouldLog(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_CRITICAL)) XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_CRITICAL, __LINE__, __FILE__, format,  ##__VA_ARGS__); X_MACRO_END
    #define X_LOG_ERROR(format, ...)    X_MACRO_BEGIN if(XSDK::XLog::_ShouldLog(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_ERROR)) XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_ERROR, __LINE__, __FILE__, format,  ##__VA_ARGS__); X_MACRO_END
    #define X_LOG_WARNING(format, ...)  X_MACRO_BEGIN if(XSDK::XLog::_ShouldLog(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_WARNING)) XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_WARNING, __LINE__, __FILE__, format,  ##__VA_ARGS__); X_MACRO_END
    #define X_LOG_NOTICE(format, ...)   X_MACRO_BEGIN if(XSDK::XLog::_ShouldLog(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_NOTICE)) XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_NOTICE, __LINE__, __FILE__, format,  ##__VA_ARGS__); X_MACRO_END
    #define X_LOG_INFO(format, ...)     X_MACRO_BEGIN if(XSDK::XLog::_ShouldLog(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_INFO)) XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_INFO, __LINE__, __FILE__, format,  ##__VA_ARGS__); X_MACRO_END
    #define X_LOG_TRACE(format, ...)    X_MACRO_BEGIN if(XSDK::XLog::_ShouldLog(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_TRACE)) XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_TRACE, __LINE__, __FILE__, format,  ##__VA_ARGS__); X_MACRO_END
    #define X_LOG_DEBUG(format, ...)    X_MACRO_BEGIN if(XSDK::XLog::_ShouldLog(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_DEBUG)) XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_DEBUG, __LINE__, __FILE__, format,  ##__VA_ARGS__); X_MACRO_END
    #define X_LOG_ASSERT(x) X_MACRO_BEGIN if(!(x)) { XSDK::XLog::Write(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_ERROR, __LINE__, __FILE__, "[Assertion Failed]"); XSDK::XLog::LogBacktrace(); } X_MACRO_END

#define X_LOG_WARNING_THROTTLED(interval_ms, key, format, ...) X_MACRO_BEGIN if(XSDK::XLog::_ShouldLog(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_WARNING)) { if(XLog::ThrottleLogMessageCheck(interval_ms, key)) { if(XLog::GetThrottleLogMessageCount(key) > 0) XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_WARNING, __LINE__, __FILE__, "Key[%s] Following message occured [%u] times, for interval[%dms]", key, XLog::GetThrottleLogMessageCount(key), interval_ms); XLog::ClearThrottleLogMessageCount(key); XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_WARNING, __LINE__, __FILE__, format, ##__VA_ARGS__); } } X_MACRO_END
#define X_LOG_NOTICE_THROTTLED(interval_ms, key, format, ...) X_MACRO_BEGIN if(XSDK::XLog::_ShouldLog(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_NOTICE)) { if(XLog::ThrottleLogMessageCheck(interval_ms, key)) { if(XLog::GetThrottleLogMessageCount(key) > 0) XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_NOTICE, __LINE__, __FILE__, "Key[%s] Following message occured [%u] times, for interval[%dms]", key, XLog::GetThrottleLogMessageCount(key), interval_ms); XLog::ClearThrottleLogMessageCount(key); XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_NOTICE, __LINE__, __FILE__, format, ##__VA_ARGS__); } } X_MACRO_END
#define X_LOG_INFO_THROTTLED(interval_ms, key, format, ...) X_MACRO_BEGIN if(XSDK::XLog::_ShouldLog(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_INFO)) { if(XLog::ThrottleLogMessageCheck(interval_ms, key)) { if(XLog::GetThrottleLogMessageCount(key) > 0) XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_INFO, __LINE__, __FILE__, "Key[%s] Following message occured [%u] times, for interval[%dms]", key, XLog::GetThrottleLogMessageCount(key), interval_ms); XLog::ClearThrottleLogMessageCount(key); XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_INFO, __LINE__, __FILE__, format, ##__VA_ARGS__); } } X_MACRO_END
#define X_LOG_TRACE_THROTTLED(interval_ms, key, format, ...) X_MACRO_BEGIN if(XSDK::XLog::_ShouldLog(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_TRACE)) { if(XLog::ThrottleLogMessageCheck(interval_ms, key)) { if(XLog::GetThrottleLogMessageCount(key) > 0) XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_TRACE, __LINE__, __FILE__, "Key[%s] Following message occured [%u] times, for interval[%dms]", key, XLog::GetThrottleLogMessageCount(key), interval_ms); XLog::ClearThrottleLogMessageCount(key); XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_TRACE, __LINE__, __FILE__, format, ##__VA_ARGS__); } } X_MACRO_END
#define X_LOG_DEBUG_THROTTLED(interval_ms, key, format, ...) X_MACRO_BEGIN if(XSDK::XLog::_ShouldLog(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_DEBUG)) { if(XLog::ThrottleLogMessageCheck(interval_ms, key)) { if(XLog::GetThrottleLogMessageCount(key) > 0) XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_DEBUG, __LINE__, __FILE__, "Key[%s] Following message occured [%u] times, for interval[%dms]", key, XLog::GetThrottleLogMessageCount(key), interval_ms); XLog::ClearThrottleLogMessageCount(key); XSDK::XLog::_NoCheckWrite(X_LGR_DFLT_MOD_ID, XSDK::LOGLEVEL_DEBUG, __LINE__, __FILE__, format, ##__VA_ARGS__); } } X_MACRO_END

class XLog
{
public:
    X_API static bool LogEnabled();
    X_API static void SetLogEnabled(bool enabled);

    X_API static uint32_t GetLogFlags();
    X_API static void SetLogFlags(const uint32_t flags);

    X_API static LOG_LEVEL GetLogLevel();
    X_API static void SetLogLevel(const LOG_LEVEL level);

    X_API static std::string LogLevelToString(const LOG_LEVEL level);
    X_API static LOG_LEVEL StringToLogLevel(const std::string& str);

    X_API static bool PrintToStdOutEnabled();
    X_API static void SetPrintToStdOutEnabled(bool enabled);

    X_API static bool PrintToFileEnabled();
    X_API static void DisablePrintToFile();
    X_API static void EnablePrintToFile(const std::string& filename, bool append);

    //Installs signal handlers for SIGUSR1 and SIGUSR2 to increase
    //and decrease the log level respectively.
    X_API static void InstallLogLevelSigHandler();

    X_API static void LogBacktrace();
    X_API static void LogBacktrace(const std::vector<std::string>& stackTrace);

    X_API static bool Write(const char* module, const LOG_LEVEL level, const char* format, ...);
    // NOTE: 'line' is defined before 'file' because there is a potential ambiguity if the above
    //       definition of Write() is called as Write(level,char*,int,char*,etc)... is the first
    //       char* the file name or the format string???
    X_API static bool Write(const char* module, const LOG_LEVEL level, const int line, const char* file, const char* format, ...);
    X_API static bool Write(const char* module, const LOG_LEVEL level, const int line, const char* file, const char* format, va_list& args);

    X_API static bool ThrottleLogMessageCheck(const unsigned int delay, const char* key);
    X_API static unsigned int GetThrottleLogMessageCount(const char* key);
    X_API static void ClearThrottleLogMessageCount(const char* key);

    X_API static void ClearFilters();
    X_API static void AddFilter(const char* filter, LOG_LEVEL level=LOGLEVEL_DEBUG);

    // Really intended to be private, but the macros need them.
    // So, please don't call them in code outside of XLogger.
    X_API static bool _ShouldLog(const char* module, const LOG_LEVEL level);
    X_API static bool _NoCheckWrite(const char* module, const LOG_LEVEL level, const int line, const char* file, const char* format, ...);
    X_API static bool _NoCheckWrite(const char* module, const LOG_LEVEL level, const int line, const char* file, const char* format, va_list& args);

private:
    X_API static bool _Write(const std::string& module, const LOG_LEVEL level, const std::string& prefix, const std::string& msg);

    static uint32_t _cCurrLogFlags;
    static LOG_LEVEL _cCurrLogLevel;

    static bool  _cLogEnabled;
    static XMutex _cMutex;
    static bool  _cPrintToStdOutEnabled;

    static std::auto_ptr<std::ofstream> _cFile;

public:
    class StaticInit
    {
    public:
        StaticInit();
    };

    friend class StaticInit;

    const static StaticInit _cStaticInit;
};

}

#endif

