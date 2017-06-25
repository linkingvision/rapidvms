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
#ifndef __ONVIF_DIS_CLIENT__
#define __ONVIF_DIS_CLIENT__

#include <cstdlib>
#include <thread>
#include <mutex>
#include <string.h>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>

#if (defined(_WIN32) || defined(_WIN64))
#pragma warning(disable:4996)
//#include <ObjBase.h>
#include <Iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "ole32.lib")
#else //linux
#include <ifaddrs.h>
#include <arpa/inet.h>
#endif // WIN32 | WIN64

class CamSearchNotify
{
public:
	virtual bool NewCam(astring strIP, astring strPort, 
			astring strModel, astring strONVIFAddr){return false;}
};

class  OnvifDisClient
{
public:
	OnvifDisClient(std::string strAddr, CamSearchNotify & pNotify)
	: m_pNotify(pNotify), m_pThread(NULL), m_bStarted(false), m_bExit(false), 
	m_strAddr(strAddr)
	{
		
	}
	~OnvifDisClient()
	{
		Stop();
	}

public:
	static std::vector<std::string> GetInterfaces()
	{
		std::vector<std::string> vecNetInterfaces;
		std::vector<std::string> ipv4;
		
#if (defined(_WIN32) || defined(_WIN64))
		ULONG ulLen = 0;
		PIP_ADAPTER_INFO lpAdapterInfo = NULL, lpNextData = NULL;

		GetAdaptersInfo(lpAdapterInfo, &ulLen);
		if (0 == ulLen)
			return ipv4;

		lpAdapterInfo = (PIP_ADAPTER_INFO)(new CHAR[ulLen]);
		if (NULL == lpAdapterInfo)
			return ipv4;

		memset(lpAdapterInfo, 0, ulLen);
		ULONG uRet = GetAdaptersInfo(lpAdapterInfo, &ulLen);
		if (uRet != ERROR_SUCCESS)
		{
		    delete [] lpAdapterInfo;
		    lpAdapterInfo = NULL;
		    
		    return vecNetInterfaces;
		}

		int m_lMaxAdaptersNum = 0;

		for (lpNextData = lpAdapterInfo; lpNextData != NULL; lpNextData = lpNextData->Next)
		{
		    //strncpy(m_host[m_lMaxAdaptersNum], lpNextData->IpAddressList.IpAddress.String, sizeof(m_host[m_lMaxAdaptersNum]));
		    IP_ADDR_STRING *pIpAddrString =&(lpNextData->IpAddressList);
		    int IPnumPerNetCard = 0;
		    do 
		    {
		        if (strcmp("0.0.0.0", pIpAddrString->IpAddress.String))
		        {
		            ++m_lMaxAdaptersNum; 
		            vecNetInterfaces.push_back(pIpAddrString->IpAddress.String);
		        }

		        //std::cout << lpNextData->AdapterName << ",?????IP??:"<< ++IPnumPerNetCard << std::endl;
		        //std::cout << "IP ??:"<< pIpAddrString->IpAddress.String << std::endl;
		        //std::cout << "????:"<< pIpAddrString->IpMask.String << std::endl;
		        //std::cout << "????:"<< lpNextData->GatewayList.IpAddress.String << std::endl;

		        pIpAddrString=pIpAddrString->Next;
		    } while (pIpAddrString);
		}

		delete [] lpAdapterInfo;
		lpAdapterInfo = NULL;

#else //#if (defined(WIN32) || defined(WIN64))

		struct ifaddrs *ifList  = NULL;

		int iRet = getifaddrs(&ifList);
		if (iRet < 0) 
		{
		    return vecNetInterfaces;
		}

		struct sockaddr_in *sin = NULL;
		struct ifaddrs *ifa     = NULL;

		for (ifa = ifList; ifa != NULL; ifa = ifa->ifa_next)
		{
		    if(ifa->ifa_addr->sa_family == AF_INET)
		    {
		        printf("\n>>> interfaceName: %s\n", ifa->ifa_name);
		        sin = (struct sockaddr_in *)ifa->ifa_addr;
		        printf(">>> ipAddress: %s\n", inet_ntoa(sin->sin_addr));

		        vecNetInterfaces.push_back(inet_ntoa(sin->sin_addr));
		        ++m_lMaxAdaptersNum ; 

		        //sin = (struct sockaddr_in *)ifa->ifa_dstaddr;
		        //printf(">>> broadcast: %s\n", inet_ntoa(sin->sin_addr));
		        //sin = (struct sockaddr_in *)ifa->ifa_netmask;
		        //printf(">>> subnetMask: %s\n", inet_ntoa(sin->sin_addr));
		    }
		}
		freeifaddrs(ifList);
#endif
		
		std::vector<std::string>::iterator it;
		for (it = vecNetInterfaces.begin(); it != vecNetInterfaces.end(); ++it)
		{
			string ip = *it;
			size_t p1 = ip.find("127.");
			if (p1 != std::string::npos) 
			{
				continue;
			}
			size_t p2 = ip.find(":");
			if (p2 != std::string::npos) 
			{
				continue;
			}
			ipv4.push_back(*it);
		}
		return ipv4;
	}

public:
	bool Start()
	{
		if (m_bStarted == true)
		{
			return true;
		}
		/* Start Search thread */
		m_pThread = new std::thread(OnvifDisClient::DisThread, this);

		m_bStarted = true;
		return true;	
	}
	bool Stop()
	{
		if (m_bStarted == true)
		{
			m_bExit = true;
			if (m_pThread)
			{
				m_pThread->join();
				delete m_pThread;
				m_pThread = NULL;
			}
		}
		return true;
	}

