#ifndef __LINK_STREAM_H__
#define __LINK_STREAM_H__
#include <string.h>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include <google/protobuf/util/json_util.h>
#include "utility/type.hpp"

#include "config/linkproto.pb.h"
#include "config/linksystem.pb.h"
#include "config/vidconf.pb.h"
#include "simplecrypt.hpp"
#include "server/factory.hpp"
#include "XSDK/XHash.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XSocket.h"
#include "XSDK/XSSLSocket.h"
#include "XSDK/XMD5.h"

#include "CivetServer.h"

using namespace Poco;


class LinkStream
{
public:
	LinkStream(Factory &pFactory);
	~LinkStream();

public:	
	bool ProcessMsg(std::string &strMsg, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessLoginReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessStartLiveReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);

	
	bool SendRespMsg(Link::LinkCmd &resp, CivetServer *server,
	                        struct mg_connection *conn);

public:

	void DataHandler1(VideoFrame& frame);
	static void DataHandler(VideoFrame& frame, void * pParam);

	bool StopAll();
	 
							  
private:
	Factory &m_pFactory;
	astring m_seesionId;
	bool m_bLogin;
	CivetServer *m_server;
	struct mg_connection *m_conn;
	bool m_bStreaming;
	astring m_strLiveviewId;
	unsigned int m_nStream;
	bool m_bPlayback;
	u8 * m_pSendBuf;
	s32 m_nSendBufSize;
};


typedef std::map<void *, LinkStream *> LinkStreamMap;


#endif /* __LINK_HANDLER_H__ */
