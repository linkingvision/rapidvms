

#ifndef _CONF_DB_IMPL_H_
#define _CONF_DB_IMPL_H_


using namespace std;

#define VSC_OAPI_DEFAULT_PORT 9080

inline void VSCHdfsRecordDefault(VidHDFSConf &pData)
{
	pData.set_strnamenode("localhost");
	pData.set_strport("8020");
	pData.set_struser("admin");
	pData.set_strpasswd("admin");
	pData.set_nfileinterval(30);
}

inline void VSCStorServerConfDefault(VidStorServerConf &pData)
{
	SimpleCrypt crypt;
	pData.set_noapiport(VSC_OAPI_DEFAULT_PORT);
	/* Default passwd is admin */
	QString strPass = "admin";
	pData.set_stradminpasswd(crypt.encryptToString(strPass).toStdString());

	return;
}

#if 0
inline BOOL SysConfDataDefault(VSCConfData &pConf)
{
	pConf.data.conf.DeviceNum = 0;
	for (s32 i = 0; i < CONF_MAP_MAX; i ++)
	{
		pConf.data.conf.DeviceMap[i] = CONF_MAP_INVALID_MIN;
	}
	for (s32 i = 0; i < CONF_MAP_MAX; i ++)
	{
		pConf.data.conf.VIPCMap[i] = CONF_MAP_INVALID_MIN;
	}

	pConf.data.conf.RTSPServerPort = VSC_RTSP_DEFAULT_PORT;
	strcpy(pConf.data.conf.MulticastStartIPV4, "226.0.0.1");
	pConf.data.conf.OAPIPort = VSC_OAPI_DEFAULT_PORT;
	pConf.data.conf.VHTTPServerPort = VSC_VHTTPS_DEFAULT_PORT;
	pConf.data.conf.VHLSServerPort = VSC_VHLSS_DEFAULT_PORT;
	pConf.data.conf.VHTTPSSLServerPort = VSC_VHTTPS_SSL_DEFAULT_PORT;

    return TRUE;
}

inline BOOL VmsConfDataDefault(VSCVmsData &pConf)
{
	memset(&pConf, 0, sizeof(VSCVmsData));

	for (s32 i = 0; i < CONF_VMS_NUM_MAX; i ++)
	{
	    	pConf.data.conf.vms[i].nId = i;
		pConf.data.conf.vms[i].Used = 0;
	}

    return TRUE;
}

inline BOOL ViewConfDataDefault(VSCViewData &pConf)
{
	memset(&pConf, 0, sizeof(VSCViewData));

	for (s32 i = 0; i < CONF_VIEW_NUM_MAX; i ++)
	{
	    	pConf.data.conf.view[i].nId = i;
		pConf.data.conf.view[i].Used = 0;
	}

    return TRUE;
}

inline BOOL VGroupConfDataDefault(VSCVGroupData &pConf)
{
	memset(&pConf, 0, sizeof(VSCVGroupData));

	for (s32 i = 0; i < CONF_VGROUP_NUM_MAX; i ++)
	{
	    	pConf.data.conf.group[i].nId = i;
			pConf.data.conf.group[i].Used = 0;
	}

    return TRUE;
}
#endif
inline s32 ConfDB::Open(astring & pPath)
{
	m_Options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(m_Options, pPath, &m_pDb);
	if (false == status.ok())
	{
	    //cerr << "Unable to open/create test database "<< pPath << endl;
	    //cerr << status.ToString() << endl;
	    VDC_DEBUG( "Unable to open/create test database %s\n", pPath.c_str());

	    return FALSE;
	}

	astring fakeKey = "fakeKey";
	astring fakeValue = "fakeValue";
	SetCmnParam(fakeKey, fakeValue);
    	return TRUE;
}


inline bool ConfDB::FindCamera(astring strCameraId)
{
	XGuard guard(m_cMutex);
	VidCameraList cameraList;
	GetCameraListConf(cameraList);
	int cameraSize = cameraList.cvidcamera_size();

	for (s32 i = 0; i < cameraList.cvidcamera_size(); i ++)
	{
		const VidCamera &cam = cameraList.cvidcamera(i);
		if (cam.strid() == strCameraId)
		{
			return true;
		}
	}

	return false;
}

