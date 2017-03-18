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
#ifndef __VE_SYSTEM_API_H__
#define __VE_SYSTEM_API_H__

#include "CivetServer.h"
#include "server/factory.hpp"
#include "oapi/oapis.hpp"
#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "rapidmedia/rapidmedia.hpp"
#include "vplay.hpp"


#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
using namespace cppkit;

#define VAPI_JPEG_SIZE 1024*1024 * 1


/* /vapi/GetCamList */
class WebAPIGetCamListHandler : public CivetHandler
{
public:
	WebAPIGetCamListHandler(Factory &pFactory)
		:m_pFactory(pFactory)
	{

	}
private:
	bool
	handleAll(const char *method,
	          CivetServer *server,
	          struct mg_connection *conn)
	{
		oapi::OAPICameraListRsp dataList;
		dataList.Num = 0;

		CameraOnlineMap pCameraOnlineMap;
		CameraRecMap pCameraRecMap;

		m_pFactory.GetCameraOnlineMap(pCameraOnlineMap);
		m_pFactory.GetCameraRecMap(pCameraRecMap);

		VidCameraList pCameraList;
		m_pFactory.GetCameraList(pCameraList);

		for (s32 i = 0; i < pCameraList.cvidcamera_size(); i ++)
		{
			const VidCamera &cam = pCameraList.cvidcamera(i);
			oapi::OAPICamera data;
			OAPIConverter::Converter((VidCamera &)cam, data);
			data.strPasswd = "******";
			data.bOnline = pCameraOnlineMap[cam.strid()];
			data.bRec = pCameraRecMap[cam.strid()];
			dataList.list.push_back(data);
			dataList.Num ++;
		}

		//autojsoncxx::to_json_string

		std::string strJson = autojsoncxx::to_pretty_json_string(dataList);
		s32 nJsonLen = strJson.length();
		if (nJsonLen <= 0)
		{
			//TODO check if it false
			return FALSE;
		}
		
		std::string s = "";
		mg_printf(conn,
		          "HTTP/1.1 200 OK\r\nContent-Type: "
		          "application/json\r\n"
				  "Content-Length: %d\r\n\r\n", nJsonLen);
		mg_printf(conn, strJson.c_str());
		return true;
	}

  public:
	bool
	handleGet(CivetServer *server, struct mg_connection *conn)
	{
		return handleAll("GET", server, conn);
	}
	bool
	handlePost(CivetServer *server, struct mg_connection *conn)
	{
		return handleAll("POST", server, conn);
	}
private:
	Factory &m_pFactory;
};


#endif /* __VE_SYSTEM_API_H__ */
