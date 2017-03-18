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

#include "vhttpserver.hpp"

#include "oapis.hpp"

#if defined (WIN32__REMOVE) && !defined (WIN64)

#include "cloudapi/basecloudapiserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>



using namespace jsonrpc;
using namespace std;
using namespace oapi;
using namespace cloudapi;

class CloudAPIServer : public BaseCloudAPIServer
{
public:
	CloudAPIServer(Factory & pFactory, AbstractServerConnector &connector, serverVersion_t type);

	virtual void notifyServer();

	virtual std::string login(const std::string& passwd, const std::string& user);
	virtual std::string logout(const std::string& sessionId) ;
	virtual std::string getAllDevices(const std::string& sessionId);
	virtual std::string getDevice(int id, const std::string& sessionId);
	virtual std::string getDeviceHlsUrl(int id, const std::string& sessionId);
		
private:
	Factory & m_pFactory;

};

CloudAPIServer::CloudAPIServer(Factory & pFactory, AbstractServerConnector &connector, serverVersion_t type) :
    BaseCloudAPIServer(connector, type), m_pFactory(pFactory)
{
}

std::string CloudAPIServer::getAllDevices(const std::string& sessionId)
{
	cloudapi::CloudAPIGetAllDevicesRet dataList;
	dataList.Num = 0;

	DeviceParamMap pDeviceMap;
	DeviceOnlineMap pDeviceOnlineMap;
	m_pFactory.GetDeviceParamMap(pDeviceMap);
	m_pFactory.GetDeviceOnlineMap(pDeviceOnlineMap);
	u16 hlsPort = 9001;

	m_pFactory.GetVHLSSPort(hlsPort);

	char strPath[256] = {};

	DeviceParamMap::iterator it = pDeviceMap.begin(); 
	for(; it!=pDeviceMap.end(); ++it)
	{
		cloudapi::CloudAPIDevice data;
		OAPIConverter::Converter(((*it).second).m_Conf.data.conf, data);
		sprintf(strPath, ":%d/live/%d.m3u8", hlsPort, data.nId);
		data.HlsUrl = strPath;
		data.Online = pDeviceOnlineMap[(*it).first];
		dataList.list.push_back(data);
		dataList.Num ++;
	}

	std::string strJson = autojsoncxx::to_pretty_json_string(dataList);
	return strJson;
}

std::string CloudAPIServer::getDevice(int id, const std::string& sessionId)
{
    return "fake";
}

std::string CloudAPIServer::login(const std::string& passwd, const std::string& user)
{
	if (m_pFactory.AuthUser((std::string&)user, (std::string&)passwd) != TRUE)
	{
		return "false";
	}
	//TODO cache the session ID
    	return "00bc8933-46aa-4dc9-9304-4bd3444ad71d";
}
std::string CloudAPIServer::logout(const std::string& sessionId) 
{
    return "true";
}
std::string CloudAPIServer::getDeviceHlsUrl(int id, const std::string& sessionId)
{
	u16 hlsPort = 9001;

	m_pFactory.GetVHLSSPort(hlsPort);

	char strPath[256] = {};
	sprintf(strPath, ":%d/live/%d.m3u8", hlsPort, id);
	std::string strHlsPath = strPath;
	
 	return strHlsPath;
}

void CloudAPIServer::notifyServer()
{
    cout << "Server got notified" << endl;
}

class VHTTPServerImpl : public QThread
{
public:
	VHTTPServerImpl(Factory & pFactory);
	~VHTTPServerImpl();
public:
	void run();
	
private:
	bool m_bQuit;
	Factory & m_pFactory;
	u16 m_pPort;
  
};

VHTTPServerImpl::VHTTPServerImpl(Factory & pFactory)
:m_bQuit(false), m_pFactory(pFactory)
{
	m_pFactory.GetVHTTPSPort(m_pPort);
}

VHTTPServerImpl::~VHTTPServerImpl()
{

}

void VHTTPServerImpl::run()
{
	HttpServer httpserver(m_pPort);

	/* hybrid server (json-rpc 1.0 & 2.0) */
	CloudAPIServer s(m_pFactory, httpserver, JSONRPC_SERVER_V1V2);
	s.StartListening();

	while(!m_bQuit)
	{
		ve_sleep(10 * 1000);
	}

	s.StopListening();
	return;
}

class VHTTPsServerImpl : public QThread
{
public:
	VHTTPsServerImpl(Factory & pFactory);
	~VHTTPsServerImpl();
public:
	void run();
	
private:
	bool m_bQuit;
	Factory & m_pFactory;
	u16 m_pPort;
  
};

VHTTPsServerImpl::VHTTPsServerImpl(Factory & pFactory)
:m_bQuit(false), m_pFactory(pFactory)
{
	m_pFactory.GetVHTTPSSSLPort(m_pPort);
}

VHTTPsServerImpl::~VHTTPsServerImpl()
{

}

void VHTTPsServerImpl::run()
{
	//TODO key
	std::string strPem = "server.pem";	
	std::string key = "server.key";	
	HttpServer httpserver(m_pPort, strPem, key);

	/* hybrid server (json-rpc 1.0 & 2.0) */
	CloudAPIServer s(m_pFactory, httpserver, JSONRPC_SERVER_V1V2);
	s.StartListening();

	while(!m_bQuit)
	{
		ve_sleep(10 * 1000);
	}

	s.StopListening();
	return;
}




VHTTPServer::VHTTPServer(Factory & pFactory)
: m_pFactory(pFactory)
{
    m_pImpl = new VHTTPServerImpl(pFactory);
    m_pImpl->start();

    m_pImplHttps= new VHTTPsServerImpl(pFactory);
    m_pImplHttps->start();
}

VHTTPServer::~VHTTPServer()
{

}
#endif

