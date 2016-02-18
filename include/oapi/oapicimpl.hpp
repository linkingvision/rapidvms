//------------------------------------------------------------------------------
// File: oapicimpl.hpp
//
// Desc: OpenCVR API - OpenCVR API.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPIC_IMPL_H_
#define __VSC_OAPIC_IMPL_H_

OAPIClient::OAPIClient(XRef<XSocket> pSocket)
:m_pSocket(pSocket)
{

}
OAPIClient::~OAPIClient()
{

}


/* Setup connect with server */
bool OAPIClient::Setup(std::string strUser, std::string strPasswd, 
		std::string strNonce)
{
	OAPIHeader header;

	/* First get Nonce */
	oapi::LoginReq req;

	/* calc the md5 and compare */
	std::string pass = strNonce + strPasswd;

	XMD5 md5Check;
	md5Check.Update((const uint8_t*)(pass.c_str()), pass.length());

	md5Check.Finalize();
	std::string md5Output = md5Check.GetAsString().c_str();

	req.User = strUser;
	req.Password = md5Output;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(req);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}

	header.cmd = htonl(OAPI_LOGIN_REQ);
	header.length = htonl(nJsonLen + 1);;

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}

bool OAPIClient::SendDeviceListRequest()
{
	OAPIHeader header;

	oapi::OAPICameraListReq req;
	req.bAll = true;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(req);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}

	header.cmd = htonl(OAPI_CAM_LIST_REQ);
	header.length = htonl(nJsonLen + 1);;

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;

}

inline bool OAPIClient::SendSysDiskListRequest()
{
	OAPIHeader header;

	oapi::OAPISysDiskListReq req;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(req);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}

	header.cmd = htonl(OAPI_SYS_DISK_LIST_REQ);
	header.length = htonl(nJsonLen + 1);;

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

inline bool OAPIClient::SendDiskListRequest()
{
	OAPIHeader header;

	oapi::OAPIDiskListReq req;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(req);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}

	header.cmd = htonl(OAPI_DISK_LIST_REQ);
	header.length = htonl(nJsonLen + 1);;

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

bool OAPIClient::SendRegNotifyRequest()
{
	OAPIHeader header;

	oapi::OAPIRegNotifyReq req;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(req);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}

	header.cmd = htonl(OAPI_REG_NOTIFY_REQ);
	header.length = htonl(nJsonLen + 1);;

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;

}

bool OAPIClient::StartLiveview(astring strId, unsigned int nStream)
{
	OAPIHeader header;

	oapi::OAPIStartLiveViewReq liveview;
	liveview.strId = strId;
	liveview.nStream = nStream;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(liveview);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_START_LIVE_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	return true;

}

bool OAPIClient::AddCam(oapi::OAPIAddCameraReq sCam)
{
	OAPIHeader header;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sCam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_ADD_CAM_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}

bool OAPIClient::SetCamSched(oapi::OAPICameraUpdateSchedReq sCam)
{
	OAPIHeader header;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sCam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_SET_CAM_SCHED_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}

bool OAPIClient::DeleteCam(astring strId)
{
	OAPIHeader header;

	oapi::OAPIDeleteCameraReq cam;
	cam.strId = strId;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(cam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_DEL_CAM_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	return true;
}

bool OAPIClient::GetCam(astring strId)
{
	OAPIHeader header;

	oapi::OAPICameraGetReq cam;
	cam.strId = strId;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(cam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_GET_CAM_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	return true;
}

bool OAPIClient::AddDisk(oapi::OAPIAddDiskReq sDisk)
{
	OAPIHeader header;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sDisk);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_ADD_DISK_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	return true;
}
bool OAPIClient::DelDisk(astring strId)
{
	OAPIHeader header;

	oapi::OAPIDelDiskReq sDisk;
	sDisk.strId = strId;
	std::string strJson = autojsoncxx::to_pretty_json_string(sDisk);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_DEL_DISK_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}

bool OAPIClient::PtzCmd(astring strId, u32 action, double param)
{
	OAPIHeader header;

	oapi::OAPIPtzCmd sCmd;
	sCmd.strId = strId;
	sCmd.nPtzCmd = (u32)action;
	sCmd.nParam = param;
	std::string strJson = autojsoncxx::to_pretty_json_string(sCmd);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_PTZ_CMD);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}


bool OAPIClient::UpdateDiskLimit(astring strId, s64 nLimit)
{
	OAPIHeader header;

	oapi::OAPIUpdateDiskLimitReq sDisk;
	sDisk.disk.strId = strId;
	sDisk.disk.nStorLimit = nLimit;
	std::string strJson = autojsoncxx::to_pretty_json_string(sDisk);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_UPDATE_DISK_LIMIT_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}

bool OAPIClient::ConfAdminPasswd(astring strOld, astring strNew)
{
	OAPIHeader header;

	oapi::OAPIConfAdminReq sReq;
	sReq.strOldPasswd = strOld;
	sReq.strNewPasswd = strNew;

	std::string strJson = autojsoncxx::to_pretty_json_string(sReq);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_CONF_ADMIN_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}

bool OAPIClient::CamSearchStart()
{
	OAPIHeader header;

	oapi::OAPISearchCamStartReq sReq;

	std::string strJson = autojsoncxx::to_pretty_json_string(sReq);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_CAM_SEARCH_START_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}
