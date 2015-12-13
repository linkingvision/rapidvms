//------------------------------------------------------------------------------
// File: storfactory impl.hpp
//
// Desc: Camera storfactory - Manage IP Camera.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_STOR_FACTORY_IMPL_H_
#define __VSC_STOR_FACTORY_IMPL_H_



inline StorFactory::StorFactory(ClientConfDB &pConf)
:m_pConf(pConf)
{

}

inline StorFactory::~StorFactory()
{

}

inline BOOL StorFactory::Init()
{

	/* Loop add the stor */
	
#if 1 //add stor to test
	{	
		VidStorList sotrList;
		VidStor stor;
		UUIDGenerator uuidCreator;
		m_pConf.GetStorListConf(sotrList);
		
		astring strId  = uuidCreator.createRandom().toString();
		stor.set_strid(strId);
		stor.set_strname(strId);
		stor.set_strip("192.168.1.1");
		stor.set_strport("80");
		stor.set_struser("admin");
		stor.set_strpasswd("admin");
		VidStor *pAddStor = sotrList.add_cvidstor();
		*pAddStor = stor;
		m_pConf.UpdateStorListConf(sotrList);
		
	}
#endif

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

inline s32 StorFactory::InitAddStor(VidStor &pStor)
{
	StorFactoryNotifyInterface &pNotify = *this;
	m_StorClientMap[pStor.strid()] = new StorClient(pStor, pNotify);

       m_StorClientOnlineMap[pStor.strid()] = false;

    return TRUE;
}


inline BOOL StorFactory::RegChangeNotify(void * pData, StorFactoryChangeNotify callback)
{
	XGuard guard(m_cMutex);
	m_Change[pData] = callback;

	return TRUE;
}


inline bool StorFactory::CallChange(StorFactoryChangeData data)
{
	//XGuard guard(m_cMutex);
	StorChangeNofityMap::iterator it = m_Change.begin(); 
	for(; it!=m_Change.end(); ++it)
	{
		if ((*it).second)
		{
			(*it).second((*it).first, data);
		}
	}	
	return true;
}


inline VidCameraList StorFactory::GetVidCameraList(astring strStor)
{	
	if (m_StorClientMap[strStor])
	{
		return m_StorClientMap[strStor]->GetVidCameraList();
	}
	
}

#if 0
inline BOOL StorFactory::SearchItems(s32 cameraId, u32 startTime, u32 endTime, u32 recordType, 
				RecordItemMap &map)
{
    return m_pVdb->SearchItems(cameraId, startTime, endTime, recordType, 
                        map);
}

inline BOOL StorFactory::SearchHasItems(s32 cameraId, u32 startTime, u32 endTime, u32 recordType)
{
    return m_pVdb->SearchHasItems(cameraId, startTime, endTime, recordType);
}
inline BOOL StorFactory::SearchAItem(s32 cameraId, u32 Time, VdbRecordItem &pItem)
{
    return m_pVdb->SearchAItem(cameraId, Time, pItem);
}

inline BOOL StorFactory::SearchAItemNear(s32 cameraId, u32 Time, VdbRecordItem &pItem)
{
    return m_pVdb->SearchAItemNear(cameraId, Time, pItem);
}
inline BOOL StorFactory::SearchNextItem(s32 cameraId, s64 LastId, VdbRecordItem &pItem)
{
    return m_pVdb->SearchNextItem(cameraId, LastId, pItem);
}
inline BOOL StorFactory::RequestAMFRead(VdbRecordItem &pItem, astring & strPath)
{
    return m_pVdb->RequestAMFRead(pItem, strPath);
}
inline BOOL StorFactory::FinishedAMFRead(VdbRecordItem &pItem, astring & strPath)
{
    return m_pVdb->FinishedAMFRead(pItem, strPath);
}

inline VDB& StorFactory::GetVdb()
{
	return *m_pVdb;
}

inline BOOL StorFactory::GetCameraParamMap(CameraParamMap &pMap)
{
    pMap = m_CameraParamMap;

    return TRUE;
}

inline BOOL StorFactory::GetCameraOnlineMap(CameraOnlineMap &pMap)
{
    pMap = m_CameraOnlineMap;

    return TRUE;
}

inline BOOL StorFactory::GetVIPCCameraParamMap(VIPCCameraParamMap &pMap)
{
    pMap = m_VIPCCameraParamMap;

    return TRUE;
}

inline s32 StorFactory::InitAddCamera(CameraParam & pParam, u32 nIndex)
{
    if (pParam.m_Conf.data.conf.nType == VSC_CAMERA_CAM)
    {
    	m_CameraMap[nIndex] = new Camera(*m_pVdb, *m_pVHdfsdb, pParam);
    }else
    {
    	m_CameraMap[nIndex] = NULL;
    }
    m_CameraParamMap[nIndex] = pParam;
    m_CameraOnlineMap[nIndex] = 0;

    return TRUE;
}

inline BOOL StorFactory::RegDataCallback(s32 nIndex, CameraDataCallbackFunctionPtr pCallback,
        void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->RegDataCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL StorFactory::UnRegDataCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegDataCallback(pParam);
    }

    UnLock();

    return TRUE;
}


