#include "link/linkstream.hpp"
#include "config/vidconf.pb.h"
#include <QtCore/QStorageInfo>
#include <QtCore/QFileInfoList>
#include <QtCore/QDir>
#include <QHostAddress>
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "pbwrapper.hpp"
#include <QTimer>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>

/*
Loop list for protobuf
	VidCameraList cameraList;
	GetCameraListConf(cameraList);
	int cameraSize = cameraList.cvidcamera_size();

	for (s32 i = 0; i < cameraList.cvidcamera_size(); i ++)
	{
		const VidCamera &cam = cameraList.cvidcamera(i);
		if (cam.strid() == strCameraId)
		{
			pCam = cam;
			return true;
		}
	}

*/

/*
Add for protobuf
	VidCamera *pCam = cameraListNew.add_cvidcamera();

*/

LinkServerPlayback::LinkServerPlayback(LinkPlaybackInterface &pPbInf, 
	Factory &pFactory, astring strId, u32 nPlaytime)
:m_pFactory(pFactory), m_pPbInf(pPbInf), m_strId(strId), m_nPlaytime(nPlaytime), 
m_pbWrapper(pFactory.GetVdb(), strId, nPlaytime, 
(PBCallbackFunctionPtr)(LinkServerPlayback::DataHandler), 
false, this), m_bQuit(false), m_pThread(NULL)
{
	m_pThread = new std::thread(LinkServerPlayback::run, this);
}
LinkServerPlayback::~LinkServerPlayback()
{
	if (m_pThread != NULL)
	{
		QuitPlay();
	}
}

void LinkServerPlayback::run1()
{
	m_pbWrapper.run();
	while(m_bQuit != true)
	{
		ve_sleep(300);
	}
}

void LinkServerPlayback::run(void* pData)
{
	LinkServerPlayback * pThread = (LinkServerPlayback *)pData;

	if (pThread)
	{
		return pThread->run1();
	}

	return;		
}

bool LinkServerPlayback::SeekToTime(u32 seekTime)
{
	m_pbWrapper.SeekToTime(seekTime);
	return true;
}
bool LinkServerPlayback::StartPlay()
{
	m_pbWrapper.StartPlay();
	return true;
}
bool LinkServerPlayback::PausePlay()
{
	m_pbWrapper.PausePlay();
	return true;
}
bool LinkServerPlayback::QuitPlay()
{
	XGuard guard(m_cMutex);
	m_bQuit = true;
	m_pbWrapper.QuitPlay();
	m_pThread->join();
	delete m_pThread;
	m_pThread = NULL;
	
	return true;
}

LinkStream::LinkStream(Factory &pFactory)
:m_pFactory(pFactory), m_bLogin(false),  m_server(NULL), m_bStreaming(false), m_nStream(-1),
m_conn(NULL), m_bPlayback(false), m_pSendBuf(NULL), m_nSendBufSize(0), m_pPlayback(NULL)
{
	UUIDGenerator uuidCreator;
	
	m_seesionId  = uuidCreator.createRandom().toString();
}
LinkStream::~LinkStream()
{
	/* stop all the stream */
	StopAll();
}

bool LinkStream::ProcessLoginReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_loginreq())
	{
		return false;
	}
	
	
	const LinkLoginReq& pLogin =  req.loginreq();

	cmdResp.set_type(Link::LINK_CMD_LOGIN_RESP);
	LinkLoginResp * resp = new LinkLoginResp;

	astring realPasswd = "admin";

	SimpleCrypt crypt;
	// here has error QString strDePasswd = m_pFactory.GetAdminPasswd(realPasswd);
	bool bRet = m_pFactory.GetAdminPasswd(realPasswd);

	//realPasswd = crypt.decryptToString(strDePasswd).toStdString();
	realPasswd = "admin";

	/* calc the md5 and compare */
	std::string pass = m_seesionId + realPasswd;

	XMD5 md5Check;
	md5Check.Update((const uint8_t*)(pass.c_str()), pass.length());

	md5Check.Finalize();
	std::string md5Output = md5Check.GetAsString().c_str();
	

	if (md5Output == pLogin.strpasswd())
	{
		resp->set_bret(true);
		m_bLogin = true;
		resp->set_bretnonce(false);
	}else
	{
		resp->set_strnonce(m_seesionId);
		resp->set_bretnonce(true);
	}

	cmdResp.set_allocated_loginresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkStream::ProcessStartLiveReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_startlivecmd())
	{
		return false;
	}

	m_server = server;
	m_conn = conn;
	
	const LinkStartLiveCmd& pReq =  req.startlivecmd();

	/* Start live */
	m_strLiveviewId = pReq.strid();
	m_nStream = pReq.nstream();

	if (m_nStream == 1)
	{
		m_pFactory.RegDataCallback(pReq.strid(),
		(CameraDataCallbackFunctionPtr)LinkStream::DataHandler, 
			(void *)this);
	}
	else if (m_nStream == 2)
	{
		m_pFactory.RegSubDataCallback(pReq.strid(),
		(CameraDataCallbackFunctionPtr)LinkStream::DataHandler, 
			(void *)this);
	}else 
	{
		m_pFactory.RegDataCallback(pReq.strid(),
		(CameraDataCallbackFunctionPtr)LinkStream::DataHandler, 
			(void *)this);
	}

	m_bStreaming = true;
	m_bPlayback = false;

	return true;
}

bool LinkStream::ProcessStopLiveReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_stoplivecmd())
	{
		return false;
	}
	
	const LinkStopLiveCmd& pReq =  req.stoplivecmd();

	StopAll();

	return true;
}

