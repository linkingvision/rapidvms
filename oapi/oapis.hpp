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
using namespace oapi;

class OAPIConverter
{
public:
	inline static BOOL Converter(VSCDeviceData__ &from, oapi::Device &to);
	inline static BOOL Converter(oapi::Device &from, VSCDeviceData__ &to);
};

/* a internal thread to process all the request of XSocket */
class OAPIServer
{
public:
	inline OAPIServer(XRef<XSocket> pSocket, Factory &pFactory);
	inline ~OAPIServer();
public:
	inline BOOL Process(OAPIHeader &header);

public:
	inline BOOL ProcessGetDevice(s32 len);	
	inline BOOL ProcessStartLive(s32 len);
	inline BOOL ProcessStopLive(s32 len);
public:
	inline void DataHandler1(VideoFrame& frame);
	inline static void DataHandler(VideoFrame& frame, void * pParam);	
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
	XRef<XSocket> m_pSocket;
	Factory &m_pFactory;
	int m_nLiveviewId;
};

#include "oapisimpl.hpp"
#endif