	void GetIPAndPort(astring &str, astring & ip, astring & port, astring & onvifAddr)
	{
	       astring strItem = "http://";
		size_t pos = 0;
		while (1)
		{
		       strItem = "http://";
			size_t p1 = str.find(strItem, pos);
			if (p1 == string::npos) return;
			size_t posIP = p1 + strItem.length();
			strItem = "/";
			size_t p2 = str.find(strItem,posIP);
			if (p2 == string::npos) return;
			string strGetIP = str.substr(posIP, p2 - posIP);
			if (strGetIP.find(".") == string::npos || 
				strGetIP.find("169") == 0)
			{
				/* IPV6 address */
				pos = p2;
				continue;
			}
			string strGetOnvifAddr;
			strItem = "http://";
			if (str.find(strItem, p2) == string::npos)
			{
				strGetOnvifAddr = str .substr(p2);
			}else
			{
				strItem = " ";
				size_t p3 = str.find(strItem, p2);
				if (p3 == string::npos) return;
				strGetOnvifAddr = str.substr(p2, p3 - p2);
			}

		       string strPort = "80";
			size_t posPort = strGetIP.find_first_of(":");
			if (posPort != std::string::npos) 
			{
				strPort = strGetIP.substr(posPort + 1);
				string strIPTemp = strGetIP.substr(0, posPort);
				strGetIP = strIPTemp;
			}
			ip = strGetIP;
			port = strPort;
			onvifAddr = strGetOnvifAddr;
			strItem = "http://";
			if (str.find(strItem, p2) == string::npos)
			{
				break;
			}else
			{
				pos = p2;
				continue;			
			}
		}
		
	}

	void GetHardwareModel(astring &str, astring & hdModel)
	{
		size_t pos1= 0;
		size_t pos2 = 0;	
		std::string strItem;
		strItem = "hardware/";
		pos2 = str.find(strItem);

		string sHardware = "unknown";
		if (pos2 != string::npos)
		{
			size_t posHardware =  pos2 + strItem.length();
			strItem = "onvif";
			pos1 = str.find(strItem,pos2);
			
			if (pos1 != string::npos)
			{
				sHardware =  str.substr(posHardware,pos1 - posHardware);
				transform(sHardware.begin(), sHardware.end(),sHardware.begin(),(int(*)(int))toupper);
			}
			
		}
		hdModel = sHardware;
	}

