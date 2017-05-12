
#ifndef __ONVIF_C_HANDLER_H__
#define __ONVIF_C_HANDLER_H__
#include <string.h>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include "wsclient.hpp"
#include "onvifcli.pb.h"
#include "onvifclidevice.pb.h"
#include "onvifclient.hpp"
#include <google/protobuf/util/json_util.h>

#include "CivetServer.h"


class OnvifCHandler
{
public:
	OnvifCHandler()
	:m_pDevice(NULL), m_pMedia(NULL),m_pPtz(NULL)
	{

	}
	~OnvifCHandler()
	{
		OnvifClearAll();
	}

	bool OnvifClearAll()
	{
		if (m_pMedia)
		{
			delete m_pMedia;
			m_pMedia = NULL;
		}

		if (m_pPtz)
		{
			delete m_pPtz;
			m_pPtz = NULL;
		}
		
		if (m_pDevice)
		{
			delete m_pDevice;
			m_pDevice = NULL;
		}


		return true;	
	}
	bool OnvifClearMedia()
	{
		if (m_pMedia)
		{
			delete m_pMedia;
			m_pMedia = NULL;
		}

		return true;
	}

	bool OnvifClearEvent()
	{
		return true;	
	}
	
	bool ProcessLoginReq(OnvifCli::OnvifCliCmd &req, CivetServer *server,
	                        struct mg_connection *conn)
	{
		long long p = (long long)conn;
		OnvifCli::OnvifCliCmd cmdResp;
		if (!req.has_loginreq())
		{
			return false;
		}
		
		OnvifClearAll();
		
		const OnvifCliDeviceLoginReq& pLogin =  req.loginreq();
		/* Process the login */
		std::string url = pLogin.strurl();
		/* Below is where to receive the event */
		std::string eventNotify = "http://192.168.22.123:9091/subscription-2";
		std::string user = pLogin.strusername();
		std::string pass =  pLogin.strpasswd();
		m_pDevice = new OnvifClientDevice(url, user, pass);
		_tds__GetCapabilitiesResponse capabilitiesResponse;
		
		cmdResp.set_type(OnvifCli::CLI_CMD_DEV_LOGIN_RESP);
		OnvifCliDeviceLoginResp * resp = new OnvifCliDeviceLoginResp;
		resp->set_strhandle(std::to_string(p));
		
		if (m_pDevice->GetCapabilities() == SOAP_OK)
		{
			resp->set_blogined(true);
		}else
		{
			resp->set_blogined(false);
		}
		m_pPtz = new OnvifClientPTZ(*(m_pDevice));
		string strUrl;
		resp->set_bhasptz(m_pDevice->GetPTZUrl(strUrl));
		resp->set_bhasevent(m_pDevice->GetEventUrl(strUrl));
		
		cmdResp.set_allocated_loginresp(resp);
		/* Init the server */

		SendRespMsg(cmdResp, server, conn);
	
		return true;
	}

	bool ProcessGetProfilesReq(OnvifCli::OnvifCliCmd &req, CivetServer *server,
	                        struct mg_connection *conn)
	{
		long long p = (long long)conn;
		OnvifCli::OnvifCliCmd cmdResp;
		if (!req.has_getprofilereq() || m_pDevice == NULL)
		{
			return false;
		}
		
		OnvifClearMedia();
		
		const OnvifCliMediaGetProfilesReq& pGetProfiles =  req.getprofilereq();

		m_pMedia = new OnvifClientMedia(*m_pDevice);

	
		cmdResp.set_type(OnvifCli::CLI_CMD_MEDIA_GET_PROFILES_RESP);
		OnvifCliMediaGetProfilesResp * resp = new OnvifCliMediaGetProfilesResp;

		_trt__GetProfilesResponse profiles;

		if (m_pMedia->GetProfiles(profiles) == SOAP_OK)
		{
			if (profiles.Profiles.size() > 0)
			{
				resp->set_bgotprofiles(true);
			}
			/* set the profiles */
			for (vector<tt__Profile * >::iterator iter =
					profiles.Profiles.begin(); 
					iter < profiles.Profiles.end(); ++iter)
			{

				tt__Profile * pProfile = *iter;
				if (pProfile && pProfile->VideoEncoderConfiguration)
				{
					tt__VideoEncoderConfiguration *pEnc = pProfile->VideoEncoderConfiguration;
					OnvifCliProfile *pro = resp->add_cprofiles();

					pro->set_strtoken(pProfile->token);
					std::string strCodecName;
					switch (pEnc->Encoding)
					{
						case tt__VideoEncoding__JPEG:
						{
							strCodecName = "MJPEG";
							break;
						}
						case tt__VideoEncoding__MPEG4:
						{
							strCodecName = "MPEG4";
							break;
						}
						case tt__VideoEncoding__H264:
						{
							strCodecName = "H264";
							break;
						}
						default:
						   	strCodecName = "UnSupport";
							break;
					};

					pro->set_strcodecname(strCodecName);
					if (pEnc->Resolution)
					{
						pro->set_nwidth(pEnc->Resolution->Width);
						pro->set_nheight(pEnc->Resolution->Height);
					}
					if (pEnc->RateControl)
					{
						pro->set_nbandwidth(pEnc->RateControl->BitrateLimit);
						pro->set_nfps(pEnc->RateControl->FrameRateLimit);
					}
				}
				
			}
			
		}else
		{
			resp->set_bgotprofiles(false);
		}
		
		cmdResp.set_allocated_getprofileresp(resp);
		SendRespMsg(cmdResp, server, conn);
	
