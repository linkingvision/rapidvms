//------------------------------------------------------------------------------
// File: oapicmn.hpp
//
// Desc: OpenCVR API - OpenCVR API Common.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPI_CMN_H_
#define __VSC_OAPI_CMN_H_
#include "oapiproto.hpp"
#include "XSDK/XHash.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XSocket.h"
#include "XSDK/XSSLSocket.h"

using namespace XSDK;
using namespace std;

typedef enum
{
    OAPI_CMD_KEEPALIVE_REQ = 1,
    OAPI_CMD_KEEPALIVE_RSQ,
    OAPI_CMD_GET_DEVICE_REQ,
    OAPI_CMD_GET_DEVICE_RSP,
    OAPI_CMD_LAST
} OAPICmd;

#ifdef WIN32
#pragma   pack(1)
#endif
typedef struct __OAPIHeader{
    u32 version;/* version */
    u32 cmd;
    u32 length;
} OAPIHeader;
#ifdef WIN32
#pragma   pack()
#endif

#endif