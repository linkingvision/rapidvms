//------------------------------------------------------------------------------
// File: oapicimpl.hpp
//
// Desc: OpenCVR API - OpenCVR API.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPIC_IMPL_H_
#define __VSC_OAPIC_IMPL_H_

OAPIClient::OAPIClient(XRef<XSocket> pSocket)
:m_pSocket(pSocket)
{

}
OAPIClient::~OAPIClient()
{

}


/* Setup connect with server */
BOOL OAPIClient::Setup(std::string strUser, std::string strPasswd, 
		std::string strNonce)
{
	OAPIHeader header;

	/* First get Nonce */
	oapi::LoginReq req;

	/* calc the md5 and compare */
	std::string pass = strNonce + strPasswd;

	XMD5 md5Check;
	md5Check.Update((const uint8_t*)(pass.c_str()), pass.length());

	md5Check.Finalize();
	std::string md5Output = md5Check.GetAsString().c_str();

	req.User = strUser;
	req.Password = md5Output;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(req);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}

	header.cmd = htonl(OAPI_CMD_LOGIN_REQ);
	header.length = htonl(nJsonLen + 1);;

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

BOOL OAPIClient::SendDeviceListRequest()
{
	OAPIHeader header;

	oapi::DeviceListReq req;
	req.bAll = true;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(req);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}

	header.cmd = htonl(OAPI_CMD_DEVICE_LIST_REQ);
	header.length = htonl(nJsonLen + 1);;

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;

}

BOOL OAPIClient::StartLiveview(int nId)
{
	OAPIHeader header;

	oapi::StartLiveViewReq liveview;
	liveview.nId = nId;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(liveview);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	header.cmd = htonl(OAPI_CMD_START_LIVE_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	return TRUE;

}

BOOL OAPIClient::StopLiveview(int nId)
{
	OAPIHeader header;

	oapi::StopLiveViewReq liveview;
	liveview.nId = nId;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(liveview);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	header.cmd = htonl(OAPI_CMD_STOP_LIVE_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	return TRUE;

}
BOOL OAPIClient::ParseDeviceList(char *pRecv, int len, oapi::DeviceListRsp &rsp)
{
	autojsoncxx::ParsingResult result;
	if (!autojsoncxx::from_json_string(pRecv, rsp, result)) 
	{
	    std::cerr << result << '\n';
	    return FALSE;
	}
	return TRUE;
}

BOOL OAPIClient::ParseLogin(char *pRecv, int len, oapi::LoginRsp &rsp)
{
	autojsoncxx::ParsingResult result;
	if (!autojsoncxx::from_json_string(pRecv, rsp, result)) 
	{
	    std::cerr << result << '\n';
	    return FALSE;
	}
	return TRUE;
}
#endif