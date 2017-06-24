
#ifndef __WS_LINK_H__
#define __WS_LINK_H__
#include <string.h>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include "link/linkhandler.hpp"
#include "link/linkstream.hpp"
#include "server/factory.hpp"

#include "CivetServer.h"


class WSLinkAuth : public CivetAuthHandler
{
public:
	virtual bool authorize(CivetServer *server, struct mg_connection *conn)
	{
		return true;
	}
};


class WSLink : public CivetWebSocketHandler
{
public:
	WSLink(Factory &pFactory, VEventServer &pEvent)
	:m_pFactory(pFactory),m_pEvent(pEvent)
	{

	}
	bool handleConnection(CivetServer *server,
	                              const struct mg_connection *conn)
	{	
		R_LOG(logRINFO,"%s %s %d \n",__FUNCTION__, __FILE__, __LINE__);
		return true;
	}
								 
	bool handleData(CivetServer *server,
	                        struct mg_connection *conn,
	                        int bits,
	                        char *data,
	                        size_t data_len)
	{
		R_LOG(logRINFO,"%s %s %d %d\n",__FUNCTION__, __FILE__, __LINE__, data_len);
		std::string strMsg(data, data_len);
		
		LinkHandlerMap::iterator it = m_map.begin(); 
		for(; it!=m_map.end(); ++it)
		{
			if ((*it).first == conn)
			{
				/* Delete the handler firstly */
				if (m_map[(void *)conn])
				{
					LinkHandler * pHandler = (LinkHandler *)m_map[(void *)conn];
					pHandler->ProcessMsg(strMsg, server, conn);
				}
			}
		}
		return true;
	}
	void handleClose(CivetServer *server,
	                         const struct mg_connection *conn)
	{
		R_LOG(logRINFO,"%s %s %d \n",__FUNCTION__, __FILE__, __LINE__);
		LinkHandlerMap::iterator it = m_map.begin(); 
		for(; it!=m_map.end(); ++it)
		{
			if ((*it).first == conn)
			{
				/* Delete the handler firstly */
				if (m_map[(void *)conn])
				{
					LinkHandler * pHandler = (LinkHandler *)m_map[(void *)conn];
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
		R_LOG(logRINFO,"%s %s %d \n",__FUNCTION__, __FILE__, __LINE__);
		LinkHandlerMap::iterator it = m_map.begin(); 
		for(; it!=m_map.end(); ++it)
		{
			if ((*it).first == conn)
			{
				/* Delete the handler firstly */
				if (m_map[(void *)conn])
				{
					LinkHandler * pHandler = (LinkHandler *)m_map[(void *)conn];
					delete pHandler;
					m_map.erase((void *)conn);
					break;
				}
			}
		}

		m_map[(void *)conn] = new LinkHandler(m_pFactory, m_pEvent);
	}								  
private:
	LinkHandlerMap m_map;
	Factory &m_pFactory;
	VEventServer &m_pEvent;
};

class WSLinkStream : public CivetWebSocketHandler
{
public:
	WSLinkStream(Factory &pFactory)
	:m_pFactory(pFactory)
	{

	}
	bool handleConnection(CivetServer *server,
	                              const struct mg_connection *conn)
	{	
		R_LOG(logRINFO,"%s %s %d \n",__FUNCTION__, __FILE__, __LINE__);
		return true;
	}
								 
	bool handleData(CivetServer *server,
	                        struct mg_connection *conn,
	                        int bits,
	                        char *data,
	                        size_t data_len)
	{
		R_LOG(logRINFO,"%s %s %d %d\n",__FUNCTION__, __FILE__, __LINE__, data_len);
		std::string strMsg(data, data_len);
		
		LinkStreamMap::iterator it = m_map.begin(); 
		for(; it!=m_map.end(); ++it)
		{
			if ((*it).first == conn)
			{
				/* Delete the handler firstly */
				if (m_map[(void *)conn])
				{
					LinkStream * pHandler = (LinkStream *)m_map[(void *)conn];
					pHandler->ProcessMsg(strMsg, server, conn);
				}
			}
		}
		return true;
	}
	void handleClose(CivetServer *server,
	                         const struct mg_connection *conn)
	{
		R_LOG(logRINFO,"%s %s %d \n",__FUNCTION__, __FILE__, __LINE__);
		LinkStreamMap::iterator it = m_map.begin();
		for(; it!=m_map.end(); ++it)
		{
			if ((*it).first == conn)
			{
				/* Delete the handler firstly */
				if (m_map[(void *)conn])
				{
					LinkStream * pHandler = (LinkStream *)m_map[(void *)conn];
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
		R_LOG(logRINFO,"%s %s %d \n",__FUNCTION__, __FILE__, __LINE__);
		LinkStreamMap::iterator it = m_map.begin();
		for(; it!=m_map.end(); ++it)
		{
			if ((*it).first == conn)
			{
				/* Delete the handler firstly */
				if (m_map[(void *)conn])
				{
					LinkStream * pHandler = (LinkStream *)m_map[(void *)conn];
					delete pHandler;
					m_map.erase((void *)conn);
					break;
				}
			}
		}

		m_map[(void *)conn] = new LinkStream(m_pFactory);
	}								  
private:
	LinkStreamMap m_map;
	Factory &m_pFactory;
};


#endif /* __WS_LINK_H__ */
