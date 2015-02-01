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
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_command_queue.h"
#include "cppkit/ck_dynamic_library.h"
#include "cppkit/ck_byte_ptr.h"

using namespace std;
using namespace cppkit;

typedef enum __MiningRetType
{
	MINING_RET_MOT = 1,
	MINING_RET_OBJS,
	MINING_RET_LAST
}MiningRetType;

typedef enum __MModuleFlag
{
	MM_MOT = 1,
	MM_TRACK = 2,
	MM_PEOPLE_CNT = 4,
	MM_ALPR = 8,
	MM_LAST
}MModuleFlag;

typedef struct __MiningRet
{
	MiningRetType ret;
	union {
		VeObjGroup objs;
		VeMotionBox mot;
	} data;	
}MiningRet;

#ifdef WIN32
typedef void (__cdecl * MiningCallbackFunctionPtr)(s32 id, MiningRet& ret, void * pParam);
#else
typedef void ( * MiningCallbackFunctionPtr)(s32 id, MiningRet& ret, void * pParam);
#endif

class MiningInterface
{
public:
	MiningInterface() {}
	virtual ~MiningInterface() {}

public:
	virtual BOOL AddChannel(s32 id) = 0;
	virtual BOOL DelChannel(s32 id) = 0;
	virtual BOOL Process(s32 id, RawFrame& frame) = 0;
	virtual BOOL RegRetCallback(MiningCallbackFunctionPtr pCallback, void * pParam);
	virtual BOOL UnRegDataCallback(void * pParam);
	virtual u32 GetFlags();
};

/* Create MiningInterface from the module  */
typedef MiningInterface * (*MiningCreateObject)();


class MiningModule
{
public:
	inline MiningModule(astring strPath);
	inline ~MiningModule();
public:
	inline BOOL AddChannel(s32 id)
	{
		if (m_module)
		{
			return m_module->AddChannel(id);
		}
		return FALSE;
	}
	inline BOOL DelChannel(s32 id)
	{
		if (m_module)
		{
			return m_module->DelChannel(id);
		}
		return FALSE;
	}
	inline BOOL Process(s32 id, RawFrame& frame)
	{
		if (m_module)
		{
			return m_module->Process(id, frame);
		}
		return FALSE;
	}
	inline BOOL RegRetCallback(MiningCallbackFunctionPtr pCallback, void * pParam)
	{
		if (m_module)
		{
			return m_module->RegRetCallback(pCallback, pParam);
		}
		return FALSE;
	}
	inline BOOL UnRegDataCallback(void * pParam)
	{
		if (m_module)
		{
			return m_module->UnRegDataCallback(pParam);
		}
		return FALSE;
	}
	inline u32 GetFlags()
	{
		if (m_module)
		{
			return m_module->GetFlags();
		}
		return FALSE;
	}
		
private:
	MiningInterface * m_module;
	ck_dynamic_library m_dynLib;
};


#endif /* __M_MODULE_HPP__ */
