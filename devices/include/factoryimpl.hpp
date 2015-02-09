//------------------------------------------------------------------------------
// File: factory impl.hpp
//
// Desc: Device factory - Manage IP Camera.
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
			VDC_DEBUG("HDD %s path %s freeSize %lld\n", ((*it).first).c_str(), 
							(*it).second.path.c_str(), status.freeSize);
			
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
    astring strSysPath = "C:\\videodb\\system";
#else
    astring strSysPath = "C:\\videodb64\\system";
#endif
#else
    astring strSysPath = "ve/videodb/system/";
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

inline BOOL Factory::Init()
{

	s32 i = 0;

	for (i = 0; i < FACTORY_DEVICE_ID_MAX; i ++)
	{
	    m_strDeviceMap[i] = 'n';
	}
	for (i = 0; i < FACTORY_DEVICE_ID_MAX; i ++)
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
	astring strPathConf = strPath + "videodb\\config";
#else
	astring strPathConf = strPath + "videodb64\\config";
#endif
#else
	astring strPathConf = strPath + "videodb/config";
#endif
	m_Conf.Open(strPathConf);

#ifndef _WIN64
	astring strPathDb = strPath + "videodb";
#else
	astring strPathDb = strPath + "videodb64";
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
	    if (sysData.data.conf.DeviceMap[i] != CONF_MAP_INVALID_MIN 
			&& sysData.data.conf.DeviceMap[i] != 0)
	    {
	        VDC_DEBUG( "%s Init Device %d\n",__FUNCTION__, i);
	        VSCDeviceData Data;
	        m_Conf.GetDeviceData(i, Data);
	        DeviceParam mParam(Data);
	        LockDeviceID(Data.data.conf.nId);
	        InitAddDevice(mParam, Data.data.conf.nId);
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
	        VIPCDeviceParam mParam(Data);
	        LockVIPCID(Data.data.conf.nId);
	        m_VIPCDeviceParamMap[i] = mParam;
	        VDC_DEBUG( "%s Id %d\n",__FUNCTION__, Data.data.conf.nId);
	    }
	}

	InitLicense();
	//m_pThread = new thread(Factory::Run, (void *)this);
	//start();
	m_HddTask = new FactoryHddTask(*this);
	m_HddTask->start();
	return TRUE;
}

inline BOOL Factory::RegDeviceChangeNotify(void * pData, FactoryDeviceChangeNotify callback)
{
	Lock();
	m_DeviceChange[pData] = callback;
	UnLock();
	return TRUE;
}
inline BOOL Factory::CallDeviceChange(FactoryDeviceChangeData data)
{
        DeviceChangeNofityMap::iterator it = m_DeviceChange.begin(); 
        for(; it!=m_DeviceChange.end(); ++it)
        {
		if ((*it).second)
		{
			(*it).second((*it).first, data);
		}
        }	
	 return TRUE;
}
inline BOOL Factory::GetLicense(astring &strLicense, astring &strHostId, int &ch, int &type)
{
	VPlay::GetLicenseInfo(strHostId, ch, type);
	return m_Conf.GetLicense(strLicense);
}
inline BOOL Factory::SetLicense(astring &strLicense)
{
	VPlay::SetLicense(strLicense);
	return m_Conf.SetLicense(strLicense);
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


inline BOOL Factory::SearchItems(s32 deviceId, u32 startTime, u32 endTime, u32 recordType, 
				RecordItemMap &map)
{
    return m_pVdb->SearchItems(deviceId, startTime, endTime, recordType, 
                        map);
}

inline BOOL Factory::SearchHasItems(s32 deviceId, u32 startTime, u32 endTime, u32 recordType)
{
    return m_pVdb->SearchHasItems(deviceId, startTime, endTime, recordType);
}
inline BOOL Factory::SearchAItem(s32 deviceId, u32 Time, VdbRecordItem &pItem)
{
    return m_pVdb->SearchAItem(deviceId, Time, pItem);
}

inline BOOL Factory::SearchAItemNear(s32 deviceId, u32 Time, VdbRecordItem &pItem)
{
    return m_pVdb->SearchAItemNear(deviceId, Time, pItem);
}
inline BOOL Factory::SearchNextItem(s32 deviceId, s64 LastId, VdbRecordItem &pItem)
{
    return m_pVdb->SearchNextItem(deviceId, LastId, pItem);
}
inline BOOL Factory::RequestAMFRead(VdbRecordItem &pItem, astring & strPath)
{
    return m_pVdb->RequestAMFRead(pItem, strPath);
}
inline BOOL Factory::FinishedAMFRead(VdbRecordItem &pItem, astring & strPath)
{
    return m_pVdb->FinishedAMFRead(pItem, strPath);
}

inline BOOL Factory::GetDeviceParamMap(DeviceParamMap &pMap)
{
    pMap = m_DeviceParamMap;

    return TRUE;
}

inline BOOL Factory::GetVIPCDeviceParamMap(VIPCDeviceParamMap &pMap)
{
    pMap = m_VIPCDeviceParamMap;

    return TRUE;
}

inline s32 Factory::InitAddDevice(DeviceParam & pParam, u32 nIndex)
{
    if (pParam.m_Conf.data.conf.nType == VSC_DEVICE_CAM)
    {
    	m_DeviceMap[nIndex] = new Device(*m_pVdb, *m_pVHdfsdb, pParam);
    }else
    {
    	m_DeviceMap[nIndex] = NULL;
    }
    m_DeviceParamMap[nIndex] = pParam;

    return TRUE;
}

inline BOOL Factory::RegDataCallback(s32 nIndex, DeviceDataCallbackFunctionPtr pCallback,
        void * pParam)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->RegDataCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::UnRegDataCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->UnRegDataCallback(pParam);
    }

    UnLock();

    return TRUE;
}