inline bool ConfDB::DeleteCamera(astring strCameraId)
{
	XGuard guard(m_cMutex);
	VidCameraList cameraList;
	VidCameraList cameraListNew;
	GetCameraListConf(cameraList);
	int cameraSize = cameraList.cvidcamera_size();

	for (s32 i = 0; i < cameraList.cvidcamera_size(); i ++)
	{
		const VidCamera &cam = cameraList.cvidcamera(i);
		if (cam.strid() != strCameraId)
		{
			VidCamera *pCam = cameraListNew.add_cvidcamera();
			*pCam = cam;
		}
	}
	UpdateCameraListConf(cameraListNew);
	return true;
}
	
inline bool ConfDB::AddCamera(VidCamera &pAddCam)
{
	XGuard guard(m_cMutex);
	
	VidCameraList cameraList;
	GetCameraListConf(cameraList);
	int cameraSize = cameraList.cvidcamera_size();

	VidCamera *pCam = cameraList.add_cvidcamera();
	*pCam = pAddCam;
	UpdateCameraListConf(cameraList);
	return true;
}

inline bool ConfDB::GetCameraConf(astring strCameraId, VidCamera &pCam)
{
	XGuard guard(m_cMutex);
	
	VidCameraList cameraList;
	GetCameraListConf(cameraList);
	int cameraSize = cameraList.cvidcamera_size();

	for (s32 i = 0; i < cameraList.cvidcamera_size(); i ++)
	{
		const VidCamera &cam = cameraList.cvidcamera(i);
		if (cam.strid() == strCameraId)
		{
			pCam = cam;
			return true;
		}
	}

	return false;
}

inline bool ConfDB::CameraRecordTemplSet(astring strCameraId, astring strTempl)
{
	XGuard guard(m_cMutex);
	
	return true;
}

inline   BOOL ConfDB::GetLicense(astring &strLicense)
{
	XGuard guard(m_cMutex);

	VSCConfLicenseKey sLicKey;
	

	leveldb::Slice key((char *)&sLicKey, sizeof(sLicKey));


	leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

	it->Seek(key);
	leveldb::Slice sysValue;

	if (it->Valid())
	{
		sysValue = it->value();
		strLicense = sysValue.ToString();
	}
	if (strLicense.length() < 10)
	{
		strLicense = "";
	}
	// Check for any errors found during the scan
	assert(it->status().ok());
	delete it;

	return TRUE;

}
inline   BOOL ConfDB::SetLicense(astring &strLicense)
{
	XGuard guard(m_cMutex);
	
	VSCConfLicenseKey sLic;
	leveldb::WriteOptions writeOptions;

	leveldb::Slice licKey((char *)&sLic, sizeof(sLic));
	leveldb::Slice licValue(strLicense);
	m_pDb->Put(writeOptions, licKey, licValue);
	return TRUE;
    
}

inline BOOL ConfDB::GetCmnParam(astring &strKey, astring &strParam)
{
	XGuard guard(m_cMutex);
	
	leveldb::Slice key(strKey);

	leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

	it->Seek(key);
	leveldb::Slice sysValue;

	if (it->Valid())
	{
		sysValue = it->value();
		strParam = sysValue.ToString();
	}else
	{
		delete it;
		return FALSE;
	}

	// Check for any errors found during the scan
	assert(it->status().ok());
	delete it;

	return TRUE;
}

inline BOOL ConfDB::SetCmnParam(astring &strKey, astring &strParam)
{
	XGuard guard(m_cMutex);
	
	leveldb::WriteOptions writeOptions;

	leveldb::Slice licKey(strKey);
	leveldb::Slice licValue(strParam);
	m_pDb->Put(writeOptions, licKey, licValue);
	return TRUE;
}



inline BOOL ConfDB::GetHdfsRecordConf(VidHDFSConf &pData)
{
	XGuard guard(m_cMutex);
	
	VSCConfHdfsRecordKey sKey;

	//pData.ParseFromString();
	//pData.SerializeToString();

	leveldb::Slice key((char *)&sKey, sizeof(sKey));


	leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

	it->Seek(key);
	leveldb::Slice sysValue("fake");

	if (it->Valid())
	{
	    sysValue = it->value();
	}

	if (pData.ParseFromString(sysValue.ToString()) == false)
	{
		VDC_DEBUG( "Hdfs Record Config is not init\n");
		delete it;
		VSCHdfsRecordDefault(pData);
		UpdateHdfsRecordConf(pData);
		return TRUE;
	}

	// Check for any errors found during the scan
	assert(it->status().ok());
	delete it;

	return TRUE;

}

