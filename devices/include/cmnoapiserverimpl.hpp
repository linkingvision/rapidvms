//------------------------------------------------------------------------------
// File: cmnserverimpl.hpp
//
// Desc: Common HTTP server - HTTP server for all application
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_CMN_OAPI_SERVER_IMPL__H_
#define __VSC_CMN_OAPI_SERVER_IMPL__H_

CmnOAPIServerSession::CmnOAPIServerSession(Factory &pFactory, XSocket * pSocket)
:m_pFactory(pFactory), m_pSocket(pSocket)
{

}
CmnOAPIServerSession::~CmnOAPIServerSession()
{

}

void CmnOAPIServerSession::run()
{
	/* Process command from oapi client  */
	OAPIHeader header;
	OAPIServer server(m_pSocket, m_pFactory);
	while(1)
	{
		m_pSocket->RawRecv((void *)&header, sizeof(header));
		if (server.Process(header) == FALSE)
		{
			/* this socket is broken */
			delete m_pSocket;
			break;
		}
	}

	/* kill my self */
	delete this;
}

/* Below is SSL */
OAPIServerWrapper::OAPIServerWrapper(Factory &pFactory)
:m_cmn(pFactory), m_ssl(pFactory)
{

}
OAPIServerWrapper::~OAPIServerWrapper()
{

}

CmnOAPIServer::CmnOAPIServer(Factory &pFactory)
:m_pFactory(pFactory)
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

		CmnOAPIServerSession *session = new CmnOAPIServerSession(m_pFactory, 
			clientSocket.Get());
		session->start();
#if 0

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
#endif
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

