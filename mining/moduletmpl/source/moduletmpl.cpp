//------------------------------------------------------------------------------
// File: moduletmpl.cpp
//
// Desc: Module template for Data Mining.
//
// Copyright (c) 2014-2018. veyesys.com All rights reserved.
//------------------------------------------------------------------------------

#include "moduletmpl.hpp"

ModuleTmpl::ModuleTmpl() 
{
}
ModuleTmpl::~ModuleTmpl() 
{
}

BOOL ModuleTmpl::AddChannel(s32 id)
{
	printf("%s Add a Channel %d \n", __FUNCTION__, id);
	return TRUE;
}
BOOL ModuleTmpl::DelChannel(s32 id)
{
	printf("%s Add Delete Channel %d \n", __FUNCTION__, id);
	return TRUE;
}

/* Process decoded or compressed data */
BOOL ModuleTmpl::Process(s32 id, VideoFrame& frame)
{
	printf("%s id %d len %d\n", __FUNCTION__, id, frame.dataLen);
	return TRUE;
}
BOOL ModuleTmpl::ProcessRaw(s32 id, RawFrame& frame)
{
	printf("%s id %d (%d, %d)\n", __FUNCTION__, id, frame.width, frame.height);
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

VE_LIBRARY_API MiningInterface * MiningCreateObject()
{
	return new ModuleTmpl;
}