/* HDFS record  */
inline BOOL ConfDB::UpdateHdfsRecordConf(VidHDFSConf &pData)
{
	XGuard guard(m_cMutex);
	
	VSCConfHdfsRecordKey sKey;

	leveldb::WriteOptions writeOptions;

	leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));

	astring strOutput;
	if (pData.SerializeToString(&strOutput) != TRUE)
	{
		return FALSE;
	}
	leveldb::Slice sysValue(strOutput);

	m_pDb->Put(writeOptions, sysKey, sysValue);

	return TRUE;
}

inline BOOL ConfDB::GetCameraListConf(VidCameraList &pData)
{
	XGuard guard(m_cMutex);
	
	VSCConfCameraKey sKey;

	leveldb::Slice key((char *)&sKey, sizeof(sKey));


	leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

	it->Seek(key);
	leveldb::Slice sysValue("fake");
	

	if (it->status().IsNotFound() != true)
	{
	    sysValue = it->value();
	}

	if (pData.ParseFromString(sysValue.ToString()) == false)
	{
		VidCameraList listDefault;
		pData = listDefault;
		VDC_DEBUG( "Camera List Config is not init\n");
		delete it;
		return TRUE;
	}

	// Check for any errors found during the scan
	assert(it->status().ok());
	delete it;

	return TRUE;

}
inline BOOL ConfDB::UpdateCameraListConf(VidCameraList &pData)
{
	XGuard guard(m_cMutex);
	
	VSCConfCameraKey sKey;

	leveldb::WriteOptions writeOptions;

	leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));

	astring strOutput;
	if (pData.SerializeToString(&strOutput) != TRUE)
	{
		return FALSE;
	}
	leveldb::Slice sysValue(strOutput);

	m_pDb->Put(writeOptions, sysKey, sysValue);

	return TRUE;
}

inline bool ConfDB::GetStorServerConf(VidStorServerConf &pData)
{
	VSCConfStorServerKey sKey;
	
	XGuard guard(m_cMutex);

	leveldb::Slice key((char *)&sKey, sizeof(sKey));


	leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

	it->Seek(key);
	leveldb::Slice sysValue("fake");
	

	if (it->status().IsNotFound() != true)
	{
	    sysValue = it->value();
	}

	if (pData.ParseFromString(sysValue.ToString()) == false)
	{
		VSCStorServerConfDefault(pData);
		VDC_DEBUG( "Stor Server Config is not init\n");
		delete it;
		return true;
	}

	// Check for any errors found during the scan
	assert(it->status().ok());
	delete it;

	return true;
}
inline bool ConfDB::SetStorServerConf(VidStorServerConf &pData)
{
	VSCConfStorServerKey sKey;

	XGuard guard(m_cMutex);

	leveldb::WriteOptions writeOptions;

	leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));

	astring strOutput;
	if (pData.SerializeToString(&strOutput) != TRUE)
	{
		return FALSE;
	}
	leveldb::Slice sysValue(strOutput);

	m_pDb->Put(writeOptions, sysKey, sysValue);

	return true;
}

inline bool ConfDB::GetRecSched(astring strId, RecordSchedWeek &pSched)
{
	if (strId == REC_SCHED_OFF)
	{
		pSched = RecordSchedWeek::CreateOff();
		return true;
	}

	if (strId == REC_SCHED_ALL_DAY)
	{
		pSched = RecordSchedWeek::CreateAllDay();
		return true;
	}

	if (strId == REC_SCHED_WORK_DAY)
	{
		pSched = RecordSchedWeek::CreateWorkDay();
		return true;
	}

	return false;
}

