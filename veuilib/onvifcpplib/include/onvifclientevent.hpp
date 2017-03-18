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
#ifndef __ONVIF_CLIENT_EVENT__
#define __ONVIF_CLIENT_EVENT__

#include <string>
#include <map>
#include <iostream>
#include <list>

#include "onvifclientdevice.hpp"
#include "soapStub.h"
#include "soapPullPointSubscriptionBindingProxy.h"
#include "soapPullPointSubscriptionBindingService.h"
#include "soapNotificationProducerBindingProxy.h"
#include "soapPullPointBindingProxy.h"
#include "soapSubscriptionManagerBindingProxy.h"
#include "soapNotificationConsumerBindingService.h"
#include "soapPullPointBindingService.h"
#include "wsseapi.h"
#include "wsaapi.h"


using namespace std;

class OnvifEvent
{
public:
	string name;
	string ip;
	string event;
	string desc;
	string time;
};

typedef bool (*OnvifEventCallback)(void* pData, OnvifEvent& event);

class OnvifClientEventNotify : public PullPointBindingService
{
public:
	OnvifClientEventNotify()
	: m_EventCnt(0), PullPointBindingService(), 
	m_pData(NULL), m_eventHandler(NULL)
	{
	
	}
public:
	bool SetCallback(void * pData, OnvifEventCallback callback)
	{
		m_pData = pData;
		m_eventHandler = callback;
		return true;
	}
	virtual int Notify(_wsnt__Notify *wsnt__Notify)
	{
		OnvifEvent event;
		//printf("\nMessage size %d\n", wsnt__Notify->NotificationMessage.size());
		if(wsnt__Notify->NotificationMessage.size() > 0 )
		{
			m_EventCnt ++;
			//printf("\nONVIF CPP Lib Event %d host %s __mixed: %s\n", m_EventCnt, host,
			//		wsnt__Notify->NotificationMessage[0]->Topic->__mixed);
			//printf("\t__any: %s\n", wsnt__Notify->NotificationMessage[0]->Topic->__any);
			//printf("\t__item: %s\n", wsnt__Notify->NotificationMessage[0]->Topic->__item);
			//printf("\t__anyAttribute: %s\n", wsnt__Notify->NotificationMessage[0]->Topic->__anyAttribute);
			//printf("\tDialect: %s\n", wsnt__Notify->NotificationMessage[0]->Topic->Dialect.c_str());
			//printf("\tMessage %s\n", wsnt__Notify->NotificationMessage[0]->Message.__any);
			event.ip =  host;
			event.event = wsnt__Notify->NotificationMessage[0]->Topic->__item;
			event.desc = wsnt__Notify->NotificationMessage[0]->Message.__any;
			//printf("\nMessage event %s\n", event.event.c_str());
			event.desc = "";
			if (m_eventHandler)
			{
				m_eventHandler(m_pData, event);
			}
		}
		return SOAP_OK;	
	}
	
public:
	virtual PullPointBindingService *copy()
	{
		return NULL;
	}
	
private:
	int m_EventCnt;
	void* m_pData;
	OnvifEventCallback m_eventHandler;
};

class OnvifClientEvent
{
public:
    OnvifClientEvent(OnvifClientDevice &device);
    ~OnvifClientEvent();
public:
	/* Add function to here */
	int Subscribe(string &notifyUrl);
	int UnSubscribe();
	int Renew();
				
private:
	OnvifClientDevice &m_Device;
	string m_RenewEndpoint;
	NotificationProducerBindingProxy  eventBinding;
	SubscriptionManagerBindingProxy eventManagerBinding;

};

inline OnvifClientEvent::OnvifClientEvent(OnvifClientDevice &device)
: m_Device(device), eventBinding(SOAP_C_UTFSTRING)
{

}

inline OnvifClientEvent::~OnvifClientEvent()
{

}

