
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/OS/StackTrace.h"
#include "XSDK/XLogger.h"

namespace XSDK
{

#ifndef WIN32

void x_sigsegv_handler( int sigNum )
{
    XLog::LogBacktrace();
    abort();
}

void x_install_segv_handler()
{
    signal( SIGSEGV, x_sigsegv_handler );
}

#endif

}
