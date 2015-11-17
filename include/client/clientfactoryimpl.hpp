//------------------------------------------------------------------------------
// File: clientfactory impl.hpp
//
// Desc: Camera clientfactory - Manage IP Camera.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_CLIENT_FACTORY_IMPL_H_
#define __VSC_CLIENT_FACTORY_IMPL_H_


inline ClientFactoryHddTask::ClientFactoryHddTask(ClientFactory &pClientFactory)
:m_ClientFactory(pClientFactory)
{
}

inline ClientFactoryHddTask::~ClientFactoryHddTask()
{

}

inline   void ClientFactoryHddTask::run()
{
	VDBDiskMap mapDisk;
	VDBDiskStatus mapStatus;
	while(1)
	{
		mapDisk.clear();
		mapStatus.clear();
		/* Get Diskmap */
		m_ClientFactory.GetDiskMap(mapDisk);
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
		m_ClientFactory.UpdateDiskStatusMap(mapStatus);
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



inline ClientFactory::ClientFactory()
{
#ifdef WIN32
#ifndef _WIN64
    astring strSysPath = "C:\\videodb\\system";
#else
    astring strSysPath = "C:\\videodb64\\system";
#endif
#else
    astring strSysPath = "ve/videodb/system/";
#endif
    m_SysPath.Open(strSysPath);
}

inline ClientFactory::~ClientFactory()
{

}

inline BOOL ClientFactory::SetSystemPath(astring &strPath)
{
    return m_SysPath.SetSystemPath(strPath);
}

inline BOOL ClientFactory::GetExportPath(astring &strPath)
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
inline BOOL ClientFactory::SetExportPath(astring &strPath)
{
	astring strKey = "ConfVideoExportKey";
	m_Conf.SetCmnParam(strKey, strPath);

	return TRUE;
}

inline BOOL ClientFactory::Init()
{

	s32 i = 0;

	for (i = 0; i < CLIENT_FACTORY_CAMERA_ID_MAX; i ++)
	{
	    m_strCameraMap[i] = 'n';
	}
	for (i = 0; i < CLIENT_FACTORY_CAMERA_ID_MAX; i ++)
	{
	    m_strVIPCMap[i] = 'n';
	}



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

	for (s32 i = 1; i < CONF_MAP_MAX; i ++)
	{
	    if (sysData.data.conf.VIPCMap[i] != CONF_MAP_INVALID_MIN 
			&& sysData.data.conf.VIPCMap[i] != 0)
	    {
	        VDC_DEBUG( "%s Init VIPC %d\n",__FUNCTION__, i);
	        VSCVIPCData Data;
	        m_Conf.GetVIPCData(i, Data);
	        VIPCCameraParam mParam(Data);
	        LockVIPCID(Data.data.conf.nId);
	        m_VIPCCameraParamMap[i] = mParam;
	        VDC_DEBUG( "%s Id %d\n",__FUNCTION__, Data.data.conf.nId);
	    }
	}

	InitLicense();
	//m_pThread = new thread(ClientFactory::Run, (void *)this);
	//start();
	m_HddTask = new ClientFactoryHddTask(*this);
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

inline BOOL ClientFactory::RegCameraChangeNotify(void * pData, ClientFactoryCameraChangeNotify callback)
{
	Lock();
	m_CameraChange[pData] = callback;
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::CallCameraChange(ClientFactoryCameraChangeData data)
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
inline BOOL ClientFactory::GetLicense(astring &strLicense, astring &strHostId, int &ch, astring &type)
{
	VPlay::GetLicenseInfo(strHostId, ch, type);
	return m_Conf.GetLicense(strLicense);
}
inline BOOL ClientFactory::SetLicense(astring &strLicense)
{
	VPlay::SetLicense(strLicense);
	return m_Conf.SetLicense(strLicense);
}

/* Get if auto login the opencvr */
inline BOOL ClientFactory::GetAutoLogin()
{
	VSCUserData pData;

	Lock();
	BOOL ret = FALSE;
	m_Conf.GetUserData(pData);
	if (pData.data.conf.AutoLogin == 1)
	{
		ret = TRUE;
	}
	
	UnLock();
	return ret;
}
inline BOOL ClientFactory::AuthUser(astring &strUser, astring &strPasswd)
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

inline BOOL ClientFactory::GetUserData(VSCUserData &pData)
{
	Lock();
	m_Conf.GetUserData(pData);	
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::SetUserData(VSCUserData &pData)
{
	Lock();
	m_Conf.UpdateUserData(pData);	
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetEmapData(VSCEmapData &pData)
{
	Lock();
	m_Conf.GetEmapData(pData);	
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::SetEmapData(VSCEmapData &pData)
{
	Lock();
	m_Conf.UpdateEmapData(pData);	
	UnLock();
	return TRUE;
}

/* (x, y) in the image, w h is the image width height */
inline BOOL ClientFactory::AddEmapCamera(s32 nIndex, u32 x, u32 y, u32 w, u32 h)
{
	VSCEmapData pData;
	Lock();
	m_Conf.GetEmapData(pData);
	if (nIndex < CONF_MAP_MAX && nIndex > 0)
	{
		pData.data.conf.emap[nIndex].x = x;
		pData.data.conf.emap[nIndex].y = y;

		pData.data.conf.emap[nIndex].w = w;
		pData.data.conf.emap[nIndex].h = h;
		
		pData.data.conf.emap[nIndex].Used = 1;
		pData.data.conf.emap[nIndex].nId = nIndex;
	}else
	{
		UnLock();
		return FALSE;
	}
	
	m_Conf.UpdateEmapData(pData);	
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::DelEmapCamera(s32 nIndex)
{
	VSCEmapData pData;
	Lock();
	m_Conf.GetEmapData(pData);

	if (nIndex < CONF_MAP_MAX && nIndex > 0)
	{
		pData.data.conf.emap[nIndex].Used = 0;
	}else
	{
		UnLock();
		return FALSE;
	}
	m_Conf.UpdateEmapData(pData);	
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::GetEmapCamera(s32 nIndex, u32 &x, u32 &y, u32 &w, u32 &h)
{
	VSCEmapData pData;
	Lock();
	m_Conf.GetEmapData(pData);

	if (nIndex < CONF_MAP_MAX && nIndex > 0)
	{
		x = pData.data.conf.emap[nIndex].x;
		y = pData.data.conf.emap[nIndex].y;
		w = pData.data.conf.emap[nIndex].w;
		h = pData.data.conf.emap[nIndex].h;
	}else
	{
		UnLock();
		return FALSE;
	}
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetEmapFile(astring &strFile)
{	
	BOOL ret = FALSE;
	VSCEmapData pData;
	Lock();
	m_Conf.GetEmapData(pData);
	if (pData.data.conf.init == 1)
	{
		ret = m_Conf.GetEmapFile(strFile);	
	}
	UnLock();
	return ret;
}
inline BOOL ClientFactory::SetEmapFile(astring &strFile)
{
	VSCEmapData pData;
	Lock();
	m_Conf.SetEmapFile(strFile);
	m_Conf.GetEmapData(pData);
	pData.data.conf.init = 1;
	m_Conf.UpdateEmapData(pData);
	
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetHdfsRecordConf(VSCHdfsRecordData &pData)
{
	Lock();
	BOOL ret = m_Conf.GetHdfsRecordData(pData);
	UnLock();
	return ret;
}
inline BOOL ClientFactory::SetHdfsRecordConf(VSCHdfsRecordData &pData)
{
	Lock();
	BOOL ret = m_Conf.UpdateHdfsRecordData(pData);
	UnLock();
	return ret;
}

inline BOOL ClientFactory::GetLang(VSCLangType &pLang)
{	
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	pLang = (VSCLangType)sys.data.conf.Language;
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::SetLang(VSCLangType &pLang)
{
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	sys.data.conf.Language = (u32)pLang;
	m_Conf.UpdateSysData(sys);
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetOAPIPort(u16 &pPort)
{	
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	pPort = sys.data.conf.OAPIPort;
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::SetOAPIPort(u16 &pPort)
{
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	sys.data.conf.OAPIPort = pPort;
	m_Conf.UpdateSysData(sys);
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetVHTTPSPort(u16 &pPort)
{	
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	pPort = sys.data.conf.VHTTPServerPort;
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::SetVHTTPSPort(u16 &pPort)
{
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	sys.data.conf.VHTTPServerPort = pPort;
	m_Conf.UpdateSysData(sys);
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetVHTTPSSSLPort(u16 &pPort)
{	
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	pPort = sys.data.conf.VHTTPSSLServerPort;
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::SetVHTTPSSSLPort(u16 &pPort)
{
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	sys.data.conf.VHTTPSSLServerPort = pPort;
	m_Conf.UpdateSysData(sys);
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetVHLSSPort(u16 &pPort)
{	
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	pPort = sys.data.conf.VHLSServerPort;
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::SetVHLSSPort(u16 &pPort)
{
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	sys.data.conf.VHLSServerPort = pPort;
	m_Conf.UpdateSysData(sys);
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetDefaultHWAccel(BOOL &pHWAccel)
{	
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	if (sys.data.conf.HWAccel == 1)
	{
		pHWAccel = TRUE;
	}else
	{
		pHWAccel = FALSE;
	}
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::SetDefaultHWAccel(BOOL &pHWAccel)
{
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	if (pHWAccel == 1)
	{
		sys.data.conf.HWAccel = 1;
	}else
	{
		sys.data.conf.HWAccel = 0;
	}
	m_Conf.UpdateSysData(sys);
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetDefaultMining(BOOL &pMining)
{	
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	if (sys.data.conf.Mining == 1)
	{
		pMining = TRUE;
	}else
	{
		pMining = FALSE;
	}
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::SetDefaultMining(BOOL &pMining)
{
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	if (pMining == 1)
	{
		sys.data.conf.Mining = 1;
	}else
	{
		sys.data.conf.Mining = 0;
	}
	m_Conf.UpdateSysData(sys);
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetRTSPServer(BOOL &pAuth, astring &pMultiAddr, u16 &pPort)
{	
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	if (sys.data.conf.RTSPAuth == 1)
	{
		pAuth = TRUE;
	}else
	{
		pAuth = FALSE;
	}
	pMultiAddr = sys.data.conf.MulticastStartIPV4;
	pPort = sys.data.conf.RTSPServerPort;
	UnLock();
	return TRUE;
}
inline BOOL ClientFactory::SetRTSPServer(BOOL &pAuth, astring &pMultiAddr, u16 &pPort)
{
	VSCConfData sys;
	Lock();
	m_Conf.GetSysData(sys);
	if (pAuth == 1)
	{
		sys.data.conf.RTSPAuth = 1;
	}else
	{
		sys.data.conf.RTSPAuth = 0;
	}
	if (pMultiAddr.length() < VSC_CONF_PARAM_MAX)
	{
		strcpy(sys.data.conf.MulticastStartIPV4, pMultiAddr.c_str());
	}

	sys.data.conf.RTSPServerPort = pPort;
	m_Conf.UpdateSysData(sys);
	UnLock();
	return TRUE;
}


inline BOOL ClientFactory::InitLicense()
{
	astring strLicense;
	m_Conf.GetLicense(strLicense);
	VPlay::SetLicense(strLicense);
	return TRUE;
}

inline BOOL ClientFactory::AddHdd(astring &strHdd, astring & strPath, s64 nSize)
{
    //astring strPathHdd = strPath + "videodb";
    return m_pVdb->AddHdd(strHdd, strPath, nSize);
}

inline BOOL ClientFactory::DelHdd(astring & strHdd)
{
	return m_pVdb->DelHdd(strHdd);
}

inline BOOL ClientFactory::HddUpdateSize(astring &strHdd, s64 nSize)
{
	return m_pVdb->HddUpdateSize(strHdd, nSize);
}

inline BOOL ClientFactory::GetDiskMap(VDBDiskMap &pMap)
{
	return m_pVdb->GetDiskMap(pMap);
}

inline BOOL ClientFactory::GetDiskStatus(VDBDiskStatus &pStatus)
{
	return m_pVdb->GetDiskStatus(pStatus);
}

inline BOOL ClientFactory::UpdateDiskStatusMap(VDBDiskStatus &pStatus)
{
	return m_pVdb->UpdateDiskStatusMap(pStatus);
}


inline BOOL ClientFactory::SearchItems(s32 cameraId, u32 startTime, u32 endTime, u32 recordType, 
				RecordItemMap &map)
{
    return m_pVdb->SearchItems(cameraId, startTime, endTime, recordType, 
                        map);
}

inline BOOL ClientFactory::SearchHasItems(s32 cameraId, u32 startTime, u32 endTime, u32 recordType)
{
    return m_pVdb->SearchHasItems(cameraId, startTime, endTime, recordType);
}
inline BOOL ClientFactory::SearchAItem(s32 cameraId, u32 Time, VdbRecordItem &pItem)
{
    return m_pVdb->SearchAItem(cameraId, Time, pItem);
}

inline BOOL ClientFactory::SearchAItemNear(s32 cameraId, u32 Time, VdbRecordItem &pItem)
{
    return m_pVdb->SearchAItemNear(cameraId, Time, pItem);
}
inline BOOL ClientFactory::SearchNextItem(s32 cameraId, s64 LastId, VdbRecordItem &pItem)
{
    return m_pVdb->SearchNextItem(cameraId, LastId, pItem);
}
inline BOOL ClientFactory::RequestAMFRead(VdbRecordItem &pItem, astring & strPath)
{
    return m_pVdb->RequestAMFRead(pItem, strPath);
}
inline BOOL ClientFactory::FinishedAMFRead(VdbRecordItem &pItem, astring & strPath)
{
    return m_pVdb->FinishedAMFRead(pItem, strPath);
}

inline VDB& ClientFactory::GetVdb()
{
	return *m_pVdb;
}

inline BOOL ClientFactory::GetCameraParamMap(CameraParamMap &pMap)
{
    pMap = m_CameraParamMap;

    return TRUE;
}

inline BOOL ClientFactory::GetCameraOnlineMap(CameraOnlineMap &pMap)
{
    pMap = m_CameraOnlineMap;

    return TRUE;
}

inline BOOL ClientFactory::GetVIPCCameraParamMap(VIPCCameraParamMap &pMap)
{
    pMap = m_VIPCCameraParamMap;

    return TRUE;
}

inline s32 ClientFactory::InitAddCamera(CameraParam & pParam, u32 nIndex)
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

inline BOOL ClientFactory::RegDataCallback(s32 nIndex, CameraDataCallbackFunctionPtr pCallback,
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

inline BOOL ClientFactory::UnRegDataCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegDataCallback(pParam);
    }

    UnLock();

    return TRUE;
}


inline BOOL ClientFactory::GetInfoFrame(s32 nIndex, InfoFrame &pFrame)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->GetInfoFrame(pFrame);
    }

    UnLock();

    return TRUE;
}

inline BOOL ClientFactory::RegSubDataCallback(s32 nIndex, CameraDataCallbackFunctionPtr pCallback,
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

inline BOOL ClientFactory::UnRegSubDataCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegDataCallback(pParam);
    }

    UnLock();

    return TRUE;
}


inline BOOL ClientFactory::GetSubInfoFrame(s32 nIndex, InfoFrame &pFrame)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->GetSubInfoFrame(pFrame);
    }

    UnLock();

    return TRUE;
}

inline BOOL ClientFactory::RegRawCallback(s32 nIndex, CameraRawCallbackFunctionPtr pCallback,
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

inline BOOL ClientFactory::UnRegRawCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegRawCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL ClientFactory::RegSeqCallback(s32 nIndex, CameraSeqCallbackFunctionPtr pCallback,
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

inline BOOL ClientFactory::UnRegSeqCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegSeqCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL ClientFactory::RegSubRawCallback(s32 nIndex, CameraRawCallbackFunctionPtr pCallback,
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

inline BOOL ClientFactory::UnRegSubRawCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegSubRawCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL ClientFactory::RegDelCallback(s32 nIndex, CameraDelCallbackFunctionPtr pCallback, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->RegDelCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}
inline BOOL ClientFactory::UnRegDelCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UnRegDelCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL ClientFactory::GetCameraOnline(s32 nIndex, BOOL &bStatus)
{
    Lock();

    if (m_CameraMap[nIndex] != NULL)
    {
        bStatus =  m_CameraMap[nIndex]->GetCameraOnline();
    }

    UnLock();

    return TRUE;
}

inline   BOOL ClientFactory::GetUrl(s32 nIndex, std::string &url)
{
    BOOL ret = FALSE;

    if (m_CameraMap[nIndex] != NULL)
    {
        ret =  m_CameraMap[nIndex]->GetUrl(url);
    }

    UnLock();

    return ret;
}

inline BOOL ClientFactory::AttachPlayer(s32 nIndex, HWND hWnd, int w, int h, 
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

inline BOOL ClientFactory::GetStreamInfo(s32 nIndex, VideoStreamInfo &pInfo)
{
	Lock();
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->GetStreamInfo(pInfo);
	}
	UnLock();

	return TRUE;
}


inline BOOL ClientFactory::UpdateWidget(s32 nIndex, HWND hWnd, int w, int h)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->UpdateWidget(hWnd, w, h);
    }
    UnLock();

	return TRUE;
}

inline BOOL ClientFactory::DetachPlayer(s32 nIndex, HWND hWnd, void * pParam)
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

inline BOOL ClientFactory::EnablePtz(s32 nIndex, HWND hWnd, bool enable)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->EnablePtz(hWnd, enable);
    }
    UnLock();

	return TRUE;
}

inline BOOL ClientFactory::DrawPtzDirection(s32 nIndex, HWND hWnd, int x1, int y1, int x2,  int y2)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->DrawPtzDirection(hWnd, x1, y1, x2, y2);
    }
    UnLock();

	return TRUE;
}

inline BOOL ClientFactory::ShowAlarm(s32 nIndex, HWND hWnd)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->ShowAlarm(hWnd);
    }
    UnLock();

	return TRUE;
}

inline BOOL ClientFactory::ClearPtzDirection(s32 nIndex, HWND hWnd)
{
    Lock();
    if (m_CameraMap[nIndex] != NULL)
    {
        m_CameraMap[nIndex]->ClearPtzDirection(hWnd);
    }
    UnLock();

	return TRUE;
}

inline BOOL ClientFactory::PtzAction(s32 nIndex, FPtzAction action, float speed)
{
	Lock();
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->PtzAction(action, speed);
	}
	UnLock();

	return TRUE;
}


inline BOOL ClientFactory::GetRecordStatus(s32 nIndex,BOOL &nStatus)
{
    CameraParam pParam;
    if (nIndex <=0 || nIndex >= CLIENT_FACTORY_CAMERA_ID_MAX)
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

inline BOOL ClientFactory::StartRecord(s32 nIndex)
{
    CameraParam pParam;
    ClientFactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= CLIENT_FACTORY_CAMERA_ID_MAX)
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
    change.type = CLIENT_FACTORY_CAMERA_RECORDING_ON;
    CallCameraChange(change);
    return TRUE;
}
inline BOOL ClientFactory::StopRecord(s32 nIndex)
{
    CameraParam pParam;
    ClientFactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= CLIENT_FACTORY_CAMERA_ID_MAX)
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
    change.type = CLIENT_FACTORY_CAMERA_RECORDING_OFF;
    CallCameraChange(change);
    return TRUE;
}


inline BOOL ClientFactory::StartHdfsRecord(s32 nIndex)
{
    CameraParam pParam;
    ClientFactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= CLIENT_FACTORY_CAMERA_ID_MAX)
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
    change.type = CLIENT_FACTORY_CAMERA_RECORDING_ON;
    CallCameraChange(change);
    return TRUE;
}
inline BOOL ClientFactory::StopHdfsRecord(s32 nIndex)
{
    CameraParam pParam;
    ClientFactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= CLIENT_FACTORY_CAMERA_ID_MAX)
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
    change.type = CLIENT_FACTORY_CAMERA_RECORDING_OFF;
    CallCameraChange(change);
    return TRUE;
}

inline BOOL ClientFactory::StartRecordAll()
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
inline BOOL ClientFactory::StopRecordAll()
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
inline BOOL ClientFactory::StartHdfsRecordAll()
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
inline BOOL ClientFactory::StopHdfsRecordAll()
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

inline BOOL ClientFactory::UpdateCameraGroup(s32 nIndex, s32 nGroup)
{
    CameraParam pParam;
    ClientFactoryCameraChangeData change;
    if (nIndex <=0 || nIndex >= CLIENT_FACTORY_CAMERA_ID_MAX)
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
    change.type = CLIENT_FACTORY_CAMERA_GROUP_CHANGE;
    CallCameraChange(change);
    return TRUE;
}

inline s32 ClientFactory::AddCamera(CameraParam & pParam)
{
	s32 nId = GetCameraID();
	ClientFactoryCameraChangeData change;
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
	change.type = CLIENT_FACTORY_CAMERA_ADD;
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

	ClientFactoryCameraChangeData change2;
	change2.id = nId;
	
	if (bCheck == DEV_OFF2ON)
	{
		change.type = CLIENT_FACTORY_CAMERA_ONLINE;
		m_CameraOnlineMap[nId] = 1;
		UnLock(); 
		CallCameraChange(change);
		Lock();
	}
	UnLock();
#endif
	
    	return nId;
}

inline BOOL ClientFactory::DelCamera(s32 nIndex)
{
    ClientFactoryCameraChangeData change;
    VDC_DEBUG( "%s DelCamera %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= CLIENT_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    change.id = nIndex;
    change.type = CLIENT_FACTORY_CAMERA_DEL;
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

inline s32 ClientFactory::AddVIPC(VIPCCameraParam & pParam)
{
    s32 nId = GetVIPCID();
    ClientFactoryCameraChangeData change;

    VDC_DEBUG( "%s GetCameraID %d\n",__FUNCTION__, nId);

    Lock();
    pParam.m_Conf.data.conf.nId = nId;
    m_VIPCCameraParamMap[nId] = pParam;

    m_Conf.AddVIPC(pParam.m_Conf, nId);
    UnLock();
    change.id = nId;
    change.type = CLIENT_FACTORY_VIPC_ADD;
    CallCameraChange(change);
	
    return nId;
}


inline s32 ClientFactory::GetVIPCParamById(VIPCCameraParam & pParam, s32 nIndex)
{
    //VDC_DEBUG( "%s GetCameraParamById %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= CLIENT_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }

    Lock();
    pParam = m_VIPCCameraParamMap[nIndex];
    UnLock();

    return TRUE;
}

inline BOOL ClientFactory::DelVIPC(s32 nIndex)
{
    ClientFactoryCameraChangeData change;
    VDC_DEBUG( "%s DelCamera %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= CLIENT_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }

    change.id = nIndex;
    change.type = CLIENT_FACTORY_VIPC_DEL;
    CallCameraChange(change);
	
    Lock();
    m_VIPCCameraParamMap.erase(nIndex);
    m_Conf.DelVIPC(nIndex);
    ReleaseVIPCID(nIndex);
    UnLock();
	

    return TRUE;
}


inline BOOL ClientFactory::GetVms(VSCVmsData &pData)
{
	Lock();
	m_Conf.GetVmsData(pData);
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetVmsById(VSCVmsDataItem &pParam, int nId)
{
	VSCVmsData VmsData;
	int id = -1;
	Lock();
	m_Conf.GetVmsData(VmsData);
	if (nId < CONF_VMS_NUM_MAX && nId > 0)
	{
		if (VmsData.data.conf.vms[nId].Used == 1)
		{
			memcpy(&pParam, &(VmsData.data.conf.vms[nId]), sizeof(VSCVmsDataItem));
		}
	}else
	{
		UnLock();
		return FALSE;
	}
	UnLock();
	return TRUE;
	
}

inline s32 ClientFactory::AddVms(VSCVmsDataItem &pParam)
{
	VSCVmsData * pVmsData = new VSCVmsData;
	VSCVmsData &VmsData = *pVmsData;
	ClientFactoryCameraChangeData change;
	int id = -1;
	Lock();
	m_Conf.GetVmsData(VmsData);
	/* Just use 1 to CONF_VMS_NUM_MAX */
	for (s32 i = 1; i < CONF_VMS_NUM_MAX; i ++)
	{
	    	if (VmsData.data.conf.vms[i].Used == 1)
	    	{
	    		continue;
	    	}else
	    	{
	    		memcpy(&(VmsData.data.conf.vms[i]), &pParam, sizeof(VSCVmsDataItem));
			VmsData.data.conf.vms[i].Used = 1;
			VmsData.data.conf.vms[i].nId = i;
			id = i;
			break;
	    	}
	}	
	m_Conf.UpdateVmsData(VmsData);
	UnLock();
	change.id = id;
	change.type = CLIENT_FACTORY_VMS_ADD;
	CallCameraChange(change);

	delete pVmsData;
	return id;
}
inline BOOL ClientFactory::DelVms(s32 Id)
{
	VSCVmsData * pVmsData = new VSCVmsData;
	VSCVmsData &VmsData = *pVmsData;
	ClientFactoryCameraChangeData change;
	Lock();
	m_Conf.GetVmsData(VmsData);
	if (Id < CONF_VMS_NUM_MAX && Id > 0)
	{
		VmsData.data.conf.vms[Id].Used = 0;
	}else
	{
		UnLock();
		delete pVmsData;
		return FALSE;
	}
	
	m_Conf.UpdateVmsData(VmsData);
	UnLock();
	change.id = Id;
	change.type = CLIENT_FACTORY_VMS_DEL;
	CallCameraChange(change);

	delete pVmsData;
	return TRUE;
}


inline BOOL ClientFactory::GetView(VSCViewData &pData)
{
	Lock();
	m_Conf.GetViewData(pData);
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetViewById(VSCViewDataItem &pParam, int nId)
{
	VSCViewData ViewData;
	int id = -1;
	Lock();
	m_Conf.GetViewData(ViewData);
	if (nId < CONF_VMS_NUM_MAX && nId > 0)
	{
		if (ViewData.data.conf.view[nId].Used == 1)
		{
			memcpy(&pParam, &(ViewData.data.conf.view[nId]), 
				sizeof(VSCViewDataItem));
		}
	}else
	{
		UnLock();
		return FALSE;
	}
	UnLock();
	return TRUE;
	
}

inline s32 ClientFactory::AddView(VSCViewDataItem &pParam)
{
	VSCViewData ViewData;
	ClientFactoryCameraChangeData change;
	int id = -1;
	Lock();
	m_Conf.GetViewData(ViewData);
	/* Just use 1 to CONF_VIEW_NUM_MAX */
	for (s32 i = 1; i < CONF_VIEW_NUM_MAX; i ++)
	{
	    	if (ViewData.data.conf.view[i].Used == 1)
	    	{
	    		continue;
	    	}else
	    	{
	    		memcpy(&(ViewData.data.conf.view[i]), &pParam, 
							sizeof(VSCViewDataItem));
			ViewData.data.conf.view[i].Used = 1;
			ViewData.data.conf.view[i].nId = i;
			id = i;
			break;
	    	}
	}	
	m_Conf.UpdateViewData(ViewData);
	UnLock();
	change.id = id;
	change.type = CLIENT_FACTORY_VIEW_ADD;
	CallCameraChange(change);
	return id;
}
inline BOOL ClientFactory::DelView(s32 Id)
{
	VSCViewData ViewData;
	ClientFactoryCameraChangeData change;
	Lock();
	m_Conf.GetViewData(ViewData);
	if (Id < CONF_VMS_NUM_MAX && Id > 0)
	{
		ViewData.data.conf.view[Id].Used = 0;
	}else
	{
		UnLock();
		return FALSE;
	}
	
	m_Conf.UpdateViewData(ViewData);
	UnLock();
	change.id = Id;
	change.type = CLIENT_FACTORY_VIEW_DEL;
	CallCameraChange(change);
	return TRUE;
}


inline BOOL ClientFactory::GetTour(VSCTourData &pData)
{
	Lock();
	m_Conf.GetTourData(pData);
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetTourById(VSCTourDataItem &pParam, int nId)
{
	VSCTourData TourData;
	int id = -1;
	Lock();
	m_Conf.GetTourData(TourData);
	if (nId < CONF_VMS_NUM_MAX && nId > 0)
	{
		if (TourData.data.conf.tour[nId].Used == 1)
		{
			memcpy(&pParam, &(TourData.data.conf.tour[nId]), 
				sizeof(VSCTourDataItem));
		}
	}else
	{
		UnLock();
		return FALSE;
	}
	UnLock();
	return TRUE;
	
}

inline s32 ClientFactory::AddTour(VSCTourDataItem &pParam)
{
	VSCTourData TourData;
	ClientFactoryCameraChangeData change;
	int id = -1;
	Lock();
	m_Conf.GetTourData(TourData);
	/* Just use 1 to CONF_TOUR_NUM_MAX */
	for (s32 i = 1; i < CONF_TOUR_NUM_MAX; i ++)
	{
	    	if (TourData.data.conf.tour[i].Used == 1)
	    	{
	    		continue;
	    	}else
	    	{
	    		memcpy(&(TourData.data.conf.tour[i]), &pParam, 
							sizeof(VSCTourDataItem));
			TourData.data.conf.tour[i].Used = 1;
			TourData.data.conf.tour[i].nId = i;
			id = i;
			break;
	    	}
	}	
	m_Conf.UpdateTourData(TourData);
	UnLock();
	change.id = id;
	change.type = CLIENT_FACTORY_TOUR_ADD;
	CallCameraChange(change);
	return id;
}
inline BOOL ClientFactory::DelTour(s32 Id)
{
	VSCTourData TourData;
	ClientFactoryCameraChangeData change;
	Lock();
	m_Conf.GetTourData(TourData);
	if (Id < CONF_VMS_NUM_MAX && Id > 0)
	{
		TourData.data.conf.tour[Id].Used = 0;
	}else
	{
		UnLock();
		return FALSE;
	}
	
	m_Conf.UpdateTourData(TourData);
	UnLock();
	change.id = Id;
	change.type = CLIENT_FACTORY_TOUR_DEL;
	CallCameraChange(change);
	return TRUE;
}



inline BOOL ClientFactory::GetVGroup(VSCVGroupData &pData)
{
	Lock();
	m_Conf.GetVGroupData(pData);
	UnLock();
	return TRUE;
}

inline BOOL ClientFactory::GetVGroupById(VSCVGroupDataItem &pParam, int nId)
{
	VSCVGroupData VGroupData;
	int id = -1;
	Lock();
	m_Conf.GetVGroupData(VGroupData);
	if (nId < CONF_VGROUP_NUM_MAX && nId > 0)
	{
		if (VGroupData.data.conf.group[nId].Used == 1)
		{
			memcpy(&pParam, &(VGroupData.data.conf.group[nId]), 
				sizeof(VSCVGroupDataItem));
		}
	}else
	{
		UnLock();
		return FALSE;
	}
	UnLock();
	return TRUE;
	
}

inline s32 ClientFactory::AddVGroup(VSCVGroupDataItem &pParam)
{
	VSCVGroupData VGroupData;
	ClientFactoryCameraChangeData change;
	int id = -1;
	Lock();
	m_Conf.GetVGroupData(VGroupData);
	/* Just use 1 to CONF_VGROUP_NUM_MAX */
	for (s32 i = 1; i < CONF_VGROUP_NUM_MAX; i ++)
	{
	    	if (VGroupData.data.conf.group[i].Used == 1)
	    	{
	    		continue;
	    	}else
	    	{
	    		memcpy(&(VGroupData.data.conf.group[i]), &pParam, 
							sizeof(VSCVGroupDataItem));
			VGroupData.data.conf.group[i].Used = 1;
			VGroupData.data.conf.group[i].nId = i;
			id = i;
			break;
	    	}
	}	
	m_Conf.UpdateVGroupData(VGroupData);
	UnLock();
	change.id = id;
	change.type = CLIENT_FACTORY_VGROUP_ADD;
	CallCameraChange(change);
	return id;
}
inline BOOL ClientFactory::DelVGroup(s32 Id)
{
	VSCVGroupData VGroupData;
	ClientFactoryCameraChangeData change;
	Lock();
	m_Conf.GetVGroupData(VGroupData);
	if (Id < CONF_VGROUP_NUM_MAX && Id > 0)
	{
		VGroupData.data.conf.group[Id].Used = 0;
	}else
	{
		UnLock();
		return FALSE;
	}
	
	m_Conf.UpdateVGroupData(VGroupData);
	UnLock();
	change.id = Id;
	change.type = CLIENT_FACTORY_VGROUP_DEL;
	CallCameraChange(change);
	return TRUE;
}



inline BOOL ClientFactory::GetCameraRtspUrl(astring & strUrl, s32 nIndex)
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

inline s32 ClientFactory::GetCameraParamById(CameraParam & pParam, s32 nIndex)
{
    //VDC_DEBUG( "%s GetCameraParamById %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= CLIENT_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }

    Lock();
    pParam = m_CameraParamMap[nIndex];
    UnLock();

    return TRUE;
}

inline s32 ClientFactory::GetCameraParamByIdTryLock(CameraParam & pParam, s32 nIndex)
{
    //VDC_DEBUG( "%s GetCameraParamById %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= CLIENT_FACTORY_CAMERA_ID_MAX)
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

inline s32 ClientFactory::GetCameraID(void)
{
    s32 id  = -1;
    s32 i = -1;

    Lock();
    for (i = 1; i < CLIENT_FACTORY_CAMERA_ID_MAX; i ++)
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

inline BOOL ClientFactory::ReleaseCameraID(s32 nID)
{
    if (nID <=0 || nID >= CLIENT_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    Lock();
    m_strCameraMap[nID] = 'n';
    UnLock();

    return TRUE;
}

inline BOOL ClientFactory::LockCameraID(s32 nID)
{
    if (nID <=0 || nID >= CLIENT_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    Lock();
    m_strCameraMap[nID] = 'y';
    UnLock();

    return TRUE;
}

inline s32 ClientFactory::GetVIPCID(void)
{
    s32 id  = -1;
    s32 i = -1;

    Lock();
    for (i = 1; i < CLIENT_FACTORY_CAMERA_ID_MAX; i ++)
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

inline BOOL ClientFactory::ReleaseVIPCID(s32 nID)
{
    if (nID <=0 || nID >= CLIENT_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    Lock();
    m_strVIPCMap[nID] = 'n';
    UnLock();

    return TRUE;
}

inline BOOL ClientFactory::LockVIPCID(s32 nID)
{
    if (nID <=0 || nID >= CLIENT_FACTORY_CAMERA_ID_MAX)
    {
        return FALSE;
    }
    Lock();
    m_strVIPCMap[nID] = 'y';
    UnLock();

    return TRUE;
}


inline void ClientFactory::Run(void * pParam)
{
    int dummy = errno;
    LPClientFactory pThread = (LPClientFactory)pParam;

    if (pThread)
    {
        //pThread->Run1();
    }
}

inline void ClientFactory::run()
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
				
				ClientFactoryCameraChangeData change;
				change.id = nIndex;
				switch (bCheck)
				{
					case DEV_OFF2ON:
					{
						change.type = CLIENT_FACTORY_CAMERA_ONLINE;
						m_CameraOnlineMap[nIndex] = 1;
						UnLock(); 
						CallCameraChange(change);
						Lock();
						break;
					}
					case DEV_ON2OFF:
					{
						change.type = CLIENT_FACTORY_CAMERA_OFFLINE;
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




#endif // __VSC_CLIENT_FACTORY_H_
