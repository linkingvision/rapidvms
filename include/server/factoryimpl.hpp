//------------------------------------------------------------------------------
// File: factory impl.hpp
//
// Desc: Camera factory - Manage IP Camera.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_FACTORY_IMPL_H_
#define __VSC_FACTORY_IMPL_H_


inline FactoryHddTask::FactoryHddTask(Factory &pFactory)
:m_Factory(pFactory)
{
}

inline FactoryHddTask::~FactoryHddTask()
{

}

inline   void FactoryHddTask::run()
{
	VDBDiskMap mapDisk;
	VDBDiskStatus mapStatus;
	while(1)
	{
		mapDisk.clear();
		mapStatus.clear();
		/* Get Diskmap */
		m_Factory.GetDiskMap(mapDisk);
	  	/* loop to find a good disk */
		for (VDBDiskMap::iterator it = mapDisk.begin(); it != mapDisk.end(); it++)
		{
			QString path((*it).second.path.c_str());
			QStorageInfo info(path);
			VdbDiskStatus status;
			status.status = HDD_DISK_ERROR;
			status.hdd = (*it).second.hdd;
			status.path = (*it).second.path;
			if (info.isValid() == true && info.isReady() == true)
			{
				status.status = HDD_DISK_OK;
			}
			if (info.isReadOnly() == true)
			{
				status.status = HDD_DISK_READ_ONLY;
			}
			status.freeSize = info.bytesAvailable()/1024;
			status.totalSize = info.bytesTotal()/1024;
			mapStatus[(*it).second.hdd] = status;
			//VDC_DEBUG("HDD %s path %s freeSize %lld\n", ((*it).first).c_str(), 
			//				(*it).second.path.c_str(), status.freeSize);
			
		}
		m_Factory.UpdateDiskStatusMap(mapStatus);
#ifdef WIN32
	        Sleep(1000 * 2);
#else
		sleep(2);
#endif
	}
}

inline void OnvifLog(char * str)
{
    VDC_DEBUG( "%s\n", str);
    return;
}



inline Factory::Factory()
{
#ifdef WIN32
#ifndef _WIN64
    astring strSysPath = "C:\\vidstor\\system";
#else
    astring strSysPath = "C:\\vidstor64\\system";
#endif
#else
    astring strSysPath = "ve/vidstor/system/";
#endif
    m_SysPath.Open(strSysPath);
}

inline Factory::~Factory()
{

}

inline BOOL Factory::SetSystemPath(astring &strPath)
{
    return m_SysPath.SetSystemPath(strPath);
}

inline BOOL Factory::GetExportPath(astring &strPath)
{
	astring strKey = "ConfVideoExportKey";

	astring strSysPath;
	if (m_SysPath.GetSystemPath(strSysPath) == FALSE)
	{
	    return FALSE;
	}

#ifdef WIN32
#ifndef _WIN64
	astring strPathDefault = strSysPath + "vidstor\\export\\";
#else
	astring strPathDefault = strSysPath + "vidstor64\\export\\";
#endif
#else
	astring strPathDefault = strSysPath + "vidstor/export/";
#endif

	if (m_Conf.GetCmnParam(strKey, strPath) == FALSE)
	{
		strPath = strPathDefault;
		m_Conf.SetCmnParam(strKey, strPath);
		Poco::File file1(strPath);
		file1.createDirectories();
		
	}
	return TRUE;
}
inline BOOL Factory::SetExportPath(astring &strPath)
{
	astring strKey = "ConfVideoExportKey";
	m_Conf.SetCmnParam(strKey, strPath);

	return TRUE;
}

