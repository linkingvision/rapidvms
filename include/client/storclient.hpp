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
#ifndef __VSC_STOR_CLIENT_H_
#define __VSC_STOR_CLIENT_H_
#include "config/confdb.hpp"
#include "config/sysdb.hpp"
#include <QThread>
#include <qdebug.h>
#include "Poco/Path.h"
#include "Poco/File.h"
#include "config/vidconf.pb.h"
#include "client/storcmn.hpp"

#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "oapi/oapic.hpp"
#include "oapi/oapis.hpp"

using namespace cppkit;
using namespace std;

typedef std::map<astring, bool> StorClientOnlineMap;
typedef std::map<astring, bool> StorClientRecMap;


/* Stor Client is the manager of the OAPI,  the thread will loop 
	for all the command from Stor */
class StorClient : public QThread
{
	Q_OBJECT
public:
	StorClient(VidStor &stor, StorFactoryNotifyInterface &pNotify);
	~StorClient();
public:
	VidCameraList GetVidCameraList();
	bool GetCameraConf(astring strCameraId, VidCamera &pCam);
	void UpdateVidCameraList(oapi::OAPICameraListRsp list);
	StorClientOnlineMap GetVidCameraOnlineList();
	StorClientRecMap GetVidCameraRecList();
	astring GetVidCameraName(astring strCam);
	bool GetOnline();
public:
	bool SearchEvent(astring strId, s64 nStart, s64 nEnd);
	bool RegRealEvent();
	bool UnRegRealEvent();
	bool HandleEvent(astring strId);
public:
	bool AddCam(VidCamera &pParam);
	bool DeleteCam(astring strId);
	bool PtzCmd(astring strId, u32 action, double param);
public:
	/* Start Stop Refresh the Stor client Thread */
	bool StartStorClient();
	bool StopStorClient();
public:
	void run();
private:
	XMutex m_cMutex;
	VidStor m_stor;
	bool m_Quit;
	/* The list is only for startup usage, if any change, it not update */
	VidCameraList m_cCamList;
	StorClientOnlineMap m_CamOnlineList;
	StorClientRecMap m_CamRecList;
	StorFactoryNotifyInterface &m_pNotify;
	bool m_bOnline;

	XRef<XSocket> m_pSocket;
};

typedef StorClient* LPStorClient;


#include "storclientimpl.hpp"

#endif