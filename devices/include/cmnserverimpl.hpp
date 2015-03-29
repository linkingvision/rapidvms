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
#define  CMN_SSL_SOCKET
#ifndef CMN_SSL_SOCKET
        XSocket socket;
#else
        XSSLSocket socket;
        socket.UsePEMCertificateFile( "C:\\videodb\\ServerCRT1.crt" );
        socket.UsePEMRSAPrivateKeyFile( "C:\\videodb\\PrivateKey1.key" );
#endif
        socket.Bind(m_port);
        socket.Listen();

	while(1)
	{
#ifndef CMN_SSL_SOCKET
		XRef<XSocket> clientSocket = socket.Accept();
#else
		XRef<XSocket> clientSocket = socket.Accept();
#endif

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
#ifndef CMN_SSL_SOCKET
		clientSocket->Shutdown(SOCKET_SHUT_FLAGS);
#endif
		clientSocket->Close();
	}
}
#endif /* __VSC_CMN_SERVER_IMPL__H_ */

