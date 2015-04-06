//------------------------------------------------------------------------------
// File: cmnoapiserver.hpp
//
// Desc: Common OAPI server - OAPI server for all application
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_CMN_OAPI_SERVER__H_
#define __VSC_CMN_OAPI_SERVER__H_
#include "utility.hpp"
#include "debug.hpp"
#include <QThread>
#include "factory.hpp"

#include "XSDK/XHash.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XSocket.h"
#include "XSDK/XSSLSocket.h"
#include "Webby/ClientSideRequest.h"
#include "Webby/ClientSideResponse.h"
#include "Webby/ServerSideRequest.h"
#include "Webby/ServerSideResponse.h"
#include "Webby/WebbyException.h"

using namespace std;
using namespace XSDK;
using namespace WEBBY;

/* each app can register a http REST path can move 
   the socket to itself, for streaming, it it for performance.
*/

using namespace UtilityLib;

class CmnOAPIServer:public QThread
{
	Q_OBJECT
public:
	inline CmnOAPIServer(Factory &pFactory);
	inline ~CmnOAPIServer();
	
public:
	void Lock()
	{
		m_Lock.lock();
	}
	void UnLock()
	{
		m_Lock.unlock();
	}
public:
	inline void run();

private:
	fast_mutex m_Lock;
	u16 m_port;
};

class CmnOAPISSLServer:public QThread
{
	Q_OBJECT
public:
	inline CmnOAPISSLServer(Factory &pFactory);
	inline ~CmnOAPISSLServer();
	
public:
	void Lock()
	{
		m_Lock.lock();
	}
	void UnLock()
	{
		m_Lock.unlock();
	}
public:
	inline void run();

private:
	fast_mutex m_Lock;
	u16 m_port;
};

typedef CmnOAPIServer* LPCmnOAPIServer;
typedef CmnOAPISSLServer* LPCmnOAPISSLServer;

class OAPIServer:
{
	Q_OBJECT
public:
	inline OAPIServer(Factory &pFactory);
	inline ~OAPIServer();
	
public:
	void Lock()
	{
		m_Lock.lock();
	}
	void UnLock()
	{
		m_Lock.unlock();
	}
public:
	inline void start()
	{
		m_cmn.start();
		m_ssl.start();
	}

private:
	fast_mutex m_Lock;
	CmnOAPIServer m_cmn;
	CmnOAPISSLServer m_ssl;
};


#include "cmnoapiserverimpl.hpp"


#endif /* __VSC_CMN_OAPI_SERVER__H_ */

