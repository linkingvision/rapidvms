
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_OS_StackTrace_h
#define XSDK_OS_StackTrace_h

#include <cstdio>

#ifndef WIN32

  #include <signal.h>
  #include <execinfo.h>
  #include <cxxabi.h>
  #include <cstdlib>

#endif

namespace XSDK
{

#ifndef WIN32

void x_sigsegv_handler( int sigNum );

void x_install_segv_handler();

#endif

}

#endif