	bool SendProbe(int nFd)
	{
		char *cxml = {"<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope x"
		    "mlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" x"
		    "mlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" x"
		    "mlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" x"
		    "mlns:xsd=\"http://www.w3.org/2001/XMLSchema\" x"
		    "mlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" x"
		    "mlns:wsdd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" x"
		    "mlns:chan=\"http://schemas.microsoft.com/ws/2005/02/duplex\" x"
		    "mlns:wsa5=\"http://www.w3.org/2005/08/addressing\" x"
		    "mlns:c14n=\"http://www.w3.org/2001/10/xml-exc-c14n#\" x"
		    "mlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\" x"
		    "mlns:xenc=\"http://www.w3.org/2001/04/xmlenc#\" x"
		    "mlns:wsc=\"http://schemas.xmlsoap.org/ws/2005/02/sc\" x"
		    "mlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" x"
		    "mlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\" x"
		    "mlns:xmime=\"http://tempuri.org/xmime.xsd\" x"
		    "mlns:xop=\"http://www.w3.org/2004/08/xop/include\" x"
		    "mlns:tt=\"http://www.onvif.org/ver10/schema\" x"
		    "mlns:wsrfbf=\"http://docs.oasis-open.org/wsrf/bf-2\" x"
		    "mlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" x"
		    "mlns:wsrfr=\"http://docs.oasis-open.org/wsrf/r-2\" x"
		    "mlns:tad=\"http://www.onvif.org/ver10/analyticsdevice/wsdl\" x"
		    "mlns:tan=\"http://www.onvif.org/ver20/analytics/wsdl\" x"
		    "mlns:tdn=\"http://www.onvif.org/ver10/network/wsdl\" x"
		    "mlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" x"
		    "mlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" x"
		    "mlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" x"
		    "mlns:timg=\"http://www.onvif.org/ver20/imaging/wsdl\" x"
		    "mlns:tls=\"http://www.onvif.org/ver10/display/wsdl\" x"
		    "mlns:tmd=\"http://www.onvif.org/ver10/deviceIO/wsdl\" x"
		    "mlns:tptz=\"http://www.onvif.org/ver20/ptz/wsdl\" x"
		    "mlns:trc=\"http://www.onvif.org/ver10/recording/wsdl\" x"
		    "mlns:trp=\"http://www.onvif.org/ver10/replay/wsdl\" x"
		    "mlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" x"
		    "mlns:trv=\"http://www.onvif.org/ver10/receiver/wsdl\" x"
		    "mlns:tse=\"http://www.onvif.org/ver10/search/wsdl\">"
		    "<SOAP-ENV:Header><wsa:MessageID>urn:uuid:6fc2dc19-3785-445a-b1d1-82063b65ddd1</wsa:MessageID>"
		    "<wsa:To SOAP-ENV:mustUnderstand=\"true\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To>"
		    "<wsa:Action SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe</wsa:Action>"
		    "</SOAP-ENV:Header><SOAP-ENV:Body><wsdd:Probe><wsdd:Types></wsdd:Types><wsdd:Scopes></wsdd:Scopes>"
		    "</wsdd:Probe></SOAP-ENV:Body></SOAP-ENV:Envelope>" };

		struct sockaddr_in addr;

		memset(&addr, 0, sizeof(addr));

		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr("239.255.255.250");
		addr.sin_port = htons(3702);

		int len = strlen(cxml);
		int nRet = sendto(nFd, cxml, len, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
		if (nRet != len)
		{
			printf("SendProbe nRet = %d, len = %d\n", nRet, len);
			return false;
		}

		return true;
	}

	bool GetAttr(std::string &strMsg, std::string strAttr, std::string &strGot)
	{
		// Attr begin & end
		std::string strAddr2 = strAttr + ">";
		const char *wsddAttr_b = strAddr2.c_str();//"<wsdd:Attr>";
		const char *wsddAttr_e = "</";//"</wsdd:Attr>";
		const char * pBuff = strMsg.c_str();
		char *find_Attr_b = strstr((char *)pBuff, wsddAttr_b);
		char *find_Attr_e =strstr(find_Attr_b, wsddAttr_e);
		if(find_Attr_b==NULL || wsddAttr_e==NULL){
			return  false;
		}

		//get the a Attr
		int len_b = strlen(wsddAttr_b);              
		int len = find_Attr_e - find_Attr_b - len_b;  

		std::string strTemp;
		strTemp.resize(len + 1);
		char *tmp = &strTemp[0];
		memset(tmp, 0, len+1);
		for(int i=0;i<len;i++)
		{
			tmp[i] = find_Attr_b[len_b+i];
			/*
			if(find_Attr_b[len_b+i] != ' ')
			{        
				tmp[i] = find_Attr_b[len_b+i] ;   
			}else
			{
				len = i ;
				break;
			}
			*/
		}
		
		std::string strNew(tmp, len);
		strGot = strNew;
		
		return true;
	}

	bool ProcessRecvMsg(std::string &strMsg)
	{
		std::string strAddr;
		std::string strTypes;
		std::string strScopes;
		if (GetAttr(strMsg, "XAddrs", strAddr) == false)
		{
			return false;
		}
		if (GetAttr(strMsg, "Types", strTypes) == false)
		{
			return false;
		}
		if (GetAttr(strMsg, "Scopes", strScopes) == false)
		{
			return false;
		}

		transform(strAddr.begin(), strAddr.end(), strAddr.begin(), (int(*)(int))tolower);
		transform(strTypes.begin(), strTypes.end(), strTypes.begin(), (int(*)(int))tolower);
		transform(strScopes.begin(), strScopes.end(), strScopes.begin(), (int(*)(int))tolower);

		if (strTypes.find("network_video_storage") != string::npos)
		{
			printf("[ONVIF]: it is a NVS device\n");
			return true;
		} 
		size_t pos1 = strTypes.find("networkvideotransmitter");
		printf( "[ONVIF]: Searched ip %s\n", strAddr.c_str());
		if (pos1 == string::npos)
		{
			return true;
		}	

		astring ip = "192.168.0.1";
		astring port = "80";
		astring hdModel = "unknown";
		astring OnvifAddr = "/onvif/device_service";
		GetIPAndPort(strAddr,  ip, port, OnvifAddr);
		GetHardwareModel(strScopes, hdModel);

		m_pNotify.NewCam(ip, port, hdModel, OnvifAddr);
		
		return true;
	}
	
public:
	static void DisThread(void *user_data)
	{	
		OnvifDisClient* pThread = (OnvifDisClient*)user_data;

		if (pThread)
		{
			return pThread->DisThread1();
		}
		return;
	}
	void DisThread1()

	{
		int fd = socket(AF_INET, SOCK_DGRAM, 0);
		if(fd < 0)
		{
			printf("socket SOCK_DGRAM error!\n");
			return ;
		}

		struct sockaddr_in addr;
		char on = 1;
		memset(&addr, 0, sizeof(addr));
		addr.sin_port = 0;
		addr.sin_family = AF_INET;

		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		unsigned requestedSize = 1024 * 1024;
		 int sizeSize = sizeof requestedSize;
		setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char*)&requestedSize, sizeSize);
		