		return true;
	}
	bool ProcessPTZReq(OnvifCli::OnvifCliCmd &req, CivetServer *server,
                        struct mg_connection *conn)
	{
		printf("%s %d PTZ request\n", __FILE__, __LINE__);
		OnvifCli::OnvifCliCmd cmdResp;
		if (!req.has_ptzreq() || m_pDevice == NULL || m_pPtz == NULL)
		{
			return false;
		}
		
		const OnvifCliPTZReq& pReq =  req.ptzreq();
		float speed = pReq.speed();
		std::string strToken = pReq.token();

		switch (pReq.cmd())
		{
			case CLI_PTZ_UP:	
			{
				_tptz__ContinuousMoveResponse resp;
				tt__PTZSpeed oSpeed;
				tt__Vector2D pan;
				pan.x = 0;
				pan.y = speed;
				oSpeed.PanTilt = &pan;
				oSpeed.Zoom = NULL;
				LONG64 Timeout = 100;
			
				m_pPtz->ContinuousMove(resp, oSpeed, Timeout, strToken);
				break;
			}
			case CLI_PTZ_DOWN:	
			{
				_tptz__ContinuousMoveResponse resp;
				tt__PTZSpeed oSpeed;
				tt__Vector2D pan;
				pan.x = 0;
				pan.y = 0 - speed;
				oSpeed.PanTilt = &pan;
				oSpeed.Zoom = NULL;
				LONG64 Timeout = 100;
			
				m_pPtz->ContinuousMove(resp, oSpeed, Timeout, strToken);
				break;
			}
			case CLI_PTZ_LEFT:	
			{
				_tptz__ContinuousMoveResponse resp;
				tt__PTZSpeed oSpeed;
				tt__Vector2D pan;
				pan.x = 0 - speed;
				pan.y = 0;
				oSpeed.PanTilt = &pan;
				oSpeed.Zoom = NULL;
				LONG64 Timeout = 100;
			
				m_pPtz->ContinuousMove(resp, oSpeed, Timeout, strToken);
				break;
			}
			case CLI_PTZ_RIGHT:	
			{
				_tptz__ContinuousMoveResponse resp;
				tt__PTZSpeed oSpeed;
				tt__Vector2D pan;
				pan.x = speed;
				pan.y = 0;
				oSpeed.PanTilt = &pan;
				oSpeed.Zoom = NULL;
				LONG64 Timeout = 100;
			
				m_pPtz->ContinuousMove(resp, oSpeed, Timeout, strToken);
				break;
			}
			case CLI_PTZ_ZOOM_IN:	
			{
				_tptz__ContinuousMoveResponse resp;
				tt__PTZSpeed oSpeed;
				tt__Vector2D pan;
				tt__Vector1D zoom;
				zoom.x = speed;
				pan.x = 0;
				pan.y = 0;
				oSpeed.PanTilt = &pan;
				oSpeed.Zoom = &zoom;
				LONG64 Timeout = 100;
			
				m_pPtz->ContinuousMove(resp, oSpeed, Timeout, strToken);
				break;
			}
			case CLI_PTZ_ZOOM_OUT:	
			{
				_tptz__ContinuousMoveResponse resp;
				tt__PTZSpeed oSpeed;
				tt__Vector2D pan;
				tt__Vector1D zoom;
				zoom.x = 0 - speed;
				pan.x = 0;
				pan.y = 0;
				oSpeed.PanTilt = &pan;
				oSpeed.Zoom = &zoom;
				LONG64 Timeout = 100;
			
				m_pPtz->ContinuousMove(resp, oSpeed, Timeout, strToken);
				break;
			}
			case CLI_PTZ_STOP:	
			{
				_tptz__StopResponse resp;
				m_pPtz->Stop(resp, false, false, strToken);
				break;
			}
			default:
				break;
		}		
		return true;
	}
	bool ProcessMsg(std::string &strMsg, CivetServer *server,
	                        struct mg_connection *conn)
	{
		OnvifCli::OnvifCliCmd cmd;
		::google::protobuf::util::Status status = 
			::google::protobuf::util::JsonStringToMessage(strMsg, &cmd);
		if (!status.ok())
		{
			return false;
		}

		switch (cmd.type())
		{
			case OnvifCli::CLI_CMD_DEV_LOGIN_REQ:
			{
				return ProcessLoginReq(cmd, server, conn);
				break;
			}
			case OnvifCli::CLI_CMD_MEDIA_GET_PROFILES_REQ:
			{
				return ProcessGetProfilesReq(cmd, server, conn);
				break;
			}
			case OnvifCli::CLI_CMD_PTZ_REQ:
			{
				return ProcessPTZReq(cmd, server, conn);
				break;
			}

			default:
			   	return false;
		};		
		

	}

	bool SendRespMsg(OnvifCli::OnvifCliCmd &resp, CivetServer *server,
	                        struct mg_connection *conn)
	{
		std::string strMsg;
		::google::protobuf::util::Status status = 
			::google::protobuf::util::MessageToJsonString(resp, &strMsg);
		if (!status.ok())
		{
			return false;
		}
		int ret = mg_websocket_write(conn, 
			WEBSOCKET_OPCODE_TEXT, strMsg.c_str(), strMsg.length());
		printf("%s %d websocket send %d\n", __FILE__, __LINE__, ret);
		if (ret == strMsg.length())
		{
			return true;
		}
		return false;
	}
							  
private:
	OnvifClientDevice *m_pDevice;
	OnvifClientMedia *m_pMedia;
	OnvifClientPTZ *m_pPtz;
};


typedef std::map<void *, OnvifCHandler *> OnvifCHandlerMap;


#endif /* __ONVIF_C_HANDLER_H__ */
