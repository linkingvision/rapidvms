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

typedef std::map<int, MiningModule *> MModuleMap;
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
};


astring MFramework::GetPluginPath()
{
#ifdef WIN32
    char exeFullPath[MAX_PATH]; // Full path

    string strPath = "";

    GetModuleFileNameA(NULL,exeFullPath, MAX_PATH);
    strPath=(string)exeFullPath;    // Get full path of the file

    int pos = strPath.find_last_of('\\', strPath.length());

    return strPath.substr(0, pos);  // Return the directory without the file name
#else
    return "./";
#endif
}

inline BOOL MFramework::Init()
{
	/* Get all the modules list and init  */
	std::string dir;

	//dir = Path::current();
	std::string pluginPath = GetPluginPath() + "/mplugins/";
	
	dir = pluginPath;

	try
	{
		DirectoryIterator it(dir);
		DirectoryIterator end;
		while (it != end)
		{
			/* loop for each dir */
			if (it->isDirectory() == true)
			{
				try 
				{
					DirectoryIterator it2(it->path());
					DirectoryIterator end2;
					while (it2 != end2)
					{
						if (it2.name().find("mplugin") == 0)
						{
							//Find a plugin
							// Call mmodule to add
						}
						++it2;
					}
				}	
				catch (Poco::Exception& exc)
				{
					continue;
				}
			}


#if 0
			Path p(it->path());
			if ()
			std::cout << (it->isDirectory() ? 'd' : '-')
					  << (it->canRead() ? 'r' : '-')
					  << (it->canWrite() ? 'w' : '-')
					  << ' '
					  << DateTimeFormatter::format(it->getLastModified(), DateTimeFormat::SORTABLE_FORMAT)
					  << ' '
					  << p.getFileName()
					  << std::endl;
#endif
			++it;
		}
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
	 	VDC_DEBUG( "Plugin init error \n");

		return ;
	}
	/* Get all the device and add channel to all the modules */
}


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
