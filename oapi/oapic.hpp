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

using namespace XSDK;
using namespace std;

/* a internal thread to process all the request of XSocket */
class OAPIClient
{
public:
	inline OAPIClient(XSocket *pSocket);
	inline ~OAPIClient();
public:
	BOOL Process();
public:

private:

};

#include "oapicimpl.hpp"
#endif