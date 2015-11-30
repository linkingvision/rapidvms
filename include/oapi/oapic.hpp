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
	inline BOOL Setup(std::string strUser, std::string strPasswd, std::string strNonce);
	inline BOOL SendDeviceListRequest();
	inline BOOL StartLiveview(int nId);
	inline BOOL StopLiveview(int nId);
/* Parse all the list */
public:
	inline BOOL ParseDeviceList(char *pRecv, int len, oapi::OAPICameraListRsp &list);
	inline BOOL ParseLogin(char *pRecv, int len, oapi::LoginRsp &rsp);

public:

private:
	XRef<XSocket> m_pSocket;
};

#include "oapicimpl.hpp"
#endif
