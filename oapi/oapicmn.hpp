//------------------------------------------------------------------------------
// File: oapicmn.hpp
//
// Desc: OpenCVR API - OpenCVR API Common.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPI_CMN_H_
#define __VSC_OAPI_CMN_H_

#ifndef NOMINMAX
#ifdef WIN32
#undef max
#undef min
#define NOMINMAX
#define NOMINMAX 
#endif
#endif
#include "oapiproto.hpp"
#include "utility/type.hpp"

using namespace std;

typedef enum
{
	OAPI_STATUS_OK = 1,
	OAPI_STATUS_FAIL,
	OAPI_STATUS_AUTH_FAIL,
	OAPI_STATUS_LAST
} OAPIStatus;

typedef enum
{
	/* Req Rsp command */
	OAPI_CMD_LOGIN_REQ = 1,
	OAPI_CMD_LOGIN_RSP,
	OAPI_CMD_KEEPALIVE_REQ,
	OAPI_CMD_KEEPALIVE_RSQ, /* CmnRetRsp */
	OAPI_CMD_DEVICE_LIST_REQ,
	OAPI_CMD_DEVICE_LIST_RSP,
	OAPI_CMD_ADD_DEVICE_REQ,
	OAPI_CMD_ADD_DEVICE_RSP, /* CmnRetRsp */
	OAPI_CMD_DEL_DEVICE_REQ,
	OAPI_CMD_DEL_DEVICE_RSP, /* CmnRetRsp */

	/* One way command */
	OAPI_CMD_START_LIVE_REQ = 10000,
	OAPI_CMD_START_LIVE_RSP, /* CmnRetRsp */
	OAPI_CMD_STOP_LIVE_REQ,
	OAPI_CMD_STOP_LIVE_RSP, /* CmnRetRsp */
	OAPI_CMD_FRAME_PUSH,/* a binrary data */
	


	/* All the deivce notify */
	OAPI_NOTIFY_DEVICE_ADD = 20000,
	OAPI_NOTIFY_DEVICE_DEL,
	OAPI_NOTIFY_DEVICE_ONLINE,
	OAPI_NOTIFY_DEVICE_OFFLINE,
	OAPI_NOTIFY_DEVICE_RECORDING_ON,
	OAPI_NOTIFY_DEVICE_RECORDING_OFF,
	OAPI_NOTIFY_DEVICE_GROUP_CHANGE,

	/* Alarm notify */
	
	OAPI_CMD_LAST
} OAPICmd;

/* Device Type */
typedef enum
{
    VSC_DEVICE_CAM = 1,
    VSC_DEVICE_LAST
} VSCDeviceType;

typedef enum
{
    VSC_SUB_DEVICE_USB_CAM = 1,
    VSC_SUB_DEVICE_FILE,
    VSC_SUB_DEVICE_RTSP,
    VSC_SUB_DEVICE_ONVIF,
    VSC_SUB_DEVICE_GB28181,

    VSC_SUB_DEVICE_LAST
} VSCDeviceSubType;

typedef enum
{
    VSC_VMS_SITE = 1,
    VSC_VMS_LAST
} VSCVmsType;

typedef enum
{
    VSC_SUB_VMS_OAPI = 1,
    VSC_SUB_VMS_LAST
} VSCVmsSubType;

/* Control command */
typedef enum
{
    LAYOUT_MODE_1 = 1,
    LAYOUT_MODE_2X2,
    LAYOUT_MODE_3X3,
    LAYOUT_MODE_4X4,
    LAYOUT_MODE_6,
    LAYOUT_MODE_8,
    LAYOUT_MODE_12p1,
    LAYOUT_MODE_5x5,
    LAYOUT_MODE_6x6,
    LAYOUT_MODE_8x8,
    LAYOUT_MODE_ONE,
    LAYOUT_MODE_1x3,
    LAYOUT_MODE_1p6,
    LAYOUT_MODE_LAST
} VideoWallLayoutMode;

/* Language */
typedef enum
{
    VSC_LANG_AUTO = 0,
    VSC_LANG_EN,
    VSC_LANG_ZH,
    VSC_LANG_LAST
} VSCLangType;

#ifdef WIN32
#pragma   pack(1)
#endif
typedef struct __OAPIHeader{
	u32 version;/* version */
	u32 cmd;
	u32 length;
	u32 magic;/* magic */
} OAPIHeader;
#ifdef WIN32
#pragma   pack()
#endif

#endif