inline BOOL Factory::Init()
{
	astring strPath;
	if (m_SysPath.GetSystemPath(strPath) == FALSE)
	{
	    return FALSE;
	}
	printf("Sys path %s\n", strPath.c_str());
#ifdef WIN32
#ifndef _WIN64
	astring strPathConf = strPath + "vidstor\\config";
#else
	astring strPathConf = strPath + "vidstor64\\config";
#endif
#else
	astring strPathConf = strPath + "vidstor/config";
#endif
	m_Conf.Open(strPathConf);

#ifndef _WIN64
	astring strPathDb = strPath + "vidstor";
#else
	astring strPathDb = strPath + "vidstor64";
#endif
	m_pVdb = new VDB(strPathDb);

	VSCHdfsRecordData HdfsConf;
	m_Conf.GetHdfsRecordConf(HdfsConf);
	astring strNameNode = HdfsConf.data.conf.NameNode;
	astring strPort = HdfsConf.data.conf.Port;
	astring strUser = HdfsConf.data.conf.User;
	m_pVHdfsdb = new VHdfsDB(strNameNode, strPort, strUser);

	VSCConfData sysData;
	m_Conf.GetSysData(sysData);
	for (s32 i = 1; i < CONF_MAP_MAX; i ++)
	{
	    if (sysData.data.conf.CameraMap[i] != CONF_MAP_INVALID_MIN 
			&& sysData.data.conf.CameraMap[i] != 0)
	    {
	        VDC_DEBUG( "%s Init Camera %d\n",__FUNCTION__, i);
	        VSCCameraData Data;
	        m_Conf.GetCameraData(i, Data);
	        CameraParam mParam(Data);
	        LockCameraID(Data.data.conf.nId);
	        InitAddCamera(mParam, Data.data.conf.nId);
	        VDC_DEBUG( "%s Id %d\n",__FUNCTION__, Data.data.conf.nId);
	    }
	}

	InitLicense();
	//m_pThread = new thread(Factory::Run, (void *)this);
	//start();
	m_HddTask = new FactoryHddTask(*this);
	m_HddTask->start();

	if (sysData.data.conf.OAPIPort == 0)
	{
		sysData.data.conf.OAPIPort = 9080;
	}
	//m_pHttpServer = new CmnHttpServer(sysData.data.conf.OAPIPort);
	//m_pHttpServer->start();

	/* Init export path */
	astring strExportPath;
	GetExportPath(strExportPath);
	return TRUE;
}

inline BOOL Factory::RegCameraChangeNotify(void * pData, FactoryCameraChangeNotify callback)
{
	Lock();
	m_CameraChange[pData] = callback;
	UnLock();
	return TRUE;
}
inline BOOL Factory::CallCameraChange(FactoryCameraChangeData data)
{
        CameraChangeNofityMap::iterator it = m_CameraChange.begin(); 
        for(; it!=m_CameraChange.end(); ++it)
        {
		if ((*it).second)
		{
			(*it).second((*it).first, data);
		}
        }	
	 return TRUE;
}
inline BOOL Factory::GetLicense(astring &strLicense, astring &strHostId, int &ch, astring &type)
{
	VPlay::GetLicenseInfo(strHostId, ch, type);
	return m_Conf.GetLicense(strLicense);
}
inline BOOL Factory::SetLicense(astring &strLicense)
{
	VPlay::SetLicense(strLicense);
	return m_Conf.SetLicense(strLicense);
}

inline BOOL Factory::AuthUser(astring &strUser, astring &strPasswd)
{
	VSCUserData pData;
	BOOL ret = FALSE;
	
	Lock();
	m_Conf.GetUserData(pData);
	if (strUser == "admin")
	{
		astring realPasswd = pData.data.conf.Passwd;
		if (realPasswd == strPasswd)
		{
			ret = TRUE;
		}
	}else
	{
		//TODO add support other users
	}
	
	
	UnLock();
	return ret;
}

inline BOOL Factory::GetUserData(VSCUserData &pData)
{
	Lock();
	m_Conf.GetUserData(pData);	
	UnLock();
	return TRUE;
}
inline BOOL Factory::SetUserData(VSCUserData &pData)
{
	Lock();
	m_Conf.UpdateUserData(pData);	
	UnLock();
	return TRUE;
}

inline BOOL Factory::GetHdfsRecordConf(VSCHdfsRecordData &pData)
{
	Lock();
	BOOL ret = m_Conf.GetHdfsRecordData(pData);
	UnLock();
	return ret;
}
inline BOOL Factory::SetHdfsRecordConf(VSCHdfsRecordData &pData)
{
	Lock();
	BOOL ret = m_Conf.UpdateHdfsRecordData(pData);
	UnLock();
	return ret;
}