#if 0
inline BOOL ConfDB::GetSystemConf(VSCConfData &pSys)
{
    VSCConfSystemKey sSysKey;

    leveldb::Slice key((char *)&sSysKey, sizeof(sSysKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCConfData))
    {
        VDC_DEBUG( "System Config is not init\n");
        delete it;
        memset(&pSys, 0, sizeof(VSCConfData));
        SysConfDataDefault(pSys);
        UpdateSysData(pSys);
        astring strLicense = " ";
        SetLicense(strLicense);//set the default license
        /* Call get system again */
        return TRUE;
    }

    memcpy(&pSys, sysValue.data(), sizeof(VSCConfData));
	if (pSys.data.conf.RTSPServerPort == 0)
	{
		pSys.data.conf.RTSPServerPort = VSC_RTSP_DEFAULT_PORT;
	}
	if (pSys.data.conf.OAPIPort == 0)
	{
		pSys.data.conf.OAPIPort = VSC_OAPI_DEFAULT_PORT;
	}
	if (pSys.data.conf.VHTTPServerPort == 0)
	{
		pSys.data.conf.VHTTPServerPort = VSC_VHTTPS_DEFAULT_PORT;
	}
	if (pSys.data.conf.VHLSServerPort == 0)
	{
		pSys.data.conf.VHLSServerPort = VSC_VHLSS_DEFAULT_PORT;
	}
	if (pSys.data.conf.VHTTPSSLServerPort == 0)
	{
		pSys.data.conf.VHTTPSSLServerPort = VSC_VHTTPS_SSL_DEFAULT_PORT;
	}

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return TRUE;

}

