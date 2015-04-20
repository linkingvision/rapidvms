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
BOOL OAPIClient::Setup(std::string strUser, std::string strPasswd)
{
	//TODO
	return TRUE;
}

BOOL OAPIClient::SendDeviceListRequest()
{
	OAPIHeader header;
	header.cmd = htonl(OAPI_CMD_DEVICE_LIST_REQ);
	header.length = 0;

	m_pSocket->Send((void *)&header, sizeof(header));

	return TRUE;

}

BOOL OAPIClient::StartLiveview(int nId)
{
	OAPIHeader header;

	oapi::LiveView liveview;
	liveview.nId = nId;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(liveview);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	header.cmd = htonl(OAPI_CMD_START_LIVE);
	header.length = htonl(nJsonLen + 1);;
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	return TRUE;

}

BOOL OAPIClient::StopLiveview(int nId)
{
	OAPIHeader header;

	oapi::LiveView liveview;
	liveview.nId = nId;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(liveview);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	header.cmd = htonl(OAPI_CMD_STOP_LIVE);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	return TRUE;

}
BOOL OAPIClient::ParseDeviceList(char *pRecv, int len, oapi::DeviceList &list)
{
	autojsoncxx::ParsingResult result;
	if (!autojsoncxx::from_json_string(pRecv, list, result)) 
	{
	    std::cerr << result << '\n';
	    return FALSE;
	}
	return TRUE;
}
#endif