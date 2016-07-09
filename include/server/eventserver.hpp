//------------------------------------------------------------------------------
// File: eventserver.hpp
//
// Desc: Event Server
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_EVENT_SERVER_H_
#define __VSC_EVENT_SERVER_H_
#include "config/confdb.hpp"
#include "server/camera.hpp"
#include "vdb.hpp"
#include "vhdfsdb.hpp"
#include "vplay.hpp"
#include "config/sysdb.hpp"
#include "server/factory.hpp"
#include <QThread>
#include <qdebug.h>
#include "Poco/Path.h"
#include "Poco/File.h"
#include "config/vidconf.pb.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XBlockingQueue.h"

#include <soci.h>
#include <firebird/soci-firebird.h>
#include <exception>



using namespace VidConf;

class VEventData
{
public:
	astring strId;
	astring strDevice;
	astring strDeviceName;
	astring strType;/* MOTION_START, MOTION_END, SMART_MOTION */
	s64 nTime;
	astring strDesc;
	bool bMetaData;
	astring strMetaData;/* the MetaData Depend the type */
};

typedef BOOL (*FunctionEventNotify)(void* pParam, VEventData data);
typedef std::map<void *, FunctionEventNotify> FunctionEventNotifyMap;

class VEventServerDbTask: public QThread
{
	Q_OBJECT
public:
	VEventServerDbTask(Factory &pFactory);
	~VEventServerDbTask();
public:
	void PushEvent(VEventData &pData);
	void UpdateDBSession(bool bIsFirst);
public:
	void run();
private:
	XSDK::XBlockingQueue<VEventData> m_Queue;
	Factory &m_Factory;
	soci::session *m_pSqlSession;
	int m_nYear;
	int m_nMonth;
	
};

//TODO VEventServerMetaDbTask for the Meta Data for the VA 

class VEventServerCallbackTask: public QThread
{
	Q_OBJECT
public:
	VEventServerCallbackTask(Factory &pFactory);
	~VEventServerCallbackTask();
public:
	void PushEvent(VEventData &pData);
	/* Register the call back for the event */
	BOOL RegEventNotify(void * pData, FunctionEventNotify callback);
	BOOL UnRegEventNotify(void * pData);	

public:
	void run();
private:
	XSDK::XBlockingQueue<VEventData> m_Queue;
	Factory &m_Factory;
	XMutex m_cMutex;

	FunctionEventNotifyMap m_NotifyMap;
};


class VEventServer: public QObject
{
	Q_OBJECT
public:
	VEventServer(Factory &pFactory);
	~VEventServer();
public:
	void PushEvent(VEventData &pData);
	/* Register the call back for the event */
	BOOL RegEventNotify(void * pData, FunctionEventNotify callback);
	BOOL UnRegEventNotify(void * pData);
public:
	BOOL Init();
private:
	VEventServerDbTask m_DbTask;
	VEventServerCallbackTask m_CallbackTask;
	Factory &m_Factory;
};


#include "eventserverimpl.hpp"

#endif // __VSC_EVENT_SERVER_H_
