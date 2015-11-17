//------------------------------------------------------------------------------
// File: clientfactory.hpp
//
// Desc: Camera clientfactory - Manage IP Camera.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_CLIENT_FACTORY_H_
#define __VSC_CLIENT_FACTORY_H_
#include "confdb.hpp"
#include "camera.hpp"
#include "vdb.hpp"
#include "vhdfsdb.hpp"
#include "vplay.hpp"
#include "sysdb.hpp"
#include "hddcamera.hpp"
#include <QThread>
#include <qdebug.h>
#include "Poco/Path.h"
#include "Poco/File.h"

typedef enum
{
    CLIENT_FACTORY_CAMERA_ADD = 1,
    CLIENT_FACTORY_CAMERA_DEL,
    CLIENT_FACTORY_CAMERA_ONLINE,
    CLIENT_FACTORY_CAMERA_OFFLINE,
    CLIENT_FACTORY_CAMERA_RECORDING_ON,
    CLIENT_FACTORY_CAMERA_RECORDING_OFF,
    /* The Camera group has been change */
    CLIENT_FACTORY_CAMERA_GROUP_CHANGE,
    CLIENT_FACTORY_VMS_ADD,
    CLIENT_FACTORY_VMS_DEL,
    CLIENT_FACTORY_VIPC_ADD,
    CLIENT_FACTORY_VIPC_DEL,
    /* Camera view add and del */
    CLIENT_FACTORY_VIEW_ADD,
    CLIENT_FACTORY_VIEW_DEL,
    /* Camera group add and del */
    CLIENT_FACTORY_VGROUP_ADD,
    CLIENT_FACTORY_VGROUP_DEL,

    /* Camera tour add and del */
    CLIENT_FACTORY_TOUR_ADD,
    CLIENT_FACTORY_TOUR_DEL,
    CLIENT_FACTORY_CAMERA_LAST
} ClientFactoryCameraChangeType;


class ClientFactoryCameraChangeData
{
public:
	ClientFactoryCameraChangeType type;
	int id;
};

typedef BOOL (*ClientFactoryCameraChangeNotify)(void* pParam, 
		ClientFactoryCameraChangeData data);

typedef std::list<LPCamera> CameraList;
typedef std::list<CameraParam> CameraParamList;
typedef std::map<int, LPCamera> CameraMap;
typedef std::map<int, CameraParam> CameraParamMap;
typedef std::map<int, int> CameraOnlineMap;
typedef std::map<int, VIPCCameraParam> VIPCCameraParamMap;
typedef std::map<void *, ClientFactoryCameraChangeNotify> CameraChangeNofityMap;
#define CLIENT_FACTORY_CAMERA_ID_MAX 4096

class ClientFactory;
class ClientFactoryHddTask:public QThread
{
	Q_OBJECT
public:
	ClientFactoryHddTask(ClientFactory &pClientFactory);
	~ClientFactoryHddTask();
public:
	void run();
private:
	ClientFactory &m_ClientFactory;
};

/* Fatory is Qthread for callback in Qt GUI */
class ClientFactory: public QThread
{
    Q_OBJECT
public:
    ClientFactory();
    ~ClientFactory();
public:
	/* Init function */
	BOOL Init();
	s32 InitAddCamera(CameraParam & pParam, u32 nIndex);
	
public:
	BOOL RegCameraChangeNotify(void * pData, ClientFactoryCameraChangeNotify callback);
	BOOL CallCameraChange(ClientFactoryCameraChangeData data);
	
public:
	BOOL GetLicense(astring &strLicense, astring &strHostId, 
							int &ch, astring &type);
	BOOL SetLicense(astring &strLicense);
	BOOL InitLicense();

	BOOL GetExportPath(astring &strPath);
	BOOL SetExportPath(astring &strPath);

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


	/* OpenCVR HTTP Server  port */
	BOOL GetVHTTPSPort(u16 &pPort);
	BOOL SetVHTTPSPort(u16 &pPort);

	/* OpenCVR HTTP Server  port */
	BOOL GetVHTTPSSSLPort(u16 &pPort);
	BOOL SetVHTTPSSSLPort(u16 &pPort);

	/* OpenCVR HLS Server  port */
	BOOL GetVHLSSPort(u16 &pPort);
	BOOL SetVHLSSPort(u16 &pPort);

	/* Defualt HW HWAccel enable or disable */
	BOOL GetDefaultHWAccel(BOOL &pHWAccel);
	BOOL SetDefaultHWAccel(BOOL &pHWAccel);

	BOOL GetRTSPServer(BOOL &pAuth, astring &pMultiAddr, u16 &pPort);
	BOOL SetRTSPServer(BOOL &pAuth, astring &pMultiAddr, u16 &pPort);

