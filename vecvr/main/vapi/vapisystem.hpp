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
#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "rapidmedia/rapidmedia.hpp"
#include "vplay.hpp"
#include <google/protobuf/util/json_util.h>


#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
using namespace cppkit;

#include "onvifclidis.hpp"

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
		CameraOnlineMap pCameraOnlineMap;
		CameraRecMap pCameraRecMap;

		m_pFactory.GetCameraOnlineMap(pCameraOnlineMap);
		m_pFactory.GetCameraRecMap(pCameraRecMap);

		VidCameraList pCameraList;
		m_pFactory.GetCameraList(pCameraList);

		for (s32 i = 0; i < pCameraList.cvidcamera_size(); i ++)
		{
			VidCamera &cam = (VidCamera &)(pCameraList.cvidcamera(i));
			cam.set_strpasswd("******");
		}
		std::string strMsg;
		::google::protobuf::util::Status status = 
			::google::protobuf::util::MessageToJsonString(pCameraList, &strMsg);

		s32 nJsonLen = strMsg.length();
		if (nJsonLen <= 0)
		{

			return true;
		}
		
		std::string s = "";
		mg_printf(conn,
		          "HTTP/1.1 200 OK\r\nContent-Type: "
		          "application/json\r\n"
				  "Content-Length: %d\r\n\r\n", nJsonLen);
		mg_printf(conn, strMsg.c_str());
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


class WebAPIGetStreamUrlHandler : public CivetHandler
{
public:
	WebAPIGetStreamUrlHandler(Factory &pFactory)
		:m_pFactory(pFactory)
	{

	}
private:
	bool
	handleAll(const char *method,
	          CivetServer *server,
	          struct mg_connection *conn)
	{
		u8 * pBuf=NULL;
		int nLen= 0;
		std::string strCamera = "";
		if (CivetServer::getParam(conn, "Camera", strCamera)  == true)
		{
			VidStreamUrlList UrlList;
#if 0
			size_t locallen;
			struct sockaddr_in local;
  			locallen = sizeof(local);  
			memset(&local, 0, locallen);
			
			#ifdef _WIN32
			#include <winsock.h>
			#endif
			#ifdef _WIN32
			  getsockname(conn->client.sock, (sockaddr *)&local, (int *)&(locallen));
			#else
			  getsockname(conn->socket, (sockaddr *)&local, (socklen_t *)&(locallen));
			#endif
#endif
			std::vector<std::string> ipList = OnvifDisClient::GetInterfaces();
			if (ipList.size() <= 0)
			{
				printf("No network adaptor found !!!\n");
				return false;
			}
			std::vector<std::string>::iterator it;
			for (it = ipList.begin(); it != ipList.end(); ++it)
			{
				printf("Network %s\n", (*it).c_str());

				//RTSP Url
				{
					VidStreamUrl *pUrl = UrlList.add_curl();
					pUrl->set_strprotocol("RTSP");
					string rtspUrl = "rtsp://" + (*it) + ":10554/live/" + strCamera;
					pUrl->set_strurl(rtspUrl);
				}
				//RTMP Url
				{
					VidStreamUrl *pUrl = UrlList.add_curl();
					pUrl->set_strprotocol("RTMP");
					string rtmpUrl = "rtmp://" + (*it) + ":11935/live/" + strCamera;
					pUrl->set_strurl(rtmpUrl);
				}

				
				//HLS Url
				{
					VidStreamUrl *pUrl = UrlList.add_curl();
					pUrl->set_strprotocol("HLS");
					string hlsUrl = "http://" + (*it) + ":10080/live/" + strCamera + "/hls.m3u8";
					pUrl->set_strurl(hlsUrl);
				}
			}

			std::string strMsg;
			::google::protobuf::util::Status status = 
				::google::protobuf::util::MessageToJsonString(UrlList, &strMsg);

			s32 nJsonLen = strMsg.length();
			if (nJsonLen <= 0)
			{

				return true;
			}

			std::string s = "";
			mg_printf(conn,
			          "HTTP/1.1 200 OK\r\nContent-Type: "
			          "application/json\r\n"
					  "Content-Length: %d\r\n\r\n", nJsonLen);
			mg_printf(conn, strMsg.c_str());
			
			
			
		}else
		{
			mg_printf(conn,
			          "HTTP/1.1 200 OK\r\nContent-Type: "
			          "text/plain\r\nConnection: close\r\n\r\n");
			mg_printf(conn, "Can't Get Stream Url!\n");			
		}

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