inline BOOL StorFactory::GetInfoFrame(s32 nIndex, InfoFrame &pFrame)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->GetInfoFrame(pFrame);
    }

    UnLock();

    return TRUE;
}

inline BOOL StorFactory::RegSubDataCallback(s32 nIndex, CameraDataCallbackFunctionPtr pCallback,
        void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->RegSubDataCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL StorFactory::UnRegSubDataCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegDataCallback(pParam);
    }

    UnLock();

    return TRUE;
}


inline BOOL StorFactory::GetSubInfoFrame(s32 nIndex, InfoFrame &pFrame)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->GetSubInfoFrame(pFrame);
    }

    UnLock();

    return TRUE;
}

inline BOOL StorFactory::RegRawCallback(s32 nIndex, CameraRawCallbackFunctionPtr pCallback,
        void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->RegRawCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL StorFactory::UnRegRawCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegRawCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL StorFactory::RegSeqCallback(s32 nIndex, CameraSeqCallbackFunctionPtr pCallback,
        void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->RegSeqCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL StorFactory::UnRegSeqCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegSeqCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL StorFactory::RegSubRawCallback(s32 nIndex, CameraRawCallbackFunctionPtr pCallback,
        void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->RegSubRawCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL StorFactory::UnRegSubRawCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegSubRawCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL StorFactory::RegDelCallback(s32 nIndex, CameraDelCallbackFunctionPtr pCallback, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->RegDelCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}
inline BOOL StorFactory::UnRegDelCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegDelCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL StorFactory::GetCameraOnline(s32 nIndex, BOOL &bStatus)
{
    Lock();

    if (m_CameraMap[nIndex] != NULL)
    {
        bStatus =  m_CameraMap[nIndex]->GetCameraOnline();
    }

    UnLock();

    return TRUE;
}

inline   BOOL StorFactory::GetUrl(s32 nIndex, std::string &url)
{
    BOOL ret = FALSE;

    if (m_CameraMap[nIndex] != NULL)
    {
        ret =  m_CameraMap[nIndex]->GetUrl(url);
    }

    UnLock();

    return ret;
}

inline BOOL StorFactory::AttachPlayer(s32 nIndex, HWND hWnd, int w, int h, 
	CameraDelCallbackFunctionPtr pCallback, void * pParam)
{
    //Lock();//For VIPC testing
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->AttachPlayer(hWnd, w, h);
	 m_CameraMap[nIndex]->RegDelCallback(pCallback, pParam);
    }
    //UnLock();

	return TRUE;
}

inline BOOL StorFactory::GetStreamInfo(s32 nIndex, VideoStreamInfo &pInfo)
{
	Lock();
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->GetStreamInfo(pInfo);
	}
	UnLock();

	return TRUE;
}


inline BOOL StorFactory::UpdateWidget(s32 nIndex, HWND hWnd, int w, int h)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UpdateWidget(hWnd, w, h);
    }
    UnLock();

	return TRUE;
}

inline BOOL StorFactory::DetachPlayer(s32 nIndex, HWND hWnd, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->DetachPlayer(hWnd);
	 m_CameraMap[nIndex]->UnRegDelCallback(pParam);
    }
    UnLock();

	return TRUE;
}

inline BOOL StorFactory::EnablePtz(s32 nIndex, HWND hWnd, bool enable)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->EnablePtz(hWnd, enable);
    }
    UnLock();

	return TRUE;
}

inline BOOL StorFactory::DrawPtzDirection(s32 nIndex, HWND hWnd, int x1, int y1, int x2,  int y2)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->DrawPtzDirection(hWnd, x1, y1, x2, y2);
    }
    UnLock();

	return TRUE;
}

