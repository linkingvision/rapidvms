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