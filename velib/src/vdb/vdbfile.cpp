/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#include "vdbfile.hpp"

VdbFile::VdbFile(astring & strPath)
{
	m_strPath = strPath;
	Open(O_RDWR, 0);
}
VdbFile::~VdbFile()
{
	Fflush();
	Close();
}

#ifdef WIN32
BOOL VdbFile::Close()
{
	//TODO check if the handle has been closed
	CloseHandle(m_handle);
	return TRUE;
}
s32 VdbFile::Read(void * buf, s32 len)
{
	DWORD nRead = 0;

	if (!ReadFile(m_handle, buf, len, &nRead, NULL))
		return -1;

	return (nRead);
}
s32 VdbFile::Write(void * buf, s32 len)
{
	DWORD nWritten = 0;

	if (!WriteFile(m_handle, buf, len, &nWritten, NULL))
		return -1;

	return (nWritten);
}
BOOL VdbFile::Fflush()
{
	if (FlushFileBuffers(m_handle))
		return TRUE;
	return FALSE;		
}


s32 VdbFile::Size()
{
	struct stat buf;
	
	if (stat(m_strPath.c_str(), &buf) == -1)
		return (-1);
	return (buf.st_size); 	
}

BOOL VdbFile::Open(s32 flags, s32 mode)
{
	DWORD fileaccess = 0, fileshare = 0, filecreate = 0, fileattr = 0;

        /* decode the access flags */
	switch (flags & (O_RDONLY | O_WRONLY | O_RDWR)) {
	case O_RDONLY:         /* read access */
		fileaccess = GENERIC_READ;
		break;
	case O_WRONLY:         /* write access */
		fileaccess = GENERIC_WRITE;
		break;
	case O_RDWR:           /* read and write access */
		fileaccess = GENERIC_READ | GENERIC_WRITE;
		break;
	default:                /* error, bad flags */
		return FALSE;
	}

        /* decode open/create method flags */
	switch (flags & (O_CREAT | O_EXCL | O_TRUNC)) {
	case 0:
	case O_EXCL:            /* ignore EXCL w/o CREAT */
		filecreate = OPEN_EXISTING;
		break;
	case O_CREAT:
		filecreate = OPEN_ALWAYS;
		break;
	case O_CREAT | O_EXCL:
	case O_CREAT | O_TRUNC | O_EXCL:
		filecreate = CREATE_NEW;
		break;
	case O_TRUNC:
	case O_TRUNC | O_EXCL:  /* ignore EXCL w/o CREAT */
		filecreate = TRUNCATE_EXISTING;
		break;
	case O_CREAT | O_TRUNC:
		filecreate = CREATE_ALWAYS;
		break;
	default:                /* error, bad flags */
		return FALSE;
	}

	fileshare |= FILE_SHARE_READ | FILE_SHARE_WRITE;
	fileattr = FILE_ATTRIBUTE_NORMAL;

	m_handle = CreateFileA(m_strPath.c_str(), fileaccess, fileshare, NULL,
			filecreate, fileattr, NULL);
			
	return TRUE;
}

BOOL VdbFile::Lseek(s32 offset, s32 whence)
{
	LARGE_INTEGER li;
	DWORD method;

	if (whence == SEEK_CUR)
		method = FILE_CURRENT;
	else if (whence == SEEK_SET)
		method = FILE_BEGIN;
	else if (whence == SEEK_END)
		method = FILE_END;
	else {
		return FALSE;
	}

	li.QuadPart = offset;
	li.LowPart = SetFilePointer(m_handle, li.LowPart, &li.HighPart, method);

	if (li.LowPart == 0xFFFFFFFF) {
		return FALSE;
	}

	return TRUE;
}
#else
BOOL VdbFile::Close()
{
	close(m_handle);
	return TRUE;
}
s32 VdbFile::Read(void * buf, s32 len)
{
	s32 ret = 0;

	ret = read(m_handle, buf, len);

	return ret;
}
s32 VdbFile::Write(void * buf, s32 len)
{
	s32 ret = 0;

	ret = write(m_handle, buf, len);

	return ret;
}
BOOL VdbFile::Fflush()
{
	fsync(m_handle);	
	return TRUE;
}


s32 VdbFile::Size()
{
    struct stat buf;

    if (fstat(m_handle, &buf) == -1)
            return FALSE;
    return (buf.st_size);
}

BOOL VdbFile::Open(s32 flags, s32 mode)
{
	s32 ret = 0;

	if ((flags & O_CREAT) != O_CREAT)
	{
		m_handle = open(m_strPath.c_str(), flags, mode);
		if (m_handle < 0)
		{
			return FALSE;
		}
		return TRUE;
	}

	m_handle = open(m_strPath.c_str(), flags, mode);
	if (m_handle < 0)
	{
		return FALSE;
	}
			
	return TRUE;
}

BOOL VdbFile::Lseek(s32 offset, s32 whence)
{
	s32 ret = 0;

	ret = lseek(m_handle, offset, whence);
	if (ret < 0)
	{
		return FALSE;
	}

	return TRUE;
}
#endif