inline BOOL Factory::GetOAPIPort(u16 &pPort)
{	
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	pPort = sys.data.conf.OAPIPort;
	UnLock();
	return TRUE;
}
inline BOOL Factory::SetOAPIPort(u16 &pPort)
{
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	sys.data.conf.OAPIPort = pPort;
	m_Conf.UpdateSysData(sys);
	UnLock();
	return TRUE;
}

inline BOOL Factory::InitLicense()
{
	astring strLicense;
	m_Conf.GetLicense(strLicense);
	VPlay::SetLicense(strLicense);
	return TRUE;
}

inline BOOL Factory::AddHdd(astring &strHdd, astring & strPath, s64 nSize)
{
    //astring strPathHdd = strPath + "videodb";
    return m_pVdb->AddHdd(strHdd, strPath, nSize);
}

inline BOOL Factory::DelHdd(astring & strHdd)
{
	return m_pVdb->DelHdd(strHdd);
}

inline BOOL Factory::HddUpdateSize(astring &strHdd, s64 nSize)
{
	return m_pVdb->HddUpdateSize(strHdd, nSize);
}

inline BOOL Factory::GetDiskMap(VDBDiskMap &pMap)
{
	return m_pVdb->GetDiskMap(pMap);
}

inline BOOL Factory::GetDiskStatus(VDBDiskStatus &pStatus)
{
	return m_pVdb->GetDiskStatus(pStatus);
}

inline BOOL Factory::UpdateDiskStatusMap(VDBDiskStatus &pStatus)
{
	return m_pVdb->UpdateDiskStatusMap(pStatus);
}


inline BOOL Factory::SearchItems(s32 cameraId, u32 startTime, u32 endTime, u32 recordType, 
				RecordItemMap &map)
{
    return m_pVdb->SearchItems(cameraId, startTime, endTime, recordType, 
                        map);
}

inline BOOL Factory::SearchHasItems(s32 cameraId, u32 startTime, u32 endTime, u32 recordType)
{
    return m_pVdb->SearchHasItems(cameraId, startTime, endTime, recordType);
}
inline BOOL Factory::SearchAItem(s32 cameraId, u32 Time, VdbRecordItem &pItem)
{
    return m_pVdb->SearchAItem(cameraId, Time, pItem);
}

inline BOOL Factory::SearchAItemNear(s32 cameraId, u32 Time, VdbRecordItem &pItem)
{
    return m_pVdb->SearchAItemNear(cameraId, Time, pItem);
}
inline BOOL Factory::SearchNextItem(s32 cameraId, s64 LastId, VdbRecordItem &pItem)
{
    return m_pVdb->SearchNextItem(cameraId, LastId, pItem);
}
inline BOOL Factory::RequestAMFRead(VdbRecordItem &pItem, astring & strPath)
{
    return m_pVdb->RequestAMFRead(pItem, strPath);
}
inline BOOL Factory::FinishedAMFRead(VdbRecordItem &pItem, astring & strPath)
{
    return m_pVdb->FinishedAMFRead(pItem, strPath);
}

inline VDB& Factory::GetVdb()
{
	return *m_pVdb;
}

inline BOOL Factory::GetCameraParamMap(CameraParamMap &pMap)
{
    pMap = m_CameraParamMap;

    return TRUE;
}

inline BOOL Factory::GetCameraOnlineMap(CameraOnlineMap &pMap)
{
    pMap = m_CameraOnlineMap;

    return TRUE;
}

inline s32 Factory::InitAddCamera(CameraParam & pParam, astring strCamId)
{

    m_CameraMap[strCamId] = new Camera(*m_pVdb, *m_pVHdfsdb, pParam);

    m_CameraParamMap[strCamId] = pParam;
    m_CameraOnlineMap[strCamId] = FALSE;

    return TRUE;
}

