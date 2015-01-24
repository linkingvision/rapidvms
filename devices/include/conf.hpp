
 
#ifndef _CONF_H_
#define _CONF_H_

#include "utility.hpp"

#define VE_VERSION "r1.0.3-20150122"
#define VE_INFO "vdceye Manager r1.0.3 2015"

#ifdef WIN32
#define VE_NVR_CLIENT 
#else
#define VE_NVR
#endif

/* NVR Client feature */
#ifdef VE_NVR_CLIENT
#define VE_RECORDER_MGR_CLIENT_SUPPORT 
#endif

/* NVR feature */
#ifdef VE_NVR
#define VE_RECORDER_MGR_SERVER_SUPPORT
#endif

#define CONF_NAME_MAX 128
/* support Camera num */
#define CONF_MAP_MAX 4096
#define CONF_USER_PASSWORD_MAX 1024
#define CONF_PATH_MAX 1024
/* 0xFF FFFF to 0xFFFF FFFF is for status for the map */
#define CONF_MAP_INVALID_MIN 0xFFFFFF
#define CONF_KEY_STR_MAX 16

/* Support VMS(site, recorder) num */
#define CONF_VMS_NUM_MAX 128
#define CONF_VIEW_NUM_MAX 128
/* IP camera Group max num */
#define CONF_VGROUP_NUM_MAX 128

#define VSC_CONF_KEY "ConfVSCSystem"
#define VSC_CONF_LIC_KEY "ConfVSCLicense"
#define VSC_CONF_CHANNEL_KEY "ConfVSCDevice"
#define VSC_CONF_VIPC_KEY "ConfVSCVIPC"
#define VSC_CONF_VMS_KEY "ConfVSCVms"
#define VSC_CONF_VIEW_KEY "ConfVSCView"
#define VSC_CONF_VGROUP_KEY "ConfVSCVGroup"
#define VSC_CONF_HDFS_RECORD_KEY "ConfVSCHdfsRec"
#define VSC_CONF_EMAP_FILE_KEY "ConfVSCEmapFile"
#define VSC_CONF_EMAP_CONF_KEY "ConfVSCEmapConf"
#define VSC_CONF_PARAM_MAX 1024
#define VSC_CONF_PARAM_S_MAX 128
/* Max camera in one view */
#define VSC_CONF_VIEW_CH_MAX 256
/* Max camera in one Group */
#define VSC_CONF_VGROUP_CH_MAX 256

typedef enum
{
    VSC_DEVICE_CAM = 1,
    VSC_DEVICE_RECORDER,

    VSC_DEVICE_LAST
} VSCDeviceType;

/* Device Type */
typedef enum
{
    VSC_SUB_DEVICE_USB_CAM = 1,
    VSC_SUB_DEVICE_FILE,
    VSC_SUB_DEVICE_RTSP,
    VSC_SUB_DEVICE_ONVIF,
    VSC_SUB_DEVICE_ONVIF_RECODER,
    VSC_SUB_DEVICE_GB28181,

    VSC_SUB_DEVICE_LAST
} VSCDeviceSubType;

typedef enum
{
    VSC_VMS_RECORDER = 1,
    VSC_VMS_SITE,
    VSC_VMS_VIRTUL_IPC,
    
    VSC_VMS_LAST
} VSCVmsType;

typedef enum
{
    VSC_SUB_VMS_PG = 1,
    VSC_SUB_VMS_ZB,
    VSC_SUB_VIPC_FILE,
    VSC_SUB_VIPC_LIVE,
    
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
    LAYOUT_MODE_LAST
} VideoWallLayoutMode;

#pragma pack(push,  1 )
typedef struct __VSCConfSystemKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfSystemKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_KEY);
    }
}VSCConfSystemKey;

typedef struct __VSCConfLicenseKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfLicenseKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_LIC_KEY);
    }
}VSCConfLicenseKey;

typedef struct __VSCConfDeviceKey {
    u32 nId;
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfDeviceKey(u32 id)
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_CHANNEL_KEY);
        nId = id;
    }
}VSCConfDeviceKey;

