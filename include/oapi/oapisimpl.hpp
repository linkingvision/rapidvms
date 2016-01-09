//------------------------------------------------------------------------------
// File: oapiimpl.hpp
//
// Desc: OpenCVR API - OpenCVR API.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPIS_IMPL_H_
#define __VSC_OAPIS_IMPL_H_

bool OAPIConverter::Converter(VidCamera &from, oapi::OAPICamera &to)
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

bool OAPIConverter::Converter(oapi::OAPICamera &from, VidCamera &to)
{
	to.set_strid(from.strId);
	to.set_strname(from.strName);
	to.set_ntype((VidConf::CameraType)from.nType);

	to.set_strip(from.strIP);
	to.set_strport(from.strPort);
	to.set_struser(from.strUser);
	to.set_strpasswd(from.strPasswd);

	to.set_stronvifaddress(from.strONVIFAddress);
	to.set_bprofiletoken(from.bProfileToken);
	to.set_strprofiletoken1(from.strProfileToken1);
	to.set_strprofiletoken2(from.strProfileToken2);

	to.set_strfile(from.strFile);
	to.set_strrtspurl(from.strRTSPUrl);

	to.set_bhwaccel(from.bHWaccel);
	to.set_bservermotion(from.bServerMotion);

	to.set_nconnecttype((VidConf::CameraConnectType)from.nConnectType);
	
	return TRUE;
}

bool OAPIConverter::Converter(oapi::OAPIDisk &from, VidDisk &to)
{
	to.set_strid(from.strId);
	to.set_strpath(from.strPath);
	to.set_ntotalsize(from.nTotalSize);
	to.set_nfreesize(from.nFreeSize);
	to.set_nstorsize(from.nStorSize);

	return true;
}
bool OAPIConverter::Converter(VidDisk &from, oapi::OAPIDisk &to)
{
	to.strId = from.strid();
	to.strPath = from.strpath();
	to.nTotalSize = from.ntotalsize();
	to.nFreeSize = from.nfreesize();
	to.nStorSize = from.nstorsize();
	
	return true;
}

OAPIServer::OAPIServer(XRef<XSocket> pSocket, Factory &pFactory)
:m_pFactory(pFactory), m_pSocket(pSocket), m_cnt(0), 
m_bLogin(FALSE), m_bStreaming(false), m_bRegNotify(false)
{
	UUIDGenerator uuidCreator;
	
	m_seesionId  = uuidCreator.createRandom().toString();
}

bool OAPIServer::CallChange(void* pParam, FactoryCameraChangeData data)
{
    int dummy = errno;
    OAPIServer * pObject = (OAPIServer * )pParam;

    if (pObject)
    {
        return pObject->CallChange1(data);
    }
}
bool OAPIServer::CallChange1(FactoryCameraChangeData data)
{
	/* Streaming channel do not call the camera change */
	if (m_bStreaming == true)
	{
		return true;
	}

	switch(data.type)
	{
		case FACTORY_CAMERA_ADD:
			return NotifyCamAdd(data);
			break;
		case FACTORY_CAMERA_DEL:
			return NotifyCamDel(data);
			break;
		case FACTORY_CAMERA_ONLINE:
			return NotifyCamOnline(data);
			break;
		case FACTORY_CAMERA_OFFLINE:
			return NotifyCamOffline(data);
			break;
		case FACTORY_CAMERA_REC_ON:
			return NotifyCamRecOn(data);
			break;
		case FACTORY_CAMERA_REC_OFF:
			return NotifyCamRecOff(data);
			break;
		default:
			break;		
	}
}

bool OAPIServer::NotifyCamAdd(FactoryCameraChangeData data)
{
	OAPIHeader header;
	oapi::OAPICamAddNotify sCam;

	OAPIConverter::Converter(data.cCam, sCam.cam);

	
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sCam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_NOTIFY_DEVICE_ADD);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	
	return true;
}
bool OAPIServer::NotifyCamDel(FactoryCameraChangeData data)
{
	OAPIHeader header;
	oapi::OAPICamNotifyId sCam;

	sCam.strId = data.id;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sCam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_NOTIFY_DEVICE_DEL);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	
	return true;
}
bool OAPIServer::NotifyCamOnline(FactoryCameraChangeData data)
{
	OAPIHeader header;
	oapi::OAPICamNotifyId sCam;

	sCam.strId = data.id;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sCam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_NOTIFY_DEVICE_ONLINE);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	
	return true;
}
bool OAPIServer::NotifyCamOffline(FactoryCameraChangeData data)
{
	OAPIHeader header;
	oapi::OAPICamNotifyId sCam;

	sCam.strId = data.id;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sCam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_NOTIFY_DEVICE_OFFLINE);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	
	return true;
}
bool OAPIServer::NotifyCamRecOn(FactoryCameraChangeData data)
{
	return true;
}
bool OAPIServer::NotifyCamRecOff(FactoryCameraChangeData data)
{
	return true;
}

OAPIServer::~OAPIServer()
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

	if (m_bRegNotify)
	{
		//TODO
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
	oapi::OAPIStartLiveViewReq liveview;
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

	m_strLiveviewId = liveview.strId;
	m_nStream = liveview.nStream;

	if (liveview.nStream == 1)
	{
		m_pFactory.RegDataCallback(liveview.strId,
		(CameraDataCallbackFunctionPtr)OAPIServer::DataHandler, 
			(void *)this);
	}
	else if (liveview.nStream == 2)
	{
		m_pFactory.RegSubDataCallback(liveview.strId,
		(CameraDataCallbackFunctionPtr)OAPIServer::DataHandler, 
			(void *)this);
	}else 
	{
		m_pFactory.RegDataCallback(liveview.strId,
		(CameraDataCallbackFunctionPtr)OAPIServer::DataHandler, 
			(void *)this);
	}

	m_bStreaming = true;

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
	oapi::OAPIStopLiveViewReq liveview;
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
	/* Just use the id and stream when started */
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

	m_bStreaming = false;

	delete [] pRecv;

	return TRUE;
}

bool OAPIServer::ProcessAddCam(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIAddCameraReq cam;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, cam, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	VidCamera addCam;

	OAPIConverter::Converter(cam.cam, addCam);

	CameraParam addParam(addCam);

	m_pFactory.AddCamera(addParam);

	delete [] pRecv;

	SendCmnRetRsp(OAPI_CMD_ADD_DEVICE_RSP, true);

	return TRUE;
}

bool OAPIServer::ProcessDeleteCam(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIDeleteCameraReq cam;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, cam, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	m_pFactory.DelCamera(cam.strId);

	delete [] pRecv;

	SendCmnRetRsp(OAPI_CMD_DEL_DEVICE_RSP, true);

	return TRUE;
}

bool OAPIServer::ProcessRegNotify(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIRegNotifyReq notify;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, cam, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}


	delete [] pRecv;
	m_pFactory.RegCameraChangeNotify(this,
		(FactoryCameraChangeNotify)(OAPIServer::CallChange));
	m_bRegNotify = true;
	SendCmnRetRsp(OAPI_REG_NOTIFY_RSP, true);

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
#else
	astring realPasswd = "admin";

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
		case OAPI_CMD_ADD_DEVICE_REQ:
			return ProcessAddCam(header.length);
			break;
		case OAPI_CMD_DEL_DEVICE_REQ:
			return ProcessDeleteCam(header.length);
			break;
		case OAPI_REG_NOTIFY_REQ:
			return ProcessRegNotify(header.length);
			break;
		default:
			break;		
	}
	return TRUE;
}

inline bool OAPIServer::SendCmnRetRsp(OAPICmd nCmd, 
				bool bRet)
{
	oapi::CmnRetRsp RetRsp;
	RetRsp.bRet = bRet;
	std::string strJson = autojsoncxx::to_pretty_json_string(RetRsp);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	OAPIHeader header;
	header.cmd = htonl(nCmd);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
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
