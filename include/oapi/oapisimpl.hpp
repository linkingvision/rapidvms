//------------------------------------------------------------------------------
// File: oapiimpl.hpp
//
// Desc: OpenCVR API - OpenCVR API.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPIS_IMPL_H_
#define __VSC_OAPIS_IMPL_H_

BOOL OAPIConverter::Converter(VidCamera &from, oapi::OAPICamera &to)
{
	to.strId = from.strid();
	to.strName = from.strname();
	to.nType = from.ntype();
	
	to.strIP = from.strip();
	to.strPort = from.strport();
	to.strUser = from.struser();
	to.strPasswd = from.strpasswd();
	
	to.strONVIFAddress = from.stronvifaddress();
	to.bProfileToken = from.bprofiletoken();
	to.strProfileToken1 = from.strprofiletoken1();
	to.strProfileToken2 = from.strprofiletoken2();

	
	to.strFile = from.strfile();
	to.strRTSPUrl = from.strrtspurl();

	to.bHWaccel = from.bhwaccel();
	to.bServerMotion = from.bservermotion();

	to.nConnectType = from.nconnecttype();
	
	return TRUE;
}

BOOL OAPIConverter::Converter(oapi::OAPICamera &from, VidCamera &to)
{
	return TRUE;
}

OAPIServer::OAPIServer(XRef<XSocket> pSocket, Factory &pFactory)
:m_pFactory(pFactory), m_pSocket(pSocket), m_cnt(0), 
m_bLogin(FALSE)
{
	UUIDGenerator uuidCreator;
	
	m_seesionId  = uuidCreator.createRandom().toString();
}
OAPIServer::~OAPIServer()
{
	//if (m_nLiveviewId > 0)
	{
		//m_pFactory.UnRegDataCallback(m_nLiveviewId, (void *)this);
	}
}

BOOL OAPIServer::ProcessStartLive(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::StartLiveViewReq liveview;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, liveview, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}

	m_pFactory.RegDataCallback(liveview.strId,
		(CameraDataCallbackFunctionPtr)OAPIServer::DataHandler, 
			(void *)this);
	m_strLiveviewId = liveview.strId;

	delete [] pRecv;

	return TRUE;
}

BOOL OAPIServer::ProcessStopLive(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::StopLiveViewReq liveview;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, liveview, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}

	m_pFactory.UnRegDataCallback(liveview.strId, (void *)this);
	m_strLiveviewId = "";

	delete [] pRecv;

	return TRUE;
}

inline BOOL OAPIServer::ProcessLogin(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::LoginReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}
	delete [] pRecv;
	pRecv = NULL;

	/* current only support admin */
	std::string UserAdmin = "admin";

	if (req.User != UserAdmin)
	{
		return FALSE;
	}
#if 0
	/* Get user data */
	VSCUserData user;
	m_pFactory.GetUserData(user);
	astring realPasswd = user.data.conf.Passwd;

	/* calc the md5 and compare */
	std::string pass = m_seesionId + realPasswd;

	XMD5 md5Check;
	md5Check.Update((const uint8_t*)(pass.c_str()), pass.length());

	md5Check.Finalize();
	std::string md5Output = md5Check.GetAsString().c_str();
	

	oapi::LoginRsp rsp;

	if (md5Output == req.Password)
	{
		rsp.bRet = true;
		m_bLogin = TRUE;
	}else
	{
		rsp.bRetNonce = true;
		rsp.bRet = false;
	}

	rsp.Nonce = m_seesionId;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(rsp);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}

	OAPIHeader header;
	header.cmd = htonl(OAPI_CMD_LOGIN_RSP);
	header.length = htonl(nJsonLen + 1);

	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
#endif
	return TRUE;
	
}

BOOL OAPIServer::ProcessGetDevice(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPICameraListReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}

	oapi::OAPICameraListRsp dataList;
	dataList.Num = 0;

	CameraOnlineMap pCameraOnlineMap;

	m_pFactory.GetCameraOnlineMap(pCameraOnlineMap);

	VidCameraList pCameraList;
	m_pFactory.GetCameraList(pCameraList);

	for (s32 i = 0; i < pCameraList.cvidcamera_size(); i ++)
	{
		const VidCamera &cam = pCameraList.cvidcamera(i);
		oapi::OAPICamera data;
		OAPIConverter::Converter((VidCamera &)cam, data);
		data.bOnline = pCameraOnlineMap[cam.strid()];
		dataList.list.push_back(data);
		dataList.Num ++;
	}

	//autojsoncxx::to_json_string

	std::string strJson = autojsoncxx::to_pretty_json_string(dataList);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	OAPIHeader header;
	header.cmd = htonl(OAPI_CMD_DEVICE_LIST_RSP);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

BOOL OAPIServer::Process(OAPIHeader &header)
{
	header.version = ntohl(header.version);
	header.cmd = ntohl(header.cmd);
	header.length = ntohl(header.length);
	if (m_bLogin == FALSE && header.cmd != OAPI_CMD_LOGIN_REQ)
	{
		//return FALSE;
	}

	switch(header.cmd)
	{
		case OAPI_CMD_LOGIN_REQ:
			return ProcessLogin(header.length);
			break;
		case OAPI_CMD_KEEPALIVE_REQ:
			break;
		case OAPI_CMD_DEVICE_LIST_REQ:
			return ProcessGetDevice(header.length);
			break;
		case OAPI_CMD_START_LIVE_REQ:
			return ProcessStartLive(header.length);
			break;
		case OAPI_CMD_STOP_LIVE_REQ:
			return ProcessStopLive(header.length);
			break;
		default:
			break;		
	}
	return TRUE;
}

inline void OAPIServer::DataHandler1(VideoFrame& frame)
{
	VideoFrameHeader frameHeader;
	OAPIHeader header;
	header.cmd = htonl(OAPI_CMD_FRAME_PUSH);
	header.length = htonl(sizeof(frameHeader) + frame.dataLen);	

	frameHeader.streamType = htonl(frame.streamType);
	frameHeader.frameType = htonl(frame.frameType);
	frameHeader.secs = htonl(frame.secs);
	frameHeader.msecs = htonl(frame.msecs);
	frameHeader.dataLen = htonl(frame.dataLen);

	printf("Send a new frame %d  length %d\n", m_cnt++, sizeof(frameHeader) + frame.dataLen);
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)&frameHeader, sizeof(frameHeader));
	m_pSocket->Send((void *)frame.dataBuf, frame.dataLen);
	
}

inline void OAPIServer::DataHandler(VideoFrame& frame, void * pParam)
{
    OAPIServer *pOapi = static_cast<OAPIServer *> (pParam);
    
    return pOapi->DataHandler1(frame);
}

#endif
