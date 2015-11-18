
 
#ifndef _CONF_H_
#define _CONF_H_

#include "utility.hpp"
#include "oapicmn.hpp"

#define VE_VERSION "r2.0.1-20151201"
#define VE_INFO "OpenCVR r2.0.1 2015"

/* 0xFF FFFF to 0xFFFF FFFF is for status for the map */
#define CONF_MAP_INVALID_MIN 0xFFFFFF
#define CONF_KEY_STR_MAX 16

#define VSC_CONF_KEY "ConfVSCSystem"
#define VSC_CONF_LIC_KEY "ConfVSCLicense"
#define VSC_CONF_CAMERA_KEY "ConfVSCCAMERA"
#define VSC_CONF_VIDSTOR_KEY "ConfVSCVidStor"
#define VSC_CONF_VIEW_KEY "ConfVSCView"
#define VSC_CONF_VGROUP_KEY "ConfVSCVGroup"
#define VSC_CONF_HDFS_RECORD_KEY "ConfVSCHdfsRec"
#define VSC_CONF_EMAP_KEY "ConfVSCEmap"
#define VSC_CONF_USER_KEY "ConfVSCUserConf"

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

typedef struct __VSCConfLicenseKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfLicenseKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_LIC_KEY);
    }
}VSCConfLicenseKey;

typedef struct __VSCConfCameraKey {
    u32 nId;
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfCameraKey(u32 id)
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_CAMERA_KEY);
        nId = id;
    }
}VSCConfCameraKey;

typedef struct __VSCConfVidStorKey {
    s8 Key[CONF_KEY_STR_MAX];
	__VSCConfVidStorKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_VIDSTOR_KEY);
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

inline void VSCHdfsRecordDataItemDefault(VSCConfHdfsRecordData__ &item)
{
    strcpy(item.NameNode, "localhost");//default for hdd
    strcpy(item.Port, "8020");//0 for hdd
    strcpy(item.User, "admin");
    strcpy(item.Password, "admin");
    item.FileInterval = 30;/* 30s */
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
