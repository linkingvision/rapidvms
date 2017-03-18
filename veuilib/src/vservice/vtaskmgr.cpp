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
#include "vtaskmgr.hpp"

VTaskMgr * g_VTaskMgr = NULL;

class VTaskMgrImpl 
{
public:
	VTaskMgrImpl();
	~VTaskMgrImpl();
public:
	bool GetTaskList(VTaskItemList &pList);
	bool AddTask(VTaskItem *pTask);
	bool DeleteTask(VTaskItem *pTask);
	int GetTaskCnt();
	void Lock(){m_Lock.lock();}
	void UnLock(){m_Lock.unlock();}
	
private:
	VTaskItemList m_list;
	fast_mutex m_Lock;
	int m_TaskCnt;
};

VTaskMgrImpl::VTaskMgrImpl()
:m_TaskCnt(0)
{

}

VTaskMgrImpl::~VTaskMgrImpl()
{

}

bool VTaskMgrImpl::GetTaskList(VTaskItemList &pList)
{
	Lock();
	
	pList = m_list;
	UnLock();
	return TRUE;
}


bool VTaskMgrImpl::AddTask(VTaskItem *pTask)
{
	if (pTask == NULL)
	{
		return FALSE;
	}
	
	Lock();
	m_TaskCnt ++;
	m_list[pTask->GetId()] = pTask;
	UnLock();
	return TRUE;
}

bool VTaskMgrImpl::DeleteTask(VTaskItem *pTask)
{

	if (pTask == NULL)
	{
		return FALSE;
	}
	
	Lock();

	m_TaskCnt --;
	if (m_TaskCnt <= 0)
	{
		m_TaskCnt = 0;
	}

	m_list.erase(pTask->GetId());

	pTask->StopTask();

	pTask->wait();

	UnLock();
	return TRUE;
}

int VTaskMgrImpl::GetTaskCnt()
{
	int cnt = 0;
	Lock();

	cnt = m_TaskCnt;

	UnLock();
	return cnt;
}

VTaskMgr::VTaskMgr()
:m_pImpl(NULL)
{
	m_pImpl = new VTaskMgrImpl();
	if (g_VTaskMgr == NULL)
	{
		g_VTaskMgr = this;
	}
}

VTaskMgr::~VTaskMgr()
{

}

bool VTaskMgr::GetTaskList(VTaskItemList &pList)
{
	if (g_VTaskMgr)
	{
		return g_VTaskMgr->m_pImpl->GetTaskList(pList);
	}
	return false;
}
bool VTaskMgr::AddTask(VTaskItem *pTask)
{
	if (g_VTaskMgr)
	{
		return g_VTaskMgr->m_pImpl->AddTask(pTask);
	}
	return false;
}
bool VTaskMgr::DeleteTask(VTaskItem *pTask)
{
	if (g_VTaskMgr)
	{
		return g_VTaskMgr->m_pImpl->DeleteTask(pTask);
	}
	return false;
}

int VTaskMgr::GetTaskCnt()
{
	if (g_VTaskMgr)
	{
		return g_VTaskMgr->m_pImpl->GetTaskCnt();
	}
	return false;
}

