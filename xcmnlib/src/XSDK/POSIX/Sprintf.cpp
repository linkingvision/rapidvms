
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/OS/Sprintf.h"

using namespace std;
using namespace XSDK;

static void fixFormat(XString* format)
{
    if(format->find("I32") == string::npos && format->find("I64") == string::npos)
        return;

    XString retval;

    size_t start = 0;
    size_t found = format->find('%');

    while(found != string::npos)
    {
        if(found + 1 < format->size() && (*format)[found + 1] == '%')
        {
            found = format->find('%', found + 2);
            continue;
        }

        size_t endChar = format->find_first_of("duxXio", found);

        if(endChar == string::npos)
        {
            found = format->find('%', found + 1);
            continue;
        }

        XString flag = format->substr(found, endChar - found + 1);
        retval.append(format->substr(start, found - start));
        start = endChar + 1;

#ifdef IS_64BIT
        flag.ReplaceAllInPlace("I64", "l");
#elif defined(IS_32BIT)
        flag.ReplaceAllInPlace("I64", "ll");
#else
#error ">> Unknown Architecture!"
#endif

        flag.EraseAllInPlace("I32");
        retval.append(flag);
        found = format->find('%', start);
    }

    retval.append(format->substr(start));
    *format = retval;
}


int XSDK::x_printf(const char* format...)
{
    XString formatStr = format;
    fixFormat(&formatStr);

    va_list args;
    va_start(args, format);
    const int retval = vprintf(formatStr.c_str(), args);
    va_end(args);

    return retval;
}


int XSDK::x_vsnprintf(char* str, size_t size, XString format, va_list args)
{
    fixFormat(&format);

    return vsnprintf(str, size, format.c_str(), args);
}


int XSDK::x_sscanf(const char* str, const char* format...)
{
    XString formatStr = format;
    fixFormat(&formatStr);
    char* mutableFormat = new char[formatStr.size() + 1];
    memcpy(mutableFormat, formatStr.c_str(), formatStr.size() + 1);

    va_list args;
    va_start(args, format);
    const int retval = vsscanf(str, mutableFormat, args);
    va_end(args);

    delete[] mutableFormat;

    return retval;
}
