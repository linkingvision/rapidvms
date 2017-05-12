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
#ifndef __ONVIF_SERVER__
#define __ONVIF_SERVER__

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#include <string>
#include <map>
#include <iostream>
#include <list>

using namespace std;

#include "onvifserverdevice.hpp"
#include "onvifserverreceiver.hpp"
#include "onvifserverevent.hpp"
#include "onvifservermedia.hpp"


class OnvifServer
{
public:
    OnvifServer(OnvifServerDevice *pDevice, 
				OnvifServerMedia *pMedia, 
				OnvifServerReceiver  *pReceiver, OnvifServerEvent *pEvent);
    ~OnvifServer();
public:
	bool Run(string host, int port, bool &bQuit);
	
private:
	OnvifServerDevice *m_pDevice;
	OnvifServerReceiver *m_pReceiver;
	OnvifServerEvent *m_pEvent;
	OnvifServerMedia *m_pMedia; 
};

inline OnvifServer::OnvifServer(OnvifServerDevice *pDevice, 
	OnvifServerMedia *pMedia, 
	OnvifServerReceiver *pReceiver, OnvifServerEvent *pEvent)
: m_pDevice(pDevice), m_pMedia(pMedia), m_pReceiver(pReceiver), m_pEvent(pEvent)
{

}

inline OnvifServer::~OnvifServer()
{

}

inline bool OnvifServer::Run(string host, int port, bool &bQuit)
{
	int ret;
	int timeStart = time(NULL);
	int currentTime = 0;	
	soap_set_mode(m_pDevice, SOAP_C_UTFSTRING);
	soap_set_mode(m_pReceiver, SOAP_C_UTFSTRING);
	soap_set_mode(m_pEvent, SOAP_C_UTFSTRING);
	soap_set_mode(m_pMedia, SOAP_C_UTFSTRING);
	m_pDevice->bind_flags = SO_REUSEADDR;
	m_pReceiver->bind_flags = SO_REUSEADDR;
	m_pEvent->bind_flags = SO_REUSEADDR;
	m_pMedia->bind_flags = SO_REUSEADDR;
	
	m_pDevice->bind(host.c_str(), port, 100);
	m_pMedia->bind(host.c_str(), port + 1, 100);
	m_pReceiver->bind(host.c_str(), port + 2, 100);
	m_pEvent->bind(host.c_str(), port + 3, 100);

	while(bQuit)
	{
		if (m_pDevice->accept() != SOAP_INVALID_SOCKET)
		{
			m_pDevice->serve();
		}
		if (m_pMedia->accept() != SOAP_INVALID_SOCKET)
		{
			m_pMedia->serve();
		}
		if (m_pReceiver->accept() != SOAP_INVALID_SOCKET)
		{
			m_pReceiver->serve();
		}
		if (m_pEvent->accept() != SOAP_INVALID_SOCKET)
		{
			m_pEvent->serve();
		}
		
	}
#if 0
    if ((ret = soap_bind(m_pSoap, host.c_str(), port, 100) == SOAP_INVALID_SOCKET ) ) 
	{
        printf("OnvifClientEventNotify::soap_bind Binding on %d port failed", port);
        return 0;
    }

	//Loop to recevie the request
	while(bQuit)
	{
		currentTime = time(NULL);
		//printf("soap_accept accepting\n");
		if( (ret = soap_accept(m_pSoap)) == SOAP_INVALID_SOCKET) {
			printf("OnvifServer soap_accept accepting timeout\n");
			continue;
		}


		OnvifServerDevice device(m_pSoap, m_pDevice->m_strDeviceEndpoint, 
			m_pDevice->m_strMediaEndpoint, m_pDevice->m_strReceiverEndpoint, 
			m_pDevice->m_strEventEndpoint);
		if (device.serve() != SOAP_OK) 
			printf("OnvifServer soap_begin_serve serve %d failed", ret);

		continue;

		if (m_pMedia->serve() != SOAP_OK) {
            printf("OnvifServer soap_begin_serve serve %d failed", ret);
            continue;
        }
	
		std::string input(m_pSoap->msgbuf);

		size_t sep_pos = input.find_first_of(" ");
		size_t val_pos = input.find_first_not_of(" ", sep_pos);
		sep_pos = input.find_first_of(" ", val_pos);
		std::string service_path = input.substr(val_pos, sep_pos - val_pos);    // service_path is the second word in the buffer.

		sep_pos = service_path.find_last_of("/");
		if (sep_pos == 0)       // Special case if there is no "/" in the service_path.
			sep_pos = -1;
		std::string service_name = service_path.substr(sep_pos + 1);
		std::string buf (m_pSoap->buf, m_pSoap->buflen);
		printf("Recevied service %s ========> \n%s\n", service_name.c_str(), buf.c_str());
               if (m_pDevice){
                    ret = m_pDevice->dispatch();
                    printf("m_pDevice %d \n", ret);
                    if (ret == SOAP_OK)
                        continue;
                }
                if (m_pMedia)
                {
                    ret = m_pMedia->dispatch();
                    printf("m_pMedia %d \n", ret);
                    if (ret == SOAP_OK)
                        continue;
                }
                if (m_pReceiver)
                {
                    ret = m_pReceiver->dispatch();
                    printf("m_pReceiver %d \n", ret);
                    if (ret == SOAP_OK)
                        continue;
                }

                if (m_pEvent)
                {
                    ret = m_pEvent->dispatch();
                    printf("m_pEvent %d \n", ret);
                    if (ret == SOAP_OK)
                        continue;
                }


	}
#endif
	return 0;
}


#endif