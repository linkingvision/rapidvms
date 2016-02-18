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
	OAPI_LOGIN_REQ = 1,
	OAPI_LOGIN_RSP,
	OAPI_KEEPALIVE_REQ,
	OAPI_KEEPALIVE_RSQ, /* CmnRetRsp */
	OAPI_CAM_LIST_REQ,
	OAPI_CAM_LIST_RSP,
	OAPI_ADD_CAM_REQ,
	OAPI_ADD_CAM_RSP, /* CmnRetRsp */
	OAPI_DEL_CAM_REQ,
	OAPI_DEL_CAM_RSP, /* CmnRetRsp */

	OAPI_GET_CAM_REQ,
	OAPI_GET_CAM_RSP,

	OAPI_SET_CAM_SCHED_REQ,
	OAPI_SET_CAM_SCHED_RSP, /* CmnRetRsp */

	/* Disk */
	OAPI_SYS_DISK_LIST_REQ,
	OAPI_SYS_DISK_LIST_RSP, 
	OAPI_DISK_LIST_REQ,
	OAPI_DISK_LIST_RSP, 
	OAPI_ADD_DISK_REQ,
	OAPI_ADD_DISK_RSP,  /* CmnRetRsp */
	OAPI_DEL_DISK_REQ,
	OAPI_DEL_DISK_RSP,  /* CmnRetRsp */
	OAPI_UPDATE_DISK_LIMIT_REQ,
	OAPI_UPDATE_DISK_LIMIT_RSP,  /* CmnRetRsp */

	/* User */
	OAPI_CONF_ADMIN_REQ,
	OAPI_CONF_ADMIN_RSP,  /* CmnRetRsp */

	/* Lic */
	OAPI_GET_LIC_REQ,
	OAPI_GET_LIC_RSP,

	/* Version */
	OAPI_GET_VER_REQ,
	OAPI_GET_VER_RSP,

	/* Search  */
	OAPI_CAM_SEARCH_START_REQ,
	OAPI_CAM_SEARCH_START_RSP,

	OAPI_CAM_SEARCH_STOP_REQ,
	OAPI_CAM_SEARCH_STOP_RSP,

	OAPI_CONF_LIC_REQ,
	OAPI_CONF_LIC_RSP,  /* CmnRetRsp */	

	OAPI_START_LIVE_REQ,
	OAPI_START_LIVE_RSP, /* CmnRetRsp */
	OAPI_STOP_LIVE_REQ,
	OAPI_STOP_LIVE_RSP, /* CmnRetRsp */

	OAPI_REG_NOTIFY_REQ,
	OAPI_REG_NOTIFY_RSP,

	/* One way command */
	OAPI_FRAME_PUSH  = 10000,/* a binrary data */
	OAPI_CAM_SAERCH_PUSH,
	OAPI_PTZ_CMD,

	/* All the deivce notify */
	OAPI_NOTIFY_DEVICE_ADD = 20000,
	OAPI_NOTIFY_DEVICE_DEL,
	OAPI_NOTIFY_DEVICE_ONLINE,
	OAPI_NOTIFY_DEVICE_OFFLINE,
	OAPI_NOTIFY_DEVICE_REC_ON,
	OAPI_NOTIFY_DEVICE_REC_OFF,

	/* Alarm notify */
	
	OAPI_LAST
} OAPICmd;

/* Same with CameraType in protobuf */
typedef enum
{
    VSC_CAM_USB = 1,
    VSC_CAM_FILE = 2,
    VSC_CAM_RTSP = 3,
    VSC_CAM_ONVIF_S = 4,
    VSC_CAM_GB28181 = 5,

    VSC_CAM_LAST
} VSCCameraType;

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
	u8 s1;/* Start */
	u8 s2;
	u8 v1;/* Version */
	u8 v2;
	u32 cmd;
	u32 length;
	u16 padding;
	u8 e1;/* End */
	u8 e2;
	__OAPIHeader()
	{	
		s1 = '\n';
		s2 = '$';
		v1 = 1;
		v2 = 0;
		e1 = '\r';
		e2 = '\n';
	}
} OAPIHeader;
#ifdef WIN32
#pragma   pack()
#endif

#endif