#ifndef _CLIENT_CONF_DB_IMPL_H_
#define _CLIENT_CONF_DB_IMPL_H_


using namespace std;

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

inline void VSCClientConfDefault(VidClientConf &pData)
{
	SimpleCrypt crypt;
	pData.set_nlang(VID_LANG_AUTO);
	pData.set_bautologin(false);
	/* Default passwd is admin */
	QString strPass = "admin";
	pData.set_stradminpasswd(crypt.encryptToString(strPass).toStdString());

	return;
}

inline s32 ClientConfDB::Open(astring & pPath)
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

inline BOOL ClientConfDB::GetCmnParam(astring &strKey, astring &strParam)
{
	XGuard guard(m_cMutex);
	
	leveldb::Slice key(strKey);

	leveldb::Status s = m_pDb->Get(leveldb::ReadOptions(), 
					key,  &strParam);
	if (s.ok() == false)
	{
		strParam = "";
		return FALSE;
	}

	return TRUE;
}

inline BOOL ClientConfDB::SetCmnParam(astring &strKey, astring &strParam)
{
	leveldb::WriteOptions writeOptions;
	XGuard guard(m_cMutex);

	leveldb::Slice licKey(strKey);
	leveldb::Slice licValue(strParam);
	m_pDb->Put(writeOptions, licKey, licValue);
	return TRUE;
}

inline   bool ClientConfDB::GetLicense(astring &strLicense)
{
	XGuard guard(m_cMutex);

	VSCConfLicenseKey sLicKey;
	

	leveldb::Slice key((char *)&sLicKey, sizeof(sLicKey));

	leveldb::Status s = m_pDb->Get(leveldb::ReadOptions(), 
					key,  &strLicense);
	if (s.ok() == false)
	{
		strLicense = "";
		return false;
	}

	return true;

}
inline   bool ClientConfDB::SetLicense(astring &strLicense)
{
	XGuard guard(m_cMutex);
	
	VSCConfLicenseKey sLic;
	leveldb::WriteOptions writeOptions;

	leveldb::Slice licKey((char *)&sLic, sizeof(sLic));
	leveldb::Slice licValue(strLicense);
	m_pDb->Put(writeOptions, licKey, licValue);
	return true;
    
}

