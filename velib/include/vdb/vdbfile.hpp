#ifndef __VDB_FILE_HPP__
#define __VDB_FILE_HPP__
#include "utility.hpp"
#include "vdbtype.hpp"

class VE_LIBRARY_API VdbFile
{
public:
    VdbFile(astring & strPath);
    ~VdbFile();
	
public:
	BOOL Open(s32 flags, s32 mode);
	BOOL Lseek(s32 offset, s32 whence);
	
	
public:
	BOOL Close();
	s32 Read(void * buf, s32 len);
	s32 Write(void * buf, s32 len);
	BOOL Fflush();
	s32 Size();

public:


private:
    astring m_strPath;
	VDB_HANDLE m_handle;
};

#endif /* __VDB_FILE_HPP__ */