	/* Defualt Mining enable or disable */
	BOOL GetDefaultMining(BOOL &pMining);
	BOOL SetDefaultMining(BOOL &pMining);

public:
	/* UI can use this for display camera tree */
	BOOL GetCameraParamMap(CameraParamMap &pMap);
	BOOL GetCameraOnlineMap(CameraOnlineMap &pMap);
       BOOL GetVIPCCameraParamMap(VIPCCameraParamMap &pMap);
       /* Camera function */
	s32 AddCamera(CameraParam & pParam);
	s32 GetCameraParamById(CameraParam & pParam, s32 nIndex);
	BOOL GetCameraRtspUrl(astring & strUrl, s32 nIndex);
	s32 GetCameraParamByIdTryLock(CameraParam & pParam, s32 nIndex);
	BOOL DelCamera(s32 nIndex);
	BOOL UpdateCameraGroup(s32 nIndex, s32 nGroup);
	
	/* VIPC function */
	s32 AddVIPC(VIPCCameraParam & pParam);
	s32 GetVIPCParamById(VIPCCameraParam & pParam, s32 nIndex);
	BOOL DelVIPC(s32 nIndex);
	
	/* Video play function */
	BOOL AttachPlayer(s32 nIndex, HWND hWnd, int w, int h, 
						CameraDelCallbackFunctionPtr pCallback, void * pParam);
	BOOL UpdateWidget(s32 nIndex, HWND hWnd, int w, int h);
	BOOL DetachPlayer(s32 nIndex, HWND hWnd, void * pParam);
	BOOL GetStreamInfo(s32 nIndex, VideoStreamInfo &pInfo);
	
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

	/* Tour */
	BOOL GetTour(VSCTourData &pData);
	s32 AddTour(VSCTourDataItem &pParam);
	BOOL DelTour(s32 Id);
	BOOL GetTourById(VSCTourDataItem &pParam, int nId);

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
	BOOL RegDataCallback(s32 nIndex, CameraDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegDataCallback(s32 nIndex, void * pParam);
	BOOL GetInfoFrame(s32 nIndex, InfoFrame &pFrame);
	BOOL RegSubDataCallback(s32 nIndex, CameraDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegSubDataCallback(s32 nIndex, void * pParam);
	BOOL GetSubInfoFrame(s32 nIndex, InfoFrame &pFrame);

	/* Raw data */
	BOOL RegRawCallback(s32 nIndex, CameraRawCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegRawCallback(s32 nIndex, void * pParam);
	BOOL RegSubRawCallback(s32 nIndex, CameraRawCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegSubRawCallback(s32 nIndex, void * pParam);

	/* Seq data  */
	BOOL RegSeqCallback(s32 nIndex, CameraSeqCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegSeqCallback(s32 nIndex, void * pParam);
	
	BOOL RegDelCallback(s32 nIndex, CameraDelCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegDelCallback(s32 nIndex, void * pParam);

	BOOL GetCameraOnline(s32 nIndex, BOOL &bStatus);
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
	BOOL SearchItems(s32 cameraId, u32 startTime, u32 endTime, u32 recordType, 
					RecordItemMap &map);
	BOOL SearchHasItems(s32 cameraId, u32 startTime, u32 endTime, 
					u32 recordType);
	BOOL SearchAItem(s32 cameraId, u32 Time, VdbRecordItem &pItem);
	BOOL SearchAItemNear(s32 cameraId, u32 Time, VdbRecordItem &pItem);
	BOOL SearchNextItem(s32 cameraId, s64 LastId, VdbRecordItem &pItem);
	BOOL RequestAMFRead(VdbRecordItem &pItem, astring & strPath);
	BOOL FinishedAMFRead(VdbRecordItem &pItem, astring & strPath);

	VDB & GetVdb();
public:
	void Lock(){m_Lock.lock();}
	bool TryLock(){return m_Lock.try_lock();}
	void UnLock(){m_Lock.unlock();}

public:
	/* Camera */
	s32 GetCameraID(void);
	BOOL ReleaseCameraID(s32 nID);
	BOOL LockCameraID(s32 nID);

	/* VIPC */
	s32 GetVIPCID(void);
	BOOL ReleaseVIPCID(s32 nID);
	BOOL LockVIPCID(s32 nID);
public:
	static void Run(void * pParam);
	void run();

private:
	CameraMap m_CameraMap;
	CameraParamMap m_CameraParamMap;
	CameraOnlineMap m_CameraOnlineMap;

	/* Virtual IP camera param */
	VIPCCameraParamMap m_VIPCCameraParamMap;
	fast_mutex m_Lock;
	tthread::thread *m_pThread;

private:
	CameraChangeNofityMap m_CameraChange;

private:
	VDB *m_pVdb;
	VHdfsDB *m_pVHdfsdb;
	ClientFactoryHddTask *m_HddTask;

private:
	s8 m_strCameraMap[CLIENT_FACTORY_CAMERA_ID_MAX];
	s8 m_strVIPCMap[CLIENT_FACTORY_CAMERA_ID_MAX];
	ConfDB m_Conf;
	SysDB m_SysPath;
private:
	//CmnHttpServer *m_pHttpServer;
};

typedef ClientFactory* LPClientFactory;


#include "clientfactoryimpl.hpp"

#endif // __VSC_CLIENT_FACTORY_H_
