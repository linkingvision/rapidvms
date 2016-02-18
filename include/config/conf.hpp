
 
#ifndef _CONF_H_
#define _CONF_H_

#include "utility.hpp"
#include "oapi/oapicmn.hpp"

#define VE_VERSION "r2.0.1-20160214"
#define VE_INFO "OpenCVR r2.0.1 2016"

/* 0xFF FFFF to 0xFFFF FFFF is for status for the map */
#define CONF_MAP_INVALID_MIN 0xFFFFFF
#define CONF_KEY_STR_MAX 16

#define VSC_CONF_KEY "ConfVSCSystem"
#define VSC_CONF_STOR_SERVER_KEY "ConfVSCStorServer" /* server */
#define VSC_CONF_CLIENT_KEY "ConfVSCClient" /* Client */
#define VSC_CONF_LIC_KEY "ConfVSCLicense"
#define VSC_CONF_CAMERA_KEY "ConfVSCCAMERA"
#define VSC_CONF_VIDSTOR_KEY "ConfVSCVidStor"
#define VSC_CONF_VIEW_KEY "ConfVSCView"
#define VSC_CONF_VGROUP_KEY "ConfVSCVGroup"
#define VSC_CONF_HDFS_RECORD_KEY "ConfVSCHdfsRec"
#define VSC_CONF_EMAP_KEY "ConfVSCEmap"
#define VSC_CONF_USER_KEY "ConfVSCUserConf"

#ifdef WIN32
#define VSC_DEFAULT_SYSPATH  "C:\\"
#else
#define VSC_DEFAULT_SYSPATH  "ve/"
#endif

#define REC_SCHED_OFF "0e4adca1-a2d6-499c-84fc-68c450e46d8f"
#define REC_SCHED_ALL_DAY "44241a90-8927-4907-a3d1-f32f29c2266e"
#define REC_SCHED_WORK_DAY "2010e075-f937-461f-a1a0-be39b385e0a5"

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

//------------------------------------------------------------------------------
// Conf Data Default function
//
//------------------------------------------------------------------------------
#if 0
inline void VSCVmsDataItemDefault(VSCVmsDataItem &item)
{
    sprintf(item.Name, "Recorder");

    strcpy(item.IP, "192.168.0.1");
    strcpy(item.Port, "9080");
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



inline void VSCUserDataItemDefault(VSCUserData__ &item)
{
	memset(&item, 0, sizeof(VSCUserData__));
	strcpy(item.Passwd, "admin");/* Default passwd for User */
	item.AutoLogin = 0;
}

inline void VSCEmapDataDefault(VSCEmapData__ &data)
{
	memset(&data, 0, sizeof(VSCEmapData__));
}

inline void VSCTourDataItemDefault(VSCTourDataItem &item)
{
    memset(&item, 0, sizeof(VSCTourDataItem));
    sprintf(item.Name, "Tour");
}

inline void VSCTourDataDefault(VSCTourData__ &data)
{
	memset(&data, 0, sizeof(VSCTourData__));
}
#endif

#pragma pack(pop)

#endif /* _CONF_H_ */
