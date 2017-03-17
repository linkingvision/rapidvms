
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XLogger.h"
#include "XSDK/OS/Sprintf.h"

#include <iostream>
#include <map>
#include <cstdio>
#include <sstream>
#include <ctime>

#include "XSDK/Types.h"
#include "XSDK/Errors.h"
#include "XSDK/XGuard.h"
#include "XSDK/XTaskBase.h"
#include "XSDK/XStackTrace.h"
#include "XSDK/XHash.h"
#include "XSDK/XTime.h"
#include "XSDK/XString.h"
#include "XSDK/XMutex.h"

#ifdef IS_LINUX
#include <signal.h>
#include <cstdlib>
#include <syslog.h>
#else
#include <Windows.h>
#endif

#define X_LOG_CHUNK_SIZE 1024

using namespace std;
using namespace XSDK;


//------------------------------------------------------------------------------
// Prototypes
//------------------------------------------------------------------------------

X_API std::string _LogLevelPrefix(XSDK::LOG_LEVEL level);
X_API void        _LogLevelHandler(int signal);


//------------------------------------------------------------------------------
// LogFlag Functions
//------------------------------------------------------------------------------

bool XLog::LogEnabled()
{
    XGuard guard(_cMutex);

    return _cLogEnabled;
}

void XLog::SetLogEnabled(bool enabled)
{
    XGuard guard(_cMutex);

    _cLogEnabled = enabled;
}


//------------------------------------------------------------------------------
// LogFlag Functions
//------------------------------------------------------------------------------

uint32_t XLog::GetLogFlags()
{
    XGuard guard(_cMutex);

    return _cCurrLogFlags;
}


void XLog::SetLogFlags(const uint32_t flags)
{
    XGuard guard(_cMutex);

    _cCurrLogFlags = flags;
}


//------------------------------------------------------------------------------
// LogLevel Functions
//------------------------------------------------------------------------------

LOG_LEVEL XLog::GetLogLevel()
{
    XGuard guard(_cMutex);

    return _cCurrLogLevel;
}


void XLog::SetLogLevel(const LOG_LEVEL level)
{
    XGuard guard(_cMutex);

    _cCurrLogLevel = level;
}

std::string XLog::LogLevelToString(const LOG_LEVEL level)
{
    switch (level)
    {
    case LOGLEVEL_DEBUG:
        return "debug";
    case LOGLEVEL_TRACE:
        return "trace";
    case LOGLEVEL_INFO:
        return "info";
    case LOGLEVEL_NOTICE:
        return "notice";
    case LOGLEVEL_WARNING:
        return "warning";
    case LOGLEVEL_ERROR:
        return "error";
    case LOGLEVEL_CRITICAL:
        return "critical";
    default:
        X_THROW(("XLog: Unknown log level (%d)", level));
    };
}

LOG_LEVEL XLog::StringToLogLevel(const std::string& str)
{
    XString level(str);
    level.ToLowerInPlace();
    if(level == "debug")
        return LOGLEVEL_DEBUG;
    else if(level == "trace")
        return LOGLEVEL_TRACE;
    else if(level == "info")
        return LOGLEVEL_INFO;
    else if(level == "notice")
        return LOGLEVEL_NOTICE;
    else if(level == "warning")
        return LOGLEVEL_WARNING;
    else if(level == "error")
        return LOGLEVEL_ERROR;
    else if(level == "critical")
        return LOGLEVEL_CRITICAL;
    else
        X_THROW(("XLog: Unknown log level (%s)", level.c_str()));
}


//------------------------------------------------------------------------------
// PrintToStdOutEnabled Functions
//------------------------------------------------------------------------------

bool XLog::PrintToStdOutEnabled()
{
    XGuard guard(_cMutex);

    return _cPrintToStdOutEnabled;
}


void XLog::SetPrintToStdOutEnabled(bool enabled)
{
    XGuard guard(_cMutex);

    _cPrintToStdOutEnabled = enabled;
}


//------------------------------------------------------------------------------
// PrintToFileEnabled Functions
//------------------------------------------------------------------------------

bool XLog::PrintToFileEnabled()
{
    return _cFile.get() != 0;
}

void XLog::DisablePrintToFile()
{
    _cFile.reset(0);
}

void XLog::EnablePrintToFile(const std::string& filename, bool append)
{
    if(append)
        _cFile.reset(new ofstream(filename.c_str(), ios_base::app));
    else
        _cFile.reset(new ofstream(filename.c_str()));
}


//------------------------------------------------------------------------------
// Other Functions
//------------------------------------------------------------------------------

