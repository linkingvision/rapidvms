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
	inline BOOL AddChannel(s32 id);
	inline BOOL DelChannel(s32 id);
	
	/* Process decoded or compressed data */
	virtual BOOL Process(s32 id, VideoFrame& frame);
	virtual BOOL ProcessRaw(s32 id, RawFrame& frame);

	/* Get the stream type of this module */
	virtual BOOL GetReqStream(MMReqStream& type);
	
	inline BOOL RegRetCallback(MiningCallbackFunctionPtr pCallback, 
						void * pParam);
	inline BOOL UnRegDataCallback(void * pParam);
	inline u32 GetFlags();
	virtual astring GetVersion();
public:
	BOOL Valid();
		
private:
	MiningInterface * m_module;
	ck_dynamic_library m_dynLib;
	astring m_strPath;
	BOOL m_Init;
};


#include "mmoduleimpl.hpp"

#endif /* __M_MODULE_HPP__ */
