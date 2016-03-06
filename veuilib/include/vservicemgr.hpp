//------------------------------------------------------------------------------
// File: vservicemgr.hpp
//
// Desc: service manager - Manage ONVIF Service.
//
// Copyright (c) 2014-2018 INTINT. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __V_SERVICE_MGR_HPP__
#define __V_SERVICE_MGR_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "factory.hpp"
#include <QThread>
#include "vonvifdismgr.hpp"
#include "vvipcmgr.hpp"
#include "vonvifpgmgr.hpp"
#include "vrtspserver.hpp"
#include "vtaskmgr.hpp"
#if defined (WIN32) && !defined (WIN64)
#include "vhttpserver.hpp"
#include "vhlsserver.hpp"
#endif

using namespace UtilityLib;

class VE_LIBRARY_API VServiceMgr
{
public:
	VServiceMgr(Factory & pFactory);
	~VServiceMgr();

public:
	static VServiceMgr * CreateObject(Factory & pFactory);
public:
	/* ONVIF server */
	VONVIFDisMgr * m_pONVIFDisMgr;
	VVIPCMgr * m_pVVIPCMgr;
	VONVIFPGMgr * m_pVONVIFPGMgr;
	
	/* RTSP Server */
	VRTSPServer *m_pVRTSPServer;
#if defined (WIN32__REMOVE) && !defined (WIN64)
	/* HTTP Server */
	VHTTPServer *m_pVHTTPServer;

	/* HLS Server */
	VHLSServer *m_pVHLSServer;
#endif

	/* task manager */
	VTaskMgr * m_pVTaskMgr;

	/* factory */
	Factory & m_pFactory;

};

#endif /* __V_SERVICE_MGR_HPP__ */
