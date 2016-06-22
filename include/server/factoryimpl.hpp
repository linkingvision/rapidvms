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
			status.freeSize = info.bytesAvailable();
			status.totalSize = info.bytesTotal();
			mapStatus[(*it).second.hdd] = status;
			//VDC_DEBUG("HDD %s path %s freeSize %lld\n", ((*it).first).c_str(), 
			//				(*it).second.path.c_str(), status.freeSize);
			
		}
		m_Factory.UpdateDiskStatusMap(mapStatus);
		ve_sleep(2000);
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

	VidHDFSConf HdfsConf;
	m_Conf.GetHdfsRecordConf(HdfsConf);
	astring strNameNode = HdfsConf.strnamenode();
	astring strPort = HdfsConf.strport();
	astring strUser = HdfsConf.struser();
	m_pVHdfsdb = new VHdfsDB(strNameNode, strPort, strUser);

	VidCameraList cameraList;
	m_Conf.GetCameraListConf(cameraList);
	int cameraSize = cameraList.cvidcamera_size();

	for (s32 i = 0; i < cameraList.cvidcamera_size(); i ++)
	{
		VidCamera cam = cameraList.cvidcamera(i);
		CameraParam pParam(cam);
		InitAddCamera(pParam, cam.strid());
	}

	InitLicense();

	//start();
	m_HddTask = new FactoryHddTask(*this);
	m_HddTask->start();

	/* Init export path */
	astring strExportPath;
	GetExportPath(strExportPath);
	return TRUE;
}

inline BOOL Factory::GetCameraOnlineMap(CameraOnlineMap &pMap)
{
	XGuard guard(m_cMutex);
    	pMap = m_CameraOnlineMap;

   	return TRUE;
}

inline BOOL Factory::GetCameraRecMap(CameraRecMap &pMap)
{
	XGuard guard(m_cMutex);
    	pMap = m_CameraRecMap;

   	return TRUE;
}

inline bool Factory::GetCameraList(VidCameraList & pCameraList)
{
	XGuard guard(m_cMutex);

	return m_Conf.GetCameraListConf(pCameraList);
}

inline BOOL Factory::RegCameraChangeNotify(void * pData, FactoryCameraChangeNotify callback)
{
	XGuard guard(m_cMutex);
	m_CameraChange[pData] = callback;

	return TRUE;
}

inline BOOL Factory::UnRegCameraChangeNotify(void * pData)
{
	XGuard guard(m_cMutex);
	m_CameraChange.erase(pData);

	return TRUE;
}