		addr.sin_addr.s_addr = inet_addr(m_strAddr.c_str());
		if (::bind(fd, (struct sockaddr *)&addr, sizeof(addr)))
		{
			printf("bind error");
			return;
		}

		SendProbe(fd);

		/* loop to receive data udp data */
		while(!m_bExit)
		{
			char rbuf[65535];
			int ret;
			fd_set fdread;
			struct timeval tv = {0, 500};

			FD_ZERO(&fdread);
			FD_SET(fd, &fdread); 

			ret = select(fd+1, &fdread, NULL, NULL, &tv); 
			if (ret == 0) // Time expired 
			{ 
				continue; 
			}
			else if (!FD_ISSET(fd, &fdread))
			{
				continue;
			}

			struct sockaddr_in addr;
			int addr_len = sizeof(struct sockaddr_in);
			int rlen = recvfrom(fd, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&addr, &addr_len);
			if (rlen <= 0)
			{
				printf("DisThread1 rlen = %d, fd = %d\r\n", rlen, fd);
				continue;
			}
			std::string strRecv(rbuf, rlen);
			ProcessRecvMsg(strRecv);
		}
		return;
	}	

private:
	CamSearchNotify &m_pNotify;
	std::thread *m_pThread;
	bool m_bStarted;
	bool m_bExit;
	std::string m_strAddr;
};

typedef std::map<std::string, OnvifDisClient *> OnvifDisClientMap;

class  OnvifDisClientMgr: public CamSearchNotify
{
public:
	OnvifDisClientMgr(CamSearchNotify & pNotify)
	: m_pNotify(pNotify), m_bStarted(false)
	{
		
	}
	~OnvifDisClientMgr()
	{
		Stop();
	}
public:
	virtual bool NewCam(astring strIP, astring strPort, 
			astring strModel, astring strONVIFAddr)
	{
		std::lock_guard<std::mutex> guard(m_lock);
		return m_pNotify.NewCam(strIP, strPort, strModel, strONVIFAddr);
	}
	bool Start()
	{
		if (m_bStarted == true)
		{
			return true;
		}
		std::vector<std::string> ipList = OnvifDisClient::GetInterfaces();
		if (ipList.size() <= 0)
		{
			printf("No network adaptor found !!!\n");
			return false;
		}
		std::vector<std::string>::iterator it;
		for (it = ipList.begin(); it != ipList.end(); ++it)
		{
			printf("Network %s\n", (*it).c_str());

			m_map[*it] = new OnvifDisClient(*it, *this);
			m_map[*it]->Start();
		}
		
		return true;
	}
	bool Stop()
	{
		if (m_bStarted == false)
		{
			return true;
		}
		OnvifDisClientMap::iterator it = m_map.begin(); 
		for(; it!=m_map.end(); ++it)
		{
			delete (*it).second;
		}
		m_map.clear();
		
		return true;
	}
		
private:
	bool m_bStarted;
	std::mutex m_lock;
	CamSearchNotify &m_pNotify;
	OnvifDisClientMap m_map;
};


#endif /* __ONVIF_DIS_CLIENT__ */
