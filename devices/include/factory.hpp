//------------------------------------------------------------------------------
// File: factory.hpp
//
// Desc: Device factory - Manage IP Camera.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_FACTORY_H_
#define __VSC_FACTORY_H_
#include "confdb.hpp"
#include "device.hpp"
#include "vdb.hpp"
#include "vhdfsdb.hpp"
#include "vplay.hpp"
#include "sysdb.hpp"
#include "hdddevice.hpp"
#include <QThread>
#include <qdebug.h>

typedef enum
{
    FACTORY_DEVICE_ADD = 1,
    FACTORY_DEVICE_DEL,
    FACTORY_DEVICE_ONLINE,
    FACTORY_DEVICE_OFFLINE,
    FACTORY_DEVICE_RECORDING_ON,
    FACTORY_DEVICE_RECORDING_OFF,
    /* The Camera group has been change */
    FACTORY_DEVICE_GROUP_CHANGE,
    FACTORY_VMS_ADD,
    FACTORY_VMS_DEL,
    FACTORY_VIPC_ADD,
    FACTORY_VIPC_DEL,
    /* Camera view add and del */
    FACTORY_VIEW_ADD,
    FACTORY_VIEW_DEL,
    /* Camera group add and del */
    FACTORY_VGROUP_ADD,
    FACTORY_VGROUP_DEL,
    FACTORY_DEVICE_LAST
} FactoryDeviceChangeType;


class FactoryDeviceChangeData
{
public:
	FactoryDeviceChangeType type;
	int id;
};

typedef BOOL (*FactoryDeviceChangeNotify)(void* pParam, 
		FactoryDeviceChangeData data);

typedef std::list<LPDevice> DeviceList;
typedef std::list<DeviceParam> DeviceParamList;
typedef std::map<int, LPDevice> DeviceMap;
typedef std::map<int, DeviceParam> DeviceParamMap;
typedef std::map<int, VIPCDeviceParam> VIPCDeviceParamMap;
typedef std::map<void *, FactoryDeviceChangeNotify> DeviceChangeNofityMap;
#define FACTORY_DEVICE_ID_MAX 4096

class Factory;
class FactoryHddTask:public QThread
{
	Q_OBJECT
public:
	FactoryHddTask(Factory &pFactory);
	~FactoryHddTask();
public:
	void run();
private:
	Factory &m_Factory;
};

/* Fatory is Qthread for callback in Qt GUI */
class Factory: public QThread
{
    Q_OBJECT
public:
    Factory();
    ~Factory();
public:
	/* Init function */
	BOOL Init();
	s32 InitAddDevice(DeviceParam & pParam, u32 nIndex);
	
public:
	BOOL RegDeviceChangeNotify(void * pData, FactoryDeviceChangeNotify callback);
	BOOL CallDeviceChange(FactoryDeviceChangeData data);
	
public:
	BOOL GetLicense(astring &strLicense, astring &strHostId, 
							int &ch, int &type);
	BOOL SetLicense(astring &strLicense);
	BOOL InitLicense();

public:
	BOOL GetAutoLogin();
	BOOL AuthUser(astring &strUser, astring &strPasswd);
	BOOL GetUserData(VSCUserData &pData);
	BOOL SetUserData(VSCUserData &pData);

/* Emap function */
public:
	BOOL GetEmapData(VSCEmapData &pData);
	BOOL SetEmapData(VSCEmapData &pData);
	BOOL AddEmapCamera(s32 nIndex, u32 x, u32 y, u32 w, u32 h);
	BOOL DelEmapCamera(s32 nIndex);
	BOOL GetEmapCamera(s32 nIndex, u32 &x, u32 &y, u32 &w, u32 &h);
	
	BOOL GetEmapFile(astring &strFile);
	BOOL SetEmapFile(astring &strFile);

public:
	BOOL GetHdfsRecordConf(VSCHdfsRecordData &pData);
	BOOL SetHdfsRecordConf(VSCHdfsRecordData &pData);
	BOOL GetLang(VSCLangType &pLang);
	BOOL SetLang(VSCLangType &pLang);

	/* OpenCVR API port */
	BOOL GetOAPIPort(u16 &pPort);
	BOOL SetOAPIPort(u16 &pPort);

	/* Defualt HW HWAccel enable or disable */
	BOOL GetDefaultHWAccel(BOOL &pHWAccel);
	BOOL SetDefaultHWAccel(BOOL &pHWAccel);

public:
	/* UI can use this for display device tree */
	BOOL GetDeviceParamMap(DeviceParamMap &pMap);
       BOOL GetVIPCDeviceParamMap(VIPCDeviceParamMap &pMap);
       /* Device function */
	s32 AddDevice(DeviceParam & pParam);
	s32 GetDeviceParamById(DeviceParam & pParam, s32 nIndex);
	BOOL GetDeviceRtspUrl(astring & strUrl, s32 nIndex);
	s32 GetDeviceParamByIdTryLock(DeviceParam & pParam, s32 nIndex);
	BOOL DelDevice(s32 nIndex);
	BOOL UpdateDeviceGroup(s32 nIndex, s32 nGroup);
	
	/* VIPC function */
	s32 AddVIPC(VIPCDeviceParam & pParam);
	s32 GetVIPCParamById(VIPCDeviceParam & pParam, s32 nIndex);
	BOOL DelVIPC(s32 nIndex);
	
	/* Video play function */
	BOOL AttachPlayer(s32 nIndex, HWND hWnd, int w, int h, 
						DeviceDelCallbackFunctionPtr pCallback, void * pParam);
	BOOL UpdateWidget(s32 nIndex, HWND hWnd, int w, int h);
	BOOL DetachPlayer(s32 nIndex, HWND hWnd, void * pParam);
	
