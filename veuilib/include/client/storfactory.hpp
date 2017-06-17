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
#ifndef __VSC_STOR_FACTORY_H_
#define __VSC_STOR_FACTORY_H_
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
#include "client/storcmn.hpp"
#include "client/storclient.hpp"
#include "config/clientconfdb.hpp"

#include "Poco/UUIDGenerator.h"

using namespace Poco;
typedef std::list<LPStorClient> StorClientList;
typedef std::map<astring, LPStorClient> StorClientMap;

class VE_LIBRARY_API StorFactory: public QObject , public StorFactoryNotifyInterface
{
    Q_OBJECT
public:
    StorFactory(ClientConfDB &pConf);
    ~StorFactory();
public:
	/* Init function */
	BOOL Init();
	bool InitAddStor(VidStor & pParam);
public:
	bool AddStor(VidStor & pParam);
	bool DeleteStor(astring strId);

public:
	bool AddCam(astring strStorId, VidCamera &pParam);
	bool DeleteCam(astring strStorId, astring strId);
	bool PtzCmd(astring strStorId, astring strId, u32 action, double param);

public:
	bool SearchEvent(astring strStorId, astring strId, s64 nStart, s64 nEnd);
	bool RegRealEvent(astring strStorId);
	bool UnRegRealEvent(astring strStorId);
	/* the Id is the eventId */
	bool HandleEvent(astring strStorId, astring strId);
	
public:
	//BOOL RegChangeNotify(void * pData, StorFactoryChangeNotify callback);
	virtual bool CallChange(StorFactoryChangeData data);
	virtual bool OnEvent(VidEvent &pEvent, VidStor &pStor);
signals:
	void SignalCallChange(int type, std::string strId, std::string strCam);
	void SignalEvent(std::string strEvent);
	void SignalEvent1();

//public slots:

	
public:
	VidCameraList GetVidCameraList(astring strStor);
	bool GetCameraConf(astring strStor, astring strCameraId, VidCamera &pCam);
	astring GetVidCameraName(astring strStor, astring strCam);
	StorClientOnlineMap GetVidCameraOnlineList(astring strStor);
	StorClientRecMap GetVidCameraRecList(astring strStor);
	bool GetOnline(astring strStor);
public:
	void run();

private:
	StorClientMap m_StorClientMap;
	StorClientOnlineMap m_StorClientOnlineMap;

	XMutex m_cMutex;
	
private:
	ClientConfDB &m_pConf;
};

typedef StorFactory* LPStorFactory;


#endif // __VSC_STOR_FACTORY_H_
