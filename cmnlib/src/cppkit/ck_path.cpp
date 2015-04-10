
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

#include "cppkit/os/ck_large_files.h"
#include "cppkit/ck_path.h"

using namespace cppkit;
using namespace std;

ck_path::ck_path( const ck_string& glob ) :
    _pathParts(),
    _done( false ),
#ifdef IS_WINDOWS
    _handle( 0 ),
    _fs()
#else
    _d( NULL )
#endif
{
    open_dir( glob );
}

ck_path::~ck_path() throw()
{
#ifdef IS_WINDOWS
    if( _handle != 0 )
        _findclose( _handle );
#else
    if( _d )
        closedir( _d );
#endif
}

bool ck_path::is_dir( const ck_string& path )
{
    struct ck_file_info fileInfo;

    if( ck_stat( path, &fileInfo ) != 0 )
        CK_THROW(( ck_string::format("Failed to stat file %s", path.c_str()).c_str() ));

    return (fileInfo.file_type == CK_DIRECTORY) ? true : false;
}

bool ck_path::is_reg( const ck_string& path )
{
    struct ck_file_info fileInfo;

    if( ck_stat( path, &fileInfo ) != 0 )
        CK_THROW(( ck_string::format("Failed to stat file %s", path.c_str()).c_str() ));

    return (fileInfo.file_type == CK_REGULAR) ? true : false;
}

bool ck_path::exists( const ck_string& path )
{
    struct ck_file_info fileInfo;

    return ck_stat( path, &fileInfo ) == 0;
}

int64_t ck_path::file_size( const ck_string& path )
{
    int64_t fileSize = 0;

    if( is_dir( path ) )
    {
        ck_path files( path );

        ck_string fileName;
        while( files.read_dir( fileName ) )
            fileSize += file_size( path + PATH_SLASH + fileName );
    }
    else
    {
        ck_file_info fileInfo;
        ck_stat( path, &fileInfo );

        fileSize += fileInfo.file_size;
    }

    return fileSize;
}

void ck_path::open_dir( const ck_string& glob )
{
    auto parts = _get_path_and_glob( glob );

#ifdef IS_WINDOWS
    if( _handle != 0 )
        _findclose( _handle );

    ck_string fullSpec = ck_string::format( "%s%s%s",
                                            parts.path.c_str(),
                                            PATH_SLASH,
                                            parts.glob.c_str() );

    wstring wspec = ck_string::convert_multi_byte_string_to_wide_string( fullSpec );

    if( (_handle = _wfindfirst( wspec.c_str(), &_fs )) == -1 )
    {
        _handle = 0;
        CK_THROW(("Unable to _wfindfirst() requested path."));
    }
#else
    if( _d )
        closedir( _d );

    _d = opendir( parts.path.c_str() );
    if( !_d )
        CK_THROW(("Unable to open directory."));
#endif
    _done = false;
    _pathParts = parts;
}

bool ck_path::read_dir( ck_string& fileName )
{
    if( _done )
        return false;

#ifdef IS_WINDOWS

    do
    {
        ck_string entryName = _fs.name;

        if( _wfindnext( _handle, &_fs ) != 0 )
            _done = true;

        if( (entryName != ".") && (entryName != "..") )
        {
            fileName = entryName;
            return true;
        }
    }
    while( !_done );

#else

    struct dirent* entry = NULL;
    while( (entry = readdir( _d )) )
    {
        ck_string entryName = entry->d_name;
        if( (entryName != ".") && (entryName != "..") )
        {
            if( !_pathParts.glob.empty() )
            {
                if( fnmatch( _pathParts.glob.c_str(), entryName.c_str(), 0 ) == 0 )
                {
                    fileName = entryName;
                    return true;
                }
            }
            else
            {
                fileName = entryName;
                return true;
            }
        }
    }

    if( entry == NULL )
        _done = true;

#endif

    return false;
}

ck_path::path_parts ck_path::_get_path_and_glob( const ck_string& glob ) const
{
    if( ck_path::is_reg( glob ) )
        CK_THROW(( "Glob passed to ck_path specifies a regular file." ));

    path_parts parts;

    if( ck_path::is_dir( glob ) )
    {
        parts.path = glob;
#ifdef IS_WINDOWS
        parts.glob = "*.*";
#else
        parts.glob = "";
#endif
    }
    else
    {
        const int lastSlash = glob.rfind( PATH_SLASH );

        parts.path = (lastSlash >= 0) ? glob.substr( 0, lastSlash ) : "";
        parts.glob = ((int)glob.size() > (lastSlash+1)) ? glob.substr(lastSlash + 1, glob.size() - (lastSlash+1)) : "";
    }

    return parts;
}
