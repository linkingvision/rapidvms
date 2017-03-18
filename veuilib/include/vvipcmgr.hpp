/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#ifndef __V_V_IPC_MGR_HPP__
#define __V_V_IPC_MGR_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "server/factory.hpp"
#include <QThread>
#include "vonvifdismgr.hpp"

using namespace UtilityLib;

typedef std::map<int, QThread *> VIPCThreadMap;

class VE_LIBRARY_API VVIPCMgr
{
public:
	VVIPCMgr(Factory & pFactory, VONVIFDisMgr & pDisMgr);
	~VVIPCMgr();
public:
	static bool DeviceChangeCallbackFunc(void* pData, 
						FactoryDeviceChangeData change);
	bool DeviceChangeCallbackFunc1(FactoryDeviceChangeData change);
public:
	void Lock(){m_Lock.lock();}
	void UnLock(){m_Lock.unlock();}
private:
    	Factory & m_pFactory;
	VONVIFDisMgr & m_pDisMgr;
       VIPCThreadMap m_ThreadMap;
	fast_mutex m_Lock;
};

#endif /* __V_V_IPC_MGR_HPP__ */
