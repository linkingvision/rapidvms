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
#ifndef __VE_MEDIA_SERVER_H__
#define __VE_MEDIA_SERVER_H__
#include "server/factory.hpp"

class VEMediaChannel;

typedef std::map<string, VEMediaChannel*> VEMediaChannelMap;

class VEMediaServer 
{
public:
	VEMediaServer(Factory &pFactory);
	~VEMediaServer();
	
public:
	static bool InitMediaServer();
	
public:
	static bool DeviceChangeCallbackFunc(void* pData, FactoryCameraChangeData change);
	bool DeviceChangeCallbackFunc1(FactoryCameraChangeData change);
	
public:
	bool InitDevices();
	bool InitOneDevice(VidCamera  pParam);
	bool DeleteOneDevice(astring strId);
	
private:
	VEMediaChannelMap m_ChannelMap;
	Factory &m_pFactory;
};

#endif /* __VE_MEDIA_SERVER_H__ */
