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
#include <QThread>

#include "vscevent.h"
#include "vsmartmotion.h"
#include "vscloading.hpp"
#include <algorithm> 

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QApplication>



bool VOnvifEventThread::OnvifEventCallbackFunc(void* pData, OnvifEvent& event)
{
	if (pData)
	{
		VOnvifEventThread * pthread = (VOnvifEventThread *)pData;
		pthread->OnvifEventCallbackFunc1(event);
	}
	return true;
}
bool VOnvifEventThread::OnvifEventCallbackFunc1(OnvifEvent& event)
{
	std::transform(event.event.begin(),event.event.end(),event.event.begin(),::tolower); 
	VDC_DEBUG("ip %s event %s desc %s\n", event.ip.c_str(), event.event.c_str(),
			event.desc.c_str());
	Lock();
	event.time = currentDateTime(time(NULL));
	OnvifMap::iterator it = m_OnvifClientMap.find(event.ip);
	VscEventType type;
	VDC_DEBUG("m_OnvifClientMap Size %d ip %s\n", m_OnvifClientMap.size(), 
		event.ip.c_str());
	if(it != m_OnvifClientMap.end())
	{
		std::size_t found = event.event.find("motion");
		if (found!= std::string::npos)
		{
			time_t current = time(NULL);
			time_t lastGotTime =  (*it).second->lastGotTime;
			if (current - lastGotTime  < 2)
			{
				VDC_DEBUG("Ignore < 2 seconds ID %d \n", m_EventId);
				UnLock();
				return true;
			}
			
			VDC_DEBUG( "ONIVF Motion Got\n");
			astring strEvtTime;
			strEvtTime = Time2String(current);		
			VEventData vEvent;
			vEvent.bMetaData = false;
			vEvent.strType = "MOTION";
			vEvent.strDevice = (*it).second->id;;
			vEvent.strDeviceName = (*it).second->deviceName;
			vEvent.strEvttime = strEvtTime;
			vEvent.nTime = current;
			vEvent.strDesc = "Motion Detection";

			m_pEventServer.PushEvent(vEvent);
		}
	}
	UnLock();
	return true;
}



bool VOnvifEventThread::DeviceChangeCallbackFunc(void* pData, 
								FactoryCameraChangeData change)
{
	if (pData)
	{
		VOnvifEventThread * pthread = (VOnvifEventThread *)pData;
		pthread->DeviceChangeCallbackFunc1(change);
	}
	return true;
}
bool VOnvifEventThread::DeviceChangeCallbackFunc1(FactoryCameraChangeData change)
{
	VDC_DEBUG( "Event Device Change Callback %s type %d Begin\n", change.id.c_str(), change.type);
	
	if (change.type == FACTORY_CAMERA_OFFLINE)
	{
		DeleteOneDevice(change.id);
	}
	if (change.type == FACTORY_CAMERA_ONLINE)
	{
		Lock();
		VidCamera  pParam;
		if (m_pFactory.GetConfDB().GetCameraConf(change.id, pParam) == true)
		{
			InitOneDevice(pParam);
		}
		UnLock();
	}
	VDC_DEBUG( "Event Device Change Callback %s type %d End \n", change.id.c_str(), change.type);

	return true;
}

