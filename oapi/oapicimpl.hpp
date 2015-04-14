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

BOOL OAPIClient::DeviceListRequest(oapi::DeviceList &list)
{
	OAPIHeader header;
	header.cmd = htonl(OAPI_CMD_GET_DEVICE_REQ);
	header.length = 0;

	m_pSocket->Send((void *)&header, sizeof(header));

	s32 nRet = m_pSocket->Recv((void *)&header, sizeof(header));
	if (nRet == sizeof(header))
	{
		header.length = ntohl(header.length);
		char *pRecv = new char[header.length + 1];
		s32 nRetBody = m_pSocket->Recv((void *)pRecv, header.length);
		if (nRetBody == header.length)
		{
			std::cerr << pRecv << '\n';
			autojsoncxx::ParsingResult result;
			if (!autojsoncxx::from_json_string(pRecv, list, result)) 
			{
			    //std::cerr << result << '\n';
			    //return FALSE;
			}
			delete pRecv;
			
		}else
		{
			delete pRecv;
			return FALSE;
		}
	}else
	{
		return FALSE;
	}
}

#endif