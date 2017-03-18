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
#ifndef __V_HDFS_DB_HPP__
#define __V_HDFS_DB_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "utility/videotype.hpp"

using namespace UtilityLib;

class HdfsRecSession;
class VHdfsDBData;
class HdfsRecWrapper;
class VE_LIBRARY_API VHdfsDB
{
public:
	typedef std::map<s32, HdfsRecSession*> _MapSession;
public:

	VHdfsDB(astring &pNameNode, astring &pPort, 
		astring &pUser);
	~VHdfsDB();
public:
	void Lock();
	void UnLock();
public:
	BOOL Config(astring &pNameNode, astring &pPort, 
		astring &pUser);
public:
	/* Start and Stop Record */
	HdfsRecSession * StartRecord(s32 deviceId, astring strName);
	BOOL FinishRecord(HdfsRecSession *pSess);
public:
	HdfsRecWrapper & GetHdfsRecWrapper();
	BOOL ReleaseHdfsRecWrapper();
	s32 GetInerval()
	{
		return m_inerval;
	}

private:
    	fast_mutex m_Lock;
	_MapSession m_MapSess;
    	VHdfsDBData * m_data;
	astring m_pNameNode;
	astring m_pPort; 
	astring m_pUser;
	s32 m_inerval;/*time in sec */
};
#endif /* __V_HDFS_DB_HPP__ */
