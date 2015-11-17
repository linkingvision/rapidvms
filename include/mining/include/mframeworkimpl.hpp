//------------------------------------------------------------------------------
// File: mframeworkimpl.hpp
//
// Desc: Data Mining Framework.
//
// Copyright (c) 2014-2018. veyesys.com All rights reserved.
//------------------------------------------------------------------------------
#ifndef __M_FRAME_WORK_IMPL_HPP__
#define __M_FRAME_WORK_IMPL_HPP__

inline MFramework::MFramework(Factory &pFactory)
:m_pFactory(pFactory)
{
}
inline MFramework::~MFramework()
{
}

inline astring MFramework::GetPluginPath()
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
				astring strName = it->path();
				try 
				{
					DirectoryIterator it2(it->path());
					DirectoryIterator end2;

					while (it2 != end2)
					{
						astring strName2 = it2->path();
						if (it2->path().find("mplugin") != std::string::npos)
						{
							/* Find a plugin and call mmodule to add */
							astring strPluginName = it2->path();
							MiningModule * pModule = 
									new MiningModule(strPluginName, m_pFactory);
							if (pModule && pModule->Valid() == TRUE)
							{
								pModule->Init();
								m_MModules[strPluginName] = pModule;
							}
							
						}
						++it2;
					}
				}	
				catch (Poco::Exception& exc)
				{
					continue;
				}
			}

			++it;
		}
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
	 	VDC_DEBUG( "Plugin init error \n");

		return FALSE;
	}

	return TRUE;
}


inline BOOL MFramework::RetHandler(s32 id, MiningRet& ret, void * pParam)
{
	int dummy = errno;
	MFramework *pMFramework = (MFramework*)pParam;

	if (pMFramework)
	{
		return pMFramework->RetHandler1(id, ret);
	}

	return TRUE;
}

inline BOOL MFramework::RetHandler1(s32 id, MiningRet& ret)
{
	m_RetQueue.post(ret);

	return TRUE;
}

inline void MFramework::run()
{
	MiningRet ret;
	while (m_bExit != TRUE)
	{
		//VDC_DEBUG("MFramework runing\n");
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

inline BOOL MFramework::DeviceChangeCallbackFunc(void* pData, 
								FactoryDeviceChangeData change)
{
	if (pData)
	{
		MFramework * pthread = (MFramework *)pData;
		pthread->DeviceChangeCallbackFunc1(change);
	}
	return true;
}
inline BOOL MFramework::DeviceChangeCallbackFunc1(FactoryDeviceChangeData change)
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