inline BOOL ConfDB::GetVmsConf(VSCVmsData &pVms)
{
    VSCConfVmsKey sVmsKey;

    leveldb::Slice key((char *)&sVmsKey, sizeof(sVmsKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCVmsData))
    {
        VDC_DEBUG( "VMS Config is not init\n");
        delete it;
        memset(&pVms, 0, sizeof(VSCVmsData));
        VmsConfDataDefault(pVms);
        UpdateVmsData(pVms);
        /* Call get system again */
        return TRUE;
    }

    memcpy(&pVms, sysValue.data(), sizeof(VSCVmsData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return TRUE;

}

inline BOOL ConfDB::GetViewConf(VSCViewData &pView)
{
    VSCConfViewKey sViewKey;

    leveldb::Slice key((char *)&sViewKey, sizeof(sViewKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCViewData))
    {
        VDC_DEBUG( "View Config is not init\n");
        delete it;
        memset(&pView, 0, sizeof(VSCViewData));
        ViewConfDataDefault(pView);
        UpdateViewData(pView);
        /* Call get system again */
        return TRUE;
    }

    memcpy(&pView, sysValue.data(), sizeof(VSCViewData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return TRUE;

}


inline BOOL ConfDB::GetVGroupConf(VSCVGroupData &pVGroup)
{
    VSCConfVGroupKey sVGroupKey;

    leveldb::Slice key((char *)&sVGroupKey, sizeof(sVGroupKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCVGroupData))
    {
        VDC_DEBUG( "Group Config is not init\n");
        delete it;
		memset(&pVGroup, 0, sizeof(VSCVGroupData));
		VGroupConfDataDefault(pVGroup);
		UpdateVGroupData(pVGroup);
        /* Call get system again */
        return TRUE;
    }

	memcpy(&pVGroup, sysValue.data(), sizeof(VSCVGroupData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return TRUE;

}

inline BOOL ConfDB::GetUserConf(VSCUserData &pData)
{
    VSCConfUserKey sKey;

    leveldb::Slice key((char *)&sKey, sizeof(sKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCUserData))
    {
        VDC_DEBUG( "User Config is not init\n");
        delete it;
	memset(&pData, 0, sizeof(VSCUserData));
	VSCUserDataItemDefault(pData.data.conf);
	UpdateUserData(pData);
        /* Call get system again */
        return TRUE;
    }

	memcpy(&pData, sysValue.data(), sizeof(VSCUserData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return TRUE;

}

inline BOOL ConfDB::GetTourConf(VSCTourData &pData)
{
    VSCConfTourKey sKey;

    leveldb::Slice key((char *)&sKey, sizeof(sKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCTourData))
    {
        VDC_DEBUG( "Tour Config is not init\n");
        delete it;
	memset(&pData, 0, sizeof(VSCTourData));
	VSCTourDataDefault(pData.data.conf);
	UpdateTourData(pData);
        /* Call get system again */
        return TRUE;
    }

	memcpy(&pData, sysValue.data(), sizeof(VSCTourData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return TRUE;

}


inline BOOL ConfDB::GetEmapConf(VSCEmapData &pData)
{
    VSCConfEmapKey sKey;

    leveldb::Slice key((char *)&sKey, sizeof(sKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCEmapData))
    {
        VDC_DEBUG( "User Config is not init\n");
        delete it;
	memset(&pData, 0, sizeof(VSCEmapData));
	VSCEmapDataDefault(pData.data.conf);
	UpdateEmapData(pData);
        /* Call get system again */
        return TRUE;
    }

	memcpy(&pData, sysValue.data(), sizeof(VSCEmapData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return TRUE;

}


inline s32 ConfDB::UpdateSysData(VSCConfData &pSysData)
{
    VSCConfSystemKey sSysKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sSysKey, sizeof(sSysKey));
    leveldb::Slice sysValue((char *)&pSysData, sizeof(VSCConfData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return TRUE;
}

inline s32 ConfDB::GetSysData(VSCConfData &pSysData)
{
    GetSystemConf(pSysData);

    return TRUE;
}

inline BOOL ConfDB::UpdateDeviceData(u32 nId, VSCDeviceData &pData)
{
    VSCConfDeviceKey sChKey(nId);
    leveldb::WriteOptions writeOptions;

    leveldb::Slice chKey((char *)&sChKey, sizeof(sChKey));
    leveldb::Slice chValue((char *)&pData, sizeof(VSCDeviceData));
    m_pDb->Put(writeOptions, chKey, chValue);

    return TRUE;
}

inline BOOL ConfDB::GetDeviceData(u32 nId, VSCDeviceData &pData)
{
    VSCConfDeviceKey sChKey(nId);

    leveldb::Slice key((char *)&sChKey, sizeof(sChKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCDeviceData))
    {
        VDC_DEBUG( "Device Can not find !!!\n");

        delete it;
        return FALSE;
    }

    memcpy(&pData, sysValue.data(), sizeof(VSCDeviceData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return TRUE;
}

inline BOOL ConfDB::UpdateVIPCData(u32 nId, VSCVIPCData &pData)
{
    VSCConfVIPCKey sChKey(nId);
    leveldb::WriteOptions writeOptions;

    leveldb::Slice chKey((char *)&sChKey, sizeof(sChKey));
    leveldb::Slice chValue((char *)&pData, sizeof(VSCDeviceData));
    m_pDb->Put(writeOptions, chKey, chValue);

    return TRUE;
}

inline BOOL ConfDB::GetVIPCData(u32 nId, VSCVIPCData &pData)
{
    VSCConfVIPCKey sChKey(nId);

    leveldb::Slice key((char *)&sChKey, sizeof(sChKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCVIPCData))
    {
        VDC_DEBUG( "Device Can not find !!!\n");

        delete it;
        return FALSE;
    }

    memcpy(&pData, sysValue.data(), sizeof(VSCVIPCData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return TRUE;
}



inline s32 ConfDB::GetVmsData(VSCVmsData &pVmsData)
{
	GetVmsConf(pVmsData);
	
	return TRUE;
}
inline s32 ConfDB::UpdateVmsData(VSCVmsData &pVmsData)
{
    VSCConfVmsKey sVmsKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sVmsKey, sizeof(sVmsKey));
    leveldb::Slice sysValue((char *)&pVmsData, sizeof(VSCVmsData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return TRUE;
}

inline s32 ConfDB::GetViewData(VSCViewData &pViewData)
{
	GetViewConf(pViewData);
	
	return TRUE;
}
inline s32 ConfDB::UpdateViewData(VSCViewData &pViewData)
{
    VSCConfViewKey sViewKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sViewKey, sizeof(sViewKey));
    leveldb::Slice sysValue((char *)&pViewData, sizeof(VSCViewData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return TRUE;
}

/* Camera Group  */
inline s32 ConfDB::GetVGroupData(VSCVGroupData &pGroupData)
{
	GetVGroupConf(pGroupData);
	
	return TRUE;
}
/* Camera Group  */
inline s32 ConfDB::UpdateVGroupData(VSCVGroupData &pVGroupData)
{
    VSCConfVGroupKey sVGroupKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sVGroupKey, sizeof(sVGroupKey));
    leveldb::Slice sysValue((char *)&pVGroupData, sizeof(VSCVGroupData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return TRUE;
}

/* HDFS record  */
inline s32 ConfDB::GetHdfsRecordData(VSCHdfsRecordData &pData)
{
	GetHdfsRecordConf(pData);
	
	return TRUE;
}

/* User  */
inline s32 ConfDB::GetUserData(VSCUserData &pData)
{
	GetUserConf(pData);
	
	return TRUE;
}

inline s32 ConfDB::UpdateUserData(VSCUserData &pData)
{
    VSCConfUserKey sKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));
    leveldb::Slice sysValue((char *)&pData, sizeof(VSCUserData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return TRUE;
}

/* Tour  */
inline s32 ConfDB::GetTourData(VSCTourData &pData)
{
	GetTourConf(pData);
	
	return TRUE;
}

inline s32 ConfDB::UpdateTourData(VSCTourData &pData)
{
    VSCConfTourKey sKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));
    leveldb::Slice sysValue((char *)&pData, sizeof(VSCTourData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return TRUE;
}


/* Emap */
inline s32 ConfDB::GetEmapData(VSCEmapData &pData)
{
	GetEmapConf(pData);
	
	return TRUE;
}
inline s32 ConfDB::UpdateEmapData(VSCEmapData &pData)
{
    VSCConfEmapKey sKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));
    leveldb::Slice sysValue((char *)&pData, sizeof(VSCEmapData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return TRUE;
}

/* Emap file Get & Set */
inline   BOOL ConfDB::GetEmapFile(astring &strFile)
{
	BOOL ret = FALSE;
	VSCConfEmapFileKey sMapKey;
	

	leveldb::Slice key((char *)&sMapKey, sizeof(sMapKey));


	leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

	it->Seek(key);
	leveldb::Slice sysValue;

	if (it->Valid())
	{
		sysValue = it->value();
		strFile = sysValue.ToString();
		ret = TRUE;
	}
	
	// Check for any errors found during the scan
	assert(it->status().ok());
	delete it;

	return ret;

}

inline   BOOL ConfDB::SetEmapFile(astring &strFile)
{
	VSCConfEmapFileKey sMapKey;
	leveldb::WriteOptions writeOptions;

	leveldb::Slice Key((char *)&sMapKey, sizeof(sMapKey));
	leveldb::Slice Value(strFile);
	m_pDb->Put(writeOptions, Key, Value);
	return TRUE;
    
}

inline s32 ConfDB::AddDevice(VSCDeviceData &pData, u32 nId)
{
    if (nId > CONF_MAP_MAX)
    {
        return FALSE;
    }
    VSCConfData SysData;
    GetSysData(SysData);

    SysData.data.conf.DeviceMap[nId] = nId;
    SysData.data.conf.DeviceNum ++;
    pData.data.conf.nId = nId;

    UpdateSysData(SysData);

    UpdateDeviceData(nId, pData);


    return TRUE;
}

inline s32 ConfDB::DelDevice(u32 nId)
{
    if (nId > CONF_MAP_MAX)
    {
        return FALSE;
    }

    VSCConfData SysData;
    GetSysData(SysData);

    SysData.data.conf.DeviceMap[nId] = CONF_MAP_INVALID_MIN;

    UpdateSysData(SysData);

    return TRUE;
}

inline s32 ConfDB::AddVIPC(VSCVIPCData &pData, u32 nId)
{
    if (nId > CONF_MAP_MAX)
    {
        return FALSE;
    }
    VSCConfData SysData;
    GetSysData(SysData);

    SysData.data.conf.VIPCMap[nId] = nId;
    SysData.data.conf.VIPCNum ++;
    pData.data.conf.nId = nId;

    UpdateSysData(SysData);

    UpdateVIPCData(nId, pData);


    return TRUE;
}

inline s32 ConfDB::DelVIPC(u32 nId)
{
    if (nId > CONF_MAP_MAX)
    {
        return FALSE;
    }

    VSCConfData SysData;
    GetSysData(SysData);

    SysData.data.conf.VIPCMap[nId] = CONF_MAP_INVALID_MIN;

    UpdateSysData(SysData);

    return TRUE;
}

#endif


#endif /* _CONF_DB_H_ */