inline BOOL Factory::GetInfoFrame(s32 nIndex, InfoFrame &pFrame)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->GetInfoFrame(pFrame);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::RegSubDataCallback(s32 nIndex, DeviceDataCallbackFunctionPtr pCallback,
        void * pParam)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->RegSubDataCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::UnRegSubDataCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->UnRegDataCallback(pParam);
    }

    UnLock();

    return TRUE;
}


inline BOOL Factory::GetSubInfoFrame(s32 nIndex, InfoFrame &pFrame)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->GetSubInfoFrame(pFrame);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::RegRawCallback(s32 nIndex, DeviceRawCallbackFunctionPtr pCallback,
        void * pParam)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->RegRawCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::UnRegRawCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->UnRegRawCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::RegSubRawCallback(s32 nIndex, DeviceRawCallbackFunctionPtr pCallback,
        void * pParam)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->RegSubRawCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::UnRegSubRawCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->UnRegSubRawCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::RegDelCallback(s32 nIndex, DeviceDelCallbackFunctionPtr pCallback, void * pParam)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->RegDelCallback(pCallback, pParam);
    }

    UnLock();

    return TRUE;
}
inline BOOL Factory::UnRegDelCallback(s32 nIndex, void * pParam)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->UnRegDelCallback(pParam);
    }

    UnLock();

    return TRUE;
}

inline BOOL Factory::GetDeviceOnline(s32 nIndex, BOOL &bStatus)
{
    Lock();

    if (m_DeviceMap[nIndex] != NULL)
    {
        bStatus =  m_DeviceMap[nIndex]->GetDeviceOnline();
    }

    UnLock();

    return TRUE;
}

inline   BOOL Factory::GetUrl(s32 nIndex, std::string &url)
{
    BOOL ret = FALSE;

    if (m_DeviceMap[nIndex] != NULL)
    {
        ret =  m_DeviceMap[nIndex]->GetUrl(url);
    }

    UnLock();

    return ret;
}

inline BOOL Factory::AttachPlayer(s32 nIndex, HWND hWnd, int w, int h)
{
    //Lock();//For VIPC testing
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->AttachPlayer(hWnd, w, h);
    }
    //UnLock();

	return TRUE;
}

inline BOOL Factory::UpdateWidget(s32 nIndex, HWND hWnd, int w, int h)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->UpdateWidget(hWnd, w, h);
    }
    UnLock();

	return TRUE;
}

inline BOOL Factory::DetachPlayer(s32 nIndex, HWND hWnd)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->DetachPlayer(hWnd);
    }
    UnLock();

	return TRUE;
}

inline BOOL Factory::EnablePtz(s32 nIndex, HWND hWnd, bool enable)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->EnablePtz(hWnd, enable);
    }
    UnLock();

	return TRUE;
}

inline BOOL Factory::DrawPtzDirection(s32 nIndex, HWND hWnd, int x1, int y1, int x2,  int y2)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->DrawPtzDirection(hWnd, x1, y1, x2, y2);
    }
    UnLock();

	return TRUE;
}

