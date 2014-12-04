//------------------------------------------------------------------------------
// File: vvipcmgr.hpp
//
// Desc: virtual IPC manager - Manage virtual IP Camera.
//
// Copyright (c) 2014-2018 INTINT. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __V_V_IPC_MGR_HPP__
#define __V_V_IPC_MGR_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "factory.hpp"
#include <QThread>
#include "vonvifdismgr.hpp"

using namespace UtilityLib;

typedef std::map<int, QThread *> VIPCThreadMap;

class VE_LIBRARY_API VVIPCMgr
{
public:
	VVIPCMgr(Factory & pFactory, VONVIFDisMgr & pDisMgr);
	~VVIPCMgr();
public:
	static bool DeviceChangeCallbackFunc(void* pData, 
						FactoryDeviceChangeData change);
	bool DeviceChangeCallbackFunc1(FactoryDeviceChangeData change);
public:
	void Lock(){m_Lock.lock();}
	void UnLock(){m_Lock.unlock();}
private:
    	Factory & m_pFactory;
	VONVIFDisMgr & m_pDisMgr;
       VIPCThreadMap m_ThreadMap;
	fast_mutex m_Lock;
};

#endif /* __V_V_IPC_MGR_HPP__ */
