
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _Webby_WebbyException_h
#define _Webby_WebbyException_h

#include "XSDK/XException.h"
#include "XSDK/XString.h"

namespace WEBBY
{
    /// General exception type for Webby.
    class WebbyException : public XSDK::XException
    {
    public:
        X_API WebbyException();

        X_API virtual ~WebbyException() throw() {}

        X_API WebbyException(const char* msg, ...);

        X_API WebbyException(const XSDK::XString& msg);
    };


    class HTTPException : public WebbyException
    {
    public:
        X_API HTTPException(int statusCode);

        X_API virtual ~HTTPException() throw() {}

        X_API HTTPException(int statusCode, const char* msg, ...);

        X_API HTTPException(int statusCode, const XSDK::XString& msg);

        int GetStatus() const { return _statusCode; }

    private:

        int _statusCode;
    };


    class HTTP400Exception : public HTTPException
    {
    public:
        X_API HTTP400Exception();

        X_API virtual ~HTTP400Exception() throw() {}

        X_API HTTP400Exception(const char* msg, ...);

        X_API HTTP400Exception(const XSDK::XString& msg);
    };


    class HTTP401Exception : public HTTPException
    {
    public:
        X_API HTTP401Exception();

        X_API virtual ~HTTP401Exception() throw() {}

        X_API HTTP401Exception(const char* msg, ...);

        X_API HTTP401Exception(const XSDK::XString& msg);
    };


    class HTTP403Exception : public HTTPException
    {
    public:
        X_API HTTP403Exception();

        X_API virtual ~HTTP403Exception() throw() {}

        X_API HTTP403Exception(const char* msg, ...);

        X_API HTTP403Exception(const XSDK::XString& msg);
    };


    class HTTP404Exception : public HTTPException
    {
    public:
        X_API HTTP404Exception();

        X_API virtual ~HTTP404Exception() throw() {}

        X_API HTTP404Exception(const char* msg, ...);

        X_API HTTP404Exception(const XSDK::XString& msg);
    };


    class HTTP415Exception : public HTTPException
    {
    public:
        X_API HTTP415Exception();

        X_API virtual ~HTTP415Exception() throw() {}

        X_API HTTP415Exception(const char* msg, ...);

        X_API HTTP415Exception(const XSDK::XString& msg);
    };


    class HTTP453Exception : public HTTPException
    {
    public:
        X_API HTTP453Exception();

        X_API virtual ~HTTP453Exception() throw() {}

        X_API HTTP453Exception(const char* msg, ...);

        X_API HTTP453Exception(const XSDK::XString& msg);
    };


    class HTTP500Exception : public HTTPException
    {
    public:
        X_API HTTP500Exception();

        X_API virtual ~HTTP500Exception() throw() {}

        X_API HTTP500Exception(const char* msg, ...);

        X_API HTTP500Exception(const XSDK::XString& msg);
    };

    class HTTP501Exception : public HTTPException
    {
    public:
        X_API HTTP501Exception();

        X_API virtual ~HTTP501Exception() throw() {}

        X_API HTTP501Exception(const char* msg, ...);

        X_API HTTP501Exception(const XSDK::XString& msg);
    };


    /// \briefe Throws an exception corresponding to the given status code
    ///         or a plain HTTPException if there isn't one.
    X_API void ThrowHTTPException(int statusCode, const char* msg, ...);
    X_API void ThrowHTTPException(int statusCode, const XSDK::XString& msg);

#define HTTP_THROW(PARAMS) \
X_MACRO_BEGIN \
    try \
    { \
        ThrowHTTPException PARAMS ; \
    } \
    catch(HTTPException& e) \
    { \
        e.SetThrowPoint(__LINE__, __FILE__); \
        std::vector<std::string> stack; \
        XSDK::XStackTrace::GetStack(stack); \
        e.SetStack(stack); \
        X_LOG_WARNING("%s thrown. Msg: \"%s\", At: %s(%d)\n", \
                      e.GetTypeName(), \
                      e.GetMsg(), \
                      __FILE__, \
                      __LINE__);\
        XSDK::XLog::LogBacktrace(stack); \
        throw; \
    } \
X_MACRO_END

}

#endif
