//------------------------------------------------------------------------------
// File: cmnserverimpl.hpp
//
// Desc: Common HTTP server - HTTP server for all application
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_CMN_OAPI_SERVER_IMPL__H_
#define __VSC_CMN_OAPI_SERVER_IMPL__H_

/* Below is SSL */
OAPIServer::OAPIServer(Factory &pFactory)
:m_cmn(pFactory), m_ssl(pFactory)
{

}
OAPIServer::~OAPIServer()
{

}

CmnOAPIServer::CmnOAPIServer(Factory &pFactory)
{

}
CmnOAPIServer::~CmnOAPIServer()
{

}

void CmnOAPIServer::run()
{
	m_port = 9080;
        XSocket socket;

        socket.Bind(m_port);
        socket.Listen();

	while(1)
	{
		XRef<XSocket> clientSocket = socket.Accept();

		ServerSideRequest request;
		try
		{
			request.ReadRequest(clientSocket);

			const URI uri = request.GetURI();

			XString body = "<!DOCTYPE html>\
			<html>\
			<head ><title>OpenCVR</title></head>\
			<body>\
			<h1>Wellcome OpenCVR //TODO</h1><br>\
			</body>\
			</html>";

			ServerSideResponse ssResponse;
			ssResponse.SetBody(body);
			ssResponse.SetContentType("text/html; charset=UTF-8");
			ssResponse.WriteResponse(clientSocket);
		}
		catch(XSDK::XException)
		{
		}
		
		clientSocket->Shutdown(SOCKET_SHUT_FLAGS);
		clientSocket->Close();
	}
}




/* Below is SSL */
CmnOAPISSLServer::CmnOAPISSLServer(Factory &pFactory)
{

}
CmnOAPISSLServer::~CmnOAPISSLServer()
{

}

void CmnOAPISSLServer::run()
{
	m_port = 9554;
	XSSLSocket socket;
	socket.UsePEMCertificateFile( "C:\\videodb\\ServerCRT1.crt" );
	socket.UsePEMRSAPrivateKeyFile( "C:\\videodb\\PrivateKey1.key" );

	socket.Bind(m_port);
	socket.Listen();

	while(1)
	{
		XRef<XSocket> clientSocket = socket.Accept();

		ServerSideRequest request;
		try
		{
			request.ReadRequest(clientSocket);

			const URI uri = request.GetURI();

			XString body = "<!DOCTYPE html>\
			<html>\
			<head ><title>OpenCVR</title></head>\
			<body>\
			<h1>Wellcome OpenCVR //TODO</h1><br>\
			</body>\
			</html>";

			ServerSideResponse ssResponse;
			ssResponse.SetBody(body);
			ssResponse.SetContentType("text/html; charset=UTF-8");
			ssResponse.WriteResponse(clientSocket);
		}
		catch(XSDK::XException)
		{
		}
		
		clientSocket->Close();
	}
}
#endif /* __VSC_CMN_OAPI_SERVER_IMPL__H_ */

