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
#ifndef __ONVIF_CLIENT_REPLAY__
#define __ONVIF_CLIENT_REPLAY__

#include <string>
#include <map>
#include <iostream>
#include <list>

#include "onvifclientdevice.hpp"
#include "soapStub.h"
#include "soapReplayBindingProxy.h"
#include "wsseapi.h"


using namespace std;

class OnvifClientReplay
{
public:
    OnvifClientReplay(OnvifClientDevice &device);
    ~OnvifClientReplay();
public:
	/* Add function to here */
	int GetReplayUri(string recordingToken, string  &replayUrl);
				
private:
	OnvifClientDevice &m_Device;
	ReplayBindingProxy  replayProxy;

};

inline OnvifClientReplay::OnvifClientReplay(OnvifClientDevice &device)
: m_Device(device), replayProxy(SOAP_C_UTFSTRING)
{

}

inline OnvifClientReplay::~OnvifClientReplay()
{

}

inline int OnvifClientReplay::GetReplayUri(string recordingToken, string  &replayUrl)
{
	string strUrl;
	string strUser;
	string strPass;
	_trp__GetReplayUri req;
	_trp__GetReplayUriResponse resp;
	tt__StreamSetup StreamSetup;
	tt__Transport Transport;
	Transport.Protocol = tt__TransportProtocol__RTSP;
	StreamSetup.Stream = tt__StreamType__RTP_Unicast;
	StreamSetup.Transport = &Transport;
	
	
	if (m_Device.GetUserPasswd(strUser, strPass) == false 
		|| m_Device.GetReplayUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	
	replayProxy.soap_endpoint =  strUrl.c_str();
	
	soap_wsse_add_Security(&replayProxy);
	soap_wsse_add_UsernameTokenDigest(&replayProxy, "Id", 
		strUser.c_str() , strPass.c_str());
		
	req.RecordingToken = recordingToken;
	req.StreamSetup = &StreamSetup;
		
	int ret = replayProxy.GetReplayUri( &req, &resp);
	if (ret == SOAP_OK)
	{
		replayUrl = resp.Uri;
	}
	return ret;

}


#endif 