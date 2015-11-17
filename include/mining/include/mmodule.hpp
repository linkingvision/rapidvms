//------------------------------------------------------------------------------
// File: mmodule.hpp
//
// Desc: Module for Data Mining.
//
// Copyright (c) 2014-2018. veyesys.com All rights reserved.
//------------------------------------------------------------------------------
#ifndef __M_MODULE_HPP__
#define __M_MODULE_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include "miningtype.hpp"
#include "minterface.hpp"
#include "minterfacemgr.hpp"
#include "factory.hpp"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_command_queue.h"
#include "cppkit/ck_dynamic_library.h"
#include "cppkit/ck_byte_ptr.h"

using namespace std;
using namespace cppkit;


typedef std::map<u32, MiningInterfaceMgr *> MInterfaceMgrMap;
class MiningModule
{
public:
	inline MiningModule(astring strPath, Factory &pFactory);
	inline ~MiningModule();
public:
	inline BOOL Valid();
	inline BOOL Init();
	inline BOOL InitOneDevice(DeviceParam & Param);
	inline BOOL DeleteOneDevice(int id);
    	inline static BOOL DeviceChangeCallbackFunc(void* pData,
						FactoryDeviceChangeData change);
    	inline BOOL DeviceChangeCallbackFunc1(FactoryDeviceChangeData change);
		
private:
	MiningInterface * m_module;
	ck_dynamic_library m_dynLib;
	MiningCreateDeviceFunc m_CreateDevice;
	astring m_strPath;
	BOOL m_Init;
	Factory &m_pFactory;
	MInterfaceMgrMap m_mgrMap;
};


#include "mmoduleimpl.hpp"

#endif /* __M_MODULE_HPP__ */
