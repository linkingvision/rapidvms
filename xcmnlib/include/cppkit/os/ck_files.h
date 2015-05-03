
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

#ifndef cppkit_files_h
#define cppkit_files_h

#include "cppkit/ck_string.h"

#ifdef IS_LINUX
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifdef IS_WINDOWS
#include <direct.h>
#endif

namespace cppkit
{

#ifdef IS_WINDOWS
const static char WRITE_MODE[] = "wb";
const static char READ_MODE[] = "rb";
#endif

#ifdef IS_LINUX
const static char READ_MODE[] = "rb, ccs=UTF-8";
const static char WRITE_MODE[] = "wb, ccs=UTF-8";
#endif

#ifdef IS_LINUX

inline int ck_mkdir(const ck_string& path)
{
	return mkdir(path.c_str(),0777);
}

inline FILE *ck_fopen( const ck_string& path, const ck_string& mode )
{
    FILE* fp;
    fp = fopen( path.c_str(), mode.c_str() );

    if ( !fp )
        CK_THROW(( "Unable to open file: %s\n", path.c_str() ));

    return fp;
}

#endif

#ifdef IS_WINDOWS

inline int ck_mkdir( const ck_string& path )
{
    return _wmkdir(path.get_wide_string().data());
}

inline FILE *ck_fopen( const ck_string& path, const ck_string& mode )
{
    errno_t err;
    FILE* fp;

    if ( err = _wfopen_s( &fp, path.get_wide_string().c_str(), mode.get_wide_string().c_str() ) != 0 )
        CK_THROW(( "fopen_s returned error code: %d.\n", (int)err ));

    if ( !fp )
        CK_THROW(( "Unable to open file: %s\n", path.c_str() ));

    return fp;
}

#endif

}

#endif
