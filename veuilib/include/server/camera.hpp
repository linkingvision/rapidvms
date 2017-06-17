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

#ifndef __VSC_CAMERA_H_
#define __VSC_CAMERA_H_

#define NOMINMAX
#ifdef WIN32
//#include <Windows.h>
//#include <Winbase.h>
#endif 

#include <algorithm>

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
#include "Poco/String.h"
#include "config/confdb.hpp"

#include "vvidonvif/vvidonvifc.hpp"

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

class VE_LIBRARY_API CameraParam
{
public:
	CameraParam();
	CameraParam(const CameraParam &pParam);
	CameraParam(VidCamera &pData);
	~CameraParam();
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
	BOOL UpdateUrl();
	BOOL UpdateUrlOnvif();
	BOOL CheckOnline();
	BOOL UpdateDefaultUrl();

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
	VidStreamList m_cStreamList;
};

/* 
 * Add Channel number for NVR integration support, a camera is a device
 * ONVIFCamera -> Camera, 1 channel 
 * File               -> Camera, 1 channel 
 * RTSP             -> Camera, 1 channel
 * USB              -> Camera, 1 channel
 * MJPEG           -> Camera, 1 channel
 * LinkVision      -> Camera, register
 * Recorder        -> Camera, dynamic channel
*/

class VE_LIBRARY_API Camera
{
public:
	Camera(ConfDB &pConfDB, VDB &pVdb, 
					VHdfsDB &pVHdfsdb, const CameraParam &pParam, 
					RecChangeFunctionPtr pCallback, void *pCallbackParam);
	~Camera();

public:
	/* Below api is for a new thread to do some network task whitch may be blocked */
	BOOL GetCameraParam(CameraParam &pParam);

public:	
	BOOL StartData();
	BOOL StopData();
	
	BOOL StartSubData();
	BOOL StopSubData();
	bool UpdateRecSched(VidCamera &pCam);
	
	CameraStatus CheckCamera(astring strUrl, astring strUrlSubStream, 
		BOOL bHasSubStream, BOOL bOnline, BOOL bOnlineUrl, 
		VidStreamList cStreamlist);

public:
	/* Data  */
	static BOOL DataHandler(void* pData, VideoFrame& frame);
	BOOL DataHandler1(VideoFrame& frame);
	static BOOL SubDataHandler(void* pData, VideoFrame& frame);
	BOOL SubDataHandler1(VideoFrame& frame);

public:
	void Lock(){m_Lock.lock();}
	void UnLock(){m_Lock.unlock();}
	void SubLock(){m_SubLock.lock();}
	void SubUnLock(){m_SubLock.unlock();}

public:
	/* Data  callback*/
	BOOL RegDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegDataCallback(void * pParam);
	BOOL RegSubDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegSubDataCallback(void * pParam);
	
	BOOL RegDelCallback(CameraDelCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegDelCallback(void * pParam);
	
	BOOL GetInfoFrame(InfoFrame &pFrame);
	BOOL GetiFrame(VideoFrame& frame);
	BOOL GetSubInfoFrame(InfoFrame &pFrame);
	
	BOOL GetCameraOnline();
	
	BOOL AttachPlayer(HWND hWnd, int w, int h);
	BOOL UpdateWidget(HWND hWnd, int w, int h);
	BOOL DetachPlayer(HWND hWnd);
	BOOL GetStreamInfo(VideoStreamInfo &pInfo);
	BOOL GetCamStreamList(VidStreamList &pList);

	BOOL ShowAlarm(HWND hWnd);
	BOOL PtzAction(FPtzAction action, float speed);
	BOOL UpdatePTZConf();
	BOOL FireAlarm(s64 nStartTime);

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
	VideoFrame m_iFrameCache;

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
	VVidOnvifCPtz m_ptz;
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

#endif // __VSC_CAMERA_H_
