

#include "stdafx.h"
#include "onvifagentlib.hpp"


int _tmain(int argc, _TCHAR* argv[])
{
	int ret;
	/* 192.168.1.1 is the NVT, 192.168.1.234 is the NVC */
	std::string url = "http://192.168.22.100/onvif/device_service";
	/* Below is where to receive the event */
	std::string eventNotify = "http://192.168.22.123:9091/subscription-2";
	std::string user = "admin";
	std::string pass =  "admin";

	OnvifAgentC agent(user, pass, url);
	OnvifAgentCProfileMap pProfiles;

	//while(1)
	{
		agent.Login();
		pProfiles.clear();
		agent.GetProfiles(pProfiles);
	}
	
	while(1)
	{
		agent.PTZAction(pProfiles.begin()->first,AGENT_PTZ_ZOOM_IN, 0.5);
		std::chrono::milliseconds dura( 1000 );
		std::this_thread::sleep_for( dura );

	}
	return 0;
}