inline BOOL StorFactory::ShowAlarm(s32 nIndex, HWND hWnd)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->ShowAlarm(hWnd);
    }
    UnLock();

	return TRUE;
}

inline BOOL StorFactory::PtzAction(s32 nIndex, FPtzAction action, float speed)
{
	Lock();
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->PtzAction(action, speed);
	}
	UnLock();

	return TRUE;
}


inline BOOL StorFactory::GetRecordStatus(s32 nIndex,BOOL &nStatus)
{
    CameraParam pParam;
    if (nIndex <=0 || nIndex >= STOR_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    if (GetCameraParamByIdTryLock(pParam, nIndex) == FALSE)
    {
        return FALSE;
    }
    if (pParam.m_Conf.data.conf.Recording == 1)
    {
        nStatus = TRUE;
    }else
    {
        nStatus = FALSE;
    }
    return TRUE;
}

inline BOOL StorFactory::StartRecord(s32 nIndex)
{
    CameraParam pParam;
    StorFactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= STOR_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    GetCameraParamById(pParam, nIndex);
    if (pParam.m_Conf.data.conf.Recording == 1)
    {
        return TRUE;
    }

    Lock();
    pParam.m_Conf.data.conf.Recording = 1;
    m_Conf.UpdateCameraData(nIndex, pParam.m_Conf);
    m_CameraParamMap[nIndex] = pParam;
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->SetRecord(TRUE);
        m_CameraMap[nIndex]->StartRecord();
    }
    UnLock();
    change.id = nIndex;
    change.type = STOR_FACTORY_CAMERA_RECORDING_ON;
    CallCameraChange(change);
    return TRUE;
}
inline BOOL StorFactory::StopRecord(s32 nIndex)
{
    CameraParam pParam;
    StorFactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= STOR_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    GetCameraParamById(pParam, nIndex);
    if (pParam.m_Conf.data.conf.Recording == 0)
    {
        return TRUE;
    }

    Lock();
    pParam.m_Conf.data.conf.Recording = 0;
    m_CameraParamMap[nIndex] = pParam;
    m_Conf.UpdateCameraData(nIndex, pParam.m_Conf);
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->SetRecord(FALSE);
        m_CameraMap[nIndex]->StopRecord();
    }
    UnLock();
    change.id = nIndex;
    change.type = STOR_FACTORY_CAMERA_RECORDING_OFF;
    CallCameraChange(change);
    return TRUE;
}


inline BOOL StorFactory::StartHdfsRecord(s32 nIndex)
{
    CameraParam pParam;
    StorFactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= STOR_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    GetCameraParamById(pParam, nIndex);
    if (pParam.m_Conf.data.conf.HdfsRecording == 1)
    {
        return TRUE;
    }

    Lock();
    pParam.m_Conf.data.conf.HdfsRecording = 1;
    m_Conf.UpdateCameraData(nIndex, pParam.m_Conf);
    m_CameraParamMap[nIndex] = pParam;
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->SetHdfsRecord(TRUE);
        m_CameraMap[nIndex]->StartHdfsRecord();
    }
    UnLock();
    change.id = nIndex;
    change.type = STOR_FACTORY_CAMERA_RECORDING_ON;
    CallCameraChange(change);
    return TRUE;
}
inline BOOL StorFactory::StopHdfsRecord(s32 nIndex)
{
    CameraParam pParam;
    StorFactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= STOR_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    GetCameraParamById(pParam, nIndex);
    if (pParam.m_Conf.data.conf.HdfsRecording == 0)
    {
        return TRUE;
    }

    Lock();
    pParam.m_Conf.data.conf.HdfsRecording = 0;
    m_CameraParamMap[nIndex] = pParam;
    m_Conf.UpdateCameraData(nIndex, pParam.m_Conf);
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->SetHdfsRecord(FALSE);
        m_CameraMap[nIndex]->StopHdfsRecord();
    }
    UnLock();
    change.id = nIndex;
    change.type = STOR_FACTORY_CAMERA_RECORDING_OFF;
    CallCameraChange(change);
    return TRUE;
}