inline BOOL Factory::ShowAlarm(s32 nIndex, HWND hWnd)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->ShowAlarm(hWnd);
    }
    UnLock();

	return TRUE;
}

inline BOOL Factory::ClearPtzDirection(s32 nIndex, HWND hWnd)
{
    Lock();
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->ClearPtzDirection(hWnd);
    }
    UnLock();

	return TRUE;
}

inline BOOL Factory::PtzAction(s32 nIndex, FPtzAction action, float speed)
{
	Lock();
	if (m_DeviceMap[nIndex] != NULL)
	{
	    m_DeviceMap[nIndex]->PtzAction(action, speed);
	}
	UnLock();

	return TRUE;
}


inline BOOL Factory::GetRecordStatus(s32 nIndex,BOOL &nStatus)
{
    DeviceParam pParam;
    if (nIndex <=0 || nIndex >= FACTORY_DEVICE_ID_MAX)
    {
        return FALSE;
    }
    if (GetDeviceParamByIdTryLock(pParam, nIndex) == FALSE)
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
    DeviceParam pParam;
    FactoryDeviceChangeData change;
    if (nIndex <=0 || nIndex >= FACTORY_DEVICE_ID_MAX)
    {
        return FALSE;
    }
    GetDeviceParamById(pParam, nIndex);
    if (pParam.m_Conf.data.conf.Recording == 1)
    {
        return TRUE;
    }

    Lock();
    pParam.m_Conf.data.conf.Recording = 1;
    m_Conf.UpdateDeviceData(nIndex, pParam.m_Conf);
    m_DeviceParamMap[nIndex] = pParam;
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->SetRecord(TRUE);
        m_DeviceMap[nIndex]->StartRecord();
    }
    UnLock();
    change.id = nIndex;
    change.type = FACTORY_DEVICE_RECORDING_ON;
    CallDeviceChange(change);
    return TRUE;
}
inline BOOL Factory::StopRecord(s32 nIndex)
{
    DeviceParam pParam;
    FactoryDeviceChangeData change;
    if (nIndex <=0 || nIndex >= FACTORY_DEVICE_ID_MAX)
    {
        return FALSE;
    }
    GetDeviceParamById(pParam, nIndex);
    if (pParam.m_Conf.data.conf.Recording == 0)
    {
        return TRUE;
    }

    Lock();
    pParam.m_Conf.data.conf.Recording = 0;
    m_DeviceParamMap[nIndex] = pParam;
    m_Conf.UpdateDeviceData(nIndex, pParam.m_Conf);
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->SetRecord(FALSE);
        m_DeviceMap[nIndex]->StopRecord();
    }
    UnLock();
    change.id = nIndex;
    change.type = FACTORY_DEVICE_RECORDING_OFF;
    CallDeviceChange(change);
    return TRUE;
}


inline BOOL Factory::StartHdfsRecord(s32 nIndex)
{
    DeviceParam pParam;
    FactoryDeviceChangeData change;
    if (nIndex <=0 || nIndex >= FACTORY_DEVICE_ID_MAX)
    {
        return FALSE;
    }
    GetDeviceParamById(pParam, nIndex);
    if (pParam.m_Conf.data.conf.HdfsRecording == 1)
    {
        return TRUE;
    }

    Lock();
    pParam.m_Conf.data.conf.HdfsRecording = 1;
    m_Conf.UpdateDeviceData(nIndex, pParam.m_Conf);
    m_DeviceParamMap[nIndex] = pParam;
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->SetHdfsRecord(TRUE);
        m_DeviceMap[nIndex]->StartHdfsRecord();
    }
    UnLock();
    change.id = nIndex;
    change.type = FACTORY_DEVICE_RECORDING_ON;
    CallDeviceChange(change);
    return TRUE;
}
inline BOOL Factory::StopHdfsRecord(s32 nIndex)
{
    DeviceParam pParam;
    FactoryDeviceChangeData change;
    if (nIndex <=0 || nIndex >= FACTORY_DEVICE_ID_MAX)
    {
        return FALSE;
    }
    GetDeviceParamById(pParam, nIndex);
    if (pParam.m_Conf.data.conf.HdfsRecording == 0)
    {
        return TRUE;
    }

    Lock();
    pParam.m_Conf.data.conf.HdfsRecording = 0;
    m_DeviceParamMap[nIndex] = pParam;
    m_Conf.UpdateDeviceData(nIndex, pParam.m_Conf);
    if (m_DeviceMap[nIndex] != NULL)
    {
        m_DeviceMap[nIndex]->SetHdfsRecord(FALSE);
        m_DeviceMap[nIndex]->StopHdfsRecord();
    }
    UnLock();
    change.id = nIndex;
    change.type = FACTORY_DEVICE_RECORDING_OFF;
    CallDeviceChange(change);
    return TRUE;
}

