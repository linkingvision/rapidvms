//------------------------------------------------------------------------------
// File: storfactory.hpp
//
// Desc: storfactory - Manage Vid Stor.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_STOR_FACTORY_H_
#define __VSC_STOR_FACTORY_H_
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
#include "client/storcmn.hpp"
#include "client/storclient.hpp"
#include "config/clientconfdb.hpp"

#include "Poco/UUIDGenerator.h"

using namespace Poco;
using namespace VidConf;

typedef std::list<LPStorClient> StorClientList;
typedef std::map<astring, LPStorClient> StorClientMap;

class StorFactory: public QObject , public StorFactoryNotifyInterface
{
    Q_OBJECT
public:
    StorFactory(ClientConfDB &pConf);
    ~StorFactory();
public:
	/* Init function */
	BOOL Init();
	bool InitAddStor(VidStor & pParam);
public:
	bool AddStor(VidStor & pParam);
	bool DeleteStor(astring strId);

public:
	bool AddCam(astring strStorId, VidCamera &pParam);
	bool DeleteCam(astring strStorId, astring strId);
	bool PtzCmd(astring strStorId, astring strId, u32 action, double param);
	
public:
	BOOL RegChangeNotify(void * pData, StorFactoryChangeNotify callback);
	virtual bool CallChange(StorFactoryChangeData data);

	
public:
	VidCameraList GetVidCameraList(astring strStor);
	astring GetVidCameraName(astring strStor, astring strCam);
	StorClientOnlineMap GetVidCameraOnlineList(astring strStor);
	StorClientRecMap GetVidCameraRecList(astring strStor);
	bool GetOnline(astring strStor);
#if 0
public:
	/* UI can use this for display camera tree */
	BOOL GetCameraParamMap(CameraParamMap &pMap);
	BOOL GetCameraOnlineMap(CameraOnlineMap &pMap);
	
    /* Camera function */
	s32 AddCamera(CameraParam & pParam);
	s32 GetCameraParamById(CameraParam & pParam, s32 nIndex);

	s32 GetCameraParamByIdTryLock(CameraParam & pParam, s32 nIndex);
	BOOL DelCamera(s32 nIndex);

	
	/* Video play function */
	BOOL AttachPlayer(s32 nIndex, HWND hWnd, int w, int h, 
						CameraDelCallbackFunctionPtr pCallback, void * pParam);
	BOOL UpdateWidget(s32 nIndex, HWND hWnd, int w, int h);
	BOOL DetachPlayer(s32 nIndex, HWND hWnd, void * pParam);
	BOOL GetStreamInfo(s32 nIndex, VideoStreamInfo &pInfo);

	BOOL PtzAction(s32 nIndex, FPtzAction action, float speed);
	BOOL ShowAlarm(s32 nIndex, HWND hWnd);
	
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
#endif

public:
	void run();

private:
	StorClientMap m_StorClientMap;
	StorClientOnlineMap m_StorClientOnlineMap;

	XMutex m_cMutex;
private:
	StorChangeNofityMap m_Change;
	
private:
	ClientConfDB &m_pConf;
};

typedef StorFactory* LPStorFactory;


#include "storfactoryimpl.hpp"

#endif // __VSC_STOR_FACTORY_H_
