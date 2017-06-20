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
#include "client/storfactory.hpp"

StorFactory::StorFactory(ClientConfDB &pConf)
:m_pConf(pConf)
{

}

StorFactory::~StorFactory()
{

}

BOOL StorFactory::Init()
{

	/* Loop add the stor */
	VidStorList storList;
	m_pConf.GetStorListConf(storList);
	int storSize = storList.cvidstor_size();

	for (s32 i = 0; i < storList.cvidstor_size(); i ++)
	{
		VidStor pStor = storList.cvidstor(i);
		InitAddStor(pStor);
	}

	return TRUE;
}

bool StorFactory::Start()
{
	XGuard guard(m_cMutex);
	StorClientMap::iterator it = m_StorClientMap.begin(); 
	for(; it!=m_StorClientMap.end(); ++it)
	{
		if ((*it).second)
		{
			((*it).second)->StorClientStart();
		}
	}
	return true;
}

bool StorFactory::InitAddStor(VidStor &pStor)
{
	StorFactoryNotifyInterface &pNotify = *this;
	m_StorClientMap[pStor.strid()] = new StorClient(pStor, pNotify);

       m_StorClientOnlineMap[pStor.strid()] = false;

    	return true;
}

bool StorFactory::AddStor(VidStor & pParam)
{
	XGuard guard(m_cMutex);
	StorFactoryChangeData change;
	
	if (m_pConf.FindStor(pParam.strid()))
	{

		m_pConf.DeleteStor(pParam.strid());

		/* Call Change */
		change.cId.set_strstorid(pParam.strid());
		change.type = STOR_FACTORY_STOR_DEL;
		guard.Release();
		CallChange(change);
		guard.Acquire();
		
		delete m_StorClientMap[pParam.strid()];
		m_StorClientMap[pParam.strid()] = NULL;
		m_StorClientMap.erase(pParam.strid());
		m_StorClientOnlineMap.erase(pParam.strid());
	}

	/* Add */
	m_pConf.AddStor(pParam);
	InitAddStor(pParam);

	/* Call Change */
	change.cId.set_strstorid(pParam.strid());
	change.type = STOR_FACTORY_STOR_ADD;
	guard.Release();
	CallChange(change);
	guard.Acquire();

	return true;
}
bool StorFactory::DeleteStor(astring strId)
{
	XGuard guard(m_cMutex);
	StorFactoryChangeData change;

	if (m_pConf.FindStor(strId))
	{
		m_pConf.DeleteStor(strId);
		/* Call Change */
		change.cId.set_strstorid(strId);
		change.type = STOR_FACTORY_STOR_DEL;
		guard.Release();
		CallChange(change);
		guard.Acquire();
		
		//delete m_StorClientMap[strId];
		/* the StorClient will be delete itself */
		m_StorClientMap[strId] = NULL;
		m_StorClientMap.erase(strId);
		m_StorClientOnlineMap.erase(strId);
		
	}

	return true;
}

bool StorFactory::CallChange(StorFactoryChangeData data)
{
#if 0
	//XGuard guard(m_cMutex);
	StorChangeNofityMap::iterator it = m_Change.begin(); 
	for(; it!=m_Change.end(); ++it)
	{
		if ((*it).second)
		{
			(*it).second((*it).first, data);
		}
	}
#endif
	std::string strId;
	std::string strCam;
	bool bRet1 = data.cId.SerializeToString(&strId);
	bool bRet2 = data.cCam.SerializeToString(&strCam);
	if (bRet1 == true && bRet2 == true)
	{
		emit(SignalCallChange(data.type, strId, strCam));
	}
	return true;
}

bool StorFactory::OnEvent(VidEvent &pEvent, VidStor &pStor)
{	
	std::string strEvent;
	std::string strStor;

	pEvent.set_strstorname(pStor.strname());
	pEvent.set_strstorid(pStor.strid());
	bool bRet = pEvent.SerializeToString(&strEvent);
	if (bRet == true)
	{
		VDC_DEBUG( "%s  Get Event %d %s  %s bsearched %d\n",__FUNCTION__, 
								pEvent.strtime().c_str(), 
								pEvent.strdevicename().c_str(), pEvent.strtype().c_str(), 
								pEvent.bsearched());
		emit(SignalEvent(strEvent));
		if (pEvent.bsearched() == false)
		{
			emit(SignalEvent1());
		}
	}
	return true;
}

