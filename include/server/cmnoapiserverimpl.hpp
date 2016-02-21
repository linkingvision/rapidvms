//------------------------------------------------------------------------------
// File: cmnserverimpl.hpp
//
// Desc: Common HTTP server - HTTP server for all application
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_CMN_OAPI_SERVER_IMPL__H_
#define __VSC_CMN_OAPI_SERVER_IMPL__H_

CmnOAPIServerSession::CmnOAPIServerSession(Factory &pFactory, XRef<XSocket> pSocket)
:m_pFactory(pFactory), m_pSocket(pSocket)
{
	/* auto delete when the thread exit */
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
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
		QCoreApplication::processEvents();
		s32 nRet = m_pSocket->Recv((void *)&header, sizeof(header), 200);
		if (nRet == sizeof(header) && server.Process(header) == TRUE)
		{
			//printf("%s---%d %d\n", __FILE__, __LINE__, nRet);
			QCoreApplication::processEvents();
			continue;
		}else
		{
			if (m_pSocket->Valid() == true)
			{
				QCoreApplication::processEvents();
				//printf("%s---%d %d\n", __FILE__, __LINE__, nRet);
				continue;
			}else
			{
				//printf("%s---%d %d\n", __FILE__, __LINE__, nRet);
				break;
			}
		}
	}

	return;
}

/* Below is SSL */
OAPIServerWrapper::OAPIServerWrapper(Factory &pFactory)
//:m_cmn(pFactory), m_ssl(pFactory)
:m_cmn(pFactory)
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
#ifdef WIN32
		ULONG mode = 1;
		ioctlsocket( clientSocket->GetSokId(), FIONBIO, &mode);
#else
		int flags = fcntl( clientSocket->GetSokId(), F_GETFL, 0);
		fcntl( clientSocket->GetSokId(), F_SETFL, flags | O_NONBLOCK );
#endif

		CmnOAPIServerSession *session = new CmnOAPIServerSession(m_pFactory, 
			clientSocket);
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
#if 0
			const WEBBY::URI uri = request.GetURI();

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
#endif
		}
		catch(XSDK::XException)
		{
		}
		
		clientSocket->Close();
	}
}
#endif /* __VSC_CMN_OAPI_SERVER_IMPL__H_ */

