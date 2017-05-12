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
#ifndef __ONVIF_CLIENT_MEDIA__
#define __ONVIF_CLIENT_MEDIA__

#include <string>
#include <map>
#include <iostream>
#include <list>

#include "onvifclientdevice.hpp"
#include "soapStub.h"
#include "soapMediaBindingProxy.h"
#include "wsseapi.h"


using namespace std;

class OnvifClientMedia
{
public:
    OnvifClientMedia(OnvifClientDevice &device);
    ~OnvifClientMedia();
public:
    int GetProfiles(_trt__GetProfilesResponse &profiles);
    int GetStreamUrl(std::string strToken, _trt__GetStreamUriResponse &url);
				
private:
	OnvifClientDevice &m_Device;
	MediaBindingProxy  mediaProxy;

};


inline OnvifClientMedia::OnvifClientMedia(OnvifClientDevice &device)
: m_Device(device), mediaProxy(SOAP_C_UTFSTRING)
{

}

inline OnvifClientMedia::~OnvifClientMedia()
{

}

inline int OnvifClientMedia::GetStreamUrl(std::string strToken, _trt__GetStreamUriResponse &url)
{
	string strUrl;
	string strUser;
	string strPass;
	_trt__GetStreamUri req;
	
	if (m_Device.GetUserPasswd(strUser, strPass) == false 
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	
	mediaProxy.soap_endpoint =  strUrl.c_str();

	req.ProfileToken = strToken;
	tt__Transport Transport;
	Transport.Protocol = tt__TransportProtocol__TCP;
	tt__StreamSetup StreamSetup;
	//TODO add multicast support
	StreamSetup.Stream = tt__StreamType__RTP_Unicast;
	StreamSetup.Transport = &Transport; 

	req.StreamSetup = &StreamSetup;
	
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id",
		strUser.c_str() , strPass.c_str());
		
	return mediaProxy.GetStreamUri(&req, &url);
}

inline int OnvifClientMedia::GetProfiles(_trt__GetProfilesResponse &profiles)
{
	string strUrl;
	string strUser;
	string strPass;
	_trt__GetProfiles	profilesReq;
	
	if (m_Device.GetUserPasswd(strUser, strPass) == false 
		|| m_Device.GetMediaUrl(strUrl) == false)
	{
		return SOAP_ERR;
	}
	
	mediaProxy.soap_endpoint =  strUrl.c_str();
	
	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id",
		strUser.c_str() , strPass.c_str());
		
	return mediaProxy.GetProfiles( &profilesReq, &profiles) ;
}


#endif /* __ONVIF_CLIENT_MEDIA__ */