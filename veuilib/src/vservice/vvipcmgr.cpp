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
#include "vvipcmgr.hpp"
#include "onvifserver.hpp"
#include "onvifserverdiscover.hpp"
#include "onvifserverevent.hpp"


class VVIPCImpl : public QThread
{
public:
    VVIPCImpl(VIPCDeviceParam &pParam);
    ~VVIPCImpl();
public:
    void run();
public:
    void StopVIPC();
	
private:
    bool m_bQuit;
    bool m_bRunning;
    VIPCDeviceParam m_Param;
    

};

VVIPCImpl::VVIPCImpl(VIPCDeviceParam &pParam)
:m_bQuit(false), m_Param(pParam), m_bRunning(false)
{

}

VVIPCImpl::~VVIPCImpl()
{
}


void VVIPCImpl::StopVIPC()
{
    m_bRunning = false;
    return;
}

void VVIPCImpl::run()
{
	m_bRunning = true;
	string ip = m_Param.m_Conf.data.conf.IP;
	string port = m_Param.m_Conf.data.conf.Port;
	int portStart = atoi(port.c_str());
#define VVIPC_PORT_LEN 128
	char strPort[VVIPC_PORT_LEN];
	memset(strPort, 0, VVIPC_PORT_LEN);
	sprintf(strPort, "%d", portStart + 1);
	string portMedia = strPort;

	memset(strPort, 0, VVIPC_PORT_LEN);
	sprintf(strPort, "%d", portStart + 2);

	string portReceiver = strPort;

	memset(strPort, 0, VVIPC_PORT_LEN);
	sprintf(strPort, "%d", portStart + 3);

	string portEvent = strPort;
	
	string strEndpoint = "http://" + ip + ":" + port + "/onvif/device_service";
	string strEndpointMedia = "http://" + ip + ":" + portMedia + "/onvif/device_service";
	string strEndpointReceiver = "http://" + ip + ":" + portReceiver + "/onvif/device_service";
	string strEndpointEvent = "http://" + ip + ":" + portEvent + "/onvif/device_service";
	
	OnvifServerEventProducer *pEventProducer = new OnvifServerEventProducer();
	OnvifServerDevice *pDevice = new OnvifServerDevice(strEndpoint, strEndpointMedia, 
			strEndpointReceiver, strEndpointReceiver);
	OnvifServerReceiver *pReceiver = new OnvifServerReceiver();
	OnvifServerEvent *pEvent = new OnvifServerEvent(*pEventProducer, strEndpoint);
	OnvifServerMedia *pMedia = new OnvifServerMedia(m_Param.m_Conf.data.conf.nStreamId);
	OnvifServer server(pDevice, pMedia, pReceiver, pEvent);

	server.Run(ip, portStart, m_bRunning);

	delete pMedia;
	delete pEvent;
	delete pDevice;
	delete pEventProducer;

    	return;
}


VVIPCMgr::VVIPCMgr(Factory & pFactory, VONVIFDisMgr & pDisMgr)
:m_pFactory(pFactory), m_pDisMgr(pDisMgr)
{
	/* Get all the vipc and add to the Dismgr and then start the gsoap thread */
	VIPCDeviceParamMap paramMap;
	m_pFactory.GetVIPCDeviceParamMap(paramMap);

	VIPCDeviceParamMap::iterator it = paramMap.begin(); 
	for(; it!=paramMap.end(); ++it)
	{
	    
	    VVIPCImpl *pImpl = new VVIPCImpl((*it).second);
	    m_pDisMgr.AddHost((*it).second.m_Conf.data.conf.IP, 
	                            (*it).second.m_Conf.data.conf.Port, "NetworkVideoTransmitter");
	    pImpl->start();
	    //thread.wait()
	    m_ThreadMap[(*it).first] = pImpl;
	}
	m_pFactory.RegDeviceChangeNotify((void *)this, 
				(FactoryDeviceChangeNotify)(VVIPCMgr::DeviceChangeCallbackFunc));
	
}

VVIPCMgr::~VVIPCMgr()
{

}

bool VVIPCMgr::DeviceChangeCallbackFunc(void* pData, 
								FactoryDeviceChangeData change)
{
	if (pData)
	{
		VVIPCMgr * pMgr = (VVIPCMgr *)pData;
		pMgr->DeviceChangeCallbackFunc1(change);
	}
	return true;
}
bool VVIPCMgr::DeviceChangeCallbackFunc1(FactoryDeviceChangeData change)
{
	VDC_DEBUG( "VVIPCMgr Device Change Callback %d type %d begin\n", change.id, change.type);
	
	if (change.type == FACTORY_VIPC_DEL)
	{
		if (m_ThreadMap[change.id])
		{
			VIPCDeviceParam Param;
			m_pFactory.GetVIPCParamById(Param, change.id);
			Lock();
			VVIPCImpl *pVIPC = dynamic_cast<VVIPCImpl*> (m_ThreadMap[change.id]);
			if (pVIPC)
				pVIPC->StopVIPC();
			/* Wait the thread to exit */
			m_ThreadMap[change.id]->wait();
			m_pDisMgr.DelHost(Param.m_Conf.data.conf.IP, 
		                        Param.m_Conf.data.conf.Port);
			delete m_ThreadMap[change.id];
			m_ThreadMap.erase(change.id);
			UnLock();
		}
		
	}
	if (change.type == FACTORY_VIPC_ADD)
	{
		VIPCDeviceParam Param;
		m_pFactory.GetVIPCParamById(Param, change.id);
		Lock();
		VVIPCImpl *pImpl = new VVIPCImpl(Param);
		m_pDisMgr.AddHost(Param.m_Conf.data.conf.IP, 
		                        Param.m_Conf.data.conf.Port, "NetworkVideoTransmitter");
		pImpl->start();
		m_ThreadMap[change.id] = pImpl;
		UnLock();
	}
	VDC_DEBUG( "VVIPCMgr Device Change Callback %d type %d begin\n", change.id, change.type);

	return true;
}