bool StorFactory::AddCam(astring strStorId, VidCamera &pParam)
{
	if (m_pConf.FindStor(strStorId) && m_StorClientMap[strStorId])
	{
		return m_StorClientMap[strStorId]->AddCam(pParam);
	}
	return false;
}

bool StorFactory::PtzCmd(astring strStorId, astring strId, u32 action, double param)
{
	if (m_pConf.FindStor(strStorId) && m_StorClientMap[strStorId])
	{
		return m_StorClientMap[strStorId]->PtzCmd(strId, action, param);
	}
	return false;
}

bool StorFactory::DeleteCam(astring strStorId, astring strId)
{
	if (m_pConf.FindStor(strStorId) && m_StorClientMap[strStorId])
	{
		return m_StorClientMap[strStorId]->DeleteCam(strId);
	}
	return false;
}


bool StorFactory::SearchEvent(astring strStorId, astring strId, s64 nStart, s64 nEnd)
{
	if (m_pConf.FindStor(strStorId) && m_StorClientMap[strStorId])
	{
		return m_StorClientMap[strStorId]->SearchEvent(strId, nStart, nEnd);
	}
	return false;
}

bool StorFactory::RegRealEvent(astring strStorId)
{
	if (m_pConf.FindStor(strStorId) && m_StorClientMap[strStorId])
	{
		return m_StorClientMap[strStorId]->RegRealEvent();
	}
	return false;
}

bool StorFactory::UnRegRealEvent(astring strStorId)
{
	if (m_pConf.FindStor(strStorId) && m_StorClientMap[strStorId])
	{
		return m_StorClientMap[strStorId]->UnRegRealEvent();
	}
	return false;
}

bool StorFactory::HandleEvent(astring strStorId, astring strId)
{
	if (m_pConf.FindStor(strStorId) && m_StorClientMap[strStorId])
	{
		return m_StorClientMap[strStorId]->HandleEvent(strId);
	}
	return false;
}

StorClientOnlineMap StorFactory::GetVidCameraOnlineList(astring strStor)
{
	StorClientOnlineMap empty;

	if (m_pConf.FindStor(strStor) && m_StorClientMap[strStor])
	{
		return m_StorClientMap[strStor]->GetVidCameraOnlineList();
	}

	return empty;
}

StorClientRecMap StorFactory::GetVidCameraRecList(astring strStor)
{
	StorClientOnlineMap empty;

	if (m_pConf.FindStor(strStor) && m_StorClientMap[strStor])
	{
		return m_StorClientMap[strStor]->GetVidCameraRecList();
	}

	return empty;
}

VidCameraList StorFactory::GetVidCameraList(astring strStor)
{	
	VidCameraList empty;
	
	if (m_pConf.FindStor(strStor) && m_StorClientMap[strStor])
	{
		return m_StorClientMap[strStor]->GetVidCameraList();
	}

	return empty;
}

bool StorFactory::GetCameraConf(astring strStor, astring strCameraId, VidCamera &pCam)
{
	if (m_pConf.FindStor(strStor) && m_StorClientMap[strStor])
	{
		return m_StorClientMap[strStor]->GetCameraConf(strCameraId, pCam);
	}

	return false;
}

astring StorFactory::GetVidCameraName(astring strStor, astring strCam)
{
	astring empty;
	if (m_pConf.FindStor(strStor) && m_StorClientMap[strStor])
	{
		return m_StorClientMap[strStor]->GetVidCameraName(strCam);
	}

	return empty;
}

bool StorFactory::GetOnline(astring strStor)
{
	if (m_pConf.FindStor(strStor) &&  m_StorClientMap[strStor])
	{
		return m_StorClientMap[strStor]->GetOnline();
	}
	return false;
}