inline BOOL Factory::RegDataCallback(s32 nIndex, CameraDataCallbackFunctionPtr pCallback,
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

inline BOOL Factory::UnRegDataCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegDataCallback(pParam);
    }

    UnLock();

    return TRUE;
}


inline BOOL Factory::GetInfoFrame(s32 nIndex, InfoFrame &pFrame)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->GetInfoFrame(pFrame);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::RegSubDataCallback(s32 nIndex, CameraDataCallbackFunctionPtr pCallback,
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

inline BOOL Factory::UnRegSubDataCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegDataCallback(pParam);
    }

    UnLock();

    return TRUE;
}


inline BOOL Factory::GetSubInfoFrame(s32 nIndex, InfoFrame &pFrame)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->GetSubInfoFrame(pFrame);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::RegRawCallback(s32 nIndex, CameraRawCallbackFunctionPtr pCallback,
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

inline BOOL Factory::UnRegRawCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegRawCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::RegSeqCallback(s32 nIndex, CameraSeqCallbackFunctionPtr pCallback,
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

inline BOOL Factory::UnRegSeqCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegSeqCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::RegSubRawCallback(s32 nIndex, CameraRawCallbackFunctionPtr pCallback,
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

inline BOOL Factory::UnRegSubRawCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegSubRawCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::RegDelCallback(s32 nIndex, CameraDelCallbackFunctionPtr pCallback, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->RegDelCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}
inline BOOL Factory::UnRegDelCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegDelCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::GetCameraOnline(s32 nIndex, BOOL &bStatus)
{
    Lock();

    if (m_CameraMap[nIndex] != NULL)
    {
        bStatus =  m_CameraMap[nIndex]->GetCameraOnline();
    }

    UnLock();

    return TRUE;
}

inline   BOOL Factory::GetUrl(s32 nIndex, std::string &url)
{
    BOOL ret = FALSE;

    if (m_CameraMap[nIndex] != NULL)
    {
        ret =  m_CameraMap[nIndex]->GetUrl(url);
    }

    UnLock();

    return ret;
}

inline BOOL Factory::GetStreamInfo(s32 nIndex, VideoStreamInfo &pInfo)
{
	Lock();
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->GetStreamInfo(pInfo);
	}
	UnLock();

	return TRUE;
}

inline BOOL Factory::PtzAction(s32 nIndex, FPtzAction action, float speed)
{
	Lock();
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->PtzAction(action, speed);
	}
	UnLock();

	return TRUE;
}


inline BOOL Factory::GetRecordStatus(s32 nIndex,BOOL &nStatus)
{
    CameraParam pParam;
    if (nIndex <=0 || nIndex >= FACTORY_CAMERA_ID_MAX)
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

inline BOOL Factory::StartRecord(s32 nIndex)
{
    CameraParam pParam;
    FactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= FACTORY_CAMERA_ID_MAX)
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
    change.type = FACTORY_CAMERA_RECORDING_ON;
    CallCameraChange(change);
    return TRUE;
}
inline BOOL Factory::StopRecord(s32 nIndex)
{
    CameraParam pParam;
    FactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= FACTORY_CAMERA_ID_MAX)
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
    change.type = FACTORY_CAMERA_RECORDING_OFF;
    CallCameraChange(change);
    return TRUE;
}


inline BOOL Factory::StartHdfsRecord(s32 nIndex)
{
    CameraParam pParam;
    FactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= FACTORY_CAMERA_ID_MAX)
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
    change.type = FACTORY_CAMERA_RECORDING_ON;
    CallCameraChange(change);
    return TRUE;
}
inline BOOL Factory::StopHdfsRecord(s32 nIndex)
{
    CameraParam pParam;
    FactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= FACTORY_CAMERA_ID_MAX)
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
    change.type = FACTORY_CAMERA_RECORDING_OFF;
    CallCameraChange(change);
    return TRUE;
}

inline BOOL Factory::StartRecordAll()
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
inline BOOL Factory::StopRecordAll()
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
inline BOOL Factory::StartHdfsRecordAll()
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
inline BOOL Factory::StopHdfsRecordAll()
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

