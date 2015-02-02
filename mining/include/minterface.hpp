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
	virtual astring GetVersion();
};

/* Create MiningInterface from the module  */
typedef MiningInterface * (*MiningCreateObject)();


#endif /* __M_INTERFACE_HPP__ */
