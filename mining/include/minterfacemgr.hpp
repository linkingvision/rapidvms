//------------------------------------------------------------------------------
// File: minterfacemgr.hpp
//
// Desc: Interface manager for Data Mining.
//
// Copyright (c) 2014-2018. veyesys.com All rights reserved.
//------------------------------------------------------------------------------
#ifndef __M_INTERFACE_MGR_HPP__
#define __M_INTERFACE_MGR_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include "miningtype.hpp"
#include "minterface.hpp"
#include "factory.hpp"
class MiningInterfaceMgr
{
public:
	inline MiningInterfaceMgr(u32 id, Factory &pFactory, MiningInterface * pDevice);
	inline ~MiningInterfaceMgr();
public:
	inline BOOL Init();
	inline void RawHandler1(RawFrame& frame);
	inline static void RawHandler(RawFrame& frame, void * pParam);	

		
private:
	MiningInterface * m_pDevice;
	u32 m_id;
	Factory &m_pFactory;
};

#include "minterfacemgrimpl.hpp"

#endif /* __M_INTERFACE_MGR_HPP__ */
