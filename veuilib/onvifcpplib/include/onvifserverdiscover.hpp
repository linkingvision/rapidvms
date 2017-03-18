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
#ifndef __ONVIF_SERVER_DISCOVER__
#define __ONVIF_SERVER_DISCOVER__

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#include <string>
#include <map>
#include <iostream>
#include <list>

#ifdef WIN32
#define snprintf _snprintf
#endif

using namespace std;

#include "soapStub.h"
#include "soapRemoteDiscoveryBindingService.h"
#include "fast_mutex.h"
#include <QUuid>

class OnvifDisItem
{
public:
    OnvifDisItem()
    {
    }
    OnvifDisItem(std::string strHost, std::string strPort,  std::string strType, 
                        std::string strUuid)
    {
        type = strType;
        host = strHost;
        port = strPort;
        uuid = strUuid;
    }
    OnvifDisItem(const OnvifDisItem &pParam)
    {
        type = pParam.type;
        host = pParam.host;
        port = pParam.port;
        uuid = pParam.uuid;
    }
    ~OnvifDisItem(){}
    OnvifDisItem & operator=(const OnvifDisItem &pParam)
    {
        type = pParam.type;
        host = pParam.host;
        port = pParam.port;
        uuid = pParam.uuid;
        return *this;
    }
public:
    std::string type;
    std::string host;
    std::string port;
    std::string uuid;
};

typedef std::map<std::string, OnvifDisItem> OnvifDisMap;

class OnvifServerDiscover : public RemoteDiscoveryBindingService
{
public:
    OnvifServerDiscover(struct soap * pData);
    ~OnvifServerDiscover();
public:
	virtual	RemoteDiscoveryBindingService *copy()
	{
		return NULL;
	}
	virtual	int Hello(ns1__HelloType tdn__Hello, ns1__ResolveType &tdn__HelloResponse)
	{
		return SOAP_OK;
	}

