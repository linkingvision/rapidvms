#ifndef __ONVIF_AGENT_WEB_SERVER_H__
#define __ONVIF_AGENT_WEB_SERVER_H__

#include "CivetServer.h"
#include "ws/wsonvifc.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#define DOCUMENT_ROOT "."
#define PORT "10000"


class OnvifAgentWebServer 
{
public:
	OnvifAgentWebServer(std::vector<std::string> cpp_options)
		:pServer(new CivetServer(cpp_options)), server(*pServer)
	{
		server.addWebSocketHandler("/onvifagentcli", h_wsapi);
	}
	~OnvifAgentWebServer(){}
	
private:
	WSOnvifC h_wsapi;
	CivetServer *pServer;
	CivetServer &server;
};

#endif /* __ONVIF_AGENT_WEB_SERVER_H__ */