inline BOOL Factory::StartRecordAll()
{
	Lock();
	DeviceParamMap DeviceMap = m_DeviceParamMap;
	UnLock();
	DeviceParamMap::iterator it = DeviceMap.begin(); 
	for(; it!=DeviceMap.end(); ++it)
	{
	    StartRecord((*it).second.m_Conf.data.conf.nId);
	}

	return TRUE;
}
inline BOOL Factory::StopRecordAll()
{
	Lock();
	DeviceParamMap DeviceMap = m_DeviceParamMap;
	UnLock();
	DeviceParamMap::iterator it = DeviceMap.begin(); 
	for(; it!=DeviceMap.end(); ++it)
	{
	    StopRecord((*it).second.m_Conf.data.conf.nId);
	}

	return TRUE;
}
inline BOOL Factory::StartHdfsRecordAll()
{
	Lock();
	DeviceParamMap DeviceMap = m_DeviceParamMap;
	UnLock();
	DeviceParamMap::iterator it = DeviceMap.begin(); 
	for(; it!=DeviceMap.end(); ++it)
	{
	    StartHdfsRecord((*it).second.m_Conf.data.conf.nId);
	}

	return TRUE;
}
inline BOOL Factory::StopHdfsRecordAll()
{
	Lock();
	DeviceParamMap DeviceMap = m_DeviceParamMap;
	UnLock();
	DeviceParamMap::iterator it = m_DeviceParamMap.begin(); 
	for(; it!=m_DeviceParamMap.end(); ++it)
	{
	    StopHdfsRecord((*it).second.m_Conf.data.conf.nId);
	}	

	return TRUE;
}

inline BOOL Factory::UpdateDeviceGroup(s32 nIndex, s32 nGroup)
{
    DeviceParam pParam;
    FactoryDeviceChangeData change;
    if (nIndex <=0 || nIndex >= FACTORY_DEVICE_ID_MAX)
    {
        return FALSE;
    }
    GetDeviceParamById(pParam, nIndex);
    if (pParam.m_Conf.data.conf.GroupId == nGroup)
    {
        return TRUE;
    }

    Lock();
    pParam.m_Conf.data.conf.GroupId = nGroup;
    m_DeviceParamMap[nIndex] = pParam;
    m_Conf.UpdateDeviceData(nIndex, pParam.m_Conf);
    UnLock();
    change.id = nIndex;
    change.type = FACTORY_DEVICE_GROUP_CHANGE;
    CallDeviceChange(change);
    return TRUE;
}

inline s32 Factory::AddDevice(DeviceParam & pParam)
{
    s32 nId = GetDeviceID();
    FactoryDeviceChangeData change;

    Lock();
    pParam.m_Conf.data.conf.nId = nId;
    if (pParam.m_Conf.data.conf.nType == VSC_DEVICE_CAM)
    {
    	m_DeviceMap[nId] = new Device(*m_pVdb, *m_pVHdfsdb, pParam);
    }else
    {
	m_DeviceMap[nId] = NULL;
    }
    m_DeviceParamMap[nId] = pParam;
    m_Conf.AddDevice(pParam.m_Conf, nId);

    UnLock();
    change.id = nId;
    change.type = FACTORY_DEVICE_ADD;
    CallDeviceChange(change);
	
    return nId;
}

