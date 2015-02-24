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
#include "Poco/DirectoryIterator.h"
#include "Poco/File.h"
#include "Poco/Path.h"

using Poco::DirectoryIterator;
using Poco::File;
using Poco::Path;

using namespace std;
using namespace cppkit;

/* All the mining data will be post a queue to here,
    Then start a thread to peek the queue to post to each device in factory
*/

typedef command_queue<MiningRet> MModuleRetQueue;

typedef std::map<astring, MiningModule *> MModuleMap;
class MFramework:public QThread
{
    Q_OBJECT
public:
	inline MFramework(Factory &pFactory);
	inline ~MFramework();
	inline BOOL Init();
public:
	inline void run();
public:
	inline static BOOL RetHandler(s32 id, MiningRet& ret, void * pParam);
	inline BOOL RetHandler1(s32 id, MiningRet& ret);
public:
	inline static BOOL DeviceChangeCallbackFunc(void* pData, FactoryDeviceChangeData change);
	inline BOOL DeviceChangeCallbackFunc1(FactoryDeviceChangeData change);
public:
	inline astring GetPluginPath();


private:
	MModuleMap m_MModules;
	MModuleRetQueue m_RetQueue;
	BOOL m_bExit;
	Factory &m_pFactory;
};

#include "mframeworkimpl.hpp"

#endif /* __M_FRAME_WORK_HPP__ */
