#include "link/LinkStream.hpp"
#include "config/vidconf.pb.h"
#include <QtCore/QStorageInfo>
#include <QtCore/QFileInfoList>
#include <QtCore/QDir>
#include "devicesearcher.h"
#include <QHostAddress>
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "pbwrapper.hpp"
#include <QTimer>

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

LinkStream::LinkStream(Factory &pFactory)
:m_pFactory(pFactory), m_bLogin(false),  m_server(NULL), m_bStreaming(false), m_nStream(-1),
m_conn(NULL), m_bPlayback(false), m_pSendBuf(NULL), m_nSendBufSize(0)
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
	QString strDePasswd = m_pFactory.GetAdminPasswd(realPasswd);

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

	if (m_bPlayback)
	{
		//Stop playback
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
		u8 *m_pSendBuf = new unsigned char[frame.dataLen + sizeof(VideoFrameHeader) + 1];
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
	printf("%s %d websocket send %d\n", __FILE__, __LINE__, ret);	
	
}

void LinkStream::DataHandler(VideoFrame& frame, void * pParam)
{
    LinkStream *pObj = static_cast<LinkStream *> (pParam);
    
    return pObj->DataHandler1(frame);
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
	printf("%s %d websocket send %d\n", __FILE__, __LINE__, ret);
	if (ret == strMsg.length())
	{
		return true;
	}
	return false;
}

