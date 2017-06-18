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
#include "pbwrapper.hpp"

#include "CivetServer.h"

using namespace Poco;

class LinkPlaybackInterface
{
public:
	virtual bool NewFrame(VideoFrame& frame){return false;}
};

class LinkServerPlayback
{
public:
	LinkServerPlayback(LinkPlaybackInterface &pPbInf, 
		Factory &pFactory, astring strId, u32 nPlaytime);
	~LinkServerPlayback();

public:
	void DataHandler1(VideoFrame& frame)
	{
		XGuard guard(m_cMutex);
		if (m_bQuit == false)
		{
			m_pPbInf.NewFrame(frame);
		}
	}

	static void DataHandler(VideoFrame& frame, void * pParam)
	{
	    LinkServerPlayback *pObj = static_cast<LinkServerPlayback *> (pParam);
	    
	    return pObj->DataHandler1(frame);
	}
public:
	void run1();
	static void run(void* pData);
	bool SeekToTime(u32 seekTime);
	bool StartPlay();
	bool PausePlay();
	bool QuitPlay();/* End of play */

private:
	Factory &m_pFactory;
	LinkPlaybackInterface &m_pPbInf;
	astring m_strId;
	u32 m_nPlaytime;
	PlaybackWrapper m_pbWrapper;
	XMutex m_cMutex;
	bool m_bQuit;
	std::thread * m_pThread;
};

class LinkStream : public LinkPlaybackInterface
{
public:
	LinkStream(Factory &pFactory);
	~LinkStream();

public:	
	bool ProcessMsg(std::string &strMsg, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessLoginReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);


	/* Live view command  */
	bool ProcessStartLiveReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessStopLiveReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);

	/* Playback command */
	bool ProcessPlayBackReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessPlayPauseReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessPlayResumeReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessPlaySeekReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessPlayStopReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	
	bool SendRespMsg(Link::LinkCmd &resp, CivetServer *server,
	                        struct mg_connection *conn);

public:

	void DataHandler1(VideoFrame& frame);
	static void DataHandler(VideoFrame& frame, void * pParam);
	virtual bool NewFrame(VideoFrame& frame);

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
	LinkServerPlayback *m_pPlayback;
};


typedef std::map<void *, LinkStream *> LinkStreamMap;


#endif /* __LINK_HANDLER_H__ */
