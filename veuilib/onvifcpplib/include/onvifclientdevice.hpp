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
#ifndef __ONVIF_CLIENT_DEVICE__
#define __ONVIF_CLIENT_DEVICE__

#include <string>
#include <map>
#include <iostream>
#include <list>

#include "soapStub.h"
#include "soapDeviceBindingProxy.h"
#include "wsseapi.h"

#define ONIVF_INET_ADDRSTRLEN  256

using namespace std;

class OnvifClientDevice
{
public:
    OnvifClientDevice(string &strUrl, string &strUser, string &strPass);
    ~OnvifClientDevice();
public:
    int GetCapabilities(_tds__GetCapabilitiesResponse &cap);
	int GetCapabilities();
public:
	bool GetMediaUrl(string &strUrl);
	bool GetPTZUrl(string &strUrl);
	bool GetImagingUrl(string &strUrl);
	bool GetReceiverUrl(string &strUrl);
	bool GetRecordingUrl(string &strUrl);
	bool GetSearchUrl(string &strUrl);
	bool GetReplayUrl(string &strUrl);
	bool GetEventUrl(string &strUrl);
public:
	bool GetLocalAddr(string &strLocal)
	{
		strLocal = m_strLoalAddr;
		return true;
	}
	
public:
	bool GetUserPasswd(string &strUser, string &strPass)
	{
		strUser = m_strUser;
		strPass = m_strPass;
		return true;
	}
	bool SetUrlUserPasswd(string &strUrl, string &strUser, string &strPass)
	{
		m_strUrl = strUrl;
		m_strUser = strUser;
		m_strPass = strPass;
		return TRUE;
	}
	
private:
	string m_strUrl;
	string m_strUser;
	string m_strPass;
	bool m_hasGetCap;
	
	/* If the Device support blow service */
	bool m_hasMedia;
	bool m_hasPTZ;
	bool m_hasImaging;
	bool m_hasReceiver;
	bool m_hasRecording;
	bool m_hasSearch;
	bool m_hasReplay;
	bool m_hasEvent;
	
	/* The Url of blow service */
	string m_strMedia;
	string m_strPTZ;
	string m_strImaging;
	string m_strReceiver;
	string m_strRecording;
	string m_strSearch;
	string m_strReplay;
	string m_strEvent;
	string m_strLoalAddr;
	DeviceBindingProxy deviceBindProxy;
};

inline OnvifClientDevice::OnvifClientDevice(string &strUrl, string &strUser, string &strPass)
:m_strUrl(strUrl), m_strUser(strUser), 
 m_strPass(strPass), m_hasGetCap(false), deviceBindProxy(SOAP_C_UTFSTRING)
{
	m_hasMedia = false;
	m_hasPTZ = false;
	m_hasImaging = false;
	m_hasReceiver = false;
	m_hasRecording = false;
	m_hasSearch = false;
	m_hasReplay = false;
	m_hasEvent = false;
}

inline OnvifClientDevice::~OnvifClientDevice()
{

}

inline bool OnvifClientDevice::GetMediaUrl(string &strUrl)
{
	if (m_hasGetCap == false || m_hasMedia == false)
	{
		return false;
	}
	
	strUrl = m_strMedia;
	
	return true;
}
inline bool OnvifClientDevice::GetPTZUrl(string &strUrl)
{
	if (m_hasGetCap == false || m_hasPTZ  == false)
	{
		return false;
	}
	
	strUrl = m_strPTZ;
	
	return true;
}
inline bool OnvifClientDevice::GetImagingUrl(string &strUrl)
{
	if (m_hasGetCap == false || m_hasImaging  == false)
	{
		return false;
	}
	
	strUrl = m_strImaging;
	
	return true;
}
inline bool OnvifClientDevice::GetReceiverUrl(string &strUrl)
{
	if (m_hasGetCap == false || m_hasReceiver  == false)
	{
		return false;
	}
	
	strUrl = m_strReceiver;
	
	return true;
}
inline bool OnvifClientDevice::GetRecordingUrl(string &strUrl)
{
	if (m_hasGetCap == false || m_hasRecording == false)
	{
		return false;
	}
	
	strUrl = m_strRecording;
	
	return true;
}
inline bool OnvifClientDevice::GetSearchUrl(string &strUrl)
{
	if (m_hasGetCap == false || m_hasSearch  == false)
	{
		return false;
	}
	
	strUrl = m_strSearch;
	
	return true;
}
inline bool OnvifClientDevice::GetReplayUrl(string &strUrl)
{
	if (m_hasGetCap == false || m_hasReplay  == false)
	{
		return false;
	}
	
	strUrl = m_strReplay;
	
	return true;
}

