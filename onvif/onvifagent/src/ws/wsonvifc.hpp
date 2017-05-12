
#ifndef __WS_ONVIF_C_H__
#define __WS_ONVIF_C_H__
#include <string.h>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include "wsclient.hpp"
#include "onvifcli.pb.h"
#include "onvifclidevice.pb.h"
#include "onvifclient.hpp"
#include "ws/onvifchandler.hpp"

#include "CivetServer.h"


class WSOnvifC : public CivetWebSocketHandler
{
public:
	WSOnvifC()
	{

	}
	bool handleConnection(CivetServer *server,
	                              const struct mg_connection *conn)
	{	
		return true;
	}
								 
	bool handleData(CivetServer *server,
	                        struct mg_connection *conn,
	                        int bits,
	                        char *data,
	                        size_t data_len)
	{
		printf("%s %s %d %d\n", __FUNCTION__, __FILE__, __LINE__, data_len);
		std::string strMsg(data, data_len);
		
		OnvifCHandlerMap::iterator it = m_map.begin(); 
		for(; it!=m_map.end(); ++it)
		{
			if ((*it).first == conn)
			{
				/* Delete the handler firstly */
				if (m_map[(void *)conn])
				{
					OnvifCHandler * pHandler = (OnvifCHandler *)m_map[(void *)conn];
					pHandler->ProcessMsg(strMsg, server, conn);
				}
			}
		}
		return true;
	}
	void handleClose(CivetServer *server,
	                         const struct mg_connection *conn)
	{
		 printf("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
		OnvifCHandlerMap::iterator it = m_map.begin(); 
		for(; it!=m_map.end(); ++it)
		{
			if ((*it).first == conn)
			{
				/* Delete the handler firstly */
				if (m_map[(void *)conn])
				{
					OnvifCHandler * pHandler = (OnvifCHandler *)m_map[(void *)conn];
					delete pHandler;
					m_map.erase((void *)conn);
					break;
				}
			}
		}
		 return;
	}
	void handleReadyState(CivetServer *server,
	                              struct mg_connection *conn)
	{
		printf("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);	
		OnvifCHandlerMap::iterator it = m_map.begin(); 
		for(; it!=m_map.end(); ++it)
		{
			if ((*it).first == conn)
			{
				/* Delete the handler firstly */
				if (m_map[(void *)conn])
				{
					OnvifCHandler * pHandler = (OnvifCHandler *)m_map[(void *)conn];
					delete pHandler;
					m_map.erase((void *)conn);
					break;
				}
			}
		}

		m_map[(void *)conn] = new OnvifCHandler;
	}								  
private:
	OnvifCHandlerMap m_map;
};


#endif /* __VE_WEB_SERVER_H__ */
