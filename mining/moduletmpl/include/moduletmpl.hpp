//------------------------------------------------------------------------------
// File: moduletmpl.hpp
//
// Desc: Module template for Data Mining.
//
// Copyright (c) 2014-2018. veyesys.com All rights reserved.
//------------------------------------------------------------------------------
#ifndef __M_MODULE_TMPL_HPP__
#define __M_MODULE_TMPL_HPP__
#include "minterface.hpp"


class ModuleTmpl :public MiningInterface
{
public:
	ModuleTmpl();
	virtual ~ModuleTmpl();

public:
	/* Channel manage */
	virtual BOOL AddChannel(s32 id);
	virtual BOOL DelChannel(s32 id);

	/* Process decoded or compressed data */
	virtual BOOL Process(s32 id, VideoFrame& frame);
	virtual BOOL ProcessRaw(s32 id, RawFrame& frame);

	/* Get the stream type of this module */
	virtual BOOL GetReqStream(MMReqStream& type);
	
	virtual BOOL RegRetCallback(MiningCallbackFunctionPtr pCallback, void * pParam);
	virtual BOOL UnRegDataCallback(void * pParam);
	virtual u32 GetFlags();
	virtual astring GetVersion();
};


#endif /* __M_MODULE_TMPL_HPP__ */
