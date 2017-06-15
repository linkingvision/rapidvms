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
#ifndef __VE_WEB_SERVER_H__
#define __VE_WEB_SERVER_H__

#include "CivetServer.h"
#include "vapi/vapiimage.hpp"
#include "vapi/vapisystem.hpp"
#include "vapi/vwsapi.hpp"
#include "link/wslink.hpp"

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
		h_GetCamList(pFactory), h_GetImage(pFactory), h_vwsapi(pFactory), h_wslink(pFactory)
	{
		server.addHandler("/vapi/GetCamList", h_GetCamList);
		server.addHandler("/vapi/GetImage", h_GetImage);
		server.addWebSocketHandler(LINK_PROTO_WS_PATH, h_wslink);
		server.addAuthHandler(LINK_PROTO_WS_PATH, h_wslinkAuth);
		server.addWebSocketHandler("/vwsapi", h_vwsapi);
		
	}
	~VEWebServer(){}
	
private:
	WebAPIGetCamListHandler h_GetCamList;
	WebAPIGetImageHandler h_GetImage;
	VwsAPI h_vwsapi;
	WSLink h_wslink;
	WSLinkAuth h_wslinkAuth;
	CivetServer *pServer;
	CivetServer &server;
	Factory &m_pFactory;
};

#endif /* __VE_WEB_SERVER_H__ */
