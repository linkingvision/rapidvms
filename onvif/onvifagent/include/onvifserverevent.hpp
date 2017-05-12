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
#ifndef __ONVIF_SERVER_EVENT__
#define __ONVIF_SERVER_EVENT__

#include <string>
#include <map>
#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include "fast_mutex.h"

#include "onvifclientdevice.hpp"
#include "soapStub.h"
#include "soapPullPointSubscriptionBindingProxy.h"
#include "soapPullPointSubscriptionBindingService.h"
#include "wsseapi.h"
#include "wsaapi.h"

#define INFO_LENGTH 128
#define LARGE_INFO_LENGTH 1024


using namespace std;

class OnvifServerEventProducer
{
public:
    OnvifServerEventProducer();
    ~OnvifServerEventProducer();
public:
	void AddConsumer(std::string consumerEndpoint );
	void NotifyMessage();
	void Lock()
	{
		m_pMutex.lock();
	}
	void UnLock()
	{
		m_pMutex.unlock();
	}
private:
	tthread::fast_mutex m_pMutex;
	std::vector<std::string> m_subscribers;
	
	
};

inline OnvifServerEventProducer::OnvifServerEventProducer()
{
}

inline OnvifServerEventProducer::~OnvifServerEventProducer()
{

}

inline void OnvifServerEventProducer::NotifyMessage()
{
	PullPointSubscriptionBindingProxy m_proxy(*(soap_new()));
	SOAP_ENV__Header m_header;
#if 0
	m_header.wsa5__Action = (char *) soap_malloc( &m_proxy, 1024 );
	strcpy(m_header.wsa5__Action, "http://docs.oasis-open.org/wsn/bw-2/NotificationConsumer/Notify");
	//m_header.wsa__MessageID = NULL;
	//m_header.wsa__To = NULL;
       //m_header.wsa__Action = NULL;
	m_proxy.soap->header = &m_header;
#endif
	
	_wsnt__Notify message;
	wsnt__NotificationMessageHolderType *holder = soap_instantiate_wsnt__NotificationMessageHolderType(&m_proxy, -1, "", "", NULL);
	wsnt__TopicExpressionType * topic = soap_instantiate_wsnt__TopicExpressionType(&m_proxy, -1, "", "", NULL );
	topic->Dialect = "http://docs.oasis-open.org/wsn/t-1/TopicExpression/Concrete";
	holder->Topic = topic;
	holder->SubscriptionReference = NULL;
	holder->ProducerReference = NULL;
	message.NotificationMessage.push_back(holder);
	Lock();
	std::vector<std::string> localSubs =  m_subscribers;
	UnLock();
        for( std::vector<std::string>::const_iterator it = localSubs.begin();
             it != localSubs.end(); ++it )
	{
	     printf("Notify %s\n", (*it).c_str());
            //m_proxy.Notify((*it).c_str(), 
			//	"http://docs.oasis-open.org/wsn/bw-2/NotificationConsumer/Notify", &message);
        }
}

inline void OnvifServerEventProducer::AddConsumer(std::string consumerEndpoint)
{
	printf("OnvifServerEventProducer::AddConsumer %s\n", consumerEndpoint.c_str());
	Lock();
	if( std::find( m_subscribers.begin(), m_subscribers.end(), consumerEndpoint ) == m_subscribers.end() ) {
		m_subscribers.push_back( consumerEndpoint );
	}
	UnLock();
	
	return;
}

class OnvifServerEvent : public PullPointSubscriptionBindingService
{
public:
	OnvifServerEvent(OnvifServerEventProducer &pProducer, string strEventEndpoint)
	: m_EventCnt(0), PullPointSubscriptionBindingService(), m_Producer(pProducer), m_strEventEndpoint(strEventEndpoint)
	{
	
	}
public:
	//TODO add callback to here
	virtual	int Notify(_wsnt__Notify *wsnt__Notify)
	{
		//printf("\nMessage size %d\n", wsnt__Notify->NotificationMessage.size());
		if(wsnt__Notify->NotificationMessage.size() > 0 )
		{
			m_EventCnt ++;
			//printf("\nONVIF CPP Lib Event %d host %s __mixed: %s\n", m_EventCnt, soap->host,
			//		wsnt__Notify->NotificationMessage[0]->Topic->__mixed);
			//printf("\t__any: %s\n", wsnt__Notify->NotificationMessage[0]->Topic->__any);
			//printf("\t__anyAttribute: %s\n", wsnt__Notify->NotificationMessage[0]->Topic->__anyAttribute);
			//printf("\tDialect: %s\n", wsnt__Notify->NotificationMessage[0]->Topic->Dialect.c_str());
			//printf("\tMessage %s\n", wsnt__Notify->NotificationMessage[0]->Message.__any);
		}
		
		return SOAP_OK;	
	}
	
public:
	virtual	PullPointSubscriptionBindingService *copy()
	{
		return NULL;
	}
	/// Web service operation 'PullMessages' (returns error code or SOAP_OK)
	virtual	int PullMessages(_tev__PullMessages *tev__PullMessages, _tev__PullMessagesResponse *tev__PullMessagesResponse) 
	{return SOAP_OK;}

	/// Web service operation 'Seek' (returns error code or SOAP_OK)
	virtual	int Seek(_tev__Seek *tev__Seek, _tev__SeekResponse *tev__SeekResponse) 
	{return SOAP_OK;}

