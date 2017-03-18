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
#ifndef VSC_EVENT_H
#define VSC_EVENT_H

#include <QWidget>
#include "utility.hpp"
#include "server/factory.hpp"
#include "QEvent"
#include <QThread>
#include "onvifclient.hpp"
#include "vevent.hpp"

using namespace std;

class OnvifClientCam
{
public:
	OnvifClientCam()
	:onvifDevice(NULL), onvifEvent(NULL)
	{
		
	}
	~OnvifClientCam()
	{
		if (onvifDevice)
		{
			delete onvifDevice;
			onvifDevice = NULL;
		}
		if (onvifEvent)
		{
			delete onvifEvent;
			onvifEvent = NULL;
		}
	}
public:
	OnvifClientDevice *onvifDevice;
	OnvifClientEvent *onvifEvent;
	string ip;
	string onvifUrl;
	string eventNotify;
	string deviceName;
	string id;
	bool servermotion;
	time_t lastGotTime;
};

typedef std::map<string, OnvifClientCam*> OnvifMap;

class VOnvifEventThread : public QThread
{
    Q_OBJECT
public:
	VOnvifEventThread(Factory &pFactory, VEventServer &pEventServer)
	 :m_EventId(0), m_OnvifNotify(), 
	 m_pFactory(pFactory), m_pEventServer(pEventServer)
	{
	}
	~VOnvifEventThread()
	{
	}
public:
	void run();

	bool UpdateDeviceMap();
public:
	static bool OnvifEventCallbackFunc(void* pData, OnvifEvent& event);
	bool OnvifEventCallbackFunc1(OnvifEvent& event);

	static bool DeviceChangeCallbackFunc(void* pData, FactoryCameraChangeData change);
	bool DeviceChangeCallbackFunc1(FactoryCameraChangeData change);
public:
	bool InitDevices();
	bool RenewDevices();
	bool InitOneDevice(VidCamera  pParam);
	bool DeleteOneDevice(astring strId);
public:
	void Lock(){m_Lock.lock();}
	bool TryLock(){return m_Lock.try_lock();}
	void UnLock(){m_Lock.unlock();}

private:
	OnvifMap m_OnvifClientMap;
	VidCameraList m_DeviceMap;
	fast_mutex m_Lock;
	s64 m_EventId;
	OnvifClientEventNotify m_OnvifNotify;
	Factory &m_pFactory;
	VEventServer &m_pEventServer;
};

#endif // VSC_EVENT_H
