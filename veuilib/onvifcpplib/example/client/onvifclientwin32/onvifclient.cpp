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
// onvifclient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "onvifclient.hpp"


int _tmain(int argc, _TCHAR* argv[])
{
	int ret;
	/* 192.168.1.1 is the NVT, 192.168.1.234 is the NVC */
	string url = "http://192.168.22.100/onvif/device_service";
	/* Below is where to receive the event */
	string eventNotify = "http://192.168.22.123:9091/subscription-2";
	string user = "admin";
	string pass =  "admin";
	OnvifClientDevice onvifDevice(url, user, pass);
	_tds__GetCapabilitiesResponse capabilitiesResponse;

	onvifDevice.GetCapabilities();

	OnvifClientMedia media(onvifDevice);
	_trt__GetProfilesResponse profiles;
	media.GetProfiles(profiles);
	
	/* Recording */
	OnvifClientRecording onvifRecording(onvifDevice);
	_trc__GetRecordingsResponse recordings;
	onvifRecording.GetRecordings(recordings);
	for (vector<tt__GetRecordingsResponseItem * >::iterator iter = 
			recordings.RecordingItem.begin(); 
			iter < recordings.RecordingItem.end(); ++iter)
	{

		tt__GetRecordingsResponseItem * pRecordings = *iter;
		printf("pRecordings %s\n", pRecordings->RecordingToken.c_str());
		tt__RecordingConfiguration *pRecordingConf = pRecordings->Configuration;
		if (pRecordingConf->Source)
		{
			printf("Recording SourceId %s\n", pRecordingConf->Source->SourceId.c_str());
			printf("Recording Name %s\n", pRecordingConf->Source->Name.c_str());
			printf("Recording Location %s\n", pRecordingConf->Source->Location.c_str());
			printf("Recording Description %s\n", pRecordingConf->Source->Description.c_str());
			printf("Recording Address %s\n", pRecordingConf->Source->Address.c_str());
		}

		string replayUrl;
		OnvifClientReplay onvifRelay(onvifDevice);
		onvifRelay.GetReplayUri(pRecordings->RecordingToken, replayUrl);
		printf("Relay Url %s\n", replayUrl.c_str());
	}
	
	/* Onvif Receiver */
	OnvifClientReceiver onvifReceiver(onvifDevice);
	_trv__GetReceiversResponse receivers;
	onvifReceiver.GetReceivers(receivers);
	for (vector<tt__Receiver * >::iterator iter = 
			receivers.Receivers.begin(); 
			iter < receivers.Receivers.end(); ++iter)
	{
		tt__Receiver * pReceivers = *iter;
		printf("pReceivers %s\n", pReceivers->Token.c_str());
	}

	OnvifClientEvent onvifEvent(onvifDevice);

	onvifEvent.Subscribe(eventNotify);
	
	OnvifClientEventNotify notify;
	
    	if (notify.bind(NULL, 9091, 100) == SOAP_INVALID_SOCKET )
	{
	        printf("OnvifClientEventNotify::soap_bind Binding on %d port failed", 9090);
	        return 0;
    	}
	int timeStart = time(NULL);
	int currentTime = 0;

	//Loop to recevie the event
	while(1)
	{
		currentTime = time(NULL);
		if (currentTime - timeStart > 30)
		{
			onvifEvent.Renew();
		}
		//printf("soap_accept accepting\n");
		if( notify.accept() == SOAP_INVALID_SOCKET) {
			printf("soap_accept accepting timeout\n");
			continue;
		}
		ret = notify.serve();
		if (ret != SOAP_OK) {
			printf("soap_begin_serve serve %d failed\n", ret);
            		continue;
        }

        	ret = notify.dispatch();
        	continue;
	}
	return 0;
}