inline int OnvifClientEvent::Subscribe(string &notifyUrl)
{
	string strUrl;
	string strUser;
	string strPass;
	_wsnt__Subscribe subscribe;
	_wsnt__SubscribeResponse  subscribeResponse;
	string strTimeOut = "PT600S";
	
	if (m_Device.GetUserPasswd(strUser, strPass) == false 
		|| m_Device.GetEventUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	
	subscribe.ConsumerReference.Address = (char *)(notifyUrl.c_str());
	subscribe.InitialTerminationTime = &strTimeOut;
	
	eventBinding.soap_endpoint =  strUrl.c_str();
	
	soap_wsse_add_Security(&eventBinding);
	soap_wsse_add_UsernameTokenDigest(&eventBinding, "Id", 
		strUser.c_str() , strPass.c_str());

	//soap_wsa_add_From(eventBinding.soap, "http://www.w3.org/2005/08/addressing/anonymous");
	//soap_wsa_add_ReplyTo(eventBinding.soap, "http://www.w3.org/2005/08/addressing/anonymous");
	//soap_wsa_add_FaultTo(eventBinding.soap, "http://www.w3.org/2005/08/addressing/anonymous");
	if (eventBinding.header->wsa5__ReplyTo == NULL)
		eventBinding.header->wsa5__ReplyTo = new wsa5__EndpointReferenceType();
	eventBinding.header->wsa5__ReplyTo->Address = "http://www.w3.org/2005/08/addressing/anonymous";
	eventBinding.header->wsa5__To = const_cast<char *>(notifyUrl.c_str());
	eventBinding.header->wsa5__Action = "http://docs.oasis-open.org/wsn/bw-2/NotificationProducer/SubscribeRequest";

	int ret = eventBinding.Subscribe(&subscribe,&subscribeResponse);
	if (ret == SOAP_OK)
	{
		if (subscribeResponse.SubscriptionReference.Address)
		{
			m_RenewEndpoint = subscribeResponse.SubscriptionReference.Address;
		}else if (subscribeResponse.SubscriptionReference.__size > 0)
		{
			m_RenewEndpoint = subscribeResponse.SubscriptionReference.__any[0];
			if (m_RenewEndpoint.length() > 0)
			{
				size_t p1 = m_RenewEndpoint.find_first_of(">");
				size_t p2 = m_RenewEndpoint.find_last_of("<");
				if (p1 != std::string::npos && p2 != std::string::npos)
				{
					std::string strTemp = m_RenewEndpoint.substr(p1 + 1, p2 - (p1 + 1));
					m_RenewEndpoint = strTemp;
				}
				
			}
		}
		printf("SubscriptionReference %s\n", m_RenewEndpoint.c_str());
		
	}
	return ret;
}

inline int OnvifClientEvent::UnSubscribe()
{
	string strUrl;
	string strUser;
	string strPass;
	_wsnt__Unsubscribe unsubscribe;
	_wsnt__UnsubscribeResponse  unsubscribeResponse;
	
	if (m_Device.GetUserPasswd(strUser, strPass) == false 
		|| m_Device.GetEventUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	
	eventManagerBinding.soap_endpoint =  strUrl.c_str();
	
	soap_wsse_add_Security(&eventManagerBinding);
	soap_wsse_add_UsernameTokenDigest(&eventManagerBinding, "Id", 
		strUser.c_str() , strPass.c_str());

	if (eventManagerBinding.header->wsa5__ReplyTo == NULL)
		eventManagerBinding.header->wsa5__ReplyTo = new wsa5__EndpointReferenceType();
	eventManagerBinding.header->wsa5__ReplyTo->Address = "http://www.w3.org/2005/08/addressing/anonymous";
	eventManagerBinding.header->wsa5__To = const_cast<char *>(m_RenewEndpoint.c_str());

		
	return eventManagerBinding.Unsubscribe(&unsubscribe,&unsubscribeResponse);

}
inline int OnvifClientEvent::Renew()
{
	string strUrl;
	string strUser;
	string strPass;
	_wsnt__Renew req;
	_wsnt__RenewResponse  resp;
	string strTimeOut = "PT600S";
	if (m_RenewEndpoint.c_str() == NULL)
	{
		return SOAP_ERR;
	}
	
	if (m_Device.GetUserPasswd(strUser, strPass) == false 
		|| m_Device.GetEventUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	
	req.TerminationTime = &strTimeOut;
	
	eventManagerBinding.soap_endpoint =  strUrl.c_str();
	
	soap_wsse_add_Security(&eventManagerBinding);
	soap_wsse_add_UsernameTokenDigest(&eventManagerBinding, "Id", 
		strUser.c_str() , strPass.c_str());

	if (eventManagerBinding.header->wsa5__ReplyTo == NULL)
		eventManagerBinding.header->wsa5__ReplyTo = new wsa5__EndpointReferenceType();
	eventManagerBinding.header->wsa5__ReplyTo->Address = "http://www.w3.org/2005/08/addressing/anonymous";
	eventManagerBinding.header->wsa5__To = const_cast<char *>(m_RenewEndpoint.c_str());

		
	int ret = eventManagerBinding.Renew(&req,&resp);
	return ret;
}


#endif 