typedef struct __VSCConfVIPCKey {
    u32 nId;
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfVIPCKey(u32 id)
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_VIPC_KEY);
        nId = id;
    }
}VSCConfVIPCKey;

typedef struct __VSCConfVmsKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfVmsKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_VMS_KEY);
    }
}VSCConfVmsKey;

typedef struct __VSCConfViewKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfViewKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_VIEW_KEY);
    }
}VSCConfViewKey;

/* Camera Group key */
typedef struct __VSCConfGroupKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfGroupKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_VGROUP_KEY);
    }
}VSCConfVGroupKey;

/* HDFS Reocrd key */
typedef struct __VSCConfHdfsRecordKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfHdfsRecordKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_HDFS_RECORD_KEY);
    }
}VSCConfHdfsRecordKey;

typedef struct __VSCConfData__ {
    u32 DeviceMap[CONF_MAP_MAX];
    u32 Language;
    u32 DeviceNum;
    u32 VIPCMap[CONF_MAP_MAX];
    u32 VIPCNum;
    u32 ConfVer;/* Current version is 0 */
}VSCConfData__;

typedef struct __VSCConfData {
    union {
        VSCConfData__ conf;
        u8 whole[1024 * 128];
    } data;
}VSCConfData;

typedef struct __VSCDeviceData__ {
	u32 nId;
	VSCDeviceType nType;
	VSCDeviceSubType nSubType;

	s8 Name[CONF_NAME_MAX];
	s8 Param[VSC_CONF_PARAM_MAX];

	s8 IP[VSC_CONF_PARAM_MAX];
	s8 Port[VSC_CONF_PARAM_MAX];
	s8 User[VSC_CONF_PARAM_MAX];
	s8 Password[VSC_CONF_PARAM_MAX];

	/* Camera Param */
	s8 RtspLocation[VSC_CONF_PARAM_MAX];
	s8 FileLocation[VSC_CONF_PARAM_MAX];
	s8 OnvifAddress[VSC_CONF_PARAM_MAX];
	s8 CameraIndex[VSC_CONF_PARAM_MAX];/* This is For USB Camera */

	u32 UseProfileToken;/* 1 stand for use, 0 stand for do not use */
	s8 OnvifProfileToken[VSC_CONF_PARAM_MAX];

	/* Recording */
	u32 Recording;/* 1 stand for recording, 0 stand for do record */
	u32 GroupId;
	u32 HdfsRecording;/* 1 stand for recording, 0 stand for do record */
	/* Second stream, only for VA */
	s8 OnvifProfileToken2[VSC_CONF_PARAM_MAX];
	u32 ConnectType;/* 0 UDP, 1 TCP, 2 Multicast, 3 HTTP */
	u32 Mining;/* 1 stand for mining, 0 stand for no mining */
	
}VSCDeviceData__;


typedef struct __VSCConfHdfsRecordData__ {
	s8 NameNode[VSC_CONF_PARAM_MAX];
	s8 Port[VSC_CONF_PARAM_MAX];
	s8 User[VSC_CONF_PARAM_MAX];
	s8 Password[VSC_CONF_PARAM_MAX];
	int FileInterval;/* In Seconds */
}VSCConfHdfsRecordData__;

typedef struct __VSCVmsDataItem__ {
	u32 nId;
	VSCVmsType nType;
	VSCVmsSubType nSubType;

	s8 Name[CONF_NAME_MAX];
	s8 Param[VSC_CONF_PARAM_MAX];

	s8 IP[VSC_CONF_PARAM_MAX];
	s8 Port[VSC_CONF_PARAM_MAX];
	s8 User[VSC_CONF_PARAM_MAX];
	s8 Password[VSC_CONF_PARAM_MAX];

	s8 OnvifAddress[VSC_CONF_PARAM_MAX];
	u32 GroupId;
	u32 Used;/* 1 stand for used, 0 stand for not used */
}VSCVmsDataItem;

typedef struct __VSCViewDataItem__ {
	u32 nId;
	s8 Name[CONF_NAME_MAX];
	/* Map for this view */
	u32 Map[VSC_CONF_VIEW_CH_MAX];
	VideoWallLayoutMode Mode;
	u32 Used;/* 1 stand for used, 0 stand for not used */
}VSCViewDataItem;


