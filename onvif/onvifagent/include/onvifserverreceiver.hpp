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
#ifndef __ONVIF_SERVER_RECEIVER__
#define __ONVIF_SERVER_RECEIVER__

#include <string>
#include <map>
#include <iostream>
#include <list>

#include "soapStub.h"
#include "soapReceiverBindingService.h"
#include "wsseapi.h"


using namespace std;

class OnvifServerReceiver : public ReceiverBindingService
{
public:
    OnvifServerReceiver();
    ~OnvifServerReceiver();
public:
	/// Create a copy
	virtual	ReceiverBindingService *copy()
	{
		return NULL;
	}
	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities(_trv__GetServiceCapabilities *trv__GetServiceCapabilities, _trv__GetServiceCapabilitiesResponse *trv__GetServiceCapabilitiesResponse) 
	{return SOAP_OK;};

	/// Web service operation 'GetReceivers' (returns error code or SOAP_OK)
	virtual	int GetReceivers(_trv__GetReceivers *trv__GetReceivers, _trv__GetReceiversResponse *trv__GetReceiversResponse) 
	{
		{
			tt__Receiver *receiver = soap_new_tt__Receiver(m_pSoap, 1);
			receiver->Token = "R001";
			tt__ReceiverConfiguration *Configuration = 
								soap_new_tt__ReceiverConfiguration(m_pSoap, 1);
			Configuration->Mode = tt__ReceiverMode__AlwaysConnect;
			Configuration->MediaUri = "rtsp://192.168.0.1/1";
			receiver->Configuration = Configuration;
			trv__GetReceiversResponse->Receivers.push_back(receiver);
		}
		{
			tt__Receiver *receiver = soap_new_tt__Receiver(m_pSoap, 1);
			receiver->Token = "R002";
			tt__ReceiverConfiguration *Configuration = 
								soap_new_tt__ReceiverConfiguration(m_pSoap, 1);
			Configuration->Mode = tt__ReceiverMode__AlwaysConnect;
			Configuration->MediaUri = "rtsp://192.168.0.1/12";
			receiver->Configuration = Configuration;
			trv__GetReceiversResponse->Receivers.push_back(receiver);
		}
		{
			tt__Receiver *receiver = soap_new_tt__Receiver(m_pSoap, 1);
			receiver->Token = "R003";
			tt__ReceiverConfiguration *Configuration = 
								soap_new_tt__ReceiverConfiguration(m_pSoap, 1);
			Configuration->Mode = tt__ReceiverMode__AlwaysConnect;
			Configuration->MediaUri = "rtsp://192.168.0.1/13";
			receiver->Configuration = Configuration;
			trv__GetReceiversResponse->Receivers.push_back(receiver);
		}
		return SOAP_OK;
	}

	/// Web service operation 'GetReceiver' (returns error code or SOAP_OK)
	virtual	int GetReceiver(_trv__GetReceiver *trv__GetReceiver, _trv__GetReceiverResponse *trv__GetReceiverResponse) {return SOAP_OK;};

	/// Web service operation 'CreateReceiver' (returns error code or SOAP_OK)
	virtual	int CreateReceiver(_trv__CreateReceiver *trv__CreateReceiver, _trv__CreateReceiverResponse *trv__CreateReceiverResponse) {return SOAP_OK;};

	/// Web service operation 'DeleteReceiver' (returns error code or SOAP_OK)
	virtual	int DeleteReceiver(_trv__DeleteReceiver *trv__DeleteReceiver, _trv__DeleteReceiverResponse *trv__DeleteReceiverResponse) {return SOAP_OK;};

	/// Web service operation 'ConfigureReceiver' (returns error code or SOAP_OK)
	virtual	int ConfigureReceiver(_trv__ConfigureReceiver *trv__ConfigureReceiver, _trv__ConfigureReceiverResponse *trv__ConfigureReceiverResponse) {return SOAP_OK;};

	/// Web service operation 'SetReceiverMode' (returns error code or SOAP_OK)
	virtual	int SetReceiverMode(_trv__SetReceiverMode *trv__SetReceiverMode, _trv__SetReceiverModeResponse *trv__SetReceiverModeResponse) {return SOAP_OK;};

	/// Web service operation 'GetReceiverState' (returns error code or SOAP_OK)
	virtual	int GetReceiverState(_trv__GetReceiverState *trv__GetReceiverState, _trv__GetReceiverStateResponse *trv__GetReceiverStateResponse) {return SOAP_OK;};

	
private:
	struct soap * m_pSoap;
};

inline OnvifServerReceiver::OnvifServerReceiver()
: ReceiverBindingService(), m_pSoap(this)
{

}

inline OnvifServerReceiver::~OnvifServerReceiver()
{

}

#endif 