	/// Web service operation 'Bye' (returns error code or SOAP_OK)
	virtual	int Bye(ns1__ByeType tdn__Bye, ns1__ResolveType &tdn__ByeResponse)
	{
		return SOAP_OK;
	}
#if 0
	/// Web service operation 'Probe' (returns error code or SOAP_OK)
	virtual	int Probe(struct wsdd__ProbeType tdn__Probe, struct wsdd__ProbeMatchesType &tdn__ProbeResponse)
	{

	       string m_strLoalAddr = inet_ntoa(m_pSoap->local.sin_addr);
		static struct wsdd__ProbeMatchType ProbeMatch;
		static struct wsdd__ScopesType scopes;
		static char MessageID[100];
		static char szXAddr[64];
		static char szEndpointReference[64];
		static struct wsa__Relationship wsa_RelatesTo;
		memset(szXAddr, 0, 64);
		strcpy(szXAddr, m_strServerEndpoint.c_str());
		unsigned char MacAddr[8] ={0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

		sprintf(szEndpointReference, "urn:uuid:11223344-5566-7788-99aa-%02x%02x%02x%02x%02x%02x",
						MacAddr[0],MacAddr[1],MacAddr[2],MacAddr[3],MacAddr[4],MacAddr[5]);

		printf("__tdn__Probe %p, %p, %d\n", tdn__ProbeResponse, tdn__ProbeResponse.ProbeMatch, tdn__ProbeResponse.__sizeProbeMatch);
		soap_default_wsdd__ProbeMatchType(m_pSoap, &ProbeMatch);
		soap_default_wsdd__ScopesType(m_pSoap, &scopes);
		strcpy(MessageID, "urn:uuid:");
		if (m_pSoap->header)
		{
			if(m_pSoap->header->wsa__MessageID)
			{
				printf("remote wsa__MessageID : %s\n",m_pSoap->header->wsa__MessageID);
				m_pSoap->header->wsa__RelatesTo =&wsa_RelatesTo;
				soap_default__wsa__RelatesTo(m_pSoap, m_pSoap->header->wsa__RelatesTo);
				m_pSoap->header->wsa__RelatesTo->__item = m_pSoap->header->wsa__MessageID;

				m_pSoap->header->wsa__MessageID = MessageID;
				m_pSoap->header->wsa__To = "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";
				m_pSoap->header->wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches";
			}
		}

		scopes.__item =  "onvif://www.onvif.org/type/Network_Video_Storage onvif://www.onvif.org/hardware/OpenCVR"
				" onvif://www.onvif.org/name/OpenCVR";

		ProbeMatch.wsa__EndpointReference.Address = szEndpointReference;
		ProbeMatch.Types = "wsdd:Network_Video_Storage";
		ProbeMatch.Scopes = &scopes;
		ProbeMatch.XAddrs = szXAddr;
		ProbeMatch.MetadataVersion = 22;

		tdn__ProbeResponse.__sizeProbeMatch = 1;
		tdn__ProbeResponse.ProbeMatch = &ProbeMatch;

		return SOAP_OK;
	}
#endif
public:
	bool AddHost(astring host, astring port, std::string type)
	{
		string strServerEndpoint = "http://" + host + ":" + port;
		//m_Map[strServerEndpoint] = type;
		return true;
	}
	bool DelHost(astring host, astring port)
	{
		string strServerEndpoint = "http://" + host + ":" + port;
		//m_Map.earse(strServerEndpoint);
		return true;
	}
	
private:
	struct soap * m_pSoap;
	string m_strServerEndpoint;
	OnvifDisMap m_Map;
};

inline OnvifServerDiscover::OnvifServerDiscover(struct soap * pData)
: RemoteDiscoveryBindingService(*pData), m_pSoap(pData)
{

}

inline OnvifServerDiscover::~OnvifServerDiscover()
{

}



class OnvifServerDiscoverTask
{
public:
    OnvifServerDiscoverTask(struct soap * pData);
    ~OnvifServerDiscoverTask();
public:
	bool Run(bool &bQuit);
	bool Run1(bool &bQuit);
	void Lock()
	{
		m_pMutex.lock();
	}
	void UnLock()
	{
		m_pMutex.unlock();
	}
	bool AddHost(astring host, astring port, astring type);
	bool DelHost(astring host, astring port);
public:
	int onvifProbeInit();
	int onvifProbeRly(char * p_msg_id,int vfd, 
						unsigned int rip,unsigned int rport, string onvif_addr, OnvifDisItem &pItem);
	int onvifProbeRecv(int fd);
	void onvifHello(int fd);
private:
	struct soap * m_pSoap;
	string m_strServerEndpoint;
	OnvifServerDiscover m_Discover;
	tthread::fast_mutex m_pMutex;
	OnvifDisMap m_Map;
};

inline OnvifServerDiscoverTask::OnvifServerDiscoverTask(struct soap * pData)
:m_pSoap(pData), m_Discover(pData)
{

}

inline OnvifServerDiscoverTask::~OnvifServerDiscoverTask()
{
	if (m_pSoap)
	{
		// TODO soap_delete(m_pSoap, );
	}
}

inline bool OnvifServerDiscoverTask::AddHost(astring host, astring port, astring type)
{
    bool ret = false;
    Lock();
    
    QUuid id = QUuid::createUuid();
    OnvifDisItem item(host, port, type, id.toString().toStdString());
    
    string strServerEndpoint = "http://" + host + ":" + port + "/onvif/device_service";
    m_Map[strServerEndpoint] = item;
    UnLock();

    return true;
}
inline bool OnvifServerDiscoverTask::DelHost(astring host, astring port)
{
    bool ret = false;
    Lock();

    string strServerEndpoint = "http://" + host + ":" + port + "/onvif/device_service";
    m_Map.erase(strServerEndpoint);
    UnLock();

    return true;
}

inline bool OnvifServerDiscoverTask::Run1(bool &bQuit)
{
	int socket = -1;
	int timeStart = time(NULL);
	int currentTime = 0;	
	int ret = 0;
	
	m_pSoap->bind_flags = SO_REUSEADDR;
	soap_imode(m_pSoap, SOAP_IO_UDP | SOAP_XML_IGNORENS);
	soap_omode(m_pSoap, SOAP_IO_UDP | SOAP_XML_IGNORENS);
	socket = soap_bind(m_pSoap, "239.255.255.250", 3702, 100);
	if (socket == SOAP_INVALID_SOCKET ) 
	{
	    printf("OnvifServerDiscoverTask::soap_bind Binding on 3702 failed");
	    return 0;
	}

	ip_mreq mcast;
	mcast.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
	mcast.imr_interface.s_addr = htonl(INADDR_ANY);
	printf("OnvifServerDiscoverTask socket %d\n", socket);
	if (setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast)) < 0)
	{
		printf("IP_ADD_MEMBERSHIP failed\n");
		soap_print_fault(m_pSoap, stderr);
		return false;
	}

	//printf("%s----%d\n", __FILE__, __LINE__);
	//Loop to recevie the request
	while(bQuit)
	{
		currentTime = time(NULL);
		
		if( (ret = soap_accept(m_pSoap)) == SOAP_INVALID_SOCKET) {
			printf("Discover soap_accept accepting timeout\n");
			continue;
		}
		
		if ((m_Discover.serve()) != SOAP_OK) {
		    printf("Discover soap_begin_serve serve %d failed", ret);
		    continue;
		}
		printf("OnvifServerDiscoverTask  %s\n", m_pSoap->host);
		std::string buf (m_pSoap->buf, m_pSoap->buflen);

		//printf("Recevied discover ========> \n%s\n", buf.c_str());
		Lock();
		ret = m_Discover.dispatch();
		printf("m_Discover %d \n", ret);
		UnLock();
		if (ret == SOAP_OK)
			continue;


	}
	return 0;
}