bool LinkStream::ProcessPlayBackReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_playbackcmd())
	{
		return false;
	}
	
	const LinkPlayBackCmd& pReq =  req.playbackcmd();

	m_server = server;
	m_conn = conn;

	/* Start playback */
	if (m_pPlayback != NULL)
	{
		m_pPlayback->QuitPlay();
		/* the playback thread will delete auto */
		delete m_pPlayback;
		m_pPlayback = NULL;
	}

	m_pPlayback = new LinkServerPlayback(*this, m_pFactory, pReq.strid(), 
						pReq.nplaytime());
	m_pPlayback->StartPlay();
	m_bPlayback = true;

	return true;	
}

bool LinkStream::ProcessPlayPauseReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_playpausecmd())
	{
		return false;
	}
	
	const LinkPlayPauseCmd& pReq =  req.playpausecmd();

	/* Start playback */
	if (m_pPlayback != NULL)
	{
		m_pPlayback->PausePlay();

	}

	return true;	
}
bool LinkStream::ProcessPlayResumeReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_playresumecmd())
	{
		return false;
	}
	
	const LinkPlayResumeCmd& pReq =  req.playresumecmd();

	/* Start playback */
	if (m_pPlayback != NULL)
	{
		m_pPlayback->StartPlay();

	}

	return true;
}

bool LinkStream::ProcessPlaySeekReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_playseekcmd())
	{
		return false;
	}
	
	const LinkPlaySeekCmd& pReq =  req.playseekcmd();

	/* Start playback */
	if (m_pPlayback != NULL)
	{
		m_pPlayback->SeekToTime(pReq.nplaytime());
	}

	return true;
}
bool LinkStream::ProcessPlayStopReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_playstopcmd())
	{
		return false;
	}
	
	const LinkPlayStopCmd& pReq =  req.playstopcmd();

	StopAll();

	return true;
}


bool LinkStream::StopAll()
{
	if (m_bStreaming == true)
	{
		if (m_nStream == 1)
		{
			m_pFactory.UnRegDataCallback(m_strLiveviewId, (void *)this);
		}
		else if (m_nStream == 2)
		{
			m_pFactory.UnRegSubDataCallback(m_strLiveviewId, (void *)this);
		}else 
		{
			m_pFactory.UnRegDataCallback(m_strLiveviewId, (void *)this);
		}

	}

	if (m_bPlayback && m_pPlayback)
	{
		m_pPlayback->QuitPlay();
		/* the playback thread will delete auto */
		delete m_pPlayback;
		m_pPlayback = NULL;
	}

	return true;
}

void LinkStream::DataHandler1(VideoFrame& frame)
{
	if (m_nSendBufSize < (frame.dataLen + sizeof(VideoFrameHeader)))
	{
		if (m_pSendBuf)
		{
			delete m_pSendBuf;
		}
		m_pSendBuf = new unsigned char[frame.dataLen + sizeof(VideoFrameHeader) + 1];
		m_nSendBufSize = frame.dataLen + sizeof(VideoFrameHeader);
	}
	VideoFrameHeader *frameHeader = (VideoFrameHeader *)m_pSendBuf;

	frameHeader->streamType = htonl(frame.streamType);
	frameHeader->frameType = htonl(frame.frameType);
	frameHeader->secs = htonl(frame.secs);
	frameHeader->msecs = htonl(frame.msecs);
	frameHeader->dataLen = htonl(frame.dataLen);

	memcpy(m_pSendBuf + sizeof(VideoFrameHeader), frame.dataBuf, frame.dataLen);

	int ret = mg_websocket_write(m_conn, 
		WEBSOCKET_OPCODE_BINARY, (const char *)m_pSendBuf, frame.dataLen + sizeof(VideoFrameHeader));
	//printf("%s %d websocket send %d\n", __FILE__, __LINE__, ret);	
	
}

void LinkStream::DataHandler(VideoFrame& frame, void * pParam)
{
    LinkStream *pObj = static_cast<LinkStream *> (pParam);
    
    return pObj->DataHandler1(frame);
}

bool LinkStream::NewFrame(VideoFrame& frame)
{
	DataHandler1(frame);
	return true;
}
	                        


bool LinkStream::ProcessMsg(std::string &strMsg, CivetServer *server,
                        struct mg_connection *conn)
{
	Link::LinkCmd cmd;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::JsonStringToMessage(strMsg, &cmd);
	if (!status.ok())
	{
		return false;
	}

	switch (cmd.type())
	{
		case Link::LINK_CMD_LOGIN_REQ:
		{
			return ProcessLoginReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_START_LIVE_CMD:
		{
			return ProcessStartLiveReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_STOP_LIVE_CMD:
		{
			return ProcessStopLiveReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_PLAY_BACK_CMD:
		{
			return ProcessPlayBackReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_PLAY_RESUME_CMD:
		{
			return ProcessPlayResumeReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_PLAY_STOP_CMD:
		{
			return ProcessPlayStopReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_PLAY_SEEK_CMD:
		{
			return ProcessPlaySeekReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_PLAY_PAUSE_CMD:
		{
			return ProcessPlayPauseReq(cmd, server, conn);
			break;
		}
		default:
		   	return false;
	};		
	

}

bool LinkStream::SendRespMsg(Link::LinkCmd &resp, CivetServer *server,
                        struct mg_connection *conn)
{
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(resp, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	int ret = mg_websocket_write(conn, 
		WEBSOCKET_OPCODE_TEXT, strMsg.c_str(), strMsg.length());
	//printf("%s %d websocket send %d\n", __FILE__, __LINE__, ret);
	if (ret == strMsg.length())
	{
		return true;
	}
	return false;
}