inline s32 Factory::AddCamera(CameraParam & pParam)
{
	s32 nId = GetCameraID();
	FactoryCameraChangeData change;
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
	change.type = FACTORY_CAMERA_ADD;
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

	FactoryCameraChangeData change2;
	change2.id = nId;
	
	if (bCheck == DEV_OFF2ON)
	{
		change.type = FACTORY_CAMERA_ONLINE;
		m_CameraOnlineMap[nId] = 1;
		UnLock(); 
		CallCameraChange(change);
		Lock();
	}
	UnLock();
#endif
	
    	return nId;
}

inline BOOL Factory::DelCamera(s32 nIndex)
{
    FactoryCameraChangeData change;
    VDC_DEBUG( "%s DelCamera %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    change.id = nIndex;
    change.type = FACTORY_CAMERA_DEL;
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

inline BOOL Factory::GetCameraRtspUrl(astring & strUrl, s32 nIndex)
{
	BOOL ret = FALSE;
	Lock();
	if (m_CameraMap[nIndex] != NULL)
	{
	    ret = m_CameraMap[nIndex]->GetCameraRtspUrl(strUrl);
	}
	UnLock();

	return ret;
}

inline s32 Factory::GetCameraParamById(CameraParam & pParam, s32 nIndex)
{
    //VDC_DEBUG( "%s GetCameraParamById %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }

    Lock();
    pParam = m_CameraParamMap[nIndex];
    UnLock();

    return TRUE;
}

inline s32 Factory::GetCameraParamByIdTryLock(CameraParam & pParam, s32 nIndex)
{
    //VDC_DEBUG( "%s GetCameraParamById %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= FACTORY_CAMERA_ID_MAX)
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

inline s32 Factory::GetCameraID(void)
{
    s32 id  = -1;
    s32 i = -1;

    Lock();
    for (i = 1; i < FACTORY_CAMERA_ID_MAX; i ++)
    {
        if (m_strCameraMap[i] == 'n')
        {
            id = i;
            m_strCameraMap[i] = 'y';
            UnLock();
            return id;
        }
    }

    UnLock();
    return id;
}

inline BOOL Factory::ReleaseCameraID(s32 nID)
{
    if (nID <=0 || nID >= FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    Lock();
    m_strCameraMap[nID] = 'n';
    UnLock();

    return TRUE;
}

inline BOOL Factory::LockCameraID(s32 nID)
{
    if (nID <=0 || nID >= FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    Lock();
    m_strCameraMap[nID] = 'y';
    UnLock();

    return TRUE;
}

inline s32 Factory::GetVIPCID(void)
{
    s32 id  = -1;
    s32 i = -1;

    Lock();
    for (i = 1; i < FACTORY_CAMERA_ID_MAX; i ++)
    {
        if (m_strVIPCMap[i] == 'n')
        {
            id = i;
            m_strVIPCMap[i] = 'y';
            UnLock();
            return id;
        }
    }

    UnLock();
    return id;
}

inline BOOL Factory::ReleaseVIPCID(s32 nID)
{
    if (nID <=0 || nID >= FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    Lock();
    m_strVIPCMap[nID] = 'n';
    UnLock();

    return TRUE;
}

inline BOOL Factory::LockVIPCID(s32 nID)
{
    if (nID <=0 || nID >= FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    Lock();
    m_strVIPCMap[nID] = 'y';
    UnLock();

    return TRUE;
}


inline void Factory::Run(void * pParam)
{
    int dummy = errno;
    LPFactory pThread = (LPFactory)pParam;

    if (pThread)
    {
        //pThread->Run1();
    }
}

inline void Factory::run()
{
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
				
				FactoryCameraChangeData change;
				change.id = nIndex;
				switch (bCheck)
				{
					case DEV_OFF2ON:
					{
						change.type = FACTORY_CAMERA_ONLINE;
						m_CameraOnlineMap[nIndex] = 1;
						UnLock(); 
						CallCameraChange(change);
						Lock();
						break;
					}
					case DEV_ON2OFF:
					{
						change.type = FACTORY_CAMERA_OFFLINE;
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
	
}




#endif // __VSC_FACTORY_H_
