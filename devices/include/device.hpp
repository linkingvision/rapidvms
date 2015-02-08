//------------------------------------------------------------------------------
// File: device.hpp
//
// Desc: Device factory - Manage IP Camera.
//
// Copyright (c) 2014-2018 opencvr.com All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_DEVICE_H_
#define __VSC_DEVICE_H_

#define NOMINMAX
#ifdef WIN32
#include <Windows.h>
#include <Winbase.h>
#endif 

#include "utility.hpp"
#include "conf.hpp"
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



using namespace UtilityLib;
using namespace std;
using namespace ONVIF;

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
} DeviceStatus;

#ifdef WIN32
typedef void (__cdecl * DeviceDataCallbackFunctionPtr)(VideoFrame& frame, void * pParam);
typedef void (__cdecl * DeviceRawCallbackFunctionPtr)(RawFrame& frame, void * pParam);
typedef void (__cdecl * DeviceDelCallbackFunctionPtr)(void * pParam);

#else
typedef void ( * DeviceDataCallbackFunctionPtr)(VideoFrame& frame, void * pParam);
typedef void ( * DeviceRawCallbackFunctionPtr)(RawFrame& frame, void * pParam);
typedef void ( * DeviceDelCallbackFunctionPtr)(void * pParam);

#endif
typedef std::map<void *, DeviceDataCallbackFunctionPtr> DeviceDataCallbackMap;
typedef std::map<void *, DeviceRawCallbackFunctionPtr> DeviceRawCallbackMap;
typedef std::map<void *, DeviceDelCallbackFunctionPtr> DeviceDelCallbackMap;

class DeviceParam
{
public:
	inline DeviceParam();
	inline DeviceParam(const DeviceParam &pParam);
	inline DeviceParam(VSCDeviceData &pData);
	inline ~DeviceParam();
	DeviceParam & operator=(const DeviceParam &pParam)
	{
		memset(&m_Conf, 0, sizeof(m_Conf));
		memcpy(&m_Conf, &(pParam.m_Conf), sizeof(m_Conf));
		m_strUrl = pParam.m_strUrl;
		m_strUrlSubStream = pParam.m_strUrlSubStream;
		m_bHasSubStream = pParam.m_bHasSubStream;
		return *this;
	}

public:
	inline BOOL UpdateUrl();
	inline BOOL UpdateUrlOnvif();
	inline BOOL CheckOnline();

public:
	VSCDeviceData m_Conf;
	BOOL m_bOnvifUrlGetted;
	astring m_strUrl;
	astring m_strUrlSubStream;
	BOOL m_bHasSubStream;
};


class VIPCDeviceParam
{
public:
	inline VIPCDeviceParam()
	{
		memset(&m_Conf, 0, sizeof(m_Conf));
		VSCVIPCDataItemDefault(m_Conf.data.conf);
	}
	inline VIPCDeviceParam(const VIPCDeviceParam &pParam)
	{
		memset(&m_Conf, 0, sizeof(m_Conf));
		memcpy(&m_Conf, &(pParam.m_Conf), sizeof(m_Conf));
	}
	inline VIPCDeviceParam(VSCVIPCData &pData)
	{
		memset(&m_Conf, 0, sizeof(m_Conf));
		memcpy(&m_Conf, &(pData), sizeof(m_Conf));
	}
	inline ~VIPCDeviceParam(){}
	VIPCDeviceParam & operator=(const VIPCDeviceParam &pParam)
	{
		memset(&m_Conf, 0, sizeof(m_Conf));

		memcpy(&m_Conf, &(pParam.m_Conf), sizeof(m_Conf));
		return *this;
	}

public:
	VSCVIPCData m_Conf;
};

class Device
{
public:
	inline Device(VDB &pVdb, VHdfsDB &pVHdfsdb, const DeviceParam &pParam);
	inline ~Device();

public:	
	inline BOOL StartData();
	inline BOOL StopData();
	
	inline BOOL StartSubData();
	inline BOOL StopSubData();

	inline BOOL StartRaw();
	inline BOOL StopRaw();
	
	inline BOOL StartSubRaw();
	inline BOOL StopSubRaw();
	
