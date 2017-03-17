
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/LargeFiles.h"
#include "XSDK/XPath.h"

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>

#include "XSDK/Errors.h"

#ifndef WIN32

    #include <dirent.h>
    #include <fnmatch.h>

#else

    #include <io.h>
    #define ALTERNATE_SLASH "/"
#endif

using namespace XSDK;

XPath::XPath(const XString& glob) :
    _names()
{
    OpenDir( glob );
}

XPath::XPath( const XPath& obj ) :
    _names( obj._names )
{
}

XPath::~XPath() throw()
{
}

XPath& XPath::operator = ( const XPath& obj )
{
    _names = obj._names;
    return *this;
}

void XPath::OpenDir(const XSDK::XString& glob)
{
    if(IsReg(glob))
        X_THROW(("Glob passed to XPath::OpenDir() specifies a regular file."));

    if(IsDir(glob))
    {
#ifndef WIN32
        _GetFileNames(glob, "");
#else
        _GetFileNames(glob, "*.*");
#endif
    }
    else
    {
        const int lastSlash = glob.rfind(PATH_SLASH);

        XString pathPart;
        if(lastSlash >= 0)
            pathPart = glob.substr(0, lastSlash);

        XString globPart;
        if(((int)glob.size()) > (lastSlash+1))
            globPart = glob.substr(lastSlash + 1, glob.size() - (lastSlash+1));

        _GetFileNames(pathPart, globPart);
    }
}

void XPath::Clear()
{
    _names.Clear();
}

bool XPath::Empty() const
{
    return _names.GetElemCount() == 0;
}

size_t XPath::GetElemCount() const
{
    return _names.GetElemCount();
}

void XPath::IterStart( bool forward )
{
    _names.IterStart( forward );
}

void XPath::Iterate()
{
    _names.Iterate();
}

XString* XPath::IterData() const
{
    return _names.IterData();
}

bool XPath::IterValid() const
{
    return _names.IterValid();
}

bool XPath::IsDir(const XString& path)
{
    struct x_file_info fileInfo;

    if(x_stat( path, &fileInfo ) != 0)
        X_THROW((XString::Format("Failed to stat file %s", path.c_str()).c_str()));

    return (fileInfo._fileType == X_DIRECTORY) ? true : false;
}

bool XPath::IsReg(const XString& path)
{
    struct x_file_info fileInfo;

    if(x_stat( path, &fileInfo ) != 0)
        X_THROW((XString::Format("Failed to stat file %s", path.c_str()).c_str()));

    return (fileInfo._fileType == X_REGULAR) ? true : false;
}

bool XPath::Exists(const XString& path)
{
    struct x_file_info fileInfo;

    return x_stat( path, &fileInfo ) == 0;
}

XString XPath::GetCurDir()
{
    XString result;
#ifndef WIN32
    char cwd[4096];
    if ( getcwd(cwd, 4096) == NULL )
        X_THROW(("CWD Path is too big!"));
#else
    wchar_t cwd[4096];
    if ( _wgetcwd(cwd,4096) == NULL )
        X_THROW(("CWD Path is too big!"));
#endif
    result = cwd;
    return result;
}

int64_t XPath::FileSize(const XString& path)
{
    int64_t fileSize = 0;

    if( IsDir( path ) )
    {
        XPath files( path );

        for( files.IterStart(); files.IterValid(); files.Iterate() )
            fileSize += FileSize( path + PATH_SLASH + *files.IterData() );
    }
    else
    {
        x_file_info fileInfo;
        x_stat( path, &fileInfo );

        fileSize += fileInfo._fileSize;
    }

    return fileSize;
}

void XPath::_GetFileNames(const XString& path, const XString& glob)
{
    _names.Clear();

#ifndef WIN32
    if(DIR* d = opendir(path.c_str()))
    {
        while(struct dirent* d_entry = readdir(d))
        {
            if ((strncmp(".", d_entry->d_name, 1) != 0) &&
                 (strncmp("..", d_entry->d_name, 2) != 0))
            {
                XString file_name = d_entry->d_name;
                if(!glob.empty())
                {
                    if (fnmatch(glob.c_str(), d_entry->d_name, 0) == 0)
                        _names.Append(file_name);
                }
                else
                    _names.Append(file_name);
            }
        }
        closedir(d);
    }
    else
        X_THROW(("Unable to opendir() requested path."));
#else
    XString full_spec;

    if(!glob.empty())
        full_spec.FormatInPlace("%s%s%s", path.c_str(), PATH_SLASH, glob.c_str());
    else
        full_spec.FormatInPlace("%s", path.c_str());

    struct _wfinddata_t fs;//files search structure
    intptr_t handle; //Search handle

    std::wstring spec = XString::ConvertMultiByteStringToWideString(full_spec);
    if((handle = _wfindfirst(spec.c_str(), &fs)) == -1)
        X_THROW(("Unable to _findfirst() requested path."));
    do
    {
        XString name(fs.name);
        if ((strncmp(".", name.c_str(), 1) != 0) && (strncmp("..", name.c_str(), 2) != 0))
            _names.Append(fs.name);
    } while((_wfindnext(handle,&fs))==0);
    _findclose(handle);
#endif
    return;
}
