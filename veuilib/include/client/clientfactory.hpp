/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
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
#include "config/videnv.hpp"

typedef enum
{
    /* Camera view add and del */
    CLIENT_FACTORY_VIEW_ADD,
    CLIENT_FACTORY_VIEW_DEL,
    /* Camera group add and del */
    CLIENT_FACTORY_VGROUP_ADD,
    CLIENT_FACTORY_VGROUP_DEL,

    /*Emap add and del */
    CLIENT_FACTORY_EMAP_ADD,
    CLIENT_FACTORY_EMAP_DEL,

    /* Camera tour add and del */
    CLIENT_FACTORY_TOUR_ADD,
    CLIENT_FACTORY_TOUR_DEL,
} ClientFactoryChangeType;


class VE_LIBRARY_API ClientFactoryChangeData
{
public:
	ClientFactoryChangeType type;
	astring id;
};

/* Fatory is Qthread for callback in Qt GUI */
class VE_LIBRARY_API ClientFactory: public QThread
{
    Q_OBJECT
public:
    ClientFactory(VidEnv &pEnv);
    ~ClientFactory();
public:
	/* Init function */
	BOOL Init();
	BOOL SetSystemPath(astring strPath);
	ClientConfDB &GetConfDB(){return m_Conf;};
	
public:
	//BOOL RegChangeNotify(void * pData, ClientFactoryChangeNotify callback);
	BOOL CallChange(ClientFactoryChangeData data);
signals:
	void SignalCallChange(int type, std::string strId);
	
public:
	BOOL GetLicense(astring &strLicense, astring &strHostId, 
							int &ch, astring &type, astring &startTime, astring &expireTime);
	BOOL SetLicense(astring &strLicense);
	BOOL InitLicense();
	
	BOOL GetExportPath(astring &strPath);
	BOOL SetExportPath(astring &strPath);
	
public:
	StorFactory & GetStorFactory(){return *m_StorFactory;}

public:
	bool GetAutoLogin();
	bool SetAutoLogin(bool bAutoLogin);
	bool GetAutoFullScreen();
	bool SetAutoFullScreen(bool bAuto);
	bool AuthUser(astring &strUser, astring &strPasswd);
	bool GetAdminPasswd(astring &strPasswd);
	bool SetAdminPasswd(astring strPasswd);
	bool GetLang(VidLanguage &nLang);
	bool SetLang(VidLanguage &nLang);
	bool AddView(VidView &pView);
	bool DelView(astring strId);
	bool AddEmap(VidEmap &pEmap);
	bool DelEmap(astring strId);
	bool AddEmapCamera(astring strId, VidEmapCamera cCam);
	bool DelEmapCamera(astring strId, astring strCamId);

	VidEnv & GetEnv(){return m_env;}

public:
	void run();
	
private:
	StorFactory *m_StorFactory;

private:
	XMutex m_cMutex;

private:
	ClientConfDB m_Conf;
	//SysDB m_SysPath;
	VidEnv &m_env;
};

typedef ClientFactory* LPClientFactory;


#endif // __VSC_CLIENT_FACTORY_H_