void VOnvifEventThread::run()
{
	UpdateDeviceMap();
	int ret;
	InitDevices();
	m_pFactory.RegCameraChangeNotify((void *)this, 
				(FactoryCameraChangeNotify)(VOnvifEventThread::DeviceChangeCallbackFunc));
	
	OnvifClientEventNotify &notify = m_OnvifNotify;
	notify.SetCallback((void *)this, (OnvifEventCallback)(VOnvifEventThread::OnvifEventCallbackFunc));
	
	if (notify.bind(NULL, 9090, 100) == SOAP_INVALID_SOCKET ) 
	{
	    VDC_DEBUG("OnvifClientEventNotify::soap_bind Binding on %d port failed", 9090);
	    return;
	}
	int timeStart = time(NULL);
	int currentTime = 0;

	//Loop to recevie the event
	while(1)
	{
		currentTime = time(NULL);
		if (currentTime - timeStart > 600)
		{
			RenewDevices();
			timeStart = time(NULL);
		}
		//printf("soap_accept accepting\n");
		if( notify.accept() == SOAP_INVALID_SOCKET) {
			//VDC_DEBUG("Event Server soap_accept accepting timeout\n");
			continue;
		}

		ret = notify.serve();	
		if ( ret != SOAP_OK) {
		    //VDC_DEBUG("Event Server soap_begin_serve serve %d failed", ret);
		    continue;
		}

	        ret = notify.dispatch();
	        continue;
	}
	return;
}

bool VOnvifEventThread::InitDevices()
{
	Lock();

	int cameraSize = m_DeviceMap.cvidcamera_size();

	for (s32 i = 0; i < m_DeviceMap.cvidcamera_size(); i ++)
	{
		VidCamera cam = m_DeviceMap.cvidcamera(i);
		InitOneDevice(cam);
	}
	UnLock();
	return true;
}

bool VOnvifEventThread::RenewDevices()
{
	Lock();
        OnvifMap::iterator it = m_OnvifClientMap.begin(); 
        for(; it!=m_OnvifClientMap.end(); ++it)
        {
			(*it).second->onvifEvent->Renew();
        }
	UnLock();
	return true;
}

bool VOnvifEventThread::DeleteOneDevice(astring strId)
{
	Lock();
        OnvifMap::iterator it = m_OnvifClientMap.begin(); 
        for(; it!=m_OnvifClientMap.end(); ++it)
        {
            if ((*it).first == strId)
            {	
            	  delete (*it).second;
               m_OnvifClientMap.erase(it);
            	break;
            }
        }
	UnLock();
	return true;
}

bool VOnvifEventThread::InitOneDevice(VidCamera  pParam)
{
	if (pParam.ntype() != VID_ONVIF_S)
	{
		return true;
	}

	astring IP = pParam.strip();
	astring Port = pParam.strport();
	astring User = pParam.struser();
	astring Password = pParam.strpasswd();
	astring OnvifAddress = pParam.stronvifaddress();

	astring OnvifDeviceService = "http://" + IP + ":" + Port + OnvifAddress;
	astring Name = pParam.strname();

	OnvifClientDevice * pOnvifDevice = new OnvifClientDevice(OnvifDeviceService, User, Password);
	pOnvifDevice->GetCapabilities();//TODO Reget GetCapabilities 
	OnvifClientEvent * pOnvifEvent = new OnvifClientEvent(*pOnvifDevice);
	astring strLocal;
	pOnvifDevice->GetLocalAddr(strLocal);

	OnvifClientCam *pClientMap = new OnvifClientCam;
	pClientMap->onvifDevice = pOnvifDevice;
	pClientMap->onvifEvent = pOnvifEvent;
	pClientMap->eventNotify = "http://" + strLocal + ":9090/subscription-2";
	pClientMap->ip = IP;
	pClientMap->onvifUrl = OnvifDeviceService;
	pClientMap->deviceName = Name;
	pClientMap->id = pParam.strid();
	pClientMap->lastGotTime = 0;
	pClientMap->servermotion = pParam.bservermotion();

	pOnvifEvent->Subscribe(pClientMap->eventNotify);
	m_OnvifClientMap[pParam.strip()] = pClientMap;

	return true;
}

bool VOnvifEventThread::UpdateDeviceMap()
{
	m_pFactory.GetCameraList(m_DeviceMap);
	return TRUE;
}

bool VEventMgr::Init()
{
	m_OnvifThread = new VOnvifEventThread(m_pFactory, m_pEventServer);
	m_SmartMotionThread = new VSmartMotionThread(m_pFactory, m_pEventServer);
	m_OnvifThread->start();
	m_SmartMotionThread->start();
	return true;
}

