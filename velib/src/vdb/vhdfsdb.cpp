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
#include "vhdfsdb.hpp"
#include "license.hpp"
#include "hdfswrapper.hpp"
#include "hdfsrecsession.hpp"

#include "cppkit/ck_memory.h"
#include "cppkit/ck_command_queue.h"
#include "ffkit/av_muxer.h"


using namespace std;
using namespace cppkit;
using namespace ffkit;

class VHdfsDBData
{
public:
	VHdfsDBData(VHdfsDB &pDB)
	:m_vDB(pDB), m_pHdfsWrapper(NULL), m_nWrapperCnt(0)
	{

	}
	~VHdfsDBData()
	{

	}

public:
	VHdfsDB &m_vDB;
	HdfsRecWrapper *m_pHdfsWrapper;
	s32 m_nWrapperCnt;
};


VHdfsDB::VHdfsDB(astring &pNameNode, astring &pPort, 
		astring &pUser)
:m_pNameNode(pNameNode), m_pPort(pPort), m_pUser(pUser), m_inerval(30)
{
	m_data = new VHdfsDBData(*this);
	ck_string ckStrNameNode = pNameNode;
	ck_string ckStrPort = pPort;
	ck_string ckStrUser = pUser;
	m_data->m_pHdfsWrapper = new HdfsRecWrapper(ckStrNameNode, 
							ckStrPort, ckStrUser);
}

VHdfsDB::~VHdfsDB()
{

}

void VHdfsDB::Lock()
{
	m_Lock.lock();
}

void VHdfsDB::UnLock()
{
	m_Lock.unlock();
}

BOOL VHdfsDB::Config(astring &pNameNode, astring &pPort, 
		astring &pUser)
{
	return TRUE;
}

HdfsRecWrapper & VHdfsDB::GetHdfsRecWrapper()
{
	Lock();
	m_data->m_nWrapperCnt ++;
	UnLock();

	return *(m_data->m_pHdfsWrapper);
}

BOOL VHdfsDB::ReleaseHdfsRecWrapper()
{
	Lock();
	m_data->m_nWrapperCnt --;
	UnLock();
	return TRUE;
}

HdfsRecSession * VHdfsDB::StartRecord(s32 deviceId, astring strName)
{
	astring strBlockPath;
	
	Lock();

	UnLock();	
	return new HdfsRecSession(deviceId, strName, *this);
}

BOOL VHdfsDB::FinishRecord(HdfsRecSession *pSess)
{
   	Lock();
	
	//
	UnLock();
	
	return TRUE;
}