void XLog::InstallLogLevelSigHandler()
{
#ifndef WIN32
    //Set up signal handler.
    if(signal(SIGUSR1, _LogLevelHandler) == SIG_ERR)
        X_LOG_ERROR("SIGUSR1 failed to initialize");

    if(signal(SIGUSR2, _LogLevelHandler) == SIG_ERR)
        X_LOG_ERROR("SIGUSR2 failed to initialize");
#endif
}


void XLog::LogBacktrace()
{
    std::vector<std::string> stack;
    XStackTrace::GetStack(stack);
    LogBacktrace(stack);
}


void XLog::LogBacktrace(const std::vector<std::string>& stack)
{
    int i = 1;
    int stackCount = (int)stack.size();

    for (std::vector<std::string>::const_iterator it = stack.begin(); it != stack.end(); ++it, ++i)
    {
#ifdef IS_LINUX
        syslog(LOGLEVEL_ERROR, "[BT](%d/%d) %s", i, stackCount, it->c_str());
#elif defined(IS_WINDOWS)
    XString fullMessage = XString::Format("[BT](%d/%d) %s", i, stackCount, it->c_str());
    OutputDebugStringW(fullMessage.get_wide_string().data());
#else
    #error ">> Unknown OS!"
#endif

        if(_cPrintToStdOutEnabled)
            printf("%s\n", it->c_str());

        if(_cFile.get())
            *_cFile << it->c_str() << endl;
    }
}


typedef struct _ThrottledLogging_t_
{
   uint64_t _throttleTimeStamp;
   unsigned int _countBetween;
}ThrottledLogging_t;

static XHash<ThrottledLogging_t> _throttledLoggingData;
static XMutex _throttledLoggingDataMutex;


bool XLog::ThrottleLogMessageCheck(const unsigned int intervalMilliSeconds, const char* key)
{
   ThrottledLogging_t* data = NULL;
   {
       XGuard guard(_throttledLoggingDataMutex);
       data = _throttledLoggingData.Find(key);
   }
   if(data == NULL)
   {//New
      ThrottledLogging_t value;
      value._countBetween = 0;
      value._throttleTimeStamp = Clock::currTick();
      {
          XGuard guard(_throttledLoggingDataMutex);
          _throttledLoggingData.Add(key,value);
      }
   }
   else
   {//Existing
      uint64_t result = (Clock::currTick() - data->_throttleTimeStamp) / hnsecsPer(MSECS);
      if(result < intervalMilliSeconds)
      {
         ++data->_countBetween;
         return false;
      }
      data->_throttleTimeStamp = Clock::currTick();
   }
   return true;
}

void XLog::ClearThrottleLogMessageCount(const char* key)
{
   ThrottledLogging_t* value = NULL;
   {
       XGuard guard(_throttledLoggingDataMutex);
       value = _throttledLoggingData.Find(key);
   }
   if(value == NULL)
      return;
   value->_countBetween = 0;
}


unsigned int XLog::GetThrottleLogMessageCount(const char* key)
{
   ThrottledLogging_t* value = NULL;
   {
       XGuard guard(_throttledLoggingDataMutex);
       value = _throttledLoggingData.Find(key);
   }
   if(value == NULL)
      return 0;
   return value->_countBetween;
}

//------------------------------------------------------------------------------
// Filter Functions
//------------------------------------------------------------------------------
typedef struct filterData_t
{
    XString _data;
    LOG_LEVEL _level;
}FilterData;

std::list<FilterData> _FilterDataSet;

bool XLog::_ShouldLog(const char* module, const LOG_LEVEL level)
{
    XGuard guard(_cMutex);
    //If logging is turned off don't log
    if(!_cLogEnabled)
        return false;
    //Check all the filters
    for (std::list<FilterData>::iterator it = _FilterDataSet.begin(); it != _FilterDataSet.end(); ++it)
    {
        //if the current msg doesn't meet the level for this feature no need to do more expensive str cmp
        if(level > it->_level)
            continue;
        XString temp = module;
        //if the filter matches the module in any way, you should log this
        if(temp.Contains(it->_data))
            return true;
    }
    //if the filters don't apply check current log level against msg log level.
    if(_cCurrLogLevel < level)
        return false;
    //If we are here log this msg
    return true;
}

void XLog::ClearFilters()
{
    XGuard guard(_cMutex);
    _FilterDataSet.clear();
}

void XLog::AddFilter(const char* filter, LOG_LEVEL level)
{
    XGuard guard(_cMutex);
    FilterData data;
    data._data = filter;
    data._level = level;
    _FilterDataSet.push_back(data);
}

//------------------------------------------------------------------------------
// Write Functions
//------------------------------------------------------------------------------