	BOOL EnablePtz(s32 nIndex, HWND hWnd, bool enable);
	BOOL DrawPtzDirection(s32 nIndex, HWND hWnd, int x1, int y1, int x2,  int y2);
	BOOL ClearPtzDirection(s32 nIndex, HWND hWnd);
	BOOL PtzAction(s32 nIndex, FPtzAction action, float speed);
	BOOL ShowAlarm(s32 nIndex, HWND hWnd);
public:
	/* VMS */
	BOOL GetVms(VSCVmsData &pData);
	s32 AddVms(VSCVmsDataItem &pParam);
	BOOL DelVms(s32 Id);
	BOOL GetVmsById(VSCVmsDataItem &pParam, int nId);

	/* View */
	BOOL GetView(VSCViewData &pData);
	s32 AddView(VSCViewDataItem &pParam);
	BOOL DelView(s32 Id);
	BOOL GetViewById(VSCViewDataItem &pParam, int nId);

	/* Camera group */
	BOOL GetVGroup(VSCVGroupData &pData);
	s32 AddVGroup(VSCVGroupDataItem &pParam);
	BOOL DelVGroup(s32 Id);
	BOOL GetVGroupById(VSCVGroupDataItem &pParam, int nId);

public:
	BOOL StartRecord(s32 nIndex);
	BOOL StopRecord(s32 nIndex);
	BOOL StartHdfsRecord(s32 nIndex);
	BOOL StopHdfsRecord(s32 nIndex);
	BOOL StartRecordAll();
	BOOL StopRecordAll();
	BOOL StartHdfsRecordAll();
	BOOL StopHdfsRecordAll();
public:
	BOOL GetRecordStatus(s32 nIndex, BOOL &bStatus);

public:
	/* Data */
	BOOL RegDataCallback(s32 nIndex, DeviceDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegDataCallback(s32 nIndex, void * pParam);
	BOOL GetInfoFrame(s32 nIndex, InfoFrame &pFrame);
	BOOL RegSubDataCallback(s32 nIndex, DeviceDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegSubDataCallback(s32 nIndex, void * pParam);
	BOOL GetSubInfoFrame(s32 nIndex, InfoFrame &pFrame);

	/* Raw data */
	BOOL RegRawCallback(s32 nIndex, DeviceRawCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegRawCallback(s32 nIndex, void * pParam);
	BOOL RegSubRawCallback(s32 nIndex, DeviceRawCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegSubRawCallback(s32 nIndex, void * pParam);

	BOOL RegDelCallback(s32 nIndex, DeviceDelCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegDelCallback(s32 nIndex, void * pParam);

	BOOL GetDeviceOnline(s32 nIndex, BOOL &bStatus);
	BOOL GetUrl(s32 nIndex, std::string &url);
	BOOL SetSystemPath(astring &strPath);

	/* Disk function */
	BOOL AddHdd(astring &strHdd, astring & strPath, s64 nSize);
	BOOL DelHdd(astring & strHdd);
	BOOL HddUpdateSize(astring &strHdd, s64 nSize);
	BOOL GetDiskMap(VDBDiskMap &pMap);
	BOOL GetDiskStatus(VDBDiskStatus &pStatus);
	BOOL UpdateDiskStatusMap(VDBDiskStatus &pStatus);

	/* Search function */
	BOOL SearchItems(s32 deviceId, u32 startTime, u32 endTime, u32 recordType, 
					RecordItemMap &map);
	BOOL SearchHasItems(s32 deviceId, u32 startTime, u32 endTime, 
					u32 recordType);
	BOOL SearchAItem(s32 deviceId, u32 Time, VdbRecordItem &pItem);
	BOOL SearchAItemNear(s32 deviceId, u32 Time, VdbRecordItem &pItem);
	BOOL SearchNextItem(s32 deviceId, s64 LastId, VdbRecordItem &pItem);
	BOOL RequestAMFRead(VdbRecordItem &pItem, astring & strPath);
	BOOL FinishedAMFRead(VdbRecordItem &pItem, astring & strPath);
public:
	void Lock(){m_Lock.lock();}
	bool TryLock(){return m_Lock.try_lock();}
	void UnLock(){m_Lock.unlock();}

public:
	/* Device */
	s32 GetDeviceID(void);
	BOOL ReleaseDeviceID(s32 nID);
	BOOL LockDeviceID(s32 nID);

	/* VIPC */
	s32 GetVIPCID(void);
	BOOL ReleaseVIPCID(s32 nID);
	BOOL LockVIPCID(s32 nID);
public:
	static void Run(void * pParam);
	void run();

private:
	DeviceMap m_DeviceMap;
	DeviceParamMap m_DeviceParamMap;

	/* Virtual IP camera param */
	VIPCDeviceParamMap m_VIPCDeviceParamMap;
	fast_mutex m_Lock;
	tthread::thread *m_pThread;

private:
	DeviceChangeNofityMap m_DeviceChange;

private:
	VDB *m_pVdb;
	VHdfsDB *m_pVHdfsdb;
	FactoryHddTask *m_HddTask;

private:
	s8 m_strDeviceMap[FACTORY_DEVICE_ID_MAX];
	s8 m_strVIPCMap[FACTORY_DEVICE_ID_MAX];
	ConfDB m_Conf;
	SysDB m_SysPath;
};

typedef Factory* LPFactory;


#include "factoryimpl.hpp"

#endif // __VSC_FACTORY_H_
