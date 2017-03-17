
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/OS.h"
#include <errno.h>

namespace XSDK
{

    XString GetLastErrorMsg()
    {
#ifdef IS_WINDOWS
        return GetErrorMsg(GetLastError());
#else
        return GetErrorMsg(errno);
#endif
    }


    XString GetErrorMsg(int error)
    {
#ifdef IS_WINDOWS
        LPVOID pStr = 0;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
                      NULL,
                      error,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      (LPTSTR)&pStr,
                      0,
                      0);
        XString str((LPTSTR)pStr);
        LocalFree(pStr);
        return str;
#else
        return XString(strerror(error));
#endif
    }

}
