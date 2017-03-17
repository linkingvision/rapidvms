
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XDynamicLibrary.h"
#include <vector>
#include <stdlib.h>

using namespace XSDK;
using namespace std;

#ifdef IS_LINUX
    #include <cxxabi.h>
    #include <execinfo.h>
#elif defined(IS_WINDOWS)
    #include "DbgHelp.h"
    #include "XSDK/OS.h"
    #include "XSDK/XPath.h"
#else
    #error ">> Unknown OS!"
#endif

//=============================================================================
// XDynamicLibrary() - ctor #1
//=============================================================================
XDynamicLibrary::XDynamicLibrary() :
    _libraryInstance(NULL),
    _libraryName("")
{
}

//=============================================================================
// XDynamicLibrary() - ctor #2
//=============================================================================
XDynamicLibrary::XDynamicLibrary(const XString& libraryName) :
    _libraryInstance(NULL),
    _libraryName(libraryName)
{
}

//=============================================================================
// ~XDynamicLibrary() -   Destroys the object, unloads the library
//                          if necessary.
//=============================================================================
// virtual
XDynamicLibrary::~XDynamicLibrary() throw()
{
    if (_libraryInstance != 0)
        Unload();
}

//=============================================================================
// Load() - Used in conjunction with ctor #2 that takes a file name arg
//          to load the library.
//=============================================================================
void XDynamicLibrary::Load(void)
{
    // Defer to our load that takes an argument.
    return Load(_libraryName);
}

//=============================================================================
// Load() - Loads the given library name.
//=============================================================================
void XDynamicLibrary::Load(const XString& libraryName)
{
    if (false == libraryName.empty())
    {
        if( _libraryName != libraryName && _libraryInstance)
            Unload();

#ifdef IS_WINDOWS
        _libraryInstance = ::LoadLibraryW(libraryName.get_wide_string().data());
#elif defined(IS_LINUX)
        _libraryInstance = dlopen(libraryName.c_str(), RTLD_NOW | RTLD_GLOBAL);
#else
    #error >> "Unknown OS!"
#endif

        if(_libraryInstance == 0)
        {
#ifdef IS_WINDOWS
            LPVOID pStr = 0;
            DWORD_PTR args[1] = { (DWORD_PTR)_libraryName.c_str() };
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
                          NULL,
                          GetLastError(),
                          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                          (LPTSTR)&pStr,
                          0,
                          (va_list*)args);
            // The string already comes with a carriage return.
            XString errorString = (LPTSTR)pStr;
            LocalFree(pStr);

            X_THROW(("Unable to load library(%s); error(%s)", libraryName.c_str(), errorString.c_str() ));
#elif defined(IS_LINUX)
            XString dlError = dlerror();

            if( dlError.Contains( "undefined symbol: " ) )
            {
                vector<XString> parts;
                dlError.Split( "undefined symbol: ", parts );

                if( parts.size() == 2 )
                {
                    XString rhs = parts[1];

                    const size_t closeParen = rhs.find( ')' );

                    XString mangledName = rhs.substr( 0, (rhs.size()-closeParen) );

                    int status = 0;
                    char* demangled = abi::__cxa_demangle( mangledName.c_str(), NULL, NULL, &status );

                    XString demangledName = demangled;

                    free( demangled );

                    X_THROW(("Unable to load library(%s); error(%s); demangled undefined symbol(%s);", libraryName.c_str(), dlError.c_str(), demangledName.c_str() ));
                }
            }
            else X_THROW(("Unable to load library(%s); error(%s)", libraryName.c_str(), dlerror() ));
#else
    #error >> "Unknown OS!"
#endif
        }

        _libraryName = libraryName;
    }
    else
       X_THROW(("Library name is empty"));

}

//=============================================================================
// ResolveSymbol - Looks up the given symbol in this library, returns a pointer
//=============================================================================
FARPROC XDynamicLibrary::ResolveSymbol(const XString& symbolName)
{
    FARPROC symPtr = 0;
    if (false == symbolName.empty())
    {
#ifdef IS_WINDOWS
    symPtr = ::GetProcAddress(_libraryInstance, symbolName.c_str());
#elif defined(IS_LINUX)
    symPtr = dlsym(_libraryInstance, symbolName.c_str());
#else
    #error >> "Unknown OS!"
#endif
    }

    return symPtr;
}

//=============================================================================
// Unload() - Unloads this library, .DLL or .so
//=============================================================================
void XDynamicLibrary::Unload(void)
{
    if (_libraryInstance != 0)
    {
#ifdef IS_WINDOWS
        ::FreeLibrary(_libraryInstance);
#elif defined(IS_LINUX)
        dlclose(_libraryInstance);
#else
    #error >> "Unknown OS!"
#endif
    }

    // Always set this to NULL, we're done.
    _libraryInstance = 0;
}
