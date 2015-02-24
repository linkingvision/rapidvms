//------------------------------------------------------------------------------
// File: mmoduleimpl.hpp
//
// Desc: Module for Data Mining.
//
// Copyright (c) 2014-2018. veyesys.com All rights reserved.
//------------------------------------------------------------------------------
#ifndef __M_MODULE_IMPL_HPP__
#define __M_MODULE_IMPL_HPP__

inline MiningModule::MiningModule(astring strPath)
:m_strPath(strPath), m_module(NULL)
{
}
inline MiningModule::~MiningModule()
{
}

inline BOOL MiningModule::AddChannel(s32 id)
{
	if (m_module)
	{
		return m_module->AddChannel(id);
	}
	return FALSE;
}
inline BOOL MiningModule::DelChannel(s32 id)
{
	if (m_module)
	{
		return m_module->DelChannel(id);
	}
	return FALSE;
}

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

#endif /* __M_MODULE_IMPL_HPP__ */
