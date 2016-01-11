//------------------------------------------------------------------------------
// File: clientfactory impl.hpp
//
// Desc: Camera clientfactory - Manage IP Camera.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_CLIENT_FACTORY_IMPL_H_
#define __VSC_CLIENT_FACTORY_IMPL_H_

inline ClientFactory::ClientFactory()
{
#ifdef WIN32
#ifndef _WIN64
    astring strSysPath = "C:\\vidstor\\clientsystem";
#else
    astring strSysPath = "C:\\vidstor64\\clientsystem";
#endif
#else
    astring strSysPath = "ve/vidstor/clientsystem/";
#endif
    m_SysPath.Open(strSysPath);
	/* Create the stor factory */
	m_StorFactory = new StorFactory(m_Conf);
}

inline ClientFactory::~ClientFactory()
{
	delete m_StorFactory;
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
	astring strPath;
	if (m_SysPath.GetSystemPath(strPath) == FALSE)
	{
	    return FALSE;
	}
	printf("Sys path %s\n", strPath.c_str());
#ifdef WIN32
#ifndef _WIN64
	astring strPathConf = strPath + "vidstor\\clientconfig";
#else
	astring strPathConf = strPath + "vidstor64\\clientconfig";
#endif
#else
	astring strPathConf = strPath + "vidstor/clientconfig";
#endif
	m_Conf.Open(strPathConf);

	/* Init export path */
	astring strExportPath;
	GetExportPath(strExportPath);

	m_StorFactory->Init();

	InitLicense();
	return TRUE;
}

inline BOOL ClientFactory::RegChangeNotify(void * pData, ClientFactoryChangeNotify callback)
{
	XGuard guard(m_cMutex);
	m_Change[pData] = callback;

	return TRUE;
}
inline BOOL ClientFactory::CallChange(ClientFactoryChangeData data)
{
	//XGuard guard(m_cMutex);
	ChangeNofityMap::iterator it = m_Change.begin(); 
	for(; it!=m_Change.end(); ++it)
	{
		if ((*it).second)
		{
			(*it).second((*it).first, data);
		}
	}	
	 return TRUE;
}

inline BOOL ClientFactory::GetLicense(astring &strLicense, astring &strHostId, int &ch, astring &type, astring &expireTime)
{
	XGuard guard(m_cMutex);
	VPlay::GetLicenseInfo(strHostId, ch, type, expireTime);
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
inline bool ClientFactory::AuthUser(astring &strUser, astring &strPasswd)
{
	/* Admin is a Super User */
	if (strUser == "admin")
	{
		VidClientConf  cData;
		m_Conf.GetClientConf(cData);
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

#if 0
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
