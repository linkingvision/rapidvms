//------------------------------------------------------------------------------
// File: mmoduleimpl.hpp
//
// Desc: Module for Data Mining.
//
// Copyright (c) 2014-2018. veyesys.com All rights reserved.
//------------------------------------------------------------------------------
#ifndef __M_MODULE_IMPL_HPP__
#define __M_MODULE_IMPL_HPP__

inline MiningModule::MiningModule(astring strPath, Factory &pFactory)
:m_strPath(strPath), m_module(NULL), m_Init(FALSE), m_pFactory(pFactory)
{
	try 
	{ 
		VDC_DEBUG( "%s Connect Mining  lib %s !.\n",__FUNCTION__, m_strPath.c_str());	
		m_dynLib.load(m_strPath); 
		m_CreateDevice = 
				(MiningCreateDeviceFunc)m_dynLib.resolve_symbol("MiningCreateDevice");
		if (m_CreateDevice != NULL)
		{
			m_Init = TRUE;
			VDC_DEBUG( "%s Mining lib load successfully.\n",__FUNCTION__);	
		}
	} catch( ... ) 
	{ 
		m_Init = FALSE;
		VDC_DEBUG( "%s Mining lib load error \n",__FUNCTION__);	
	}	
}
inline MiningModule::~MiningModule()
{
}

BOOL MiningModule::DeviceChangeCallbackFunc(void* pData, 
								FactoryDeviceChangeData change)
{
	if (pData)
	{
		MiningModule * pModule = (MiningModule *)pData;
		pModule->DeviceChangeCallbackFunc1(change);
	}
	return TRUE;
}
BOOL MiningModule::DeviceChangeCallbackFunc1(FactoryDeviceChangeData change)
{
	VDC_DEBUG( "MiningModule Device Change Callback %d type %d Begin\n", 
					change.id, change.type);
	
	if (change.type == FACTORY_DEVICE_DEL)
	{
		DeleteOneDevice(change.id);
	}
	if (change.type == FACTORY_DEVICE_ADD)
	{
		DeviceParam Param;
		m_pFactory.GetDeviceParamById(Param, change.id);
		
		InitOneDevice(Param);
	}
	VDC_DEBUG( "MiningModule Device Change Callback %d type %d End \n", 
					change.id, change.type);

	return true;
}

BOOL MiningModule::DeleteOneDevice(int id)
{
	MiningInterfaceMgr *pMgr = NULL;
        MInterfaceMgrMap::iterator it = m_mgrMap.begin(); 
        for(; it!=m_mgrMap.end(); ++it)
        {
            if ((*it).second->GetId() == id)
            {
				pMgr = (*it).second;
				m_mgrMap.erase(it);
            	break;
            }
        }

	if (pMgr)
	{
		delete pMgr;
		pMgr = NULL;
	}

	return TRUE;
}

inline BOOL MiningModule::Valid()
{
	return m_Init;
}

inline BOOL MiningModule::InitOneDevice(DeviceParam & Param)
{
	u32 id = Param.m_Conf.data.conf.nId;
	if (Param.m_Conf.data.conf.Mining == 0)
	{
		VDC_DEBUG( "%s Device %d not enable Mining.\n",__FUNCTION__, id);	
		return FALSE;
	}
	MiningInterface *pInterface = m_CreateDevice(id);	
	if (pInterface == NULL)
	{
		return FALSE;
	}
	MiningInterfaceMgr *pMgr = new MiningInterfaceMgr(id, m_pFactory, pInterface);

	m_mgrMap[id] = pMgr;


	return TRUE;
}

inline BOOL MiningModule::Init()
{
	if (Valid() == FALSE)
	{
		return FALSE;
	}

	/* Loop to get all the camera that enable the mining */
	DeviceParamMap pDeviceMap;
	m_pFactory.GetDeviceParamMap(pDeviceMap);

        DeviceParamMap::iterator it = pDeviceMap.begin(); 
        for(; it!=pDeviceMap.end(); ++it)
        {
            InitOneDevice((*it).second);
        }

	
	m_pFactory.RegDeviceChangeNotify((void *)this, 
			(FactoryDeviceChangeNotify)(MiningModule::DeviceChangeCallbackFunc));


	return true;
}


#endif /* __M_MODULE_IMPL_HPP__ */
