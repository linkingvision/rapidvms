
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// cppkit - http://www.cppkit.org
/// Copyright (c) 2013, Tony Di Croce
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted
/// provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
///    the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
///    and the following disclaimer in the documentation and/or other materials provided with the
///    distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
/// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
/// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#include "cppkit/os/ck_sprintf.h"

using namespace std;
using namespace cppkit;

#ifndef IS_WINDOWS

static void fix_format(ck_string* format)
{
    if(format->find("I32") == string::npos && format->find("I64") == string::npos)
        return;

    ck_string retval;

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

        ck_string flag = format->substr(found, endChar - found + 1);
        retval.append(format->substr(start, found - start));
        start = endChar + 1;

#ifdef IS_64BIT
        flag = flag.replace_all("I64", "l");
#elif defined(IS_32BIT)
        flag = flag.replace_all("I64", "ll");
#else
#error ">> Unknown Architecture!"
#endif
        flag = flag.erase_all("I32");
        retval.append(flag);
        found = format->find('%', start);
    }

    retval.append(format->substr(start));
    *format = retval;
}

int cppkit::ck_printf(const char* format...)
{
    ck_string format_str = format;
    fix_format(&format_str);

    va_list args;
    va_start(args, format);
    const int retval = vprintf(format_str.c_str(), args);
    va_end(args);

    return retval;
}

int cppkit::ck_vsnprintf(char* str, size_t size, ck_string format, va_list args)
{
    fix_format(&format);

    return vsnprintf(str, size, format.c_str(), args);
}

int cppkit::ck_sscanf(const char* str, const char* format...)
{
    ck_string format_str = format;
    fix_format(&format_str);
    char* mutable_format = new char[format_str.size() + 1];
    memcpy(mutable_format, format_str.c_str(), format_str.size() + 1);

    va_list args;
    va_start(args, format);
    const int retval = vsscanf(str, mutable_format, args);
    va_end(args);

    delete[] mutable_format;

    return retval;
}

#endif