inline int OnvifServerDiscoverTask::onvifProbeInit()
{
	int len = 65535;
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd < 0)
	{
		printf("socket SOCK_DGRAM error!\n");
		return -1;
	}	
	
	if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(char *)&len,sizeof(int)))
	{  
		printf("error!setsockopt SO_REUSEADDR failed!\n");
		closesocket(fd);
		return -1;
	}
	
    	struct sockaddr_in addr;
	int addr_len = sizeof(addr);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(3702);

	if(::bind(fd,(struct sockaddr *)&addr,sizeof(addr)) == -1)
	{
		//printf("Bind udp socket fail,error = %s\r\n",sys_os_get_socket_error());
		closesocket(fd);
		return -1;
	}

	if(setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char*)&len, sizeof(int)))
	{
		printf("setsockopt SO_SNDBUF error!\n");
		closesocket(fd);
		return -1;
	}

	if(setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char*)&len, sizeof(int)))
	{
		printf("setsockopt SO_RCVBUF error!\n");
		closesocket(fd);
		return -1;
	}

	struct ip_mreq mcast;
	mcast.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
	mcast.imr_interface.s_addr = INADDR_ANY;

	if(setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast)) < 0)
	{
		closesocket(fd);
		return -1;
	}

	return fd;
}

inline int OnvifServerDiscoverTask::onvifProbeRly(char * p_msg_id,int vfd, 
    unsigned int rip,unsigned int rport, string onvif_addr, OnvifDisItem &pItem)
{
	if (p_msg_id == NULL)
	{
		return -1;
	}
	
        char send_buffer[1024 * 64] = {0};
        char mac_addr[64] = {0};
        sprintf(mac_addr, "%s", "4419b7046f76");

        sprintf(send_buffer,
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
		"<SOAP-ENV:Envelope "
		"xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" "
		"xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" "
		"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
		"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
		"xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" "
		"xmlns:wsa5=\"http://www.w3.org/2005/08/addressing\" "
		"xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" "
		"xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" "
		"xmlns:c14n=\"http://www.w3.org/2001/10/xml-exc-c14n#\" "
		"xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\" "
		"xmlns:xenc=\"http://www.w3.org/2001/04/xmlenc#\" "
		"xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" "
		"xmlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\" "
		"xmlns:xmime=\"http://tempuri.org/xmime.xsd\" "
		"xmlns:xop=\"http://www.w3.org/2004/08/xop/include\" "
		"xmlns:x0018=\"http://tempuri.org/x0018.xsd\" "
		"xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
		"xmlns:ns1=\"http://docs.oasis-open.org/wsrf/bf-2\" "
		"xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" "
		"xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" "
		"xmlns:dndl=\"http://www.onvif.org/ver10/network/wsdl/DiscoveryLookupBinding\" "
		"xmlns:dnrd=\"http://www.onvif.org/ver10/network/wsdl/RemoteDiscoveryBinding\" "
		"xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
		"xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" "
		"xmlns:ns3=\"http://docs.oasis-open.org/wsrf/r-2\" "
		"xmlns:ns10=\"http://www.onvif.org/ver10/events/wsdl/PullPointBinding\" "
		"xmlns:ns11=\"http://www.onvif.org/ver10/events/wsdl/CreatePullPointBinding\" "
		"xmlns:ns12=\"http://www.onvif.org/ver10/events/wsdl/PausableSubscriptionManagerBinding\" "
		"xmlns:ns5=\"http://www.onvif.org/ver10/events/wsdl/PullPointSubscriptionBinding\" "
		"xmlns:ns6=\"http://www.onvif.org/ver10/events/wsdl/EventBinding\" "
		"xmlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" "
		"xmlns:ns7=\"http://www.onvif.org/ver10/events/wsdl/SubscriptionManagerBinding\" "
		"xmlns:ns8=\"http://www.onvif.org/ver10/events/wsdl/NotificationProducerBinding\" "
		"xmlns:ns9=\"http://www.onvif.org/ver10/events/wsdl/NotificationConsumerBinding\" "
		"xmlns:ter=\"http://www.onvif.org/ver10/error\" "
		"xmlns:tns1=\"http://www.onvif.org/ver10/topics\" "
		"xmlns:tptz=\"http://www.onvif.org/ver20/ptz/wsdl\">"
		"<SOAP-ENV:Header>"
		    "<wsa:MessageID>urn:uuid:%s</wsa:MessageID>"
		"<wsa:RelatesTo>%s</wsa:RelatesTo>"
		"<wsa:To>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:To>"
		"<wsa:Action>http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches</wsa:Action>"
		"</SOAP-ENV:Header>"
		    "<SOAP-ENV:Body>"
		        "<d:ProbeMatches>"
		        "<d:ProbeMatch>"
		        "<wsa:EndpointReference>"
		                "<wsa:Address>urn:uuid:%s</wsa:Address>"
		        "</wsa:EndpointReference>"
		        "<d:Types>dn:NetworkVideoTransmitter</d:Types>"
		        "<d:Scopes>"
		            "onvif://www.onvif.org/location/country/china "
		            "onvif://www.onvif.org/hardware/rapidvms "
		            "onvif://www.onvif.org/name/rapidvms "
		            "onvif://www.onvif.org/type/video_encoder "
		            "onvif://www.onvif.org/type/audio_encoder "	
		        "</d:Scopes>"
		        "<d:XAddrs>%s</d:XAddrs>"
		        "<d:MetadataVersion>10</d:MetadataVersion>"
		        "</d:ProbeMatch>"
		        "</d:ProbeMatches>"
		    "</SOAP-ENV:Body>"
		"</SOAP-ENV:Envelope>", pItem.uuid.c_str(),p_msg_id, pItem.uuid.c_str(), onvif_addr.c_str());
	

	//sprintf(send_buffer,xml_probe_format,mac_addr,p_msg_id, onvif_addr.c_str());

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = rip;
	addr.sin_port = rport;

	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd < 0)
	{
		printf("socket SOCK_DGRAM error!\n");
		return -1;
	}	
#if 0
    	struct sockaddr_in addrLocal;
	int addr_len = sizeof(addrLocal);

	addrLocal.sin_family = AF_INET;
	addrLocal.sin_addr.s_addr = inet_addr(pItem.host.c_str());
	addrLocal.sin_port = htons(3702);

	if(bind(fd,(struct sockaddr *)&addrLocal,sizeof(addrLocal)) == -1)
	{
		//printf("Bind udp socket fail,error = %s\r\n",sys_os_get_socket_error());
		closesocket(fd);
		return -1;
	}
#endif

	int len = strlen(send_buffer);
	printf("Send ======> %s \n\n", send_buffer);
	int rlen = sendto(fd, send_buffer, len,0,(struct sockaddr *)&addr,sizeof(struct sockaddr_in));
	if(rlen != len)
	{
		printf("OnvifServerDiscover::RecvLen = %d,SendLen = %d,ip=0x%08x\r\n",rlen,len,ntohl(rip));
	}

       closesocket(fd);

	return rlen;

}

inline char* strtrim(char* s)
{
	
	int len=0,i=0,j=0,k=0;
	len = strlen(s);
	if( len == 0 ) return NULL;

	j = len -1;
	while(s[j] == ' ' || s[i] == '\t' )
	{
		s[j--] = '\0';
		if(j < 0 ) break;
	}

	while( s[i] == ' ' || s[i] == '\t' )  i++;
	if( i == 0 ) return s;
        while( s[k] != '\0') s[k++] = s[i++];

	return s;

}


inline string OnvifServerGetMsgId(char *rbuf)
{
    char message_id[1024] = {0};
    strtrim(rbuf);
    char * msgStart = strstr(rbuf, "MessageID>");
    

    
    if (msgStart)
    {
        msgStart = msgStart + strlen("MessageID>");
        char *msgEnd = strstr(msgStart, "<");
        if (msgEnd != NULL)
        {
            snprintf(message_id, msgEnd - msgStart, "%s",  msgStart);
        }
    }
    return message_id;
    
}

