/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#ifndef _CONF_H_
#define _CONF_H_

#include "utility.hpp"
#include "config/confver.hpp"

#define LINK_PROTO_WS_PATH "/linkproto"
#define LINK_PROTO_WS_STREAM_PATH "/linkstream"

/* 0xFF FFFF to 0xFFFF FFFF is for status for the map */
#define CONF_MAP_INVALID_MIN 0xFFFFFF
#define CONF_KEY_STR_MAX 16

/* The length of key max < 16 !!! */
#define VSC_CONF_KEY "ConfVSCSystem"
#define VSC_CONF_STOR_SERVER_KEY "ConfVSCStorS" /* server */
#define VSC_CONF_CLIENT_KEY "ConfVSCClient" /* Client */
#define VSC_CONF_LIC_KEY "ConfVSCLicense"
#define VSC_CONF_CAMERA_KEY "ConfVSCCAMERA"
#define VSC_CONF_VIDSTOR_KEY "ConfVSCVidStor"
#define VSC_CONF_VIEW_KEY "ConfVSCView"
#define VSC_CONF_TOUR_KEY "ConfVSCTour"
#define VSC_CONF_VGROUP_KEY "ConfVSCVGroup"
#define VSC_CONF_HDFS_RECORD_KEY "ConfVSCHdfsRec"
#define VSC_CONF_EMAP_KEY "ConfVSCEmap"
#define VSC_CONF_USER_KEY "ConfVSCUserCnf"

#ifdef WIN32
#define VSC_DEFAULT_SYSPATH  "C:\\"
#else
#define VSC_DEFAULT_SYSPATH  "ve/"
#endif

#define REC_SCHED_OFF "0e4adca1-a2d6-499c-84fc-68c450e46d8f"
#define REC_SCHED_ALL_DAY "44241a90-8927-4907-a3d1-f32f29c2266e"
#define REC_SCHED_WORK_DAY "2010e075-f937-461f-a1a0-be39b385e0a5"
#define VVID_UUID_NULL "a17bdf7d-9588-4bf3-8667-3fbaeaabe59e"
#define VVID_UUID_ALL "a1947501-930e-476f-a8b1-9002b0a6ca66"
#define VVID_MAIN_VIEW "18531236-e5df-415b-b547-2440115d1164"

//------------------------------------------------------------------------------
// Conf keys, for leveldb
//
//------------------------------------------------------------------------------

#pragma pack(push,  1 )
typedef struct __VSCConfSystemKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfSystemKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_KEY);
    }
}VSCConfSystemKey;

typedef struct __VSCConfStorServerKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfStorServerKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_STOR_SERVER_KEY);
    }
}VSCConfStorServerKey;

typedef struct __VSCConfClientKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfClientKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_CLIENT_KEY);
    }
}VSCConfClientKey;

typedef struct __VSCConfLicenseKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfLicenseKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_LIC_KEY);
    }
}VSCConfLicenseKey;

typedef struct __VSCConfCameraKey {
    s8 Key[CONF_KEY_STR_MAX];
	__VSCConfCameraKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_CAMERA_KEY);
    }
}VSCConfCameraKey;

/* HDFS Reocrd key */
typedef struct __VSCConfHdfsRecordKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfHdfsRecordKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_HDFS_RECORD_KEY);
    }
}VSCConfHdfsRecordKey;

typedef struct __VSCConfVidStorKey {
    s8 Key[CONF_KEY_STR_MAX];
	__VSCConfVidStorKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_VIDSTOR_KEY);
    }
}VSCConfVidStorKey;

typedef struct __VSCConfViewKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfViewKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_VIEW_KEY);
    }
}VSCConfViewKey;

typedef struct __VSCConfTourKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfTourKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_TOUR_KEY);
    }
}VSCConfTourKey;

/* Camera Group key */
typedef struct __VSCConfGroupKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfGroupKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_VGROUP_KEY);
    }
}VSCConfVGroupKey;

/* Emap configuration, storage the emap conf, such as the camera location*/
typedef struct __VSCConfEmapKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfEmapKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_EMAP_KEY);
    }
}VSCConfEmapKey;

/* User key */
typedef struct __VSCConfUserKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfUserKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_USER_KEY);
    }
}VSCConfUserKey;

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

#pragma pack(pop)

#endif /* _CONF_H_ */
