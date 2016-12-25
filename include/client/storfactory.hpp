//------------------------------------------------------------------------------
// File: storfactory.hpp
//
// Desc: storfactory - Manage Vid Stor.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

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
using namespace VidConf;

typedef std::list<LPStorClient> StorClientList;
typedef std::map<astring, LPStorClient> StorClientMap;

class StorFactory: public QObject , public StorFactoryNotifyInterface
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


#include "storfactoryimpl.hpp"

#endif // __VSC_STOR_FACTORY_H_
