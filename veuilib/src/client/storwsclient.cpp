#include "client/storwsclient.hpp"

StorWebSocketClient::StorWebSocketClient(std::string strHost, std::string strPort, std::string strPath)
	:m_wsConn(NULL), m_bConnected(false), m_strHost(strHost), m_strPort(strPort),
m_strPath(strPath), m_msgId(0), m_bOnline(false), m_pThread(NULL), m_bExit(false), 
m_bLogined(false)
{
	memset(m_ebuf, 0, WS_EBUF_LEN);

}
StorWebSocketClient::~StorWebSocketClient()
{
	Disconnect();
}

bool StorWebSocketClient::StartKeepThread()
{
	/* start keepalive thread */
	m_pThread = new std::thread(StorWebSocketClient::Thread, this);

	return true;
}

bool StorWebSocketClient::Disconnect()
{
	R_LOG(logRINFO,"%s %s %d\n",__FUNCTION__, __FILE__, __LINE__);
	if (m_bExit == false)
	{
		m_bExit = true;
		if (m_pThread)
		{
			m_pThread->join();
			delete m_pThread;
			m_pThread = NULL;
		}
		if (m_wsConn)
		{
			mg_close_connection(m_wsConn);
			m_wsConn = NULL;
		}
	}
	R_LOG(logRINFO,"%s %s %d\n",__FUNCTION__, __FILE__, __LINE__);

	return true;
}

bool StorWebSocketClient::SendDeviceListRequest()
{
	if (Connected() == false)
	{
		if (Connect() == false)
		{
			return false;
		}
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_CAM_LIST_REQ);
	LinkListCamReq * req = new LinkListCamReq;
	req->set_ball(true);
	
	cmd.set_allocated_camlistreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}

	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	return true;
}

bool StorWebSocketClient::SendRegNotifyRequest()
{
	if (Connected() == false)
	{
		if (Connect() == false)
		{
			return false;
		}
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_REG_NOTIFY_REQ);
	LinkRegNotifyReq * req = new LinkRegNotifyReq;
	
	cmd.set_allocated_regnotifyreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}

	if (SendMsg(strMsg) == false)
	{
		return false;
	}
	return true;
}

bool StorWebSocketClient::RegRealEvent()
{
	return true;
}
bool StorWebSocketClient::UnRegRealEvent()
{
	return true;
}

bool StorWebSocketClient::ProcessLoginResp(Link::LinkCmd &cmd, astring strUser, astring strPasswd)
{
	if (!cmd.has_loginresp())
	{
		return false;
	}
	const LinkLoginResp& pResp =  cmd.loginresp();

	/* Password error need relogin */
	if (pResp.bretnonce() == true)
	{
		//ve_sleep(300);
		return Login(strUser, strPasswd, pResp.strnonce());
	}

	if (pResp.bret() == true)
	{
		m_bLogined = true;
		ProcessLogined();
	}

	if (pResp.bret() == true && IsKeep() == true)
	{
		SendDeviceListRequest();
		SendRegNotifyRequest();
		RegRealEvent();
		
	}
	
	return true;
}

bool StorWebSocketClient::Login(astring strUser, astring strPasswd, astring strNonce)
{
	if (Connected() == false)
	{
		if (Connect() == false)
		{
			return false;
		}
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_LOGIN_REQ);
	LinkLoginReq * req = new LinkLoginReq;
	req->set_strusername(strUser);

	std::string pass = strNonce + strPasswd;

	XMD5 md5Check;
	md5Check.Update((const uint8_t*)(pass.c_str()), pass.length());

	md5Check.Finalize();
	std::string md5Output = md5Check.GetAsString().c_str();

	req->set_strpasswd(md5Output);
	
	cmd.set_allocated_loginreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}
	return true;
	
}

