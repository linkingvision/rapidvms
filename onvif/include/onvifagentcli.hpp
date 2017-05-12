
#ifndef __ONVIF_AGENT_CLIENT__
#define __ONVIF_AGENT_CLIENT__

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
#include "onvifclimedia.pb.h"
#include <google/protobuf/util/json_util.h>

typedef enum
{
	AGENT_PTZ_UP = 0,
	AGENT_PTZ_DOWN,
	AGENT_PTZ_LEFT,
	AGENT_PTZ_RIGHT,
	AGENT_PTZ_ZOOM_IN,
	AGENT_PTZ_ZOOM_OUT,
	AGENT_PTZ_STOP,
	AGENT_PTZ_LAST
} OnvifCAgentPtzAct;

class OnvifAgentCProfile
{
public:
	OnvifAgentCProfile()
	:nWidth(0), nHeight(0), nFps(0), nBandwidth(0), bGotUrl(false)
	{
		
	}
public:
	std::string strProfileToken;
	std::string strCodecName;
	std::string strNameDisplay;
	std::string strRtspUrl;
	int nWidth;
	int nHeight;
	int nFps;
	int nBandwidth;
	bool bGotUrl;
};

typedef std::map<std::string, OnvifAgentCProfile> OnvifAgentCProfileMap;

class  OnvifAgentC : public WebSocketClient
{
public:
	OnvifAgentC(std::string strUser, std::string strPasswd, std::string strUrl)
	:WebSocketClient("localhost", "10000", "/onvifagentcli"), m_bLogin(false), 
	m_strUser(strUser), m_strPasswd(strPasswd), m_strUrl(strUrl), m_bHasPTZ(false)
	{
		/* Connect to the remote server */
		Connect();
	}
	~OnvifAgentC()
	{}

public:
	virtual bool ProcessRecvMsg(char *data, size_t data_len)
	{
		/* lock guard */
		std::lock_guard<std::mutex> guard(m_lock);
		std::string strMsg(data, data_len);
		::google::protobuf::util::Status status = 
			::google::protobuf::util::JsonStringToMessage(strMsg, &m_lastCmd);
		if (!status.ok())
		{
			return false;
		}
		
		m_msgId ++;
		return true;
	}
public:
	bool Login()
	{
		if (Connected() == false)
		{
			if (Connect() == false)
			{
				return false;
			}
		}

		OnvifCli::OnvifCliCmd cmd;
		cmd.set_type(OnvifCli::CLI_CMD_DEV_LOGIN_REQ);
		OnvifCliDeviceLoginReq * req = new OnvifCliDeviceLoginReq;
		req->set_strusername(m_strUser);
		req->set_strpasswd(m_strPasswd);
		req->set_strurl(m_strUrl);
		cmd.set_allocated_loginreq(req);
		std::string strMsg;
		::google::protobuf::util::Status status = 
			::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
		if (!status.ok())
		{
			return false;
		}
		long long lastMsgId = 0;
		/* only lock here */
		{
			std::lock_guard<std::mutex> guard(m_lock);
			lastMsgId = m_msgId;
		}
		
		if (SendMsg(strMsg) == false)
		{
			return false;
		}

		OnvifCli::OnvifCliCmd respCmd;

		if (GetRespMsg(lastMsgId, respCmd) == false)
		{
			return false;
		}

		if (!respCmd.has_loginresp())
		{
			return false;
		}
		const OnvifCliDeviceLoginResp& pResp =  respCmd.loginresp();
		
		printf("%s %d websocket login handle %s id %lld login %d\n", __FILE__, __LINE__, 
					pResp.strhandle().c_str(), lastMsgId, pResp.blogined());
		
		m_bLogin = pResp.blogined();
		m_strHandle = pResp.strhandle();
		m_bHasPTZ = pResp.bhasptz();
		return m_bLogin;
		
	}
	bool GetRespMsg(long long lastId, OnvifCli::OnvifCliCmd & respCmd)
	{
		int i = 20;
		while(i --)
		{
			std::chrono::milliseconds dura(100);
			std::this_thread::sleep_for(dura);

			/* lock the guard */
			std::lock_guard<std::mutex> guard(m_lock);
			if (m_msgId > lastId)
			{
				respCmd = m_lastCmd;
				return true;
			}
			
		}
		return false;
	}
		
