//------------------------------------------------------------------------------
// File: minterface.hpp
//
// Desc: Interface for Data Mining.
//
// Copyright (c) 2014-2018. veyesys.com All rights reserved.
//------------------------------------------------------------------------------
#ifndef __M_INTERFACE_HPP__
#define __M_INTERFACE_HPP__
#include "utility.hpp"
#include "videotype.hpp"
#include "miningtype.hpp"

using namespace std;

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

typedef enum __MMReqStream
{
	MM_STREAM_MAIN = 1,
	MM_STREAM_SUB = 2,
	MM_STREAM_MAIN_RAW = 4,
	MM_STREAM_SUB_RAW = 8,
	MM_STREAM_SEQ = 16,
	MM_STREAM_LAST
}MMReqStream;


typedef struct __MiningRet
{
	MiningRetType ret;
	union {
		VeObjGroup objs;
		VeMotionBox mot;
	} data;	
}MiningRet;

#ifdef WIN32
typedef void (__cdecl * MiningCallbackFunctionPtr)(MiningRet& ret, void * pParam);
#else
typedef void ( * MiningCallbackFunctionPtr)(MiningRet& ret, void * pParam);
#endif

class MiningInterface
{
public:
	MiningInterface() {}
	virtual ~MiningInterface() {}

public:
	/* Process decoded or compressed data */
	virtual BOOL Process(VideoFrame& frame) = 0;
	virtual BOOL ProcessRaw(RawFrame& frame) = 0;
	virtual BOOL ProcessSeq(VideoSeqFrame& seq) = 0;

	/* Get the stream type of this module */
	virtual BOOL GetReqStream(MMReqStream& type) = 0;
	
	virtual BOOL RegRetCallback(MiningCallbackFunctionPtr pCallback, void * pParam) = 0;
	virtual BOOL UnRegDataCallback(void * pParam) = 0;
	virtual u32 GetFlags() = 0;
	virtual astring GetVersion() = 0;
	virtual u32 GetId() = 0;
	
};

/* Create MiningInterface from the module MiningCreateDevice function  */
typedef MiningInterface * (*MiningCreateDeviceFunc)(u32 id);

#ifdef __cplusplus
extern "C" {
#endif
VE_LIBRARY_API MiningInterface * MiningCreateDevice(u32 id);
#ifdef __cplusplus
}
#endif

#endif /* __M_INTERFACE_HPP__ */