inline BOOL Factory::DelDevice(s32 nIndex)
{
    FactoryDeviceChangeData change;
    VDC_DEBUG( "%s DelDevice %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= FACTORY_DEVICE_ID_MAX)
    {
        return FALSE;
    }

    //TODO check is this device can be delete or not
    Lock();
    VDC_DEBUG( "%s Cleanup Begin\n",__FUNCTION__);
    m_DeviceMap[nIndex]->Cleanup();
    VDC_DEBUG( "%s Cleanup End\n",__FUNCTION__);
    delete m_DeviceMap[nIndex];
    m_DeviceMap[nIndex] = NULL;
    m_DeviceParamMap.erase(nIndex);
    m_DeviceMap.erase(nIndex);
    m_Conf.DelDevice(nIndex);
    UnLock();
    ReleaseDeviceID(nIndex);
	
    change.id = nIndex;
    change.type = FACTORY_DEVICE_DEL;
    CallDeviceChange(change);
    return TRUE;
}

inline s32 Factory::AddVIPC(VIPCDeviceParam & pParam)
{
    s32 nId = GetVIPCID();
    FactoryDeviceChangeData change;

    VDC_DEBUG( "%s GetDeviceID %d\n",__FUNCTION__, nId);

    Lock();
    pParam.m_Conf.data.conf.nId = nId;
    m_VIPCDeviceParamMap[nId] = pParam;

    m_Conf.AddVIPC(pParam.m_Conf, nId);
    UnLock();
    change.id = nId;
    change.type = FACTORY_VIPC_ADD;
    CallDeviceChange(change);
	
    return nId;
}


inline s32 Factory::GetVIPCParamById(VIPCDeviceParam & pParam, s32 nIndex)
{
    //VDC_DEBUG( "%s GetDeviceParamById %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= FACTORY_DEVICE_ID_MAX)
    {
        return FALSE;
    }

    Lock();
    pParam = m_VIPCDeviceParamMap[nIndex];
    UnLock();

    return TRUE;
}

inline BOOL Factory::DelVIPC(s32 nIndex)
{
    FactoryDeviceChangeData change;
    VDC_DEBUG( "%s DelDevice %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= FACTORY_DEVICE_ID_MAX)
    {
        return FALSE;
    }

    change.id = nIndex;
    change.type = FACTORY_VIPC_DEL;
    CallDeviceChange(change);
	
    Lock();
    m_VIPCDeviceParamMap.erase(nIndex);
    m_Conf.DelVIPC(nIndex);
    ReleaseVIPCID(nIndex);
    UnLock();
	

    return TRUE;
}


inline BOOL Factory::GetVms(VSCVmsData &pData)
{
	Lock();
	m_Conf.GetVmsData(pData);
	UnLock();
	return TRUE;
}

inline BOOL Factory::GetVmsById(VSCVmsDataItem &pParam, int nId)
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
	return TRUE;
	
}

inline s32 Factory::AddVms(VSCVmsDataItem &pParam)
{
	VSCVmsData VmsData;
	FactoryDeviceChangeData change;
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
	change.type = FACTORY_VMS_ADD;
	CallDeviceChange(change);
	return id;
}
inline BOOL Factory::DelVms(s32 Id)
{
	VSCVmsData VmsData;
	FactoryDeviceChangeData change;
	Lock();
	m_Conf.GetVmsData(VmsData);
	if (Id < CONF_VMS_NUM_MAX && Id > 0)
	{
		VmsData.data.conf.vms[Id].Used = 0;
	}else
	{
		UnLock();
		return FALSE;
	}
	
	m_Conf.UpdateVmsData(VmsData);
	UnLock();
	change.id = Id;
	change.type = FACTORY_VMS_DEL;
	CallDeviceChange(change);
	return TRUE;
}


inline BOOL Factory::GetView(VSCViewData &pData)
{
	Lock();
	m_Conf.GetViewData(pData);
	UnLock();
	return TRUE;
}

inline BOOL Factory::GetViewById(VSCViewDataItem &pParam, int nId)
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

inline s32 Factory::AddView(VSCViewDataItem &pParam)
{
	VSCViewData ViewData;
	FactoryDeviceChangeData change;
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
	change.type = FACTORY_VIEW_ADD;
	CallDeviceChange(change);
	return id;
}
inline BOOL Factory::DelView(s32 Id)
{
	VSCViewData ViewData;
	FactoryDeviceChangeData change;
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
	change.type = FACTORY_VIEW_DEL;
	CallDeviceChange(change);
	return TRUE;
}


inline BOOL Factory::GetVGroup(VSCVGroupData &pData)
{
	Lock();
	m_Conf.GetVGroupData(pData);
	UnLock();
	return TRUE;
}

inline BOOL Factory::GetVGroupById(VSCVGroupDataItem &pParam, int nId)
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

inline s32 Factory::AddVGroup(VSCVGroupDataItem &pParam)
{
	VSCVGroupData VGroupData;
	FactoryDeviceChangeData change;
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
	change.type = FACTORY_VGROUP_ADD;
	CallDeviceChange(change);
	return id;
}
inline BOOL Factory::DelVGroup(s32 Id)
{
	VSCVGroupData VGroupData;
	FactoryDeviceChangeData change;
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
	change.type = FACTORY_VGROUP_DEL;
	CallDeviceChange(change);
	return TRUE;
}



inline BOOL Factory::GetDeviceRtspUrl(astring & strUrl, s32 nIndex)
{
	BOOL ret = FALSE;
	Lock();
	if (m_DeviceMap[nIndex] != NULL)
	{
	    ret = m_DeviceMap[nIndex]->GetDeviceRtspUrl(strUrl);
	}
	UnLock();

	return ret;
}

inline s32 Factory::GetDeviceParamById(DeviceParam & pParam, s32 nIndex)
{
    //VDC_DEBUG( "%s GetDeviceParamById %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= FACTORY_DEVICE_ID_MAX)
    {
        return FALSE;
    }

    Lock();
    pParam = m_DeviceParamMap[nIndex];
    UnLock();

    return TRUE;
}

inline s32 Factory::GetDeviceParamByIdTryLock(DeviceParam & pParam, s32 nIndex)
{
    //VDC_DEBUG( "%s GetDeviceParamById %d\n",__FUNCTION__, nIndex);
    if (nIndex <=0 || nIndex >= FACTORY_DEVICE_ID_MAX)
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
    pParam = m_DeviceParamMap[nIndex];
    UnLock();

    return TRUE;
}

inline s32 Factory::GetDeviceID(void)
{
    s32 id  = -1;
    s32 i = -1;

    Lock();
    for (i = 1; i < FACTORY_DEVICE_ID_MAX; i ++)
    {
        if (m_strDeviceMap[i] == 'n')
        {
            id = i;
            m_strDeviceMap[i] = 'y';
            UnLock();
            return id;
        }
    }

    UnLock();
    return id;
}

inline BOOL Factory::ReleaseDeviceID(s32 nID)
{
    if (nID <=0 || nID >= FACTORY_DEVICE_ID_MAX)
    {
        return FALSE;
    }
    Lock();
    m_strDeviceMap[nID] = 'n';
    UnLock();

    return TRUE;
}

inline BOOL Factory::LockDeviceID(s32 nID)
{
    if (nID <=0 || nID >= FACTORY_DEVICE_ID_MAX)
    {
        return FALSE;
    }
    Lock();
    m_strDeviceMap[nID] = 'y';
    UnLock();

    return TRUE;
}

inline s32 Factory::GetVIPCID(void)
{
    s32 id  = -1;
    s32 i = -1;

    Lock();
    for (i = 1; i < FACTORY_DEVICE_ID_MAX; i ++)
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
    if (nID <=0 || nID >= FACTORY_DEVICE_ID_MAX)
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
    if (nID <=0 || nID >= FACTORY_DEVICE_ID_MAX)
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
    int lastIdx = 0;
    /* Create the thread to update the Disk status */
    //First updateDisk Status and then start the thread to update the disk status
    while (1)
    {

        Lock();
        DeviceMap::iterator it = m_DeviceMap.begin(); 
        for(; it!=m_DeviceMap.end(); ++it)
        {
            s32 nIndex = (*it).first;
            DeviceMap::iterator next = it;
	     next ++;
	     //VDC_DEBUG( "%s run CheckDevice %d   1\n",__FUNCTION__, nIndex);
	     
	     if (lastIdx +1 > nIndex  && next != m_DeviceMap.end())
	     {
			continue;
	     }
            if (next == m_DeviceMap.end())
            {
            	  lastIdx = 0; 
            }else
            {
            	 lastIdx = nIndex;
            }
	    //VDC_DEBUG( "%s run CheckDevice %d   2\n",__FUNCTION__, nIndex);
	     if (m_DeviceMap[nIndex])
	     {
	        FactoryDeviceChangeData change;
		 change.id = nIndex;
		switch (m_DeviceMap[nIndex]->CheckDevice())
		{
		    case DEV_OFF2ON:
		    {
		        change.type = FACTORY_DEVICE_ONLINE;
			 UnLock(); 
			 CallDeviceChange(change);
			 Lock();
		        break;
		    }
		    case DEV_ON2OFF:
		    {
		        change.type = FACTORY_DEVICE_OFFLINE;
			 UnLock(); 
			 CallDeviceChange(change);
			 Lock();
		        break;
		    }
		    default:
		    {

		        break;
		    }
		}
	     }
	     break;
        }   
        UnLock();
#ifdef WIN32
        Sleep(1000 * 2);
#else
	sleep(2);
#endif
    }
}




#endif // __VSC_FACTORY_H_
