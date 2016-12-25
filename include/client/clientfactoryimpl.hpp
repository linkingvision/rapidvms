//------------------------------------------------------------------------------
// File: clientfactory impl.hpp
//
// Desc: Camera clientfactory - Manage IP Camera.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_CLIENT_FACTORY_IMPL_H_
#define __VSC_CLIENT_FACTORY_IMPL_H_

inline ClientFactory::ClientFactory(VidEnv &pEnv)
:m_env(pEnv)
{
    //astring strTemp = m_env.GetAppConfPath("clientsystem");
    //m_SysPath.Open(strTemp);
	/* Create the stor factory */
    m_StorFactory = new StorFactory(m_Conf);
}

inline ClientFactory::~ClientFactory()
{
	delete m_StorFactory;
}

inline BOOL ClientFactory::SetSystemPath(astring strPath)
{
    //return m_SysPath.SetSystemPath(strPath);
    return TRUE;
}

inline BOOL ClientFactory::GetExportPath(astring &strPath)
{
	astring strKey = "ConfVideoExportKey";

	if (m_Conf.GetCmnParam(strKey, strPath) == FALSE)
	{
		strPath = m_env.GetAppConfPath("export");
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
	astring strPath;
	//if (m_SysPath.GetSystemPath(strPath) == FALSE)
	//{
	//    SetSystemPath(m_env.GetConfDir());
	//}
	printf("Sys path %s\n", strPath.c_str());
	astring strConf = m_env.GetAppConfPath("clientconfig");
	m_Conf.Open(strConf);

	/* Init export path */
	astring strExportPath;
	GetExportPath(strExportPath);

	m_StorFactory->Init();

	InitLicense();
	return TRUE;
}

inline BOOL ClientFactory::CallChange(ClientFactoryChangeData data)
{
#if 0
	//XGuard guard(m_cMutex);
	ChangeNofityMap::iterator it = m_Change.begin(); 
	for(; it!=m_Change.end(); ++it)
	{
		if ((*it).second)
		{
			(*it).second((*it).first, data);
		}
	}
#else
	emit(SignalCallChange(data.type, data.id));
#endif
	 return TRUE;
}

inline BOOL ClientFactory::GetLicense(astring &strLicense, astring &strHostId, int &ch, 
	astring &type, astring &startTime, astring &expireTime)
{
	XGuard guard(m_cMutex);
	VPlay::GetLicenseInfo(strHostId, ch, type, startTime, expireTime);
	return m_Conf.GetLicense(strLicense);
}
inline BOOL ClientFactory::SetLicense(astring &strLicense)
{
	XGuard guard(m_cMutex);
	VPlay::SetLicense(strLicense);
	return m_Conf.SetLicense(strLicense);
}

inline BOOL ClientFactory::InitLicense()
{
	astring strLicense;
	m_Conf.GetLicense(strLicense);
	VPlay::SetLicense(strLicense);
	return TRUE;
}

inline bool ClientFactory::GetAutoLogin()
{
	VidClientConf  cData;
	m_Conf.GetClientConf(cData);

	return cData.bautologin();	
}
inline bool ClientFactory::SetAutoLogin(bool bAutoLogin)
{
	VidClientConf  cData;
	m_Conf.GetClientConf(cData);
	cData.set_bautologin(bAutoLogin);
	m_Conf.SetClientConf(cData);

	return true;	
}

inline bool ClientFactory::GetLang(VidLanguage &nLang)
{
	VidClientConf  cData;
	m_Conf.GetClientConf(cData);
	nLang = cData.nlang();

	return true;
}
inline bool ClientFactory::SetLang(VidLanguage &nLang)
{
	VidClientConf  cData;
	m_Conf.GetClientConf(cData);
	cData.set_nlang(nLang);
	m_Conf.SetClientConf(cData);

	return true;	
}

inline bool ClientFactory::AuthUser(astring &strUser, astring &strPasswd)
{
	/* Admin is a Super User */
	if (strUser == "admin")
	{
		VidClientConf  cData;
		m_Conf.GetClientConf(cData);
		SimpleCrypt crypt;
		astring strDummy = cData.stradminpasswd().c_str();
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
inline bool ClientFactory::GetAdminPasswd(astring &strPasswd)
{
	VidClientConf  cData;
	m_Conf.GetClientConf(cData);
	strPasswd = cData.stradminpasswd();
	
	return true;
}
inline bool ClientFactory::SetAdminPasswd(astring strPasswd)
{
	VidClientConf  cData;
	m_Conf.GetClientConf(cData);
	cData.set_stradminpasswd(strPasswd);
	m_Conf.SetClientConf(cData);

	return true;	
}

inline bool ClientFactory::AddView(VidView &pView)
{
	ClientFactoryChangeData data;
	bool ret = m_Conf.AddView(pView);

	data.type = CLIENT_FACTORY_VIEW_ADD;
	data.id = pView.strid();
	CallChange(data);
	return ret;
}
inline bool ClientFactory::DelView(astring strId)
{
	ClientFactoryChangeData data;
	bool ret = m_Conf.DeleteView(strId);

	data.type = CLIENT_FACTORY_VIEW_DEL;
	data.id = strId;
	CallChange(data);
	return ret;
}

inline bool ClientFactory::AddEmap(VidEmap &pEmap)
{
	ClientFactoryChangeData data;
	bool ret = m_Conf.AddEmap(pEmap);

	data.type = CLIENT_FACTORY_EMAP_ADD;
	data.id = pEmap.strid();
	CallChange(data);
	return ret;
}
inline bool ClientFactory::DelEmap(astring strId)
{
	ClientFactoryChangeData data;
	bool ret = m_Conf.DeleteEmap(strId);

	data.type = CLIENT_FACTORY_EMAP_DEL;
	data.id = strId;
	CallChange(data);
	return ret;
}

inline bool ClientFactory::AddEmapCamera(astring strId, VidEmapCamera cCam)
{
	bool ret = false;
	bool bAdd = false;

	VidEmap pEmap;
	VidEmap pEmapNew;

	if (m_Conf.GetEmapConf(strId, pEmap) == false)
	{
		return false;
	}

	pEmapNew = pEmap;
	pEmapNew.clear_ccamera();

	int camSize = pEmap.ccamera_size();

	for (s32 i = 0; i < pEmap.ccamera_size(); i++)
	{
		const VidEmapCamera &emapCam = pEmap.ccamera(i);
		if (emapCam.cid().strcameraid() == strId)
		{
			VidEmapCamera *pAddCam = pEmapNew.add_ccamera();
			*pAddCam = cCam;
			bAdd = true;
		}
		else
		{
			VidEmapCamera *pAddCam = pEmapNew.add_ccamera();
			*pAddCam = emapCam;
		}
	}

	if (bAdd == false)
	{
		VidEmapCamera *pAddCam = pEmapNew.add_ccamera();
		*pAddCam = cCam;
	}

	m_Conf.DeleteEmap(strId);
	m_Conf.AddEmap(pEmapNew);

	return true;
}
inline bool ClientFactory::DelEmapCamera(astring strId, astring strCamId)
{
	bool ret = false;

	VidEmap pEmap;
	VidEmap pEmapNew;

	if (m_Conf.GetEmapConf(strId, pEmap) == false)
	{
		return false;
	}

	pEmapNew = pEmap;
	pEmapNew.clear_ccamera();

	int camSize = pEmap.ccamera_size();

	for (s32 i = 0; i < pEmap.ccamera_size(); i ++)
	{
		const VidEmapCamera &emapCam = pEmap.ccamera(i);
		if (emapCam.cid().strcameraid() != strId)
		{
			VidEmapCamera *pAddCam = pEmapNew.add_ccamera();
			*pAddCam = emapCam;
		}
	}

	m_Conf.DeleteEmap(strId);
	m_Conf.AddEmap(pEmapNew);

	return true;
	
}

#if 0

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
#endif

inline void ClientFactory::run()
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
#endif
	
}




#endif // __VSC_CLIENT_FACTORY_H_