	bool GetProfiles(OnvifAgentCProfileMap &pProfiles)
	{
		/* not logined */
		if (m_bLogin == false)
		{
			return false;
		}
		OnvifCli::OnvifCliCmd cmd;
		cmd.set_type(OnvifCli::CLI_CMD_MEDIA_GET_PROFILES_REQ);
		OnvifCliMediaGetProfilesReq * req = new OnvifCliMediaGetProfilesReq;
		req->set_strstrhandle(m_strHandle);
		cmd.set_allocated_getprofilereq(req);
		std::string strMsg;
		::google::protobuf::util::Status status = 
			::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
		if (!status.ok())
		{
			return false;
		}
		long long lastMsgId = 0;
		/* only lock here */
		{
			std::lock_guard<std::mutex> guard(m_lock);
			lastMsgId = m_msgId;
		}
		
		if (SendMsg(strMsg) == false)
		{
			return false;
		}

		OnvifCli::OnvifCliCmd respCmd;

		if (GetRespMsg(lastMsgId, respCmd) == false)
		{
			return false;
		}

		if (!respCmd.has_getprofileresp())
		{
			return false;
		}
		const OnvifCliMediaGetProfilesResp& pResp =  respCmd.getprofileresp();
		
		printf("%s %d websocket got profiles %d\n", __FILE__, __LINE__,  pResp.bgotprofiles());

		/* loop to get the profiles */
		int nSize = pResp.cprofiles_size();

		for (int i = 0; i < pResp.cprofiles_size(); i ++)
		{
			const OnvifCliProfile &pProfile = pResp.cprofiles(i);
			OnvifAgentCProfile pro;
			pro.strProfileToken = pProfile.strtoken();
			pro.strCodecName = pProfile.strcodecname();
			if (pProfile.bgoturl())
			{
				pro.strRtspUrl = pProfile.strrtspurl();
			}
			pro.nBandwidth = pProfile.nbandwidth();
			pro.nWidth = pProfile.nwidth();
			pro.nHeight = pProfile.nheight();
			pro.nFps = pProfile.nfps();
			pro.strNameDisplay = pro.strCodecName + " " + std::to_string(pro.nWidth) +  " " +
				+"x" + " " + std::to_string(pro.nHeight) + " " +  std::to_string(pro.nFps) +
				"fps" + " " +  std::to_string(pro.nBandwidth) + "bps";
			
			pProfiles[pro.strProfileToken] = pro;
		}		
		
		return pResp.bgotprofiles();
	}

	bool PTZAction(std::string strToken, OnvifCAgentPtzAct act, float speed)
	{
		if (m_bHasPTZ == false)
		{
			return false;
		}
		/* not logined */
		if (m_bLogin == false)
		{
			return false;
		}
		OnvifCli::OnvifCliCmd cmd;
		cmd.set_type(OnvifCli::CLI_CMD_PTZ_REQ);
		OnvifCliPTZReq * req = new OnvifCliPTZReq;
		//req->set_strstrhandle(m_strHandle);
		req->set_token(strToken);
		req->set_speed(speed);
		req->set_cmd((OnvifCliPTZCmdType)act);
		cmd.set_allocated_ptzreq(req);
		std::string strMsg;
		::google::protobuf::util::Status status = 
			::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
		if (!status.ok())
		{
			return false;
		}
		long long lastMsgId = 0;
		/* only lock here */
		{
			std::lock_guard<std::mutex> guard(m_lock);
			lastMsgId = m_msgId;
		}
		
		if (SendMsg(strMsg) == false)
		{
			return false;
		}

		OnvifCli::OnvifCliCmd respCmd;

		if (GetRespMsg(lastMsgId, respCmd) == false)
		{
			return false;
		}

		if (!respCmd.has_ptzresp())
		{
			return false;
		}
		const OnvifCliPTZResp& pResp =  respCmd.ptzresp();
		
		printf("%s %d websocket got ptz resp %d\n", __FILE__, __LINE__,  pResp.bret());

		return pResp.bret();
	}

private:
	bool m_bLogin;
	std::string m_strUser;
	std::string m_strPasswd;
	std::string m_strUrl;
	OnvifCli::OnvifCliCmd m_lastCmd;
	std::string m_strHandle;
	bool m_bHasPTZ;
};


#endif /* __ONVIF_AGENT_CLIENT__ */
