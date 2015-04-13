//------------------------------------------------------------------------------
// File: oapiimpl.hpp
//
// Desc: OpenCVR API - OpenCVR API.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPIS_IMPL_H_
#define __VSC_OAPIS_IMPL_H_

OAPIServer::OAPIServer(XRef<XSocket> pSocket, Factory &pFactory)
:m_pFactory(pFactory), m_pSocket(pSocket)
{

}
OAPIServer::~OAPIServer()
{

}

BOOL OAPIServer::ProcessGetDevice(s32 len)
{
#if 0
	char *pRecv = new char [len + 1];
	
	s32 nRet = m_pSocket->Recv((void *)pRecv, len);
	if (nRet == len)
	{

		
	}else
	{
		/* this socket is broken */
		delete pRecv;
		return FALSE;
	}
#endif
#if 0
	autojsoncxx::ParsingResult result;
	Person p;
	if (!autojsoncxx::from_json_file("person.json", p, result)) {
	    std::cerr << result << '\n';
	    return -1;
	}

#endif
	if (len != 0)
	{
		return FALSE;
	}
	oapi::Device data;
	data.nId = 100;
	data.Name = "oapitest";

	oapi::DeviceList dataList;
	dataList.Num = 2;
	dataList.list.push_back(data);

	data.nId = 101;
	data.Name = "oapitest1";
	dataList.list.push_back(data);

	//autojsoncxx::to_json_string

	std::string strJson = autojsoncxx::to_pretty_json_string(dataList);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	OAPIHeader header;
	header.cmd = htonl(OAPI_CMD_GET_DEVICE_RSP);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

BOOL OAPIServer::Process(OAPIHeader &header)
{
	header.version = ntohl(header.version);
	header.cmd = ntohl(header.cmd);
	header.length = ntohl(header.length);

	switch(header.cmd)
	{
		case OAPI_CMD_KEEPALIVE_REQ:
			break;
		case OAPI_CMD_GET_DEVICE_REQ:
			return ProcessGetDevice(header.length);
			break;
		default:
			break;		
	}
	return TRUE;
}

#endif