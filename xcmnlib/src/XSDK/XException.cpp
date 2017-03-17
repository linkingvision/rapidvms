
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XException.h"
#include "XSDK/XString.h"
#include "XSDK/XStackTrace.h"

using namespace std;
using namespace XSDK;

XException::XException()
    : exception(),
      _typeName("XSDK::XException"),
      _msg(),
      _lineNum(0),
      _srcFile(),
      _stackTrace(),
      _whatMsg()
{
}


XException::XException(const string& msg)
    : exception(),
      _typeName("XSDK::XException"),
      _msg(msg),
      _lineNum(0),
      _srcFile(),
      _stackTrace(),
      _whatMsg()
{
}


XException::XException(const char* msg, ...)
    : exception(),
      _typeName("XSDK::XException"),
      _msg(),
      _lineNum(0),
      _srcFile(),
      _stackTrace(),
      _whatMsg()
{
    va_list args;
    va_start(args, msg);
    _msg = XString::Format(msg, args);
    va_end(args);
}


XException::~XException() throw()
{}

void XException::SetTypeName(const std::string& typeName)
{
    _typeName = typeName;
}


const char* XException::GetTypeName() const
{
    return _typeName.c_str();
}


const char* XException::GetMsg() const
{
    return _msg.c_str();
}


void XException::SetMsg(std::string msg)
{
    _msg = msg;
}


void XException::SetThrowPoint(int line, const char* file)
{
    _lineNum = line;
    _srcFile = file;
}


const std::vector<std::string>& XException::GetStack() const
{
    return _stackTrace;
}


void XException::SetStack(const std::vector<std::string>& stackTrace)
{
    _stackTrace = stackTrace;
}


const char* XException::what() const throw()
{
    _whatMsg = XString::Format("%s thrown from [%s:%d]: \"%s\"",
                                GetTypeName(),
                                GetSrcFile(),
                                GetLineNum(),
                                GetMsg());

    return _whatMsg.c_str();
}


FileNotFoundException::FileNotFoundException()
    : XException()
{}

FileNotFoundException::~FileNotFoundException() throw()
{}

FileNotFoundException::FileNotFoundException(const char* msg, ...) : XException()
{
    va_list args;
    va_start(args, msg);
    SetMsg(XString::Format(msg, args));
    va_end(args);
}

FileNotFoundException::FileNotFoundException(const string& msg)
    : XException(msg)
{}