bool XLog::Write(const char* module, const LOG_LEVEL level, const char* format, ...)
{
    X_ASSERT(format != NULL);

    if(!format)
        return false;

    XGuard guard(_cMutex);

    if(!_ShouldLog(module, level)) return true;

    va_list args;
    va_start(args, format);
    const bool result = _NoCheckWrite(module, level, 0, NULL, format, args);
    va_end(args);

    return result;
}


bool XLog::Write(const char* module, const LOG_LEVEL level, const int line, const char* file, const char* format, ...)
{
    X_ASSERT(format != NULL);

    if(!format)
        return false;

    XGuard guard(_cMutex);
    if(!_ShouldLog(module, level)) return true;

    va_list args;
    va_start(args, format);
    const bool result = _NoCheckWrite(module, level, line, file, format, args);
    va_end(args);

    return result;
}


bool XLog::Write(const char* module, const LOG_LEVEL level, const int line, const char* file, const char* format, va_list& args)
{
    XGuard guard(_cMutex);
    if(!_ShouldLog(module, level)) return true;

    return _NoCheckWrite(module, level, line, file, format, args);
}


//------------------------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------------------------

std::string _LogLevelPrefix(XSDK::LOG_LEVEL level)
{
    std::string prefix;

    switch(level)
    {
        case LOGLEVEL_CRITICAL:
        {
            prefix = std::string(kLogCriticalPrefix);
            break;
        }
        case LOGLEVEL_ERROR:
        {
            prefix = std::string(kLogErrorPrefix);
            break;
        }
        case LOGLEVEL_WARNING:
        {
            prefix = std::string(kLogWarningPrefix);
            break;
        }
        case LOGLEVEL_NOTICE:
        {
            prefix = std::string(kLogNoticePrefix);
            break;
        }
        case LOGLEVEL_INFO:
        {
            prefix = std::string(kLogInfoPrefix);
            break;
        }
        case LOGLEVEL_TRACE:
        {
            prefix = std::string(kLogTracePrefix);
            break;
        }
        case LOGLEVEL_DEBUG:
        {
            prefix = std::string(kLogDebugPrefix);
            break;
        }
    }

    return prefix;
}

void _LogLevelHandler(int signal)
{
#ifndef WIN32
    switch(signal)
    {
        case SIGUSR1:
        {
            const LOG_LEVEL level = XLog::GetLogLevel();
            if(level < LOGLEVEL_DEBUG)
                XLog::SetLogLevel(LOG_LEVEL(level + 1));
            else
                XLog::SetPrintToStdOutEnabled(!XLog::PrintToStdOutEnabled());
            break;
        }
        case SIGUSR2:
        {
            const LOG_LEVEL level = XLog::GetLogLevel();
            if(level > LOGLEVEL_CRITICAL)
                XLog::SetLogLevel(LOG_LEVEL(level - 1));
            break;
        }
        default:
        break;
    };
#endif
}

bool XLog::_NoCheckWrite(const char* module, const LOG_LEVEL level, const int line, const char* file, const char* format, ...)
{
    X_ASSERT(format != NULL);

    if(!format)
        return false;

    XGuard guard(_cMutex);

    va_list args;
    va_start(args, format);
    const bool result = _NoCheckWrite(module, level, line, file, format, args);
    va_end(args);

    return result;
}

bool XLog::_NoCheckWrite(const char* module, const LOG_LEVEL level, const int line, const char* file, const char* format, va_list& args)
{
    X_ASSERT(format != NULL);

    if(!format)
        return false;

    XGuard guard(_cMutex);

    XString log_msg = XString::Format(format, args);
#ifndef WIN32
    //Note: must escape all of the % with %% so syslog can display them correctly
    log_msg.ReplaceAllInPlace("%", "%%");
#endif

    // Write the message to the log.
    bool result = false;
    if(! log_msg.empty())
    {
        stringstream ex_log_msg;

        // Create the extended information string if flags are enabled.
        if(_cCurrLogFlags)
        {
            // Prepend the time
            if(_cCurrLogFlags & kLogTimestamp)
            {
                time_t now = time(0);
                string t = ctime(&now);

                if(!t.empty())
                    t.erase(t.size()-1); //ctime appends a newline
                else
                    t = "ctime error";

                ex_log_msg << '[' << t << "] ";
            }

            // Prepend the thread Id
            if(_cCurrLogFlags & kLogThreadID)
                ex_log_msg << "Thread ID [" << XTaskBase::CurrThreadID() << "] ";

            // Prepend the file and line number.
            if((_cCurrLogFlags & kLogFileLine) && file)
                ex_log_msg << file << '(' << line << "): ";
        }

        // Append our loglevel prefix no matter what.
        ex_log_msg << _LogLevelPrefix(level);

        result = _Write(module, level, ex_log_msg.str(), log_msg);
    }

    return result;
}

