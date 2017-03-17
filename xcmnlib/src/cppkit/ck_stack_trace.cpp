
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
///
/// The views and conclusions contained in the software and documentation are those of the authors and
/// should not be interpreted as representing official policies, either expressed or implied, of the cppkit
/// Project.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#include "cppkit/ck_stack_trace.h"
#include "cppkit/ck_types.h"

#include <string>
#include <stdlib.h>

#ifdef IS_LINUX
  #include <cxxabi.h>
  #include <execinfo.h>
#endif

using namespace cppkit;
using namespace std;

ck_string ck_stack_trace::get_stack()
{
    ck_string stack;

#ifdef IS_LINUX
    const int size = 256;
    char** buffer = NULL;
    int traceSize = 0;
    void* trace[size];

    traceSize = backtrace(trace, size);

    buffer = backtrace_symbols(trace, traceSize);

    for( int i = 0; i < traceSize; i++ )
    {
        string stackFrame = buffer[i];
        const size_t openParen = stackFrame.find('(');
        const size_t plus = stackFrame.find('+', openParen);
        string mangledFunc = stackFrame.substr(openParen + 1, plus - openParen - 1);

        if(mangledFunc.size() > 2 &&
           (mangledFunc[0] == '_' || mangledFunc[1] == 'Z'))
        {
            int status = 0;
            char* demangled = abi::__cxa_demangle(mangledFunc.c_str(), NULL, NULL, &status);

            if( demangled )
            {
                stack += ck_string::format( "%s\n",
                                        (stackFrame.substr(0, openParen + 1) + demangled + ')').c_str() );

                free( demangled );
            }
            else stack += ck_string::format( "%s\n", mangledFunc.c_str() );
        }
        else stack += ck_string::format( "%s\n", mangledFunc.c_str() );
    }

    free(buffer);
#else
    stack = "Currently, cppkit for win32 does not support stack traces in exceptions.";
#endif

    return stack;
}

vector<ck_string> ck_stack_trace::get_stack_frame_names()
{
    vector<ck_string> stack;

#ifdef IS_LINUX
    const int size = 256;
    char** buffer = NULL;
    int traceSize = 0;
    void* trace[size];

    traceSize = backtrace(trace, size);

    buffer = backtrace_symbols(trace, traceSize);

    for( int i = 0; i < traceSize; i++ )
    {
        string stackFrame = buffer[i];
        const size_t openParen = stackFrame.find('(');
        const size_t plus = stackFrame.find('+', openParen);
        string mangledFunc = stackFrame.substr(openParen + 1, plus - openParen - 1);

        if(mangledFunc.size() > 2 &&
           (mangledFunc[0] == '_' || mangledFunc[1] == 'Z'))
        {
            int status = 0;
            char* demangled = abi::__cxa_demangle(mangledFunc.c_str(), NULL, NULL, &status);

            if( demangled )
            {
                stack.push_back( ck_string(stackFrame.substr(0, openParen + 1) + demangled + ')') );

                free( demangled );
            }
            else
            {
                stack.push_back( ck_string( mangledFunc ) );
            }
        }
        else
        {
            stack.push_back( ck_string( mangledFunc ) );
        }
    }

    free(buffer);
#else
    ck_string msg( "Currently, cppkit for win32 does not support stack traces in exceptions." );
    stack.push_back( msg );
#endif

    return stack;
}