inline bool ClientConfDB::GetClientConf(VidClientConf &pData)
{
	VSCConfClientKey sKey;
	astring strValue;
	
	XGuard guard(m_cMutex);

	leveldb::Slice key((char *)&sKey, sizeof(sKey));

	leveldb::Status s = m_pDb->Get(leveldb::ReadOptions(), 
					key,  &strValue);
	if (s.ok() == false)
	{
		strValue = "fake";
	}

	if (pData.ParseFromString(strValue) == false)
	{
		VSCClientConfDefault(pData);
		VDC_DEBUG( "Client Config is not init\n");
		return true;
	}

	return true;
}
inline bool ClientConfDB::SetClientConf(VidClientConf &pData)
{
	VSCConfClientKey sKey;

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

inline bool ClientConfDB::FindStor(astring strStorId)
{
	XGuard guard(m_cMutex);
	
	VidStorList storList;
	GetStorListConf(storList);
	int storSize = storList.cvidstor_size();

	for (s32 i = 0; i < storList.cvidstor_size(); i ++)
	{
		const VidStor &stor = storList.cvidstor(i);
		if (stor.strid() == strStorId)
		{
			return true;
		}
	}

	return false;
}
inline bool ClientConfDB::DeleteStor(astring strStorId)
{
	XGuard guard(m_cMutex);

	VidStorList storList;
	VidStorList storListNew;
	GetStorListConf(storList);
	int storSize = storList.cvidstor_size();

	for (s32 i = 0; i < storList.cvidstor_size(); i ++)
	{
		const VidStor &stor = storList.cvidstor(i);
		if (stor.strid() != strStorId)
		{
			VidStor *pAddStor = storListNew.add_cvidstor();
			*pAddStor = stor;
		}
	}

	UpdateStorListConf(storListNew);
	return true;
}
inline bool ClientConfDB::AddStor(VidStor &pStor)
{
	XGuard guard(m_cMutex);
	
	VidStorList storList;

	GetStorListConf(storList);
	
	VidStor *pAddStor = storList.add_cvidstor();
	*pAddStor = pStor;
	UpdateStorListConf(storList);

	return true;
}



inline BOOL ClientConfDB::GetStorListConf(VidStorList &pData)
{
	VSCConfVidStorKey sKey;
	astring strValue;
	
	XGuard guard(m_cMutex);

	leveldb::Slice key((char *)&sKey, sizeof(sKey));

	leveldb::Status s = m_pDb->Get(leveldb::ReadOptions(), 
					key,  &strValue);
	if (s.ok() == false)
	{
		strValue = "fake";
	}

	if (pData.ParseFromString(strValue) == false)
	{
		VidStorList listDefault;
		pData = listDefault;
		VDC_DEBUG( "Stor List Config is not init\n");
		return TRUE;
	}

	return TRUE;

}
inline BOOL ClientConfDB::UpdateStorListConf(VidStorList &pData)
{
	VSCConfVidStorKey sKey;

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

	return TRUE;
}

inline bool ClientConfDB::GetStorConf(astring strId, VidStor &pStor)
{
	XGuard guard(m_cMutex);
	
	VidStorList storList;
	GetStorListConf(storList);
	int storSize = storList.cvidstor_size();

	for (s32 i = 0; i < storList.cvidstor_size(); i ++)
	{
		const VidStor &stor = storList.cvidstor(i);
		if (stor.strid() == strId)
		{
			pStor = stor;
			return true;
		}
	}

	return false;
}

inline bool ClientConfDB::FindView(astring strViewId)
{
	XGuard guard(m_cMutex);
	
	VidViewList viewList;
	GetViewListConf(viewList);
	int viewSize = viewList.cvidview_size();

	for (s32 i = 0; i < viewList.cvidview_size(); i ++)
	{
		const VidView &view = viewList.cvidview(i);
		if (view.strid() == strViewId)
		{
			return true;
		}
	}

	return false;
}
inline bool ClientConfDB::DeleteView(astring strViewId)
{
	XGuard guard(m_cMutex);

	VidViewList viewList;
	VidViewList viewListNew;
	GetViewListConf(viewList);
	int viewSize = viewList.cvidview_size();

	for (s32 i = 0; i < viewList.cvidview_size(); i ++)
	{
		const VidView &view = viewList.cvidview(i);
		if (view.strid() != strViewId)
		{
			VidView *pAddView = viewListNew.add_cvidview();
			*pAddView = view;
		}
	}

	UpdateViewListConf(viewListNew);
	return true;
}
inline bool ClientConfDB::AddView(VidView &pView)
{
	XGuard guard(m_cMutex);
	
	VidViewList viewList;

	GetViewListConf(viewList);
	
	VidView *pAddView = viewList.add_cvidview();
	*pAddView = pView;
	UpdateViewListConf(viewList);

	return true;
}



inline BOOL ClientConfDB::GetViewListConf(VidViewList &pData)
{
	VSCConfViewKey sKey;
	astring strValue;
	
	XGuard guard(m_cMutex);

	leveldb::Slice key((char *)&sKey, sizeof(sKey));

	leveldb::Status s = m_pDb->Get(leveldb::ReadOptions(), 
					key,  &strValue);
	if (s.ok() == false)
	{
		strValue = "fake";
	}

	if (pData.ParseFromString(strValue) == false)
	{
		VidViewList listDefault;
		pData = listDefault;
		VDC_DEBUG( "View List Config is not init\n");
		return TRUE;
	}

	return TRUE;

}
inline BOOL ClientConfDB::UpdateViewListConf(VidViewList &pData)
{
	VSCConfViewKey sKey;

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

	return TRUE;
}

inline bool ClientConfDB::GetViewConf(astring strId, VidView &pView)
{
	XGuard guard(m_cMutex);
	
	VidViewList viewList;
	GetViewListConf(viewList);
	int storSize = viewList.cvidview_size();

	for (s32 i = 0; i < viewList.cvidview_size(); i ++)
	{
		const VidView &view = viewList.cvidview(i);
		if (view.strid() == strId)
		{
			pView = view;
			return true;
		}
	}

	return false;
}

inline bool ClientConfDB::FindTour(astring strTourId)
{
	XGuard guard(m_cMutex);
	
	VidTourList tourList;
	GetTourListConf(tourList);
	int tourSize = tourList.cvidtour_size();

	for (s32 i = 0; i < tourList.cvidtour_size(); i ++)
	{
		const VidTour &tour = tourList.cvidtour(i);
		if (tour.strid() == strTourId)
		{
			return true;
		}
	}

	return false;
}
inline bool ClientConfDB::DeleteTour(astring strTourId)
{
	XGuard guard(m_cMutex);

	VidTourList tourList;
	VidTourList tourListNew;
	GetTourListConf(tourList);
	int tourSize = tourList.cvidtour_size();

	for (s32 i = 0; i < tourList.cvidtour_size(); i ++)
	{
		const VidTour &tour = tourList.cvidtour(i);
		if (tour.strid() != strTourId)
		{
			VidTour *pAddTour = tourListNew.add_cvidtour();
			*pAddTour = tour;
		}
	}

	UpdateTourListConf(tourListNew);
	return true;
}

inline bool ClientConfDB::AddTour(VidTour &pTour)
{
	XGuard guard(m_cMutex);
	
	VidTourList tourList;

	GetTourListConf(tourList);
	
	VidTour *pAddTour = tourList.add_cvidtour();
	*pAddTour = pTour;
	UpdateTourListConf(tourList);

	return true;
}



inline BOOL ClientConfDB::GetTourListConf(VidTourList &pData)
{
	VSCConfTourKey sKey;
	astring strValue;
	
	XGuard guard(m_cMutex);

	leveldb::Slice key((char *)&sKey, sizeof(sKey));

	leveldb::Status s = m_pDb->Get(leveldb::ReadOptions(), 
					key,  &strValue);
	if (s.ok() == false)
	{
		strValue = "fake";
	}

	if (pData.ParseFromString(strValue) == false)
	{
		VidTourList listDefault;
		pData = listDefault;
		VDC_DEBUG( "Tour List Config is not init\n");
		return TRUE;
	}

	return TRUE;

}
inline BOOL ClientConfDB::UpdateTourListConf(VidTourList &pData)
{
	VSCConfTourKey sKey;

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

	return TRUE;
}

inline bool ClientConfDB::GetTourConf(astring strId, VidTour &pTour)
{
	XGuard guard(m_cMutex);
	
	VidTourList tourList;
	GetTourListConf(tourList);
	int tourSize = tourList.cvidtour_size();

	for (s32 i = 0; i < tourList.cvidtour_size(); i ++)
	{
		const VidTour &tour = tourList.cvidtour(i);
		if (tour.strid() == strId)
		{
			pTour = tour;
			return true;
		}
	}

	return false;
}

inline bool ClientConfDB::FindGroup(astring strGroupId)
{
	XGuard guard(m_cMutex);
	
	VidGroupList groupList;
	GetGroupListConf(groupList);
	int groupSize = groupList.cvidgroup_size();

	for (s32 i = 0; i < groupList.cvidgroup_size(); i ++)
	{
		const VidGroup &group = groupList.cvidgroup(i);
		if (group.strid() == strGroupId)
		{
			return true;
		}
	}

	return false;
}
inline bool ClientConfDB::DeleteGroup(astring strGroupId)
{
	XGuard guard(m_cMutex);

	VidGroupList groupList;
	VidGroupList groupListNew;
	GetGroupListConf(groupList);
	int groupSize = groupList.cvidgroup_size();

	for (s32 i = 0; i < groupList.cvidgroup_size(); i ++)
	{
		const VidGroup &group = groupList.cvidgroup(i);
		if (group.strid() != strGroupId)
		{
			VidGroup *pAddGroup = groupListNew.add_cvidgroup();
			*pAddGroup = group;
		}
	}

	UpdateGroupListConf(groupListNew);
	return true;
}
inline bool ClientConfDB::AddGroup(VidGroup &pGroup)
{
	XGuard guard(m_cMutex);
	
	VidGroupList groupList;

	GetGroupListConf(groupList);
	
	VidGroup *pAddGroup = groupList.add_cvidgroup();
	*pAddGroup = pGroup;
	UpdateGroupListConf(groupList);

	return true;
}



inline BOOL ClientConfDB::GetGroupListConf(VidGroupList &pData)
{
	VSCConfVGroupKey sKey;
	astring strValue;
	
	XGuard guard(m_cMutex);

	leveldb::Slice key((char *)&sKey, sizeof(sKey));

	leveldb::Status s = m_pDb->Get(leveldb::ReadOptions(), 
					key,  &strValue);
	if (s.ok() == false)
	{
		strValue = "fake";
	}

	if (pData.ParseFromString(strValue) == false)
	{
		VidGroupList listDefault;
		pData = listDefault;
		VDC_DEBUG( "Group List Config is not init\n");
		return TRUE;
	}

	return TRUE;

}
inline BOOL ClientConfDB::UpdateGroupListConf(VidGroupList &pData)
{
	VSCConfVGroupKey sKey;

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

	return TRUE;
}

inline bool ClientConfDB::GetGroupConf(astring strId, VidGroup &pGroup)
{
	XGuard guard(m_cMutex);
	
	VidGroupList groupList;
	GetGroupListConf(groupList);
	int groupSize = groupList.cvidgroup_size();

	for (s32 i = 0; i < groupList.cvidgroup_size(); i ++)
	{
		const VidGroup &group = groupList.cvidgroup(i);
		if (group.strid() == strId)
		{
			pGroup = group;
			return true;
		}
	}

	return false;
}

inline bool ClientConfDB::FindEmap(astring strEmapId)
{
	XGuard guard(m_cMutex);
	
	VidEmapList emapList;
	GetEmapListConf(emapList);
	int emapSize = emapList.cvidemap_size();

	for (s32 i = 0; i < emapList.cvidemap_size(); i ++)
	{
		const VidEmap &emap = emapList.cvidemap(i);
		if (emap.strid() == strEmapId)
		{
			return true;
		}
	}

	return false;
}
inline bool ClientConfDB::DeleteEmap(astring strEmapId)
{
	XGuard guard(m_cMutex);

	VidEmapList emapList;
	VidEmapList emapListNew;
	GetEmapListConf(emapList);
	int emapSize = emapList.cvidemap_size();

	for (s32 i = 0; i < emapList.cvidemap_size(); i ++)
	{
		const VidEmap &emap = emapList.cvidemap(i);
		if (emap.strid() != strEmapId)
		{
			VidEmap *pAddEmap = emapListNew.add_cvidemap();
			*pAddEmap = emap;
		}
	}

	UpdateEmapListConf(emapListNew);
	return true;
}
inline bool ClientConfDB::AddEmap(VidEmap &pEmap)
{
	XGuard guard(m_cMutex);
	
	VidEmapList emapList;

	GetEmapListConf(emapList);
	
	VidEmap *pAddEmap = emapList.add_cvidemap();
	*pAddEmap = pEmap;
	UpdateEmapListConf(emapList);

	return true;
}



inline BOOL ClientConfDB::GetEmapListConf(VidEmapList &pData)
{
	VSCConfEmapKey sKey;
	astring strValue;
	
	XGuard guard(m_cMutex);

	leveldb::Slice key((char *)&sKey, sizeof(sKey));

	leveldb::Status s = m_pDb->Get(leveldb::ReadOptions(), 
					key,  &strValue);
	if (s.ok() == false)
	{
		strValue = "fake";
	}

	if (pData.ParseFromString(strValue) == false)
	{
		VidEmapList listDefault;
		pData = listDefault;
		VDC_DEBUG( "Emap List Config is not init\n");
		return TRUE;
	}

	return TRUE;

}
inline BOOL ClientConfDB::UpdateEmapListConf(VidEmapList &pData)
{
	VSCConfEmapKey sKey;

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

	return TRUE;
}

inline bool ClientConfDB::GetEmapConf(astring strId, VidEmap &pEmap)
{
	XGuard guard(m_cMutex);
	
	VidEmapList emapList;
	GetEmapListConf(emapList);
	int emapSize = emapList.cvidemap_size();

	for (s32 i = 0; i < emapList.cvidemap_size(); i ++)
	{
		const VidEmap &emap = emapList.cvidemap(i);
		if (emap.strid() == strId)
		{
			pEmap = emap;
			return true;
		}
	}

	return false;
}


#endif /* _CONF_DB_H_ */
