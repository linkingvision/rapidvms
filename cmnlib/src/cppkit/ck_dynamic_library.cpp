
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// cppkit - http://www.cppkit.org
/// Copyright (c) 2014, Tony Di Croce
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

#include "cppkit/ck_dynamic_library.h"
#include "cppkit/os/ck_error_msg.h"
#include <vector>
#include <stdlib.h>

using namespace cppkit;
using namespace std;

#ifdef IS_WINDOWS
    #include "Windows.h"
    #include "DbgHelp.h"
#else
    #include <cxxabi.h>
    #include <execinfo.h>
#endif

ck_dynamic_library::ck_dynamic_library() :
    _libraryInstance( NULL ),
    _libraryName( "" )
{
}

ck_dynamic_library::ck_dynamic_library(const ck_string& libraryName) :
    _libraryInstance( NULL ),
    _libraryName( libraryName )
{
}

ck_dynamic_library::~ck_dynamic_library() throw()
{
    if( _libraryInstance != 0 )
        unload();
}

void ck_dynamic_library::load()
{
    return load( _libraryName );
}

void ck_dynamic_library::load( const ck_string& libraryName )
{
    if( false == libraryName.empty() )
    {
        if( (_libraryName != libraryName) && _libraryInstance )
            unload();

#ifdef IS_WINDOWS
        _libraryInstance = ::LoadLibraryW( libraryName.get_wide_string().data() );
#else
        _libraryInstance = dlopen( libraryName.c_str(), RTLD_NOW | RTLD_GLOBAL );
#endif

        if( _libraryInstance == 0 )
        {
#ifdef IS_WINDOWS
            LPVOID str = 0;
            DWORD_PTR args[1] = { (DWORD_PTR)_libraryName.c_str() };

            FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
                           NULL,
                           GetLastError(),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           (LPTSTR)&str,
                           0,
                           (va_list*)args );

            // The string already comes with a carriage return.
            ck_string errorString = (LPTSTR)str;
            LocalFree( str );

            CK_THROW(("Unable to load library(%s); error(%s)",
                      libraryName.c_str(), errorString.c_str() ));
#else
            ck_string dlError = dlerror();

            if( dlError.contains( "undefined symbol: " ) )
            {
                vector<ck_string> parts = dlError.split( "undefined symbol: " );

                if( parts.size() == 2 )
                {
                    ck_string rhs = parts[1];

                    const size_t closeParen = rhs.find( ')' );

                    ck_string mangledName = rhs.substr( 0, (rhs.size()-closeParen) );

                    int status = 0;
                    char* demangled = abi::__cxa_demangle( mangledName.c_str(), NULL, NULL, &status );

                    ck_string demangledName = demangled;

                    free( demangled );

                    CK_THROW(("Unable to load library(%s); error(%s); demangled undefined symbol(%s);",
                              libraryName.c_str(), dlError.c_str(), demangledName.c_str() ));
                }
            }
            else CK_THROW(("Unable to load library(%s); error(%s)", libraryName.c_str(), dlerror() ));
#endif
        }

        _libraryName = libraryName;
    }
    else CK_THROW(("Library name is empty"));
}

void* ck_dynamic_library::resolve_symbol( const ck_string& symbolName )
{
    void* symPtr = 0;

    if( false == symbolName.empty() )
    {
#ifdef IS_WINDOWS
        symPtr = ::GetProcAddress( (HMODULE)_libraryInstance, symbolName.c_str() );
#else
        symPtr = dlsym( _libraryInstance, symbolName.c_str() );
#endif
    }

    return symPtr;
}

void ck_dynamic_library::unload()
{
    if( _libraryInstance != 0 )
    {
#ifdef IS_WINDOWS
        ::FreeLibrary( (HMODULE)_libraryInstance );
#else
        dlclose( _libraryInstance );
#endif

        _libraryInstance = 0;
    }
}