bool XLog::_Write(const std::string& module, const LOG_LEVEL level, const std::string& prefix, const std::string& msg)
{
#ifdef IS_LINUX

   int priority = LOG_USER;

    switch(level)
    {
        case LOGLEVEL_CRITICAL: {priority |= LOG_CRIT;    break;}
        case LOGLEVEL_ERROR:    {priority |= LOG_ERR;     break;}
        case LOGLEVEL_WARNING:  {priority |= LOG_WARNING; break;}
        case LOGLEVEL_NOTICE:   {priority |= LOG_NOTICE;  break;}
        case LOGLEVEL_INFO:     {priority |= LOG_INFO;    break;}
        case LOGLEVEL_TRACE:    {priority |= LOG_INFO;    break;}
        //LOG_DEBUG doesn't normally show up in syslog, so we're going
        //to use the syslog level LOG_INFO for it (but it will still
        //be filtered at DEBUG level by XLogger's filtering).
        case LOGLEVEL_DEBUG:    {priority |= LOG_INFO;   break;}
        default:
        {
            X_ASSERT(false);

            return false;
        }
    };

    // No need to chunk log, just write it all
    if(msg.size() <= X_LOG_CHUNK_SIZE)
    {
        stringstream msgStream;
        msgStream << prefix << " [" << module << "] " << msg;
        syslog(priority, msgStream.str().c_str());
    }
    // Write chunks to syslog to avoid message length errors
    else
    {
        size_t msgSize = msg.size();

        // Get chunk count
        int chunkCount = (int)(msgSize / X_LOG_CHUNK_SIZE);
        if((msgSize % X_LOG_CHUNK_SIZE) != 0)
            chunkCount++;

        size_t pos = 0;
        int i = 1;
        for(; pos < msgSize; pos += X_LOG_CHUNK_SIZE, ++i)
        {
            size_t endpos = pos + X_LOG_CHUNK_SIZE;
            stringstream msgChunk;
            msgChunk << prefix << "[" << module << "]" << "(" << i << "/" << chunkCount << ") ";

            if(endpos < msgSize)
                msgChunk << msg.substr(pos, X_LOG_CHUNK_SIZE);
            // If the end position is past the end of the string, just go to the end.
            else
                msgChunk << msg.substr(pos);

            syslog(priority, msgChunk.str().c_str());
        }
    }
#elif defined(IS_WINDOWS)
    XString fullMessage = " ";
    fullMessage.append(prefix);
    fullMessage.append(" [");
    fullMessage.append(module);
    fullMessage.append("] ");
    fullMessage.append(msg);
    static const wchar_t NEW_LINE[] = L"\n";
    OutputDebugStringW(fullMessage.get_wide_string().data());
    OutputDebugStringW(NEW_LINE);
#else
    #error ">> Unknown OS!"
#endif

    if(_cPrintToStdOutEnabled)
        cout << " " << msg << endl;

    if(_cFile.get())
        *_cFile << " " << msg << endl;

    return true;
}


//------------------------------------------------------------------------------
// Static Initialization
//------------------------------------------------------------------------------

#ifndef NDEBUG
uint32_t XLog::_cCurrLogFlags = kLogFileLine | kLogThreadID | kLogTimestamp;
LOG_LEVEL XLog::_cCurrLogLevel = LOGLEVEL_DEBUG;
#else
uint32_t XLog::_cCurrLogFlags = 0;
LOG_LEVEL XLog::_cCurrLogLevel = LOGLEVEL_WARNING;
#endif

bool XLog::_cLogEnabled = true;
XMutex XLog::_cMutex;
bool XLog::_cPrintToStdOutEnabled = false;
auto_ptr<ofstream> XLog::_cFile;

XLog::StaticInit::StaticInit()
{
#ifndef WIN32
    //Set up syslog.
    openlog(0, 0, LOG_USER);

    //Set up signal handler.
    if(signal(SIGUSR1, _LogLevelHandler) == SIG_ERR)
    {
        //For some reason, if I use LOGLEVEL_ERROR directly instead of creating this
        //extraneous variable, the compiler complains about LOGLEVEL_ERROR not being
        //in this scope, but using the extraneous variable works, so it gets to be
        //there to get rid of the problem.
        const LOG_LEVEL error = LOGLEVEL_ERROR;
        syslog(error, "SIGUSR1 failed to initialize");
    }
    if(signal(SIGUSR2, _LogLevelHandler) == SIG_ERR)
    {
        const LOG_LEVEL error = LOGLEVEL_ERROR;
        syslog(error, "SIGUSR2 failed to initialize");
    }
#endif
}


const XLog::StaticInit _cStaticInit;

