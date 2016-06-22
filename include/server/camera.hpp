//------------------------------------------------------------------------------
// File: camera.hpp
//
// Desc: Camera factory - Manage IP Camera.
//
// Copyright (c) 2014-2018 opencvr.com All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_CAMERA_H_
#define __VSC_CAMERA_H_

#define NOMINMAX
#ifdef WIN32
#include <Windows.h>
#include <Winbase.h>
#endif 

#include "utility.hpp"
#include "config/conf.hpp"
#include "devicemanagement.h"
#include "ptzmanagement.h"
#include "media_management/profiles.h"
#include "media_management/streamuri.h"
#include "device_management/capabilities.h"
#include "mediamanagement.h"
#include "vdb.hpp"
#include "vhdfsdb.hpp"
#include "hdfsrecsession.hpp"
#include "vplay.hpp"
#include "debug.hpp"
#include "config/vidconf.pb.h"
#include "Poco/UUIDGenerator.h"
#include "recordwrapper.hpp"
#include "Poco/URI.h"

using namespace VidConf;
using namespace UtilityLib;
using namespace std;
using namespace ONVIF;
//using namespace Poco;

typedef enum
{
	F_PTZ_UP = 1,
	F_PTZ_DOWN,
	F_PTZ_LEFT,
	F_PTZ_RIGHT,
	F_PTZ_ZOOM_IN,
	F_PTZ_ZOOM_OUT,
	F_PTZ_STOP,
	F_PTZ_LAST
} FPtzAction;

typedef enum
{
	DEV_OFF2ON = 1,
	DEV_ON2OFF,
	DEV_NO_CHANGE,
	DEV_LAST
} CameraStatus;


/* Camera video data callback */
#ifdef WIN32__
/* Compressed data callback */
typedef void (__cdecl * CameraDataCallbackFunctionPtr)(VideoFrame& frame, void * pParam);
/* Raw data callback */
typedef void (__cdecl * CameraRawCallbackFunctionPtr)(RawFrame& frame, void * pParam);
/* Video Seq callback */
typedef void (__cdecl * CameraSeqCallbackFunctionPtr)(VideoSeqFrame& frame, void * pParam);
/* Camera Delete function */
typedef void (__cdecl * CameraDelCallbackFunctionPtr)(void * pParam);

#else
/* Compressed data callback */
typedef void ( * CameraDataCallbackFunctionPtr)(VideoFrame& frame, void * pParam);
/* Raw data callback */
typedef void ( * CameraRawCallbackFunctionPtr)(RawFrame& frame, void * pParam);
/* Video Seq callback */
typedef void ( * CameraSeqCallbackFunctionPtr)(VideoSeqFrame& frame, void * pParam);
/* Camera Delete function */
typedef void ( * CameraDelCallbackFunctionPtr)(void * pParam);
#endif
typedef std::map<void *, CameraDataCallbackFunctionPtr> CameraDataCallbackMap;
typedef std::map<void *, CameraRawCallbackFunctionPtr> CameraRawCallbackMap;
typedef std::map<void *, CameraSeqCallbackFunctionPtr> CameraSeqCallbackMap;
typedef std::map<void *, CameraDelCallbackFunctionPtr> CameraDelCallbackMap;

class CameraParam
{
public:
	inline CameraParam();
	inline CameraParam(const CameraParam &pParam);
	inline CameraParam(VidCamera &pData);
	inline ~CameraParam();
	CameraParam & operator=(const CameraParam &pParam)
	{
		m_Conf = pParam.m_Conf;
		if (pParam.m_strUrl.length() > 0)
		{
			m_strUrl = pParam.m_strUrl;
		}
		m_strUrlSubStream = pParam.m_strUrlSubStream;
		m_bHasSubStream = pParam.m_bHasSubStream;
		m_bOnvifUrlGetted = pParam.m_bOnvifUrlGetted;
		m_Online = pParam.m_Online;
		return *this;
	}

public:
	inline BOOL UpdateUrl();
	inline BOOL UpdateUrlOnvif();
	inline BOOL CheckOnline();
	inline BOOL UpdateDefaultUrl();

public:
	VidCamera m_Conf;
	BOOL m_bOnvifUrlGetted;
	astring m_strUrl;
	astring m_strUrlSubStream;
	BOOL m_bHasSubStream;