inline bool OnvifClientDevice::GetEventUrl(string &strUrl)
{
	if (m_hasGetCap == false || m_hasEvent == false)
	{
		return false;
	}
	
	strUrl = m_strEvent;
	
	return true;
}

inline int OnvifClientDevice::GetCapabilities()
{
	_tds__GetCapabilitiesResponse cap;
	
	return GetCapabilities(cap);
}

inline int OnvifClientDevice::GetCapabilities(_tds__GetCapabilitiesResponse &cap)
{
	deviceBindProxy.soap_endpoint =  m_strUrl.c_str();
	char str[ONIVF_INET_ADDRSTRLEN];
	
	soap_wsse_add_Security(&deviceBindProxy);
	soap_wsse_add_UsernameTokenDigest(&deviceBindProxy, "Id", 
		m_strUser.c_str() , m_strPass.c_str());
		
	_tds__GetCapabilities capabilities;
	capabilities.Category.push_back(tt__CapabilityCategory__All);
	if (deviceBindProxy.GetCapabilities(&capabilities,&cap)!= SOAP_OK||
		cap.Capabilities == NULL)
	{
		return SOAP_ERR;
	}
	m_strLoalAddr = inet_ntoa(deviceBindProxy.local.sin_addr);

	/* Media */
	if (cap.Capabilities->Media != NULL)
	{
		m_hasMedia  = TRUE;
		m_strMedia = cap.Capabilities->Media->XAddr;
	}
	/* PTZ */
	if (cap.Capabilities->PTZ != NULL)
	{
		m_hasPTZ   = TRUE;
		m_strPTZ = cap.Capabilities->PTZ->XAddr;
	}
	
	/* Event */
	if (cap.Capabilities->Events != NULL)
	{
		m_hasEvent   = TRUE;
		m_strEvent = cap.Capabilities->Events->XAddr;
	}
	
	/* Imaging */
	if (cap.Capabilities->Imaging != NULL)
	{
		m_hasImaging   = TRUE;
		m_strImaging = cap.Capabilities->Imaging->XAddr;
	}
	
	/* Extension */
	if (cap.Capabilities->Extension != NULL)
	{
		/* Receiver */
		if (cap.Capabilities->Extension->Receiver != NULL)
		{
			m_hasReceiver     = TRUE;
			m_strReceiver  = cap.Capabilities->Extension->Receiver->XAddr;
		}
		/* Recording */
		if (cap.Capabilities->Extension->Recording != NULL)
		{
			m_hasRecording   = TRUE;
			m_strRecording = cap.Capabilities->Extension->Recording->XAddr;
		}
		/* Search */
		if (cap.Capabilities->Extension->Search != NULL)
		{
			m_hasSearch   = TRUE;
			m_strSearch = cap.Capabilities->Extension->Search->XAddr;
		}
		/* Replay */
		if (cap.Capabilities->Extension->Replay != NULL)
		{
			m_hasReplay    = TRUE;
			m_strReplay  = cap.Capabilities->Extension->Replay->XAddr;
		}
	}

	m_hasGetCap = TRUE;
	return SOAP_OK;
}


#endif