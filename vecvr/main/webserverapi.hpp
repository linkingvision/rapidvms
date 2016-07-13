#ifndef __VE_WEB_SERVER_API_H__
#define __VE_WEB_SERVER_API_H__

#include "CivetServer.h"
#include "server/factory.hpp"
#include "oapi/oapis.hpp"

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

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


#endif /* __VE_WEB_SERVER_H__ */
