#ifndef __V_WS_API_H__
#define __V_WS_API_H__

#include "CivetServer.h"
#include "server/factory.hpp"

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

class VwsAPI : public CivetWebSocketHandler
{
public:
	VwsAPI(Factory &pFactory)
		:m_pFactory(pFactory)
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
								VDC_DEBUG("%s %s %d %d\n", __FUNCTION__, __FILE__, __LINE__, data_len);
								mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, "data1", 5);
								return true;
							}
	void handleClose(CivetServer *server,
	                         const struct mg_connection *conn)
							 {
								 VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
								 return;
							 }
	void handleReadyState(CivetServer *server,
	                              struct mg_connection *conn)
	{
		VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);			  
	}								  
private:
	Factory &m_pFactory;
};


#endif /* __VE_WEB_SERVER_H__ */