inline BOOL Factory::CallCameraChange(FactoryCameraChangeData data)
{
	XGuard guard(m_cMutex);
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

inline BOOL Factory::RecChangeHandler(astring strId, bool bRec, void * pParam)
{
	int dummy = errno;
	LPFactory pFactory = (LPFactory)pParam;

	if (pFactory)
	{
	    return pFactory->RecChangeHandler1(strId, bRec);
	}

	return TRUE;

	
}
inline BOOL Factory::RecChangeHandler1(astring strId, bool bRec)
{
	FactoryCameraChangeData data;
	data.id = strId;
	if (bRec == true)
	{
		data.type = FACTORY_CAMERA_REC_ON;
		m_CameraRecMap[strId] = 1;
	}else
	{
		data.type = FACTORY_CAMERA_REC_OFF;
		m_CameraRecMap[strId] = 0;
	}

	CallCameraChange(data);

	return TRUE;
}

inline BOOL Factory::GetLicense(astring &strLicense, astring &strHostId, int &ch, astring &type, astring &expireTime)
{
	VPlay::GetLicenseInfo(strHostId, ch, type, expireTime);
	return m_Conf.GetLicense(strLicense);
}
inline BOOL Factory::SetLicense(astring &strLicense)
{
	VPlay::SetLicense(strLicense);
	return m_Conf.SetLicense(strLicense);
}

inline BOOL Factory::InitLicense()
{
	astring strLicense;
	m_Conf.GetLicense(strLicense);
	VPlay::SetLicense(strLicense);
	return TRUE;
}

inline bool Factory::AuthUser(astring &strUser, astring &strPasswd)
{
	/* Admin is a Super User */
	if (strUser == "admin")
	{
		VidStorServerConf  cData;
		m_Conf.GetStorServerConf(cData);
		SimpleCrypt crypt;
		QString strDePasswd = cData.stradminpasswd().c_str();
		if (crypt.decryptToString(strDePasswd).toStdString() == strPasswd)
		{
			return true;
		}else
		{
			return false;
		}
	}

	return false;
}
inline bool Factory::GetAdminPasswd(astring &strPasswd)
{
	VidStorServerConf  cData;
	m_Conf.GetStorServerConf(cData);
	strPasswd = cData.stradminpasswd();
	
	return true;
}
inline bool Factory::SetAdminPasswd(astring strPasswd)
{
	VidStorServerConf  cData;
	m_Conf.GetStorServerConf(cData);
	cData.set_stradminpasswd(strPasswd);
	m_Conf.SetStorServerConf(cData);

	return true;	
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


inline BOOL Factory::SearchItems(astring cameraId, u32 startTime, u32 endTime, u32 recordType, 
				RecordItemMap &map)
{
    return m_pVdb->SearchItems(cameraId, startTime, endTime, recordType, 
                        map);
}

inline BOOL Factory::SearchHasItems(astring cameraId, u32 startTime, u32 endTime, u32 recordType)
{
    return m_pVdb->SearchHasItems(cameraId, startTime, endTime, recordType);
}

inline VDB& Factory::GetVdb()
{
	return *m_pVdb;
}


inline s32 Factory::InitAddCamera(CameraParam & pParam, astring strCamId)
{

	m_CameraMap[strCamId] = new Camera(m_Conf, *m_pVdb, 
			*m_pVHdfsdb, pParam, 
			(RecChangeFunctionPtr)Factory::RecChangeHandler, 
			(void *)this);

	m_CameraOnlineMap[strCamId] = FALSE;
	m_CameraRecMap[strCamId] = FALSE;

	if (pParam.m_Conf.strname() == "Camera")
	{
		pParam.m_Conf.set_strname(pParam.m_Conf.strid());
	}

    return TRUE;
}

inline BOOL Factory::RegDataCallback(astring nIndex, CameraDataCallbackFunctionPtr pCallback,
        void * pParam)
{
	XGuard guard(m_cMutex);
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->RegDataCallback(pCallback, pParam);
	}

	return TRUE;
}

inline BOOL Factory::UnRegDataCallback(astring nIndex, void * pParam)
{
	XGuard guard(m_cMutex);
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->UnRegDataCallback(pParam);
	}

    return TRUE;
}


inline BOOL Factory::GetInfoFrame(astring nIndex, InfoFrame &pFrame)
{
	XGuard guard(m_cMutex);
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->GetInfoFrame(pFrame);
	}

	return TRUE;
}

inline BOOL Factory::RegSubDataCallback(astring nIndex, CameraDataCallbackFunctionPtr pCallback,
        void * pParam)
{
	XGuard guard(m_cMutex);
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->RegSubDataCallback(pCallback, pParam);
	}

	return TRUE;
}

inline BOOL Factory::UnRegSubDataCallback(astring nIndex, void * pParam)
{
	XGuard guard(m_cMutex);
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->UnRegDataCallback(pParam);
	}

	return TRUE;
}


inline BOOL Factory::GetSubInfoFrame(astring nIndex, InfoFrame &pFrame)
{
	XGuard guard(m_cMutex);
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->GetSubInfoFrame(pFrame);
	}

    	return TRUE;
}


inline BOOL Factory::GetCameraOnline(astring nIndex, BOOL &bStatus)
{
	XGuard guard(m_cMutex);

	if (m_CameraMap[nIndex] != NULL)
	{
	    bStatus =  m_CameraMap[nIndex]->GetCameraOnline();
	}

	return TRUE;
}