inline BOOL StorFactory::StartRecordAll()
{
	Lock();
	CameraParamMap CameraMap = m_CameraParamMap;
	UnLock();
	CameraParamMap::iterator it = CameraMap.begin(); 
	for(; it!=CameraMap.end(); ++it)
	{
	    StartRecord((*it).second.m_Conf.data.conf.nId);
	}

	return TRUE;
}
inline BOOL StorFactory::StopRecordAll()
{
	Lock();
	CameraParamMap CameraMap = m_CameraParamMap;
	UnLock();
	CameraParamMap::iterator it = CameraMap.begin(); 
	for(; it!=CameraMap.end(); ++it)
	{
	    StopRecord((*it).second.m_Conf.data.conf.nId);
	}

	return TRUE;
}
inline BOOL StorFactory::StartHdfsRecordAll()
{
	Lock();
	CameraParamMap CameraMap = m_CameraParamMap;
	UnLock();
	CameraParamMap::iterator it = CameraMap.begin(); 
	for(; it!=CameraMap.end(); ++it)
	{
	    StartHdfsRecord((*it).second.m_Conf.data.conf.nId);
	}

	return TRUE;
}
inline BOOL StorFactory::StopHdfsRecordAll()
{
	Lock();
	CameraParamMap CameraMap = m_CameraParamMap;
	UnLock();
	CameraParamMap::iterator it = m_CameraParamMap.begin(); 
	for(; it!=m_CameraParamMap.end(); ++it)
	{
	    StopHdfsRecord((*it).second.m_Conf.data.conf.nId);
	}	

	return TRUE;
}

inline BOOL StorFactory::UpdateCameraGroup(s32 nIndex, s32 nGroup)
{
    CameraParam pParam;
    StorFactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= STOR_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    GetCameraParamById(pParam, nIndex);
    if (pParam.m_Conf.data.conf.GroupId == nGroup)
    {
        return TRUE;
    }

    Lock();
    pParam.m_Conf.data.conf.GroupId = nGroup;
    m_CameraParamMap[nIndex] = pParam;
    m_Conf.UpdateCameraData(nIndex, pParam.m_Conf);
    UnLock();
    change.id = nIndex;
    change.type = STOR_FACTORY_CAMERA_GROUP_CHANGE;
    CallCameraChange(change);
    return TRUE;
}

inline s32 StorFactory::AddCamera(CameraParam & pParam)
{
	s32 nId = GetCameraID();
	StorFactoryCameraChangeData change;
	Camera *pCamera = NULL;
	CameraParam pParam2;

	Lock();
	pParam.m_Conf.data.conf.nId = nId;
	if (pParam.m_Conf.data.conf.nType == VSC_CAMERA_CAM)
	{
		m_CameraMap[nId] = new Camera(*m_pVdb, *m_pVHdfsdb, pParam);
		pCamera = m_CameraMap[nId]; 
	}else
	{
		m_CameraMap[nId] = NULL;
		UnLock();
		return -1;
	}
	m_CameraParamMap[nId] = pParam;
	m_CameraOnlineMap[nId] = 0;
	m_Conf.AddCamera(pParam.m_Conf, nId);

	UnLock();
	change.id = nId;
	change.type = STOR_FACTORY_CAMERA_ADD;
	CallCameraChange(change);
#if 0
	/* Try to online the camera and lock */
	Lock();
	pCamera->GetCameraParam(pParam2);
	UnLock();

	pParam2.m_wipOnline = pParam2.CheckOnline();
	if (pParam2.m_OnlineUrl == FALSE)
	{
		pParam2.m_wipOnlineUrl = pParam2.UpdateUrl();
	}

	/* Try to make the camera online */
	Lock();
	CameraStatus bCheck = pCamera->CheckCamera(pParam2.m_strUrl, 
			pParam2.m_strUrlSubStream, pParam2.m_bHasSubStream, 
			pParam2.m_wipOnline, pParam2.m_wipOnlineUrl);

	StorFactoryCameraChangeData change2;
	change2.id = nId;
	
	if (bCheck == DEV_OFF2ON)
	{
		change.type = STOR_FACTORY_CAMERA_ONLINE;
		m_CameraOnlineMap[nId] = 1;
		UnLock(); 
		CallCameraChange(change);
		Lock();
	}
	UnLock();
#endif
	
    	return nId;
}

