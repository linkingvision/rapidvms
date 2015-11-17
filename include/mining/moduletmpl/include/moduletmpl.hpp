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
	ModuleTmpl(u32 id);
	virtual ~ModuleTmpl();

public:
	/* Process decoded or compressed data */
	virtual BOOL Process(VideoFrame& frame);
	virtual BOOL ProcessRaw(RawFrame& frame);
	virtual BOOL ProcessSeq(VideoSeqFrame& frame);

	/* Get the stream type of this module */
	virtual BOOL GetReqStream(MMReqStream& type);
	
	virtual BOOL RegRetCallback(MiningCallbackFunctionPtr pCallback, void * pParam);
	virtual BOOL UnRegDataCallback(void * pParam);
	virtual u32 GetFlags();
	virtual astring GetVersion();
	virtual u32 GetId() 
	{
		return m_id;
	}
private:
	u32 m_id;
};


#endif /* __M_MODULE_TMPL_HPP__ */
