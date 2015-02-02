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
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_command_queue.h"
#include "cppkit/ck_dynamic_library.h"
#include "cppkit/ck_byte_ptr.h"

using namespace std;
using namespace cppkit;




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
	virtual astring GetVersion()
	{
		if (m_module)
		{
			return m_module->GetVersion();
		}
		return FALSE;
	}
		
private:
	MiningInterface * m_module;
	ck_dynamic_library m_dynLib;
};


#endif /* __M_MODULE_HPP__ */
