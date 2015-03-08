//------------------------------------------------------------------------------
// File: cmnserver.hpp
//
// Desc: Common HTTP server - HTTP server for all application
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_CMN_SERVER__H_
#define __VSC_CMN_SERVER__H_
#include "utility.hpp"
#include "debug.hpp"
#include <QThread>

#include "XSDK/XHash.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XSocket.h"
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

class CmnHttpServer:public QThread
{
	Q_OBJECT
public:
	inline CmnHttpServer(u16 port);
	inline ~CmnHttpServer();
	
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

typedef CmnHttpServer* LPCmnHttpServer;


#include "cmnserverimpl.hpp"


#endif /* __VSC_CMN_SERVER__H_ */

