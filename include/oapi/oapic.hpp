//------------------------------------------------------------------------------
// File: oapic.hpp
//
// Desc: OpenCVR API - OpenCVR API Client.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPIC_H_
#define __VSC_OAPIC_H_
#include "oapicmn.hpp"
#include "oapiproto.hpp"
#include "XSDK/XHash.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XSocket.h"
#include "XSDK/XSSLSocket.h"
#include "XSDK/XMD5.h"


using namespace XSDK;
using namespace std;
using namespace Poco;

/* a internal thread to process all the request of XSocket */
class OAPIClient
{
public:
	inline OAPIClient(XRef<XSocket> pSocket);
	inline ~OAPIClient();
/* send request */
public:
	inline bool Setup(std::string strUser, std::string strPasswd, std::string strNonce);
	inline bool SendDeviceListRequest();
	inline bool SendRegNotifyRequest();
	inline bool SendDiskListRequest();
	inline bool SendSysDiskListRequest();
	inline bool StartLiveview(astring strId, unsigned int nStream);
	inline bool StopLiveview(astring strId, unsigned int nStream);

	inline bool AddCam(oapi::OAPIAddCameraReq sCam);
	inline bool DeleteCam(astring strId);
	inline bool AddDisk(oapi::OAPIAddDiskReq sDisk);
	inline bool DelDisk(astring strId);
	inline bool ConfAdminPasswd(astring strOld, astring strNew);
	inline bool GetLic();
	inline bool ConfLic(astring strLic);
	inline bool CamSearchStart();
	inline bool CamSearchStop();
	/* Parse all the list */
public:
	inline bool ParseDeviceList(char *pRecv, int len, oapi::OAPICameraListRsp &list);
	inline bool ParseLogin(char *pRecv, int len, oapi::LoginRsp &rsp);
	inline bool ParseDevice(char *pRecv, int len, oapi::OAPICamAddNotify &pCam);
	inline bool ParseDeviceStrId(char *pRecv, int len, astring &pStrId);
	inline bool ParseDiskList(char *pRecv, int len, oapi::OAPIDiskListRsp &list);
	inline bool ParseSysDiskList(char *pRecv, int len, oapi::OAPISysDiskListRsp &list);
	inline bool ParseLic(char *pRecv, int len, astring &strLic, astring &strHostId, 
							int &ch, astring &type, astring &expireTime);
	
private:
	XRef<XSocket> m_pSocket;
};

#include "oapicimpl.hpp"
#endif