inline int OnvifServerDiscoverTask::onvifProbeRecv(int fd)
{
	char rbuf[1024*64] = {'\0'};
	char message_id[128];
	memset(message_id,0,sizeof(message_id));

	struct sockaddr_in addr;
	int addr_len = sizeof(struct sockaddr_in);
	int rlen = recvfrom(fd, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&addr, (socklen_t*)&addr_len);
	if(rlen < 0)
		return -1;
       printf("Recv ======> %s \n\n", rbuf);
       string strMsgId = OnvifServerGetMsgId(rbuf);
       printf("Msg ID ======> %s \n\n", strMsgId.c_str());
       

	unsigned int src_ip = addr.sin_addr.s_addr;
	unsigned int src_port = addr.sin_port;

        Lock();
        OnvifDisMap::iterator it = m_Map.begin(); 
        for(; it!=m_Map.end(); ++it)
        {
            if (strstr(rbuf, "NetworkVideoTransmitter") != NULL)
            {
            	onvifProbeRly((char *)(strMsgId.c_str()), fd, src_ip, src_port, (*it).first, (*it).second);
            	//onvifProbeRly(message_id, fd, src_ip, src_port, "http://192.168.22.123/");
            }

        }
        UnLock();
        

	return 0;
}

//char xml_hdr[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
//char soap_head[] = "<SOAP-ENV:Envelope %s><SOAP-ENV:Body>";
//char soap_tailer[] ="</SOAP-ENV:Body></SOAP-ENV:Envelope>";
inline void OnvifServerDiscoverTask::onvifHello(int fd)
{
    char p_buf[4096];
    int offset = 0;
    int mlen = 4096;
    
    offset += snprintf(p_buf+offset, mlen-offset, "%s", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    offset += snprintf(p_buf+offset, mlen-offset, "<SOAP-ENV:Envelope %s><SOAP-ENV:Body>", 
        "xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" "
        "xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" " 
        "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" " 
        "xmlns:wsa5=\"http://www.w3.org/2005/08/addressing\" " 
        "xmlns:xmime=\"http://tempuri.org/xmime.xsd\" " 
        "xmlns:xop=\"http://www.w3.org/2004/08/xop/include\" " 
        "xmlns:ns1=\"http://docs.oasis-open.org/wsrf/bf-2\" " 
        "xmlns:tt=\"http://www.onvif.org/ver10/schema\" " 
        "xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" " 
        "xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" " 
        "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" " 
        "xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" "
        "xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" "
        "xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" ");

    offset += snprintf(p_buf+offset, mlen-offset, 
        "<d:Hello>"
            "<wsa:EndpointReference>"
                "<wsa:Address>urn:rapidvms</wsa:Address>"
            "</wsa:EndpointReference>"
            "<d:Types>tds:device</d:Types>"
            "<d:Scopes MatchBy=\"onvif://www.onvif.org/location/country/china "
		        "onvif://www.onvif.org/hardware/rapidvms "
		        "onvif://www.onvif.org/name/rapidvms "
		        "onvif://www.onvif.org/type/video_encoder \">"
            "</d:Scopes>"
            "<d:XAddrs>http://%s:%d</d:XAddrs>"
            "<d:MetadataVersion>22</d:MetadataVersion>"
        "</d:Hello>", 
       // get_local_ip(), get_onvif_serv_port());
	   "0.0.0.0", "0");
    
    offset += snprintf(p_buf+offset, mlen-offset, "</SOAP-ENV:Body></SOAP-ENV:Envelope>");

    struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("239.255.255.250");
	addr.sin_port = htons(3702);
	
    int rlen = sendto(fd, p_buf, offset, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
	if(rlen != offset)
	{
		printf("OnvifServerDiscover::RecvLen = %d,SendLen = %d \r\n",rlen,offset);
	}
	
}



inline bool OnvifServerDiscoverTask::Run(bool &bQuit)
{
	int fd = onvifProbeInit();
	if( fd < 0 )
	{
		printf(" fd failed, maybe already running\r\n");
		return false;
	}

	onvifHello(fd);
	
	while(!bQuit)
	{
		onvifProbeRecv(fd);
	}
	return 0;
}


#endif
