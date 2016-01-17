//------------------------------------------------------------------------------
// File: clientfactory.hpp
//
// Desc: Camera clientfactory - Manage IP Camera.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_CLIENT_FACTORY_H_
#define __VSC_CLIENT_FACTORY_H_
#include "config/confdb.hpp"
#include "server/camera.hpp"
#include "vdb.hpp"
#include "vhdfsdb.hpp"
#include "vplay.hpp"
#include "config/sysdb.hpp"
#include "server/hdddevice.hpp"
#include <QThread>
#include <qdebug.h>
#include "Poco/Path.h"
#include "Poco/File.h"
#include "config/vidconf.pb.h"
#include "config/clientconfdb.hpp"
#include "client/storfactory.hpp"
#include "simplecrypt.hpp"

using namespace VidConf;

typedef enum
{
    /* Camera view add and del */
    CLIENT_FACTORY_VIEW_ADD,
    CLIENT_FACTORY_VIEW_DEL,
    /* Camera group add and del */
    CLIENT_FACTORY_VGROUP_ADD,
    CLIENT_FACTORY_VGROUP_DEL,

    /* Camera tour add and del */
    CLIENT_FACTORY_TOUR_ADD,
    CLIENT_FACTORY_TOUR_DEL,
} ClientFactoryChangeType;


class ClientFactoryChangeData
{
public:
	ClientFactoryChangeType type;
	int id;
};

typedef bool (*ClientFactoryChangeNotify)(void* pParam, 
		ClientFactoryChangeData data);

typedef std::map<void *, ClientFactoryChangeNotify> ChangeNofityMap;

/* Fatory is Qthread for callback in Qt GUI */
class ClientFactory: public QThread
{
    Q_OBJECT
public:
    ClientFactory();
    ~ClientFactory();
public:
	/* Init function */
	BOOL Init();
	BOOL SetSystemPath(astring &strPath);
	ClientConfDB &GetConfDB(){return m_Conf;};
	
public:
	BOOL RegChangeNotify(void * pData, ClientFactoryChangeNotify callback);
	BOOL CallChange(ClientFactoryChangeData data);
	
public:
	BOOL GetLicense(astring &strLicense, astring &strHostId, 
							int &ch, astring &type, astring &expireTime);
	BOOL SetLicense(astring &strLicense);
	BOOL InitLicense();
	
	BOOL GetExportPath(astring &strPath);
	BOOL SetExportPath(astring &strPath);
	
public:
	StorFactory & GetStorFactory(){return *m_StorFactory;}

public:
	bool GetAutoLogin();
	bool SetAutoLogin(bool bAutoLogin);
	bool AuthUser(astring &strUser, astring &strPasswd);
	bool GetAdminPasswd(astring &strPasswd);
	bool SetAdminPasswd(astring strPasswd);
	bool GetLang(VidLanguage &nLang);
	bool SetLang(VidLanguage &nLang);

#if 0

/* Emap function */
public:
	BOOL GetEmapData(VSCEmapData &pData);
	BOOL SetEmapData(VSCEmapData &pData);
	BOOL AddEmapCamera(s32 nIndex, u32 x, u32 y, u32 w, u32 h);
	BOOL DelEmapCamera(s32 nIndex);
	BOOL GetEmapCamera(s32 nIndex, u32 &x, u32 &y, u32 &w, u32 &h);
	
	BOOL GetEmapFile(astring &strFile);
	BOOL SetEmapFile(astring &strFile);

public:
	BOOL GetLang(VSCLangType &pLang);
	BOOL SetLang(VSCLangType &pLang);
	
public:
	/* VMS */
	BOOL GetVms(VSCVmsData &pData);
	s32 AddVms(VSCVmsDataItem &pParam);
	BOOL DelVms(s32 Id);
	BOOL GetVmsById(VSCVmsDataItem &pParam, int nId);

	/* View */
	BOOL GetView(VSCViewData &pData);
	s32 AddView(VSCViewDataItem &pParam);
	BOOL DelView(s32 Id);
	BOOL GetViewById(VSCViewDataItem &pParam, int nId);

	/* Camera group */
	BOOL GetVGroup(VSCVGroupData &pData);
	s32 AddVGroup(VSCVGroupDataItem &pParam);
	BOOL DelVGroup(s32 Id);
	BOOL GetVGroupById(VSCVGroupDataItem &pParam, int nId);

	/* Tour */
	BOOL GetTour(VSCTourData &pData);
	s32 AddTour(VSCTourDataItem &pParam);
	BOOL DelTour(s32 Id);
	BOOL GetTourById(VSCTourDataItem &pParam, int nId);
#endif

public:
	void run();
	
private:
	StorFactory *m_StorFactory;

private:
	XMutex m_cMutex;

private:
	ChangeNofityMap m_Change;

private:
	ClientConfDB m_Conf;
	SysDB m_SysPath;
};

typedef ClientFactory* LPClientFactory;


#include "clientfactoryimpl.hpp"

#endif // __VSC_CLIENT_FACTORY_H_
