/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#include "server/cmnoapiserver.hpp"

CmnOAPIServerSession::CmnOAPIServerSession(Factory &pFactory, VEventServer &pEvent, XRef<XSocket> pSocket)
:m_pFactory(pFactory), m_pSocket(pSocket), m_pEvent(pEvent)
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
	OAPIServer server(m_pSocket, m_pFactory, m_pEvent);
	while(1)
	{
		QCoreApplication::processEvents();
		s32 nRet = m_pSocket->Recv((void *)&header, sizeof(header), XDuration(MSECS, 500));
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
				if (nRet == 0)				
				{					
					ve_sleep(20);				
				}
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
OAPIServerWrapper::OAPIServerWrapper(Factory &pFactory, VEventServer &pEvent)
//:m_cmn(pFactory), m_ssl(pFactory)
:m_cmn(pFactory, pEvent)
{

}
OAPIServerWrapper::~OAPIServerWrapper()
{

}

CmnOAPIServer::CmnOAPIServer(Factory &pFactory, VEventServer &pEvent)
:m_pFactory(pFactory), m_pEvent(pEvent)
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
#if 0
#ifdef WIN32
		ULONG mode = 1;
		ioctlsocket( clientSocket->GetSokId(), FIONBIO, &mode);
#else
		int flags = fcntl( clientSocket->GetSokId(), F_GETFL, 0);
		fcntl( clientSocket->GetSokId(), F_SETFL, flags | O_NONBLOCK );
#endif
#endif

		CmnOAPIServerSession *session = new CmnOAPIServerSession(m_pFactory, 
			m_pEvent, clientSocket);
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

