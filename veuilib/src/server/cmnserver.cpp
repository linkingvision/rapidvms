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
#include "server/cmnserver.hpp"

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
		}
		catch(XSDK::XException)
		{
		}
		
		clientSocket->Close();
	}
}

