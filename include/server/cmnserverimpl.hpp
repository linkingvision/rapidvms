//------------------------------------------------------------------------------
// File: cmnserverimpl.hpp
//
// Desc: Common HTTP server - HTTP server for all application
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_CMN_SERVER_IMPL__H_
#define __VSC_CMN_SERVER_IMPL__H_

CmnHttpServer::CmnHttpServer(u16 port)
:m_port(port)
{

}
CmnHttpServer::~CmnHttpServer()
{

}

void CmnHttpServer::run()
{
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




/* Below is HTTPS */
CmnHttpsServer::CmnHttpsServer(u16 port)
:m_port(port)
{

}
CmnHttpsServer::~CmnHttpsServer()
{

}

void CmnHttpsServer::run()
{
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
#endif /* __VSC_CMN_SERVER_IMPL__H_ */

