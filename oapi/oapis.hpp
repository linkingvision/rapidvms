//------------------------------------------------------------------------------
// File: oapis.hpp
//
// Desc: OpenCVR API - OpenCVR API.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPIS_H_
#define __VSC_OAPIS_H_
#include "oapicmn.hpp"
#include "factory.hpp"
#include "oapiproto.hpp"
#include "XSDK/XHash.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XSocket.h"
#include "XSDK/XSSLSocket.h"

using namespace XSDK;
using namespace std;

/* a internal thread to process all the request of XSocket */
class OAPIServer
{
public:
	inline OAPIServer(XSocket *pSocket, Factory &pFactory);
	inline ~OAPIServer();
public:
	BOOL Process();
public:
	void Lock()
	{
		m_Lock.lock();
	}
	void UnLock()
	{
		m_Lock.unlock();
	}

private:
	fast_mutex m_Lock;
};

#include "oapisimpl.hpp"
#endif