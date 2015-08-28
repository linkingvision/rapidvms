

#ifndef _CONF_DB_IMPL_H_
#define _CONF_DB_IMPL_H_


using namespace std;

#define VSC_RTSP_DEFAULT_PORT 9554
#define VSC_OAPI_DEFAULT_PORT 9080
#define VSC_VHTTPS_DEFAULT_PORT 9000
#define VSC_VHLSS_DEFAULT_PORT 9001

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
    return TRUE;
}

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

inline BOOL ConfDB::GetHdfsRecordConf(VSCHdfsRecordData &pData)
{
    VSCConfHdfsRecordKey sKey;

    leveldb::Slice key((char *)&sKey, sizeof(sKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCHdfsRecordData))
    {
        VDC_DEBUG( "Hdfs Record Config is not init\n");
        delete it;
		memset(&pData, 0, sizeof(VSCHdfsRecordData));
		VSCHdfsRecordDataItemDefault(pData.data.conf);
		UpdateHdfsRecordData(pData);
        /* Call get system again */
        return TRUE;
    }

	memcpy(&pData, sysValue.data(), sizeof(VSCHdfsRecordData));

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

/* HDFS record  */
inline s32 ConfDB::UpdateHdfsRecordData(VSCHdfsRecordData &pData)
{
    VSCConfHdfsRecordKey sKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));
    leveldb::Slice sysValue((char *)&pData, sizeof(VSCHdfsRecordData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

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

inline   BOOL ConfDB::GetLicense(astring &strLicense)
{
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
	
	// Check for any errors found during the scan
	assert(it->status().ok());
	delete it;

	return TRUE;

}
inline   BOOL ConfDB::SetLicense(astring &strLicense)
{
	VSCConfLicenseKey sLic;
	leveldb::WriteOptions writeOptions;

	leveldb::Slice licKey((char *)&sLic, sizeof(sLic));
	leveldb::Slice licValue(strLicense);
	m_pDb->Put(writeOptions, licKey, licValue);
	return TRUE;
    
}

inline BOOL ConfDB::GetCmnParam(astring &strKey, astring &strParam)
{
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
	leveldb::WriteOptions writeOptions;

	leveldb::Slice licKey(strKey);
	leveldb::Slice licValue(strParam);
	m_pDb->Put(writeOptions, licKey, licValue);
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




#endif /* _CONF_DB_H_ */
