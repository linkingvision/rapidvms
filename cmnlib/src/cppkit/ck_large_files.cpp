
#include "cppkit/os/ck_large_files.h"

#ifdef IS_LINUX
#include <fcntl.h>
#endif

#ifdef IS_WINDOWS
#include <io.h>
#endif

namespace cppkit
{

#ifdef IS_WINDOWS

CK_API int ck_stat( const ck_string& file_name, struct ck_file_info* file_info )
{
    // XXX - Needs wide character conversion

    ck_string path = (file_name.ends_with("\\")||file_name.ends_with("/"))?file_name.substr(0,file_name.length()-1):file_name;
    int result = -1;
    struct __stat64 sfi;

    if( _wstat64( path.get_wide_string().data(), &sfi ) == 0 )
    {
        file_info->file_size = sfi.st_size;
        file_info->file_type = (sfi.st_mode & _S_IFDIR) ? CK_DIRECTORY : CK_REGULAR;
        file_info->access_time = sfi.st_atime;

        // Note: On Win32, we currently hard code this to a reasonable value.
        file_info->optimal_block_size = 4096;

        result = 0;;
    }
    return result;
}

CK_API int ck_fseeko( FILE* file, int64_t offset, int whence )
{
    return _fseeki64( file, offset, whence );
}

CK_API int64_t ck_ftello( FILE* file )
{
    return _ftelli64( file );
}

CK_API int ck_filecommit( FILE* file )
{
    fflush(file);

    // Tell windows to commit all buffered file data to disk.
    int fd = _fileno(file);
    int err = _commit(fd);

    return err;
}

#ifdef IS_WINDOWS
#define FILENO _fileno
#else
#define FILENO fileno
#endif

CK_API int ck_fallocate( FILE* file, int64_t size )
{
    return ( _chsize_s( FILENO( file ), size ) == 0) ? 0 : -1;
}

#else

CK_API int ck_stat( const ck_string& file_name, struct ck_file_info* file_info )
{
    struct stat sfi;

    if( stat( file_name.c_str(), &sfi ) == 0 )
    {
        file_info->file_size = sfi.st_size;
        file_info->file_type = (sfi.st_mode & S_IFDIR) ? CK_DIRECTORY : CK_REGULAR;
        file_info->access_time = sfi.st_atime;
        file_info->optimal_block_size = sfi.st_blksize;

        return 0;
    }

    return -1;
}

CK_API int ck_fseeko( FILE* file, int64_t offset, int whence )
{
    return fseeko( file, offset, whence );
}

CK_API int64_t ck_ftello( FILE* file )
{
    return ftello( file );
}

CK_API int ck_filecommit( FILE* file )
{
    fflush(file);

    // sync all data associated with file stream to disk.
    int fd = fileno(file);
    fdatasync(fd);

    // advise kernel to dump cached data from memory.
    int err = posix_fadvise64(fd, 0, 0, POSIX_FADV_DONTNEED);

    return err;
}

CK_API int ck_fallocate( FILE* file, int64_t size )
{
    return ( posix_fallocate64( fileno( file ), 0, size ) == 0 ) ? 0 : -1;
}

#endif

CK_API bool exists( const ck_string& file_name )
{
    ck_file_info file_info;
    return ck_stat(file_name, &file_info) == 0;
}

ck_string temp_dir()
{
#ifdef IS_LINUX
    const char* tmp_dir = getenv("TMPDIR");
    if(tmp_dir && exists(tmp_dir))
        return tmp_dir;

    tmp_dir = getenv("TEMP");
    if(tmp_dir && exists(tmp_dir))
        return tmp_dir;

    tmp_dir = getenv("TMP");
    if(tmp_dir && exists(tmp_dir))
        return tmp_dir;

    tmp_dir = "/tmp";
    if(exists(tmp_dir))
        return tmp_dir;

    tmp_dir = "/var/tmp";
    if(exists(tmp_dir))
        return tmp_dir;

    tmp_dir = "/usr/tmp";
    if(exists(tmp_dir))
        return tmp_dir;

    return ".";
#else
    wchar_t buf[MAX_PATH];
    DWORD len = GetTempPathW(MAX_PATH, buf);
    return len != 0 ? ck_string(buf, len) : ".";
#endif
}

}
