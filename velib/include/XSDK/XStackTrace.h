
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XStackTrace_h
#define XSDK_XStackTrace_h

//Using std::string instead of XSDK::XString because of header circular dependencies.
#include <string>
#include <vector>

namespace XSDK
{

class XStackTrace
{
public:

    X_API static std::string GetStack();
    X_API static void GetStack( std::vector<std::string>& stack );
    X_API static std::string StackToString( const std::vector<std::string>& stack );

private:
    XStackTrace();
    XStackTrace( const XStackTrace& obj );
    XStackTrace& operator = ( const XStackTrace& obj );
};

}

#endif

