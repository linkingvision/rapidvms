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
		//TODO return;
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
	//TODO add callback device delete from factory

	return true;
}
#if 0
/* Process decoded or compressed data */
inline BOOL MiningModule::Process(s32 id, VideoFrame& frame)
{
	if (m_module)
	{
		return m_module->Process(id, frame);
	}
	return FALSE;
}
inline BOOL MiningModule::ProcessRaw(s32 id, RawFrame& frame)
{
	if (m_module)
	{
		return m_module->ProcessRaw(id, frame);
	}
	return FALSE;
}

/* Get the stream type of this module */
inline BOOL MiningModule::GetReqStream(MMReqStream& type)
{
	if (m_module)
	{
		return m_module->GetReqStream(type);
	}
	return FALSE;
}

inline BOOL MiningModule::RegRetCallback(MiningCallbackFunctionPtr pCallback, void * pParam)
{
	if (m_module)
	{
		return m_module->RegRetCallback(pCallback, pParam);
	}
	return FALSE;
}
inline BOOL MiningModule::UnRegDataCallback(void * pParam)
{
	if (m_module)
	{
		return m_module->UnRegDataCallback(pParam);
	}
	return FALSE;
}
inline u32 MiningModule::GetFlags()
{
	if (m_module)
	{
		return m_module->GetFlags();
	}
	return FALSE;
}
inline astring MiningModule::GetVersion()
{
	if (m_module)
	{
		return m_module->GetVersion();
	}
	return FALSE;
}
#endif

#endif /* __M_MODULE_IMPL_HPP__ */