inline BOOL Factory::GetStreamInfo(astring nIndex, VideoStreamInfo &pInfo)
{
	XGuard guard(m_cMutex);
	if (m_CameraMap[nIndex] != NULL)
	{
	    m_CameraMap[nIndex]->GetStreamInfo(pInfo);
	}

	return TRUE;
}

inline BOOL Factory::UpdateRecSched(astring strCamId, VidCamera &pCam)
{
	XGuard guard(m_cMutex);
	if (m_CameraMap[strCamId] != NULL)
	{
		m_CameraMap[strCamId]->UpdateRecSched(pCam);
		/* Save configuration of the sched */
		m_Conf.DeleteCamera(strCamId);
		m_Conf.AddCamera(pCam);
	}
	
	return TRUE;	
}

inline BOOL Factory::PtzAction(astring strCamId, FPtzAction action, float speed)
{
	XGuard guard(m_cMutex);
	if (m_CameraMap[strCamId] != NULL)
	{
	    m_CameraMap[strCamId]->PtzAction(action, speed);
	}

	return TRUE;
}

inline astring Factory::AddCamera(CameraParam & pParam)
{
	FactoryCameraChangeData change;
	VidCamera pCamCurr;
	/* if the camera exist, first delete */
	if (GetCamera(pParam.m_Conf.strid(), pCamCurr) == true)
	{
		DelCamera(pParam.m_Conf.strid());
	}

	XGuard guard(m_cMutex);

	InitAddCamera(pParam, pParam.m_Conf.strid());

	m_Conf.AddCamera(pParam.m_Conf);

	guard.Release();
	change.id = pParam.m_Conf.strid();
	change.type = FACTORY_CAMERA_ADD;
	//change.cCam = pParam.m_Conf;
	CallCameraChange(change);
	guard.Acquire();

    	return pParam.m_Conf.strid();
}

inline BOOL Factory::DelCamera(astring nIndex)
{
	FactoryCameraChangeData change;

	change.id = nIndex;
	change.type = FACTORY_CAMERA_DEL;
	CallCameraChange(change);

	XGuard guard(m_cMutex);
	delete m_CameraMap[nIndex];
	m_CameraMap[nIndex] = NULL;
	m_CameraOnlineMap.erase(nIndex);
	m_CameraRecMap.erase(nIndex);
	int size1 = m_CameraMap.size();
	m_CameraMap.erase(nIndex);
	int size2 = m_CameraMap.size();
	m_Conf.DeleteCamera(nIndex);

	return TRUE;
}

inline BOOL Factory::GetCamera(astring strId, VidCamera & pCam)
{
	XGuard guard(m_cMutex);
	return m_Conf.GetCameraConf(strId, pCam);
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
			XGuard guard(m_cMutex);
			CameraMap::iterator it = m_CameraMap.begin(); 
			for(; it!=m_CameraMap.end(); ++it)
			{	
				astring nIndex = (*it).first;
				CameraParam pParam;
				Camera *pCamera = m_CameraMap[nIndex];
				if (pCamera == NULL)
				{
					continue;//TODO
				}
				pCamera->GetCameraParam(pParam);
				paramMap[nIndex] = pParam;
			}
		}
		{
			/* Loop all the cameraparam */
			CameraParamMap::iterator it = paramMap.begin(); 
			for(; it!=paramMap.end(); ++it)
			{	
				/* Loop to check the camera and update the url */
				astring nIndex = (*it).first;
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
				astring nIndex = (*it).first;
				XGuard guard(m_cMutex);
				CameraMap::iterator it1 = m_CameraMap.find(nIndex), 
							ite1 = m_CameraMap.end();

				if (it1 == ite1) 
				{
					/* the id may be delete */
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
						guard.Release();
						CallCameraChange(change);
						guard.Acquire();
						break;
					}
					case DEV_ON2OFF:
					{
						change.type = FACTORY_CAMERA_OFFLINE;
						m_CameraOnlineMap[nIndex] = 0;
						guard.Release();
						CallCameraChange(change);
						guard.Acquire();
						break;
					}
					default:
					{

						break;
					}
				}
			}
		}
		ve_sleep(1000 * 30);
	}
	
}




#endif // __VSC_FACTORY_H_
