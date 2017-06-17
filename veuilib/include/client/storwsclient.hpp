
#ifndef __STOR_WS_CLIENT__
#define __STOR_WS_CLIENT__

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

#include "config/confdb.hpp"
#include "config/sysdb.hpp"
#include "utility/videotype.hpp"

#include "civetweb.h"
#include "XSDK/XHash.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XSocket.h"
#include "XSDK/XSSLSocket.h"
#include "XSDK/XMD5.h"
#include "config/linkproto.pb.h"
#include "config/linksystem.pb.h"
#include "config/vidconf.pb.h"
#include <google/protobuf/util/json_util.h>

#define WS_EBUF_LEN 1024

class VE_LIBRARY_API StorWebSocketClient
{
public:
	StorWebSocketClient(std::string strHost, std::string strPort, std::string strPath);
	~StorWebSocketClient();
public:
	bool Disconnect();
	virtual bool ProcessRecvMsg(char *data, size_t data_len)
	{
		return true;
	}
	virtual bool ProcessOffline()
	{
		return true;
	}
	virtual bool ProcessOnline()
	{
		return true;
	}
	virtual bool IsKeep()
	{
		return true;
	}
	bool SendDeviceListRequest();

	bool SendRegNotifyRequest();

	bool RegRealEvent();

	bool ProcessLoginResp(Link::LinkCmd &cmd, astring strUser, astring strPasswd);
	
	bool Login(astring strUser, astring strPasswd, astring strNonce);
	
public:
	static int WSDataHandler(struct mg_connection *conn,
                              int flags,
                              char *data,
                              size_t data_len,
                              void *user_data);
	/* handle the data */
	int WSDataHandler1(struct mg_connection *conn,
                              int flags,
                              char *data,
                              size_t data_len);

	static void WSCloseHandler(struct mg_connection *conn, void *user_data);
	/* handle close */
	void WSCloseHandler1(struct mg_connection *conn);
	bool Connect();
	bool SendMsg(std::string &strMsg);
	
	bool Connected();
	static bool Thread(void* pData);
	bool ThreadRun();

private:
	struct mg_connection *m_wsConn;
	bool m_bConnected;
	std::string m_strHost;
	std::string m_strPort;
	std::string m_strPath;
	char m_ebuf[WS_EBUF_LEN];
	bool m_bExit;
protected:
	std::mutex m_lock;
	long long m_msgId;
	bool m_bOnline;
	std::thread * m_pThread;
	std::mutex m_ConnectLock;
	
};


#endif /* __STOR_WS_CLIENT__ */
