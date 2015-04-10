
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_XException_h
#define _XSDK_XException_h

#include "XSDK/XSharedLib.h"

#include <string>
#include <exception>

#include "XSDK/XLogger.h"
#include "XSDK/XStackTrace.h"

namespace XSDK
{

/// XSDK base exception type.
class XException : public std::exception
{
public:
    X_API XException();

    X_API XException(const std::string& msg);

    X_API XException(const char* msg, ...);

    X_API virtual ~XException() throw();

    X_API virtual const char* what() const throw();

    /// \brief This is used by the throwing macros so that derived exceptions
    ///        can have their names used in their messages rather than XException.
    X_API void SetTypeName(const std::string& typeName);
    X_API const char* GetTypeName() const;

    //Note: For some unknown reason we cannot define these methods in the header
    //or else we get linker errors in Visual Studio.
    X_API const char* GetMsg() const;
    X_API void SetMsg(std::string msg);

    X_API void SetThrowPoint(int line, const char* file);
    X_API int GetLineNum() const { return _lineNum; }
    X_API const char* GetSrcFile() const { return _srcFile.c_str(); }

    X_API const std::vector<std::string>& GetStack() const;
    X_API void SetStack(const std::vector<std::string>& stackTrace);

private:
    std::string _typeName;
    std::string _msg;

    int _lineNum;
    std::string _srcFile;
    std::vector<std::string> _stackTrace;

    mutable std::string _whatMsg;
};

class FileNotFoundException : public XSDK::XException
{
public:
    X_API FileNotFoundException();

    X_API virtual ~FileNotFoundException() throw();

    X_API FileNotFoundException(const char* msg, ...);

    X_API FileNotFoundException(const std::string& msg);
};


}

/// Throws an XExepction and logs (at level WARNING) before throwing it.
#define X_THROW(PARAMS) \
X_MACRO_BEGIN \
    XSDK::XException _exception PARAMS ; \
    _exception.SetThrowPoint(__LINE__, __FILE__); \
    std::vector<std::string> stack; \
    XSDK::XStackTrace::GetStack(stack); \
    _exception.SetStack(stack); \
    X_LOG_WARNING("Exception thrown. Msg: \"%s\", At: %s(%d)\n", \
            _exception.GetMsg(), \
            __FILE__, \
            __LINE__); \
    XSDK::XLog::LogBacktrace(stack); \
    throw _exception; \
X_MACRO_END

/// Throws an XException without logging
#define X_THROW_NO_LOG(PARAMS)                                          \
X_MACRO_BEGIN                                                           \
    XSDK::XException _exception PARAMS ;                             \
    _exception.SetThrowPoint(__LINE__, __FILE__);                    \
    std::vector<std::string> stack;                                   \
    XSDK::XStackTrace::GetStack(stack);                                 \
    _exception.SetStack(stack);                                      \
    throw _exception;                                                \
X_MACRO_END

/// Throws an XExepction and logs (at level CRITICAL) before throwing it.
#define X_THROW_CRITICAL(PARAMS) \
X_MACRO_BEGIN \
    XSDK::XException _exception PARAMS ; \
    _exception.SetThrowPoint(__LINE__, __FILE__); \
    std::vector<std::string> stack; \
    XSDK::XStackTrace::GetStack(stack); \
    _exception.SetStack(stack); \
    X_LOG_CRITICAL("Exception thrown. Msg: \"%s\", At: %s(%d)\n", \
            _exception.GetMsg(), \
            __FILE__, \
            __LINE__); \
    XSDK::XLog::LogBacktrace(stack); \
    throw _exception; \
X_MACRO_END

/// Puts double quotes around its argument.
/// ENQUOTE_EXPAND is required if you want to pass a macro to ENQUOTE.
#define ENQUOTE_EXPAND(tok) #tok
#define ENQUOTE(tok) ENQUOTE_EXPAND(tok)

/// Throws the given exception type and logs (at level WARNING) before throwing it.
#define X_STHROW(ETYPE, PARAMS) \
X_MACRO_BEGIN \
    ETYPE _exception PARAMS ; \
    _exception.SetThrowPoint(__LINE__, __FILE__); \
    _exception.SetTypeName(ENQUOTE(ETYPE));           \
    std::vector<std::string> stack; \
    XSDK::XStackTrace::GetStack(stack); \
    _exception.SetStack(stack); \
    X_LOG_WARNING("%s thrown. Msg: \"%s\", At: %s(%d)\n", \
            _exception.GetTypeName(), \
            _exception.GetMsg(), \
            __FILE__, \
            __LINE__);\
    XSDK::XLog::LogBacktrace(stack); \
    throw _exception; \
X_MACRO_END

/// Throw the given exception type without logging
#define X_STHROW_NO_LOG(ETYPE, PARAMS)             \
X_MACRO_BEGIN                                     \
    ETYPE _exception PARAMS ;              \
    _exception.SetThrowPoint(__LINE__, __FILE__); \
    _exception.SetTypeName(ENQUOTE(ETYPE));           \
    std::vector<std::string> stack; \
    XSDK::XStackTrace::GetStack(stack); \
    _exception.SetStack(stack); \
    throw _exception;                             \
X_MACRO_END

/// A version of assert that logs when the assertion fails.
#ifndef NDEBUG
#define X_ASSERT(x) \
X_MACRO_BEGIN \
    const bool rx = x; \
    X_LOG_ASSERT(rx); \
    assert(rx); \
X_MACRO_END
#else
#define X_ASSERT(x)
#endif

#endif