/* IP Camera Group */
typedef struct __VSCVGroupDataItem__ {
	u32 nId;
	s8 Name[CONF_NAME_MAX];
	/* Map for this group */
	u32 Map[VSC_CONF_VGROUP_CH_MAX];
	u32 Used;/* 1 stand for used, 0 stand for not used */
}VSCVGroupDataItem;

typedef struct __VSCVIPCDataItem__ {
	u32 nId;
	VSCVmsType nType;
	VSCVmsSubType nSubType;

	s8 Name[CONF_NAME_MAX];
	s8 Param[VSC_CONF_PARAM_S_MAX];
	s32 nStreamId;

	s8 IP[VSC_CONF_PARAM_S_MAX];
	s8 Port[VSC_CONF_PARAM_S_MAX];
	s8 User[VSC_CONF_PARAM_S_MAX];
	s8 Password[VSC_CONF_PARAM_S_MAX];

	s8 OnvifAddress[VSC_CONF_PARAM_S_MAX];
}VSCVIPCDataItem__;


typedef struct __VSCVmsData__ {
	VSCVmsDataItem vms[CONF_VMS_NUM_MAX];
}VSCVmsData__;

typedef struct __VSCViewData__ {
	VSCViewDataItem view[CONF_VIEW_NUM_MAX];
}VSCViewData__;

typedef struct __VSCVGroupData__ {
	VSCVGroupDataItem group[CONF_VGROUP_NUM_MAX];
}VSCVGroupData__;


typedef struct __VSCDeviceData {
    union {
        VSCDeviceData__ conf;
        u8 whole[1024 * 128];
    } data;
}VSCDeviceData;

typedef struct __VSCVmsData {
    union {
        VSCVmsData__ conf;
        u8 whole[1024 * 128];
    } data;
}VSCVmsData;

typedef struct __VSCViewData {
    union {
        VSCViewData__ conf;
        u8 whole[1024 * 128];
    } data;
}VSCViewData;

typedef struct __VSCVGroupData {
    union {
        VSCVGroupData__ conf;
        u8 whole[1024 * 128];
    } data;
}VSCVGroupData;


typedef struct __VSCVIPCData {
    union {
        VSCVIPCDataItem__ conf;
        u8 whole[1024 * 128];
    } data;
}VSCVIPCData;

typedef struct __VSCHdfsRecordData {
    union {
        VSCConfHdfsRecordData__ conf;
        u8 whole[1024 * 128];
    } data;
}VSCHdfsRecordData;

inline void VSCVmsDataItemDefault(VSCVmsDataItem &item)
{
    sprintf(item.Name, "Recorder");

    strcpy(item.IP, "192.168.0.1");
    strcpy(item.Port, "80");
    strcpy(item.User, "admin");
    strcpy(item.Password, "admin");
    strcpy(item.Param, "none");
    item.Used = 0;
    item.nId = 0;
    item.GroupId = 0;
}

inline void VSCViewDataItemDefault(VSCViewDataItem &item)
{
    memset(&item, 0, sizeof(VSCViewDataItem));
    sprintf(item.Name, "View");
    item.Mode = LAYOUT_MODE_3X3;
}

inline void VSCVGroupDataItemDefault(VSCVGroupDataItem &item)
{
    memset(&item, 0, sizeof(VSCVGroupDataItem));
    sprintf(item.Name, "Group");
}

inline void VSCVIPCDataItemDefault(VSCVIPCDataItem__ &item)
{
    sprintf(item.Name, "Virutal IPC");

    strcpy(item.IP, "192.168.0.1");
    strcpy(item.Port, "8000");
    strcpy(item.User, "admin");
    strcpy(item.Password, "admin");
    item.nStreamId = 1;
}

inline void VSCHdfsRecordDataItemDefault(VSCConfHdfsRecordData__ &item)
{
    strcpy(item.NameNode, "localhost");//default for hdd
    strcpy(item.Port, "8020");//0 for hdd
    strcpy(item.User, "admin");
    strcpy(item.Password, "admin");
    item.FileInterval = 30;/* 30s */
}

#pragma pack(pop)

#endif /* _CONF_H_ */
