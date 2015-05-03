
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

#ifndef cppkit_path_h
#define cppkit_path_h

#include "cppkit/ck_string.h"
#include "cppkit/ck_exception.h"
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef IS_WINDOWS
    #include <io.h>
    #define ALTERNATE_SLASH "/"
#else
    #include <dirent.h>
    #include <fnmatch.h>
#endif

#ifndef IS_WINDOWS
#define PATH_SLASH "/"
#else
#define PATH_SLASH "\\"
#endif

namespace cppkit
{

/// This class provides cross platform directory listing, supporting standard Unix and
/// Windows shell wildcard expansion (globing). Any code that needs to access directory
/// lists can do so through this class in a platform independant manner.
class ck_path
{
    struct path_parts
    {
        ck_string path;
        ck_string glob;
    };

 public:
    /// Constructor, takes optionally globbed path.
    /// \param glob A path and optional shell wildcard glob.
    CK_API ck_path( const ck_string& glob );
    CK_API ck_path( const ck_path& ) = delete;

    CK_API virtual ~ck_path() throw();

    CK_API ck_path& operator = ( const ck_path& ) = delete;

    // Returns true if parameter is a path to a directory.
    // \param path The path to check for directoryness.
    CK_API static bool is_dir( const ck_string& path );

    // Returns true if parameter is a path to a regular file.
    // \param path The path to check for regular fileness.
    CK_API static bool is_reg( const ck_string& path );

    // Returns true if parameter is a path that exists.
    // \param path The path to check existence of.
    CK_API static bool exists( const ck_string& path );

    // Returns the total size on disk of the file or directory specified by argument.
    // \param path The file or directory to get the size of.
    CK_API static int64_t file_size( const ck_string& path );

    CK_API void open_dir( const ck_string& glob );

    CK_API bool read_dir( ck_string& fileName );

 private:

    path_parts _pathParts;
    bool _done;

#ifdef IS_WINDOWS
    intptr_t _handle;
    struct _wfinddata_t _fs;
#else
    DIR* _d;
#endif

    path_parts _get_path_and_glob( const ck_string& glob ) const;
};

}

#endif
