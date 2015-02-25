//------------------------------------------------------------------------------
// File: moduletmpl.cpp
//
// Desc: Module template for Data Mining.
//
// Copyright (c) 2014-2018. veyesys.com All rights reserved.
//------------------------------------------------------------------------------

#include "moduletmpl.hpp"

ModuleTmpl::ModuleTmpl(u32 id) 
:m_id(id)
{
}
ModuleTmpl::~ModuleTmpl() 
{
}


/* Process decoded or compressed data */
BOOL ModuleTmpl::Process(VideoFrame& frame)
{
	printf("%s id %d len %d\n", __FUNCTION__, m_id, frame.dataLen);
	return TRUE;
}
BOOL ModuleTmpl::ProcessRaw(RawFrame& frame)
{
	printf("%s id %d (%d, %d)\n", __FUNCTION__, m_id, frame.width, frame.height);
	return TRUE;
}

/* Get the stream type of this module */
BOOL ModuleTmpl::GetReqStream(MMReqStream& type)
{
	/* use subraw video for processing */
	type = MM_STREAM_SUB_RAW;
	return TRUE;
}

BOOL ModuleTmpl::RegRetCallback(MiningCallbackFunctionPtr pCallback, void * pParam)
{
	return TRUE;
}
BOOL ModuleTmpl::UnRegDataCallback(void * pParam)
{
	return TRUE;
}
u32 ModuleTmpl::GetFlags()
{
	
	return MM_TRACK | MM_PEOPLE_CNT | MM_MOT;
}

astring ModuleTmpl::GetVersion()
{
	return "0.0.0";
}

VE_LIBRARY_API MiningInterface * MiningCreateDevice(u32 id)
{
	return new ModuleTmpl(id);
}