bool OAPIClient::CamSearchStop()
{
	OAPIHeader header;

	oapi::OAPISearchCamStopReq sReq;

	std::string strJson = autojsoncxx::to_pretty_json_string(sReq);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_CAM_SEARCH_STOP_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}

bool OAPIClient::GetLic()
{
	OAPIHeader header;

	oapi::OAPIGetLicReq sReq;

	std::string strJson = autojsoncxx::to_pretty_json_string(sReq);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_GET_LIC_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}


bool OAPIClient::GetVer()
{
	OAPIHeader header;

	oapi::OAPIGetVerReq sReq;

	std::string strJson = autojsoncxx::to_pretty_json_string(sReq);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_GET_VER_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}

bool OAPIClient::ConfLic(astring strLic)
{
	OAPIHeader header;

	oapi::OAPIConfLicReq sReq;
	sReq.strLic = strLic;

	std::string strJson = autojsoncxx::to_pretty_json_string(sReq);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_CONF_LIC_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}

bool OAPIClient::StopLiveview(astring strId, unsigned int nStream)
{
	OAPIHeader header;

	oapi::OAPIStopLiveViewReq liveview;
	liveview.strId = strId;
	liveview.nStream = nStream;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(liveview);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_STOP_LIVE_REQ);
	header.length = htonl(nJsonLen + 1);
	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	return true;

}
bool OAPIClient::ParseDeviceList(char *pRecv, int len, oapi::OAPICameraListRsp &rsp)
{
	autojsoncxx::ParsingResult result;
	if (!autojsoncxx::from_json_string(pRecv, rsp, result)) 
	{
	    std::cerr << result << '\n';
	    return false;
	}
	return true;
}

bool OAPIClient::ParseDiskList(char *pRecv, int len, oapi::OAPIDiskListRsp &list)
{
	autojsoncxx::ParsingResult result;
	if (!autojsoncxx::from_json_string(pRecv, list, result)) 
	{
	    std::cerr << result << '\n';
	    return false;
	}
	return true;
}

bool OAPIClient::ParseSysDiskList(char *pRecv, int len, oapi::OAPISysDiskListRsp &list)
{
	autojsoncxx::ParsingResult result;
	if (!autojsoncxx::from_json_string(pRecv, list, result)) 
	{
	    std::cerr << result << '\n';
	    return false;
	}
	return true;
}

bool OAPIClient::ParseLic(char *pRecv, int len, astring &strLic, astring &strHostId, 
							int &ch, astring &type, astring &expireTime)
{	
	oapi::OAPIGetLicRsp rsp;
	autojsoncxx::ParsingResult result;
	if (!autojsoncxx::from_json_string(pRecv, rsp, result)) 
	{
	    std::cerr << result << '\n';
	    return false;
	}
	strLic = rsp.strLic;
	strHostId = rsp.strHostId;
	ch = rsp.nCh;
	type = rsp.strType;
	expireTime = rsp.strExpireTime;
	return true;
}

bool OAPIClient::ParseVer(char *pRecv, int len, astring &strVer, astring &strInfo)
{
	oapi::OAPIGetVerRsp rsp;
	autojsoncxx::ParsingResult result;
	if (!autojsoncxx::from_json_string(pRecv, rsp, result)) 
	{
	    std::cerr << result << '\n';
	    return false;
	}

	strVer = rsp.strVer;
	strInfo = rsp.strInfo;
	return true;
}

bool OAPIClient::ParseLogin(char *pRecv, int len, oapi::LoginRsp &rsp)
{
	autojsoncxx::ParsingResult result;
	if (!autojsoncxx::from_json_string(pRecv, rsp, result)) 
	{
	    std::cerr << result << '\n';
	    return false;
	}
	return true;
}

bool OAPIClient::ParseDevice(char *pRecv, int len, oapi::OAPICamAddNotify &pCam)
{
	autojsoncxx::ParsingResult result;
	if (!autojsoncxx::from_json_string(pRecv, pCam, result)) 
	{
	    std::cerr << result << '\n';
	    return false;
	}
	return true;
}
bool OAPIClient::ParseDeviceStrId(char *pRecv, int len, astring &pStrId)
{
	autojsoncxx::ParsingResult result;

	oapi::OAPICamNotifyId strId;
	
	if (!autojsoncxx::from_json_string(pRecv, strId, result)) 
	{
	    std::cerr << result << '\n';
	    return false;
	}
	pStrId = strId.strId;
	return true;
}

bool OAPIClient::ParseGetCam(char *pRecv, int len, oapi::OAPICameraGetRsp &pCam)
{
	autojsoncxx::ParsingResult result;
	if (!autojsoncxx::from_json_string(pRecv, pCam, result)) 
	{
	    std::cerr << result << '\n';
	    return false;
	}
	return true;
}

inline bool OAPIClient::ParseSearchNotify(char *pRecv, int len, oapi::OAPICamSearchedNotify &pCam)
{
	autojsoncxx::ParsingResult result;
	if (!autojsoncxx::from_json_string(pRecv, pCam, result)) 
	{
	    std::cerr << result << '\n';
	    return false;
	}
	return true;
}


#endif
