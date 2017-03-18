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