	/// Web service operation 'SetSynchronizationPoint' (returns error code or SOAP_OK)
	virtual	int SetSynchronizationPoint(_tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, _tev__SetSynchronizationPointResponse *tev__SetSynchronizationPointResponse) 
	{return SOAP_OK;}

	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities(_tev__GetServiceCapabilities *tev__GetServiceCapabilities, _tev__GetServiceCapabilitiesResponse *tev__GetServiceCapabilitiesResponse) 
	{return SOAP_OK;}

	/// Web service operation 'CreatePullPointSubscription' (returns error code or SOAP_OK)
	virtual	int CreatePullPointSubscription(_tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, _tev__CreatePullPointSubscriptionResponse *tev__CreatePullPointSubscriptionResponse) 
	{return SOAP_OK;}

	/// Web service operation 'GetEventProperties' (returns error code or SOAP_OK)
	virtual	int GetEventProperties(_tev__GetEventProperties *tev__GetEventProperties, _tev__GetEventPropertiesResponse *tev__GetEventPropertiesResponse) 
	{return SOAP_OK;}

	/// Web service operation 'Renew' (returns error code or SOAP_OK)
	virtual	int Renew(_wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse *wsnt__RenewResponse) 
	{return SOAP_OK;}

	/// Web service operation 'Unsubscribe' (returns error code or SOAP_OK)
	virtual	int Unsubscribe(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse) 
	{return SOAP_OK;}

	/// Web service operation 'Subscribe' (returns error code or SOAP_OK)
	virtual	int Subscribe(_wsnt__Subscribe *wsnt__Subscribe, _wsnt__SubscribeResponse *wsnt__SubscribeResponse) 
	{
		std::string consumer( wsnt__Subscribe->ConsumerReference.Address );

		wsnt__SubscribeResponse->SubscriptionReference.Address = (char *) soap_malloc(this, sizeof(char) * INFO_LENGTH);
		strcpy( wsnt__SubscribeResponse->SubscriptionReference.Address,
		        m_strEventEndpoint.c_str() );

		time_t current = time(0);
		wsnt__SubscribeResponse->CurrentTime = (time_t *)soap_malloc(this, sizeof(time_t *));
		memcpy(wsnt__SubscribeResponse->CurrentTime, &current, sizeof(time_t));
		wsnt__SubscribeResponse->TerminationTime = (time_t *)soap_malloc(this, sizeof(time_t *));
		current += 1000000;
		memcpy(wsnt__SubscribeResponse->TerminationTime, &current, sizeof(time_t));

		m_Producer.AddConsumer( consumer );
		return SOAP_OK;
	}

	/// Web service operation 'GetCurrentMessage' (returns error code or SOAP_OK)
	virtual	int GetCurrentMessage(_wsnt__GetCurrentMessage *wsnt__GetCurrentMessage, _wsnt__GetCurrentMessageResponse *wsnt__GetCurrentMessageResponse) 
	{return SOAP_OK;}

	/// Web service operation 'GetMessages' (returns error code or SOAP_OK)
	virtual	int GetMessages(_wsnt__GetMessages *wsnt__GetMessages, _wsnt__GetMessagesResponse *wsnt__GetMessagesResponse) 
	{return SOAP_OK;}

	/// Web service operation 'DestroyPullPoint' (returns error code or SOAP_OK)
	virtual	int DestroyPullPoint(_wsnt__DestroyPullPoint *wsnt__DestroyPullPoint, _wsnt__DestroyPullPointResponse *wsnt__DestroyPullPointResponse) 
	{return SOAP_OK;}

	/// Web service one-way operation 'Notify' (return error code, SOAP_OK (no response), or send_Notify_empty_response())
	virtual	int Notify_(_wsnt__Notify *wsnt__Notify) 
	{return SOAP_OK;}

	/// Web service operation 'CreatePullPoint' (returns error code or SOAP_OK)
	virtual	int CreatePullPoint(_wsnt__CreatePullPoint *wsnt__CreatePullPoint, _wsnt__CreatePullPointResponse *wsnt__CreatePullPointResponse) 
	{return SOAP_OK;}

	/// Web service operation 'Renew' (returns error code or SOAP_OK)
	virtual	int Renew_(_wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse *wsnt__RenewResponse) 
	{return SOAP_OK;}

	/// Web service operation 'Unsubscribe' (returns error code or SOAP_OK)
	virtual	int Unsubscribe_(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse) 
	{return SOAP_OK;}

	/// Web service operation 'PauseSubscription' (returns error code or SOAP_OK)
	virtual	int PauseSubscription(_wsnt__PauseSubscription *wsnt__PauseSubscription, _wsnt__PauseSubscriptionResponse *wsnt__PauseSubscriptionResponse) 
	{return SOAP_OK;}

	/// Web service operation 'ResumeSubscription' (returns error code or SOAP_OK)
	virtual	int ResumeSubscription(_wsnt__ResumeSubscription *wsnt__ResumeSubscription, _wsnt__ResumeSubscriptionResponse *wsnt__ResumeSubscriptionResponse) 
	{return SOAP_OK;}
	
private:
	int m_EventCnt;
	OnvifServerEventProducer &m_Producer;
	string m_strEventEndpoint;
};


#endif