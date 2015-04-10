
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XGuard.h"

#ifdef IS_LINUX
    #include <cxxabi.h>
    #include <execinfo.h>
#elif defined(IS_WINDOWS)
    #include "DbgHelp.h"
#else
    #error ">> Unknown OS!"
#endif

#include "XSDK/XStackTrace.h"
#include "XSDK/XString.h"

#include <cstdlib>


using namespace XSDK;
using namespace std;


string XStackTrace::GetStack()
{
    vector<string> stack;
    GetStack(stack);
    return StackToString(stack);
}

void XStackTrace::GetStack(vector<string>& stack)
{
#ifdef IS_LINUX

    const int size = 256;
    void* trace[size];
    int traceSize = backtrace(trace, size);
    char** buffer = backtrace_symbols(trace, traceSize);

    for(int i = 0; i < traceSize; ++i)
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

            if(demangled)
            {
                stack.push_back(stackFrame.substr(0, openParen + 1) + demangled + ')');

                free(demangled);
            }
            else
                stack.push_back(mangledFunc);
        }
        else
            stack.push_back(mangledFunc);
    }

    free(buffer);

#elif defined(IS_WINDOWS)

    static XMutex lock;
    XGuard guard(lock);

    const int size = 256;
    const size_t maxNameLen = 255;
    void* trace[size];
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, 0, true);
    unsigned short traceSize = CaptureStackBackTrace(0, size, trace, 0);
    SYMBOL_INFO* symbolInfo = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + maxNameLen + 1, 1);
    symbolInfo->MaxNameLen = maxNameLen;
    symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);

    for(unsigned int i = 0; i < traceSize; ++i)
    {
        char* file = "Unknown file";
        int line = 0;
        char* symbol = "Unknown symbol";
        DWORD displacement;

        IMAGEHLP_LINE64 fileLineInfo;
        fileLineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
        if(SymGetLineFromAddr64(process, DWORD64(trace[i]), &displacement, &fileLineInfo))
        {
            file = fileLineInfo.FileName;
            line = (int)fileLineInfo.LineNumber;
        }

        if(SymFromAddr(process, DWORD64(trace[i]), 0, symbolInfo))
            symbol = symbolInfo->Name;

        stack.push_back(XString::Format("%s(%d): %s()", file, line, symbol));
    }

    free(symbolInfo);

    SymCleanup(process);

#else
    #error ">> Unknown OS!"
#endif
}

string XStackTrace::StackToString( const vector<string>& stack )
{
    string stackStr;

    for(vector<string>::const_iterator iter = stack.begin(),
                                       end  = stack.end();
        iter != end;
        ++iter)
    {
        stackStr.append(*iter);
        stackStr.append("\n");
    }

    return stackStr;
}
