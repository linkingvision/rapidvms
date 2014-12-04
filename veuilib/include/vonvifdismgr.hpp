//------------------------------------------------------------------------------
// File: vonvifdismgr.hpp
//
// Desc: onvif discover manager - Manage ONVIF Discover.
//
// Copyright (c) 2014-2018 INTINT. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __V_ONVIF_DIS_MGR_HPP__
#define __V_ONVIF_DIS_MGR_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "factory.hpp"
#include <QThread>

using namespace UtilityLib;


class VONVIFDisMgrImpl;
class VE_LIBRARY_API VONVIFDisMgr
{
public:
	VONVIFDisMgr();
	~VONVIFDisMgr();
public:
	/* Type is Network_Video_Storage or NetworkVideoTransmitter */
	bool AddHost(astring host, astring port, astring type);
	bool DelHost(astring host, astring port);
	
private:
	VONVIFDisMgrImpl *m_pImpl;
};

#endif /* __V_ONVIF_DIS_MGR_HPP__ */