int StorWebSocketClient::WSDataHandler(struct mg_connection *conn,
                          int flags,
                          char *data,
                          size_t data_len,
                          void *user_data)
{	
	StorWebSocketClient* pWS = (StorWebSocketClient*)user_data;

	if (pWS)
	{
	    return pWS->WSDataHandler1(conn, flags, data, data_len);
	}
	return true;
}
/* handle the data */
int StorWebSocketClient::WSDataHandler1(struct mg_connection *conn,
                          int flags,
                          char *data,
                          size_t data_len)

{
	R_LOG(logRINFO,"%s %s %d %d\n",__FUNCTION__, __FILE__, __LINE__, data_len);
	return ProcessRecvMsg(data, data_len);
}

void StorWebSocketClient::WSCloseHandler(struct mg_connection *conn, void *user_data)
{	
	StorWebSocketClient* pWS = (StorWebSocketClient*)user_data;

	if (pWS)
	{
	    return pWS->WSCloseHandler1(conn);
	}
	
	return;
}
/* handle close */
void StorWebSocketClient::WSCloseHandler1(struct mg_connection *conn)

{
	ProcessOffline();
	std::lock_guard<std::mutex> guard(m_ConnectLock);
	m_bConnected = false;
	m_bLogined = false;
	R_LOG(logRINFO,"%s %s %d\n",__FUNCTION__, __FILE__, __LINE__);
	
}
bool StorWebSocketClient::Connect()
{
	std::lock_guard<std::mutex> guard(m_ConnectLock);
	if (m_bConnected == true)
	{
		return m_bConnected;
	}
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
										   (mg_websocket_data_handler)StorWebSocketClient::WSDataHandler,
										   (mg_websocket_close_handler)StorWebSocketClient::WSCloseHandler,
										   this);
	if (m_wsConn != NULL)
	{
		m_bConnected = true;
	}

	return m_bConnected;
}
bool StorWebSocketClient::SendMsg(std::string &strMsg)
{
	std::lock_guard<std::mutex> guard(m_ConnectLock);
	if (m_bConnected == false)
	{
		return false;
	}
	int ret = mg_websocket_client_write(m_wsConn, 
		WEBSOCKET_OPCODE_TEXT, strMsg.c_str(), strMsg.length());
	
	//R_LOG(logRINFO,"%s %s %d websocket send %d\n",__FUNCTION__, __FILE__, 
	//			__LINE__, ret);
	if (ret == strMsg.length())
	{
		return true;
	}
	return false;
}

bool StorWebSocketClient::Connected()
{
	std::lock_guard<std::mutex> guard(m_ConnectLock);
	return m_bConnected;
}
bool StorWebSocketClient::Thread(void* pData)
{
	StorWebSocketClient * pThread = (StorWebSocketClient *)pData;

	if (pThread)
	{
		return pThread->ThreadRun();
	}
	return false;		
}
bool StorWebSocketClient::ThreadRun()
{
	while(m_bExit == false)
	{
		
		/* loop to send keepalive & reconnect */
		{
			if (Connected() == false)
			{
				if (Connect() == true)
				{
					ProcessOnline();
				}
			}
			Link::LinkCmd cmd;
			cmd.set_type(Link::LINK_CMD_KEEPALIVE_REQ);
			LinkKeepaliveReq * req = new LinkKeepaliveReq;
			req->set_nkeepalive(100);
			cmd.set_allocated_keepalivereq(req);
			std::string strMsg;
			::google::protobuf::util::Status status = 
				::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
			if (!status.ok())
			{
				continue;
			}

			SendMsg(strMsg);
		}
		int i = 40;
		while(m_bExit == false && i > 0)
		{
			i --;
			ve_sleep(50);
		}
		if (NeedReconnect() == true)
		{
			//std::lock_guard<std::mutex> guard(m_ConnectLock);
			if (m_bConnected == true)
			{
				if (m_wsConn)
				{
					mg_close_connection(m_wsConn);
					m_wsConn = NULL;
				}
				m_bConnected = false;
			}

		}
	}

	return true;

}

