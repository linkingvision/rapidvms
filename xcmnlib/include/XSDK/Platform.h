
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifdef WIN32
#pragma warning(disable:4127)
#endif

#ifndef XSDK_Platform_h
#define XSDK_Platform_h

// [tdistler] Certain files rely on this being defined here.
#ifdef WIN32
  #define _WINSOCKAPI_
  #include <windows.h>

  #include <malloc.h> // Needed for aligned memory
#else
  #include <stdlib.h> // Needed for aligned memory
#endif


// We have to use a for-loop on Windows to avoid compile warning 4127.
// We tried disabling this with a pragma, but it caused weird compile
// errors in some cases.
#define X_MACRO_BEGIN do {
#define X_MACRO_END   }while(0)

#define X_MACRO_END_LOOP_FOREVER   }while(1)

#endif//XSDK_Platform_h
