//------------------------------------------------------------------------------
// File: vevent.hpp
//
// Desc: onvif event - Manage ONVIF IPC event.
//
// Copyright (c) 2014-2018. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __V_EVENT_HPP__
#define __V_EVENT_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "server/factory.hpp"
#include "server/eventserver.hpp"
#include <QThread>

using namespace UtilityLib;

class VOnvifEventThread;
class VSmartMotionThread;

class VE_LIBRARY_API VEventMgr
{
public:
	VEventMgr(Factory &pFactory, VEventServer &pEventServer)
	:m_pFactory(pFactory), m_pEventServer(pEventServer), m_OnvifThread(NULL), 
	m_SmartMotionThread(NULL)
	{
	}
	~VEventMgr()
	{
		
	}
public:
	bool Init();
private:
	VOnvifEventThread *m_OnvifThread;
	VSmartMotionThread *m_SmartMotionThread;
	Factory &m_pFactory;
	VEventServer &m_pEventServer;
};


#endif /* __V_EVENT_HPP__ */
