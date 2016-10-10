#ifndef __VE_WEB_SERVER_H__
#define __VE_WEB_SERVER_H__

#include "CivetServer.h"
#include "vapi/vapiimage.hpp"
#include "vapi/vapisystem.hpp"
#include "vapi/vwsapi.hpp"

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#define DOCUMENT_ROOT "."
#define PORT "9081"


class VEWebServer 
{
public:
	VEWebServer(std::vector<std::string> cpp_options, Factory &pFactory)
		:pServer(new CivetServer(cpp_options)), server(*pServer), m_pFactory(pFactory), 
		h_GetCamList(pFactory), h_GetImage(pFactory), h_vwsapi(pFactory)
	{
		server.addHandler("/vapi/GetCamList", h_GetCamList);
		server.addHandler("/vapi/GetImage", h_GetImage);
		server.addWebSocketHandler("/vwsapi", h_vwsapi);
		

	}
	~VEWebServer(){}
	
private:
	WebAPIGetCamListHandler h_GetCamList;
	WebAPIGetImageHandler h_GetImage;
	VwsAPI h_vwsapi;
	CivetServer *pServer;
	CivetServer &server;
	Factory &m_pFactory;
};

#endif /* __VE_WEB_SERVER_H__ */