inline BOOL StorFactory::DelCamera(s32 nIndex)
{
    StorFactoryCameraChangeData change;
    VDC_DEBUG( "%s DelCamera %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= STOR_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    change.id = nIndex;
    change.type = STOR_FACTORY_CAMERA_DEL;
    CallCameraChange(change);

    Lock();
    VDC_DEBUG( "%s Cleanup Begin\n",__FUNCTION__);
    m_CameraMap[nIndex]->Cleanup();
    VDC_DEBUG( "%s Cleanup End\n",__FUNCTION__);
    delete m_CameraMap[nIndex];
    m_CameraMap[nIndex] = NULL;
    m_CameraParamMap.erase(nIndex);
    m_CameraOnlineMap.erase(nIndex);
    int size1 = m_CameraMap.size();
    m_CameraMap.erase(nIndex);
    int size2 = m_CameraMap.size();
    m_Conf.DelCamera(nIndex);
    UnLock();
    ReleaseCameraID(nIndex);
	
    return TRUE;
}

inline s32 StorFactory::GetCameraParamById(CameraParam & pParam, s32 nIndex)
{
    //VDC_DEBUG( "%s GetCameraParamById %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= STOR_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }

    Lock();
    pParam = m_CameraParamMap[nIndex];
    UnLock();

    return TRUE;
}

inline s32 StorFactory::GetCameraParamByIdTryLock(CameraParam & pParam, s32 nIndex)
{
    //VDC_DEBUG( "%s GetCameraParamById %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= STOR_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }

#if 1
    if (TryLock() == false)
    {
        return FALSE;
    }
#else
    Lock();
#endif
    pParam = m_CameraParamMap[nIndex];
    UnLock();

    return TRUE;
}

#endif
inline void StorFactory::run()
{
	while(1)
	{
		ve_sleep(1000 * 90);
	}
#if 0

	CameraParamMap paramMap;
	/* Create the thread to update the Disk status */
	while (1)
	{
		paramMap.clear();
		{
			/* Got all the camera param */
			Lock();
			CameraMap::iterator it = m_CameraMap.begin(); 
			for(; it!=m_CameraMap.end(); ++it)
			{	
				s32 nIndex = (*it).first;
				CameraParam pParam;
				Camera *pCamera = m_CameraMap[nIndex];
				if (pCamera == NULL)
				{
					continue;//TODO
				}
				pCamera->GetCameraParam(pParam);
				paramMap[nIndex] = pParam;
			}
			UnLock();
		}
		{
			/* Loop all the cameraparam */
			CameraParamMap::iterator it = paramMap.begin(); 
			for(; it!=paramMap.end(); ++it)
			{	
				/* Loop to check the camera and update the url */
				s32 nIndex = (*it).first;
				(*it).second.m_wipOnline = (*it).second.CheckOnline();
				if ((*it).second.m_OnlineUrl == FALSE)
				{
					(*it).second.m_wipOnlineUrl = (*it).second.UpdateUrl();
			
					if ((*it).second.m_wipOnlineUrl == FALSE)
					{
						(*it).second.m_wipOnline = FALSE;
					}
					
				}
			}
		}
		{
			/* Loop all the cameraparam result and set to camera */
			CameraParamMap::iterator it = paramMap.begin(); 
			for(; it!=paramMap.end(); ++it)
			{	
				/* Loop to check the camera and update the url */
				s32 nIndex = (*it).first;
				Lock();
				CameraMap::iterator it1 = m_CameraMap.find(nIndex), 
							ite1 = m_CameraMap.end();

				if (it1 == ite1) 
				{
					/* the id may be delete */
					UnLock();
					continue;
				}

				CameraStatus bCheck = m_CameraMap[nIndex]->CheckCamera(
					(*it).second.m_strUrl, (*it).second.m_strUrlSubStream, 
					(*it).second.m_bHasSubStream, 
					(*it).second.m_wipOnline, (*it).second.m_wipOnlineUrl);
				
				StorFactoryCameraChangeData change;
				change.id = nIndex;
				switch (bCheck)
				{
					case DEV_OFF2ON:
					{
						change.type = STOR_FACTORY_CAMERA_ONLINE;
						m_CameraOnlineMap[nIndex] = 1;
						UnLock(); 
						CallCameraChange(change);
						Lock();
						break;
					}
					case DEV_ON2OFF:
					{
						change.type = STOR_FACTORY_CAMERA_OFFLINE;
						m_CameraOnlineMap[nIndex] = 0;
						UnLock(); 
						CallCameraChange(change);
						Lock();
						break;
					}
					default:
					{

						break;
					}
				}
				UnLock();
			}
		}
		ve_sleep(1000 * 90);
	}
#endif
	
}




#endif // __VSC_STOR_FACTORY_H_
