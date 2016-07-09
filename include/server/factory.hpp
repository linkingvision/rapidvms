//------------------------------------------------------------------------------
// File: factory.hpp
//
// Desc: Camera factory - Manage IP Camera.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_FACTORY_H_
#define __VSC_FACTORY_H_
#include "config/confdb.hpp"
#include "server/camera.hpp"
#include "vdb.hpp"
#include "vhdfsdb.hpp"
#include "vplay.hpp"
#include "config/sysdb.hpp"
#include "server/hdddevice.hpp"
#include <QThread>
#include <qdebug.h>
#include "Poco/Path.h"
#include "Poco/File.h"
#include "config/vidconf.pb.h"

using namespace VidConf;

typedef enum
{
    FACTORY_CAMERA_ADD = 1,
    FACTORY_CAMERA_DEL,
    FACTORY_CAMERA_ONLINE,
    FACTORY_CAMERA_OFFLINE,
    FACTORY_CAMERA_REC_ON,
    FACTORY_CAMERA_REC_OFF,
    FACTORY_CAMERA_HDFS_REC_ON,
    FACTORY_CAMERA_HDFS_REC_OFF,

    FACTORY_CAMERA_LAST
} FactoryCameraChangeType;


class FactoryCameraChangeData
{
public:
	FactoryCameraChangeType type;
	astring id;
	//VidCamera cCam;
};

typedef BOOL (*FactoryCameraChangeNotify)(void* pParam, 
		FactoryCameraChangeData data);

typedef std::list<LPCamera> CameraList;
typedef std::list<CameraParam> CameraParamList;
typedef std::map<astring, LPCamera> CameraMap;
typedef std::map<astring, CameraParam> CameraParamMap;
typedef std::map<astring, bool> CameraOnlineMap;
typedef std::map<astring, bool> CameraRecMap;
typedef std::map<void *, FactoryCameraChangeNotify> CameraChangeNofityMap;

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
	s32 InitAddCamera(CameraParam & pParam, astring strCamId);
	ConfDB &GetConfDB(){return m_Conf;};
	
public:
	BOOL RegCameraChangeNotify(void * pData, FactoryCameraChangeNotify callback);
	BOOL UnRegCameraChangeNotify(void * pData);
	BOOL CallCameraChange(FactoryCameraChangeData data);
	static BOOL RecChangeHandler(astring strId, bool bRec, void * pParam);
	BOOL RecChangeHandler1(astring strId, bool bRec);
	
public:
	BOOL GetLicense(astring &strLicense, astring &strHostId, 
							int &ch, astring &type, astring &expireTime);
	BOOL SetLicense(astring &strLicense);
	BOOL InitLicense();

	BOOL GetExportPath(astring &strPath);
	BOOL SetExportPath(astring &strPath);

	BOOL GetEventDBConf(VidEventDBConf &pConf);

public:
	bool AuthUser(astring &strUser, astring &strPasswd);
	bool GetAdminPasswd(astring &strPasswd);
	bool SetAdminPasswd(astring strPasswd);
public:
	BOOL GetCameraOnlineMap(CameraOnlineMap &pMap);
	BOOL GetCameraRecMap(CameraRecMap &pMap);
	bool GetCameraList(VidCameraList & pCameraList);

       /* Camera function */
	astring AddCamera(CameraParam & pParam);
	BOOL GetCameraRtspUrl(astring & strUrl, astring strCamId);
	BOOL DelCamera(astring strCamId);
	BOOL GetCamera(astring strId, VidCamera & pCam);
	BOOL PtzAction(astring strCamId, FPtzAction action, float speed);
	BOOL UpdateRecSched(astring strCamId, VidCamera &pCam);

public:
	/* Disk function */
	BOOL AddHdd(astring &strHdd, astring & strPath, s64 nSize);
	BOOL DelHdd(astring & strHdd);
	BOOL HddUpdateSize(astring &strHdd, s64 nSize);
	BOOL GetDiskMap(VDBDiskMap &pMap);
	BOOL GetDiskStatus(VDBDiskStatus &pStatus);
	BOOL UpdateDiskStatusMap(VDBDiskStatus &pStatus);

	/* Search function */
	BOOL SearchItems(astring strCamId, u32 startTime, u32 endTime, u32 recordType, 
					RecordItemMap &map);
	BOOL SearchHasItems(astring strCamId, u32 startTime, u32 endTime, 
					u32 recordType);

	VDB & GetVdb();


public:
	BOOL GetStreamInfo(astring strCamId, VideoStreamInfo &pInfo);
	BOOL GetCamStreamList(astring strCamId, VidStreamList &pList);
	/* Data */
	BOOL RegDataCallback(astring strCamId, CameraDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegDataCallback(astring strCamId, void * pParam);
	BOOL GetInfoFrame(astring strCamId, InfoFrame &pFrame);
	BOOL RegSubDataCallback(astring strCamId, CameraDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegSubDataCallback(astring strCamId, void * pParam);
	BOOL GetSubInfoFrame(astring strCamId, InfoFrame &pFrame);

	BOOL GetCameraOnline(astring strCamId, BOOL &bStatus);
	BOOL SetSystemPath(astring &strPath);

public:
	void run();

private:
	CameraMap m_CameraMap;
	CameraOnlineMap m_CameraOnlineMap;
	CameraRecMap m_CameraRecMap;

	XMutex m_cMutex;

private:
	CameraChangeNofityMap m_CameraChange;

private:
	VDB *m_pVdb;
	VHdfsDB *m_pVHdfsdb;
	FactoryHddTask *m_HddTask;

private:
	ConfDB m_Conf;
	SysDB m_SysPath;
};

typedef Factory* LPFactory;


#include "factoryimpl.hpp"

#endif // __VSC_FACTORY_H_