	inline BOOL StartRecord();
	inline BOOL StopRecord();
	inline BOOL StartHdfsRecord();
	inline BOOL StopHdfsRecord();
	
	inline BOOL SetRecord(BOOL bRecording);
	inline BOOL SetHdfsRecord(BOOL bRecording);
	inline DeviceStatus CheckDevice();

public:
	/* Data  */
	inline static BOOL DataHandler(void* pData, VideoFrame& frame);
	inline BOOL DataHandler1(VideoFrame& frame);
	inline static BOOL SubDataHandler(void* pData, VideoFrame& frame);
	inline BOOL SubDataHandler1(VideoFrame& frame);
	/* Raw Data */
	inline static BOOL RawHandler(void* pData, RawFrame& frame);
	inline BOOL RawHandler1(RawFrame& frame);
	inline static BOOL SubRawHandler(void* pData, RawFrame& frame);
	inline BOOL SubRawHandler1(RawFrame& frame);

public:
	inline void Lock(){m_Lock.lock();}
	inline void UnLock(){m_Lock.unlock();}
	inline void SubLock(){m_SubLock.lock();}
	inline void SubUnLock(){m_SubLock.unlock();}

public:
	/* Data  callback*/
	inline BOOL RegDataCallback(DeviceDataCallbackFunctionPtr pCallback, void * pParam);
	inline BOOL UnRegDataCallback(void * pParam);
	inline BOOL RegSubDataCallback(DeviceDataCallbackFunctionPtr pCallback, void * pParam);
	inline BOOL UnRegSubDataCallback(void * pParam);

	/*Raw Data  callback*/
	inline BOOL RegRawCallback(DeviceRawCallbackFunctionPtr pCallback, void * pParam);
	inline BOOL UnRegRawCallback(void * pParam);
	inline BOOL RegSubRawCallback(DeviceRawCallbackFunctionPtr pCallback, void * pParam);
	inline BOOL UnRegSubRawCallback(void * pParam);


	inline BOOL RegDelCallback(DeviceDelCallbackFunctionPtr pCallback, void * pParam);
	inline BOOL UnRegDelCallback(void * pParam);
	
	inline BOOL GetInfoFrame(InfoFrame &pFrame);
	inline BOOL GetSubInfoFrame(InfoFrame &pFrame);
	inline BOOL Cleanup();
	
	inline BOOL GetDeviceOnline();
	inline BOOL GetUrl(std::string &url);
	inline BOOL GetDeviceRtspUrl(astring & strUrl);
	
	inline BOOL AttachPlayer(HWND hWnd, int w, int h);
	inline BOOL UpdateWidget(HWND hWnd, int w, int h);
	inline BOOL DetachPlayer(HWND hWnd);

	inline BOOL EnablePtz(HWND hWnd, bool enable);
	inline BOOL DrawPtzDirection(HWND hWnd, int x1, int y1, int x2,  int y2);
	inline BOOL ClearPtzDirection(HWND hWnd);
	inline BOOL ShowAlarm(HWND hWnd);
	inline BOOL PtzAction(FPtzAction action, float speed);
	inline BOOL UpdatePTZConf();

private:
	VPlay m_vPlay;
	VPlay m_vPlaySubStream;
	BOOL m_bStarted;
	BOOL m_bSubStarted;
	DeviceDataCallbackMap m_DataMap;
	DeviceDataCallbackMap m_SubDataMap;

	DeviceRawCallbackMap m_RawMap;
	DeviceRawCallbackMap m_SubRawMap;

	DeviceDelCallbackMap m_DelMap;

private:
	s32 m_nDeviceType;
	s32 m_nDeviceSubType;
	DeviceParam m_param;
	tthread::thread *m_pThread;
	fast_mutex m_Lock;
	fast_mutex m_SubLock;
private:
	VDB &m_pVdb;
	VHdfsDB &m_pVHdfsdb;
	RecordSession *m_pRecord;
	HdfsRecSession *m_pHdfsRecord;
	BOOL m_Online;
	BOOL m_OnlineUrl;

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

	s32 m_nRawRef;
	s32 m_nSubRawRef;
};

typedef DeviceParam* LPDeviceParam;
typedef Device* LPDevice;

#include "deviceimpl.hpp"
#endif // __VSC_DEVICE_H_