	BOOL m_Online;
	BOOL m_OnlineUrl;

	/* backend status  */
	BOOL m_wipOnline;
	BOOL m_wipOnlineUrl;
};

class Camera
{
public:
	inline Camera(ConfDB &pConfDB, VDB &pVdb, 
					VHdfsDB &pVHdfsdb, const CameraParam &pParam, 
					RecChangeFunctionPtr pCallback, void *pCallbackParam);
	inline ~Camera();

public:
	/* Below api is for a new thread to do some network task whitch may be blocked */
	inline BOOL GetCameraParam(CameraParam &pParam);

public:	
	inline BOOL StartData();
	inline BOOL StopData();
	
	inline BOOL StartSubData();
	inline BOOL StopSubData();
	inline bool UpdateRecSched(VidCamera &pCam);
	
	inline CameraStatus CheckCamera(astring strUrl, astring strUrlSubStream, 
		BOOL bHasSubStream, BOOL bOnline, BOOL bOnlineUrl);

public:
	/* Data  */
	inline static BOOL DataHandler(void* pData, VideoFrame& frame);
	inline BOOL DataHandler1(VideoFrame& frame);
	inline static BOOL SubDataHandler(void* pData, VideoFrame& frame);
	inline BOOL SubDataHandler1(VideoFrame& frame);

public:
	inline void Lock(){m_Lock.lock();}
	inline void UnLock(){m_Lock.unlock();}
	inline void SubLock(){m_SubLock.lock();}
	inline void SubUnLock(){m_SubLock.unlock();}

public:
	/* Data  callback*/
	inline BOOL RegDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam);
	inline BOOL UnRegDataCallback(void * pParam);
	inline BOOL RegSubDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam);
	inline BOOL UnRegSubDataCallback(void * pParam);
	
	inline BOOL RegDelCallback(CameraDelCallbackFunctionPtr pCallback, void * pParam);
	inline BOOL UnRegDelCallback(void * pParam);
	
	inline BOOL GetInfoFrame(InfoFrame &pFrame);
	inline BOOL GetSubInfoFrame(InfoFrame &pFrame);
	
	inline BOOL GetCameraOnline();
	
	inline BOOL AttachPlayer(HWND hWnd, int w, int h);
	inline BOOL UpdateWidget(HWND hWnd, int w, int h);
	inline BOOL DetachPlayer(HWND hWnd);
	inline BOOL GetStreamInfo(VideoStreamInfo &pInfo);

	inline BOOL ShowAlarm(HWND hWnd);
	inline BOOL PtzAction(FPtzAction action, float speed);
	inline BOOL UpdatePTZConf();

private:
	VPlay *m_pvPlay;
	VPlay *m_pvPlaySubStream;
	VPlay &m_vPlay;
	VPlay &m_vPlaySubStream;
	BOOL m_bStarted;
	BOOL m_bSubStarted;
	CameraDataCallbackMap m_DataMap;
	CameraDataCallbackMap m_SubDataMap;

	CameraDelCallbackMap m_DelMap;

private:
	CameraParam m_param;
	fast_mutex m_Lock;
	fast_mutex m_SubLock;

private:
	VDB &m_pVdb;
	RecordSession *m_pRecord;
	RecordSchedWeekMap m_cSchedMap;
	RecordWrapper m_cRecordWrapper;
	
private:
	ContinuousMove m_continuousMove;
	ONVIF::Stop m_stop;
	PtzManagement *m_ptz;
	BOOL m_ptzInited;

	InfoFrame m_infoData;
	BOOL m_bGotInfoData;
	s32 m_nDataRef;

private:
	InfoFrame m_infoSubData;
	BOOL m_bGotInfoSubData;
	s32 m_nSubDataRef;

private:
	ConfDB &m_pConfDB;
};

typedef CameraParam* LPCameraParam;
typedef Camera* LPCamera;

#include "cameraimpl.hpp"
#endif // __VSC_CAMERA_H_
