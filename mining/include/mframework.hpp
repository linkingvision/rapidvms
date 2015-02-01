//------------------------------------------------------------------------------
// File: mframework.hpp
//
// Desc: Data Mining Framework.
//
// Copyright (c) 2014-2018. veyesys.com All rights reserved.
//------------------------------------------------------------------------------
#ifndef __M_FRAME_WORK_HPP__
#define __M_FRAME_WORK_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include "miningtype.hpp"
#include "mmodule.hpp"
#include "factory.hpp"
#include <QThread>
#include <qdebug.h>
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_command_queue.h"
#include "cppkit/ck_dynamic_library.h"
#include "cppkit/ck_byte_ptr.h"

using namespace std;
using namespace cppkit;

/* All the mining data will be post a queue to here,
    Then start a thread to peek the queue to post to each device in factory
*/

typedef command_queue<MiningRet> MModuleRetQueue;

typedef std::map<int, MiningModule *> MModuleMap;
class MFramework:public QThread
{
    Q_OBJECT
public:
	inline MFramework(Factory &pFactory);
	inline ~MFramework();
public:
	inline void run();
public:
	inline static BOOL RetHandler(s32 id, MiningRet& ret, void * pParam);
	inline BOOL RetHandler1(s32 id, MiningRet& ret);
public:
	static BOOL DeviceChangeCallbackFunc(void* pData, FactoryDeviceChangeData change);
	BOOL DeviceChangeCallbackFunc1(FactoryDeviceChangeData change);

private:
	MModuleMap m_MModules;
	MModuleRetQueue m_RetQueue;
	BOOL m_bExit;
};


BOOL MFramework::RetHandler(s32 id, MiningRet& ret, void * pParam)
{
	int dummy = errno;
	MFramework *pMFramework = (MFramework)pParam;

	if (pMFramework)
	{
		return pMFramework->RetHandler1(id, ret);
	}

	return TRUE;
}

BOOL MFramework::RetHandler1(s32 id, MiningRet& ret)
{
	m_RetQueue.post(ret);

	return TRUE;
}

void MFramework::run()
{
	MiningRet ret;
	while (m_bExit != TRUE)
	{
		if (m_RetQueue.size() > 0)
		{
			ret = m_RetQueue.pop();
			//send the ret va data to factory for showing
			continue;
		}else
		{
			ve_sleep(1000);
		}
	}
	return;
}

bool MFramework::DeviceChangeCallbackFunc(void* pData, 
								FactoryDeviceChangeData change)
{
	if (pData)
	{
		MFramework * pthread = (MFramework *)pData;
		pthread->DeviceChangeCallbackFunc1(change);
	}
	return true;
}
bool MFramework::DeviceChangeCallbackFunc1(FactoryDeviceChangeData change)
{
	VDC_DEBUG( "Event Device Change Callback %d type %d Begin\n", change.id, change.type);
	
	if (change.type == FACTORY_DEVICE_OFFLINE)
	{

	}
	if (change.type == FACTORY_DEVICE_ONLINE)
	{
		
	}
	VDC_DEBUG( "Event Device Change Callback %d type %d End \n", change.id, change.type);

	return TRUE;
}

#endif /* __M_FRAME_WORK_HPP__ */
