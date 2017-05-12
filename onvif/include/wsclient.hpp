
#ifndef __WS_CLIENT__
#define __WS_CLIENT__

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

#include "civetweb.h"

#define WS_EBUF_LEN 1024

class  WebSocketClient
{
public:
	WebSocketClient(std::string strHost, std::string strPort, std::string strPath)
	:m_wsConn(NULL), m_bConnected(false), m_strHost(strHost), m_strPort(strPort), 
	 m_strPath(strPath), m_msgId(0)
	{
		memset(m_ebuf, 0, WS_EBUF_LEN);
	}
	~WebSocketClient()
	{
		if (m_wsConn)
		{
			mg_close_connection(m_wsConn);
		}
	}
public:
	virtual bool ProcessRecvMsg(char *data, size_t data_len){return true;}
	
public:
	static void WSDataHandler(struct mg_connection *conn,
                              int flags,
                              char *data,
                              size_t data_len,
                              void *user_data)
	{	
		WebSocketClient* pWS = (WebSocketClient*)user_data;

		if (pWS)
		{
		    return pWS->WSDataHandler1(conn, flags, data, data_len);
		}
		return;
	}
	/* handle the data */
	void WSDataHandler1(struct mg_connection *conn,
                              int flags,
                              char *data,
                              size_t data_len)

	{

		printf("%s %d websocket recv %d\n", __FILE__, __LINE__, data_len);
		ProcessRecvMsg(data, data_len);

		return;
	}

	static void WSCloseHandler(struct mg_connection *conn, void *user_data)
	{	
		WebSocketClient* pWS = (WebSocketClient*)user_data;

		if (pWS)
		{
		    return pWS->WSCloseHandler1(conn);
		}
		return;
	}
	/* handle close */
	void WSCloseHandler1(struct mg_connection *conn)

	{
		m_bConnected = false;
		printf("Client: Close handler\n");
	}
	bool Connect()
	{
		if (m_wsConn)
		{
			mg_close_connection(m_wsConn);
			m_wsConn = NULL;
		}
		m_wsConn = mg_connect_websocket_client(m_strHost.c_str(),
											   atoi(m_strPort.c_str()),
											   0,
											   m_ebuf,
											   sizeof(m_ebuf),
											   m_strPath.c_str(),
											   NULL,
											   (mg_websocket_data_handler)WebSocketClient::WSDataHandler,
											   (mg_websocket_close_handler)WebSocketClient::WSCloseHandler,
											   this);
		if (m_wsConn != NULL)
		{
			m_bConnected = true;
		}

		return m_bConnected;
	}
	bool SendMsg(std::string &strMsg)
	{
		int ret = mg_websocket_client_write(m_wsConn, 
			WEBSOCKET_OPCODE_TEXT, strMsg.c_str(), strMsg.length());
		printf("%s %d websocket send %d\n", __FILE__, __LINE__, ret);
		if (ret == strMsg.length())
		{
			return true;
		}
		return false;
	}
	
	bool Connected()
	{
		return m_bConnected;
	}

private:
	struct mg_connection *m_wsConn;
	bool m_bConnected;
	std::string m_strHost;
	std::string m_strPort;
	std::string m_strPath;
	char m_ebuf[WS_EBUF_LEN];
protected:
	std::mutex m_lock;
	long long m_msgId;
	
};


#endif /* __WS_CLIENT__ */
