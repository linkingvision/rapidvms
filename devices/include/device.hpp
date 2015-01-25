//------------------------------------------------------------------------------
// File: device.hpp
//
// Desc: Device factory - Manage IP Camera.
//
// Copyright (c) 2014-2018 vdceye.com All rights reserved.
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
#include "datasink.hpp"
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
#else
typedef void ( * DeviceDataCallbackFunctionPtr)(VideoFrame& frame, void * pParam);
typedef void ( * DeviceRawCallbackFunctionPtr)(RawFrame& frame, void * pParam);

#endif
typedef std::map<void *, DeviceDataCallbackFunctionPtr> DeviceDataCallbackMap;
typedef std::map<void *, DeviceRawCallbackFunctionPtr> DeviceRawCallbackMap;

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

inline string GetProgramDir()
{
#ifdef WIN32
    char exeFullPath[MAX_PATH]; // Full path

    string strPath = "";

    GetModuleFileNameA(NULL,exeFullPath, MAX_PATH);
    strPath=(string)exeFullPath;    // Get full path of the file

    int pos = strPath.find_last_of('\\', strPath.length());

    return strPath.substr(0, pos);  // Return the directory without the file name
#else
    return "ve/";
#endif
}


DeviceParam::DeviceParam()
{
	static int CameraNum = 0;
	memset(&m_Conf, 0, sizeof(m_Conf));
	m_Conf.data.conf.nId = 0;
	m_Conf.data.conf.nType = VSC_DEVICE_CAM;
	m_Conf.data.conf.nSubType = VSC_SUB_DEVICE_ONVIF;

	sprintf(m_Conf.data.conf.Name, "Camera");
	//strcpy(m_Conf.data.conf.Name, "CAMERA ");

	strcpy(m_Conf.data.conf.IP, "192.168.0.1");
	strcpy(m_Conf.data.conf.Port, "80");
	strcpy(m_Conf.data.conf.User, "admin");
	strcpy(m_Conf.data.conf.Password, "admin");

	strcpy(m_Conf.data.conf.RtspLocation, "/");
	string filePath = GetProgramDir() +  "/camera.mov";
	strcpy(m_Conf.data.conf.FileLocation, filePath.c_str());
	strcpy(m_Conf.data.conf.OnvifAddress, "/onvif/device_service");
	strcpy(m_Conf.data.conf.CameraIndex, "1");

	m_Conf.data.conf.UseProfileToken = 0;
	//m_Conf.data.conf.Recording = 1;//Default start recording
	m_Conf.data.conf.Recording = 0;//Default start recording//xsmart
	strcpy(m_Conf.data.conf.OnvifProfileToken, "quality_h264");
	strcpy(m_Conf.data.conf.OnvifProfileToken2, "second_h264");
	m_bOnvifUrlGetted = FALSE;
	m_bHasSubStream = FALSE;

}

DeviceParam::DeviceParam(VSCDeviceData &pData)
{
    memset(&m_Conf, 0, sizeof(m_Conf));

    memcpy(&m_Conf, &(pData), sizeof(m_Conf));
    //UpdateUrl();
}

inline std::string Replace(std::string &str, const char *string_to_replace, const char *new_string)
{
  // Find the first string to replace
  int index = str.find(string_to_replace);
  // while there is one
  while(index != std::string::npos)
  {
    // Replace it
    str.replace(index, strlen(string_to_replace), new_string);
    // Find the next one
    index = str.find(string_to_replace, index + strlen(new_string));
  }
  return str;
}

BOOL DeviceParam::CheckOnline()
{
    if (m_Conf.data.conf.nSubType == VSC_SUB_DEVICE_FILE 
		|| m_Conf.data.conf.nSubType == VSC_SUB_DEVICE_RTSP)
    {
    	return TRUE;
    }
    astring IP = m_Conf.data.conf.IP;
    astring Port = m_Conf.data.conf.Port;
    astring User = m_Conf.data.conf.User;
    astring Password = m_Conf.data.conf.Password;
    astring OnvifAddress = m_Conf.data.conf.OnvifAddress;

    astring OnvifDeviceService = "http://" + IP + ":" + Port + OnvifAddress;
	astring url = "rtsp://" + IP + ":" + "554" + "/";
     VDC_DEBUG( "%s  tryCheckDevice Begin \n",__FUNCTION__);
     if (TryCheckDevice(IP, Port) == false)
     {
       VDC_DEBUG( "%s tryCheckDevice False \n",__FUNCTION__);    
	return false;
     }
     VDC_DEBUG( "%s  tryCheckDevice End \n",__FUNCTION__);
    DeviceManagement *pDm = new DeviceManagement(OnvifDeviceService.c_str(), 
                            User.c_str(), Password.c_str());
    
    if (pDm  == NULL)
    {
        VDC_DEBUG( "%s new DeviceManagement error \n",__FUNCTION__);
        return FALSE;
    }
    
    Capabilities * pMediaCap = pDm->getCapabilitiesMedia();
    if (pMediaCap == NULL)
    {
        VDC_DEBUG( "%s getCapabilitiesMedia error \n",__FUNCTION__);
        delete pDm;
        return FALSE;
    }

    return TRUE;
}

BOOL DeviceParam::UpdateUrlOnvif()
{
	astring IP = m_Conf.data.conf.IP;
	astring Port = m_Conf.data.conf.Port;
	astring User = m_Conf.data.conf.User;
	astring Password = m_Conf.data.conf.Password;
	astring OnvifAddress = m_Conf.data.conf.OnvifAddress;

	astring OnvifDeviceService = "http://" + IP + ":" + Port + OnvifAddress;
	astring url = "rtsp://" + IP + ":" + "554" + "/";
	astring urlSubStream = "rtsp://" + IP + ":" + "554" + "/";

	DeviceManagement *pDm = new DeviceManagement(OnvifDeviceService.c_str(), 
	                        User.c_str(), Password.c_str());

	if (pDm  == NULL)
	{
	    VDC_DEBUG( "%s new DeviceManagement error \n",__FUNCTION__);
	    return FALSE;
	}

	Capabilities * pMediaCap = pDm->getCapabilitiesMedia();
	if (pMediaCap == NULL)
	{
	    VDC_DEBUG( "%s getCapabilitiesMedia error \n",__FUNCTION__);
	    delete pDm;
	    return FALSE;
	}

	MediaManagement *pMedia = new MediaManagement(pMediaCap->mediaXAddr(), 
	                            User.c_str(), Password.c_str());
	if (pMedia == NULL)
	{
	    VDC_DEBUG( "%s new MediaManagement error \n",__FUNCTION__);
	    delete pDm;
	    delete pMediaCap;
	    return FALSE;
	}

	Profiles *pProfileS = pMedia->getProfiles();
	if (pProfileS == NULL)
	{
	    VDC_DEBUG( "%s new getProfiles error \n",__FUNCTION__);
	    delete pDm;
	    delete pMediaCap;
	    delete pMedia;
	    return FALSE;
	}
	if (pProfileS->m_toKenPro.size() > 0)
	{
	    VDC_DEBUG( "%s m_toKenPro size %d \n",__FUNCTION__, pProfileS->m_toKenPro.size());
		QString strToken;
		if (m_Conf.data.conf.UseProfileToken == 1)
		{
			strToken = m_Conf.data.conf.OnvifProfileToken;
		}else
		{
			strToken = pProfileS->m_toKenPro[0];
			strcpy(m_Conf.data.conf.OnvifProfileToken, pProfileS->m_toKenPro[0].toStdString().c_str());
		}
		StreamUri *pUri = pMedia->getStreamUri(strToken);
		if (pUri)
		{
			m_strUrl = pUri->uri().toStdString();
			delete pUri;
		}
	}
	if (pProfileS->m_toKenPro.size() > 1)
	{
		VDC_DEBUG( "%s m_toKenPro SubStream size %d \n",__FUNCTION__, pProfileS->m_toKenPro.size());
		QString strToken;
		if (m_Conf.data.conf.UseProfileToken == 1)
		{
			strToken = m_Conf.data.conf.OnvifProfileToken2;
		}else
		{
			strToken = pProfileS->m_toKenPro[1];
			strcpy(m_Conf.data.conf.OnvifProfileToken2, pProfileS->m_toKenPro[1].toStdString().c_str());
		}
		StreamUri *pUri = pMedia->getStreamUri(strToken);
		if (pUri)
		{
			m_strUrlSubStream = pUri->uri().toStdString();
			m_bHasSubStream = TRUE;
			delete pUri;
		}
	}

	/* rtsp://admin:12345@192.168.1.1:554/Streaming/Channels/1\
	?transportmode=unicast&profile=Profile_1 */
	//astring urlWithUser = "rtsp://" + User + ":" + Password + "@";
	//Replace(strUrl, "rtsp://", urlWithUser.c_str());

	m_bOnvifUrlGetted = TRUE;

	delete pDm;
	delete pMediaCap;
	delete pMedia;
	delete pProfileS;
	
	return TRUE;
}

BOOL DeviceParam::UpdateUrl()
{
    //TODO RTSP ONVIF call onvif sdk to get a Stream URL
    if (m_Conf.data.conf.nSubType == VSC_SUB_DEVICE_FILE)
    {
        m_strUrl = m_Conf.data.conf.FileLocation;
	 m_bHasSubStream = FALSE;
    }

    if (m_Conf.data.conf.nSubType == VSC_SUB_DEVICE_RTSP)
    {
        char url[512];
        sprintf(url, "rtsp://%s:%s%s",
                m_Conf.data.conf.IP,
                m_Conf.data.conf.Port, m_Conf.data.conf.RtspLocation);
        m_strUrl = url;
	 m_bHasSubStream = FALSE;

    }

    if (m_Conf.data.conf.nSubType == VSC_SUB_DEVICE_ONVIF)
    {
        return UpdateUrlOnvif();
    }
    VDC_DEBUG( "%s url %s\n",__FUNCTION__, m_strUrl.c_str());
    return TRUE;
}

DeviceParam::DeviceParam(const DeviceParam &pParam)
{
    memset(&m_Conf, 0, sizeof(m_Conf));

    memcpy(&m_Conf, &(pParam.m_Conf), sizeof(m_Conf));
}


DeviceParam::~DeviceParam()
{
}

Device::Device(VDB &pVdb, VHdfsDB &pVHdfsdb, const DeviceParam &pParam)
:m_bStarted(FALSE), m_param(pParam),
m_pVdb(pVdb), m_pVHdfsdb(pVHdfsdb), m_pRecord(NULL), 
m_pHdfsRecord(NULL), 
m_Online(FALSE), 
m_OnlineUrl(FALSE), m_ptzInited(FALSE), 
m_ptz(NULL), m_bGotInfoData(FALSE), m_nDataRef(0)
{
	if (strcmp(pParam.m_Conf.data.conf.Name, "Camera") == 0)
	{
	  	sprintf((char *)pParam.m_Conf.data.conf.Name, "Camera %d", pParam.m_Conf.data.conf.nId);
	}
	
	m_param = pParam;
	return ;
}

Device::~Device()
{

}

DeviceStatus Device::CheckDevice()
{
    if (m_param.CheckOnline() == TRUE)
    {
        /* Camera from offline to online */
        if (m_OnlineUrl == FALSE)
        {
		if (m_param.UpdateUrl() == FALSE)
		{
		    return  DEV_NO_CHANGE;
		}
		if (m_param.m_Conf.data.conf.nSubType == VSC_SUB_DEVICE_FILE)
		{
			m_vPlay.Init(m_param.m_strUrl);
		}else
		{
			m_vPlay.Init(TRUE, m_param.m_strUrl, m_param.m_Conf.data.conf.User,
				m_param.m_Conf.data.conf.Password);
			VDC_DEBUG( "%s url %s\n",__FUNCTION__, m_param.m_strUrl.c_str());
			if (m_param.m_bHasSubStream == TRUE)
			{
				m_vPlaySubStream.Init(TRUE, m_param.m_strUrlSubStream, m_param.m_Conf.data.conf.User,
					m_param.m_Conf.data.conf.Password);			
			}
		}
		VDC_DEBUG( "%s url %s\n",__FUNCTION__, m_param.m_strUrl.c_str());

		StartRecord();
		StartHdfsRecord();
		m_OnlineUrl = TRUE;
		UpdatePTZConf();
        }
        if (m_Online == FALSE)
        {
            m_Online = TRUE;
            return DEV_OFF2ON;
        }
    }else
    {
        if (m_Online == TRUE)
        {
            m_Online = FALSE;
            return DEV_ON2OFF;
        }
    }

    return DEV_NO_CHANGE;
    
}

BOOL Device::AttachPlayer(HWND hWnd, int w, int h)
{
    m_vPlay.AttachWidget(hWnd, w, h);

    return TRUE;
}

BOOL Device::UpdateWidget(HWND hWnd, int w, int h)
{
    m_vPlay.UpdateWidget(hWnd, w, h);

    return TRUE;
}

BOOL Device::DetachPlayer(HWND hWnd)
{
    m_vPlay.DetachWidget(hWnd);
    
    return TRUE;
}

inline BOOL Device::EnablePtz(HWND hWnd, bool enable)
{
	m_vPlay.EnablePtz(hWnd, enable);
	return TRUE;
}
BOOL Device::DrawPtzDirection(HWND hWnd, int x1, int y1, int x2,  int y2)
{
	m_vPlay.DrawPtzDirection(hWnd, x1, y1, x2, y2);
	return TRUE;
}
BOOL Device::ClearPtzDirection(HWND hWnd)
{
	m_vPlay.ClearPtzDirection(hWnd);
	return TRUE;
}

BOOL Device::ShowAlarm(HWND hWnd)
{
	m_vPlay.ShowAlarm(hWnd);
	return TRUE;
}

 BOOL Device::UpdatePTZConf()
{
    if (m_param.m_Conf.data.conf.nSubType != VSC_SUB_DEVICE_ONVIF)
    {
    	return TRUE;
    }
    QString strToken;
    astring IP = m_param.m_Conf.data.conf.IP;
    astring Port = m_param.m_Conf.data.conf.Port;
    astring User = m_param.m_Conf.data.conf.User;
    astring Password = m_param.m_Conf.data.conf.Password;
    astring OnvifAddress = m_param.m_Conf.data.conf.OnvifAddress;

    astring OnvifDeviceService = "http://" + IP + ":" + Port + OnvifAddress;

    DeviceManagement *pDm = new DeviceManagement(OnvifDeviceService.c_str(), 
                            User.c_str(), Password.c_str());
    
    if (pDm  == NULL)
    {
        VDC_DEBUG( "%s new DeviceManagement error \n",__FUNCTION__);
        return FALSE;
    }

    Capabilities * pMediaCap = pDm->getCapabilitiesMedia();
    if (pMediaCap == NULL)
    {
        VDC_DEBUG( "%s getCapabilitiesPtz error \n",__FUNCTION__);
        delete pDm;
        return FALSE;
    }

    MediaManagement *pMedia = new MediaManagement(pMediaCap->mediaXAddr(), 
                                User.c_str(), Password.c_str());
    if (pMedia == NULL)
    {
        VDC_DEBUG( "%s new MediaManagement error \n",__FUNCTION__);
        delete pDm;
        delete pMediaCap;
        return FALSE;
    }

    Profiles *pProfileS = pMedia->getProfiles();
    if (pProfileS == NULL)
    {
        VDC_DEBUG( "%s new getProfiles error \n",__FUNCTION__);
        delete pDm;
        delete pMediaCap;
        delete pMedia;
        return FALSE;
    }
    if (pProfileS->m_toKenPro.size() > 0)
    {
        	VDC_DEBUG( "%s m_toKenPro size %d \n",__FUNCTION__, pProfileS->m_toKenPro.size());
		
		if (m_param.m_Conf.data.conf.UseProfileToken == 1)
		{
			strToken = m_param.m_Conf.data.conf.OnvifProfileToken;
		}else
		{
			strToken = pProfileS->m_toKenPro[0];		
		}
        
    }else
    {
    	return FALSE;
    }

    Capabilities * pPtz = pDm->getCapabilitiesPtz();
    if (pPtz == NULL)
    {
        VDC_DEBUG( "%s getCapabilitiesPtz error \n",__FUNCTION__);
        delete pDm;
        delete pMediaCap;
        delete pMedia;
        return FALSE;
    }

    string strPtz = pPtz->ptzXAddr().toStdString();
    m_ptz  = new PtzManagement(pPtz->ptzXAddr(), 
                                User.c_str(), Password.c_str());
    if (m_ptz == NULL)
    {
        VDC_DEBUG( "%s getCapabilitiesPtz error \n",__FUNCTION__);
        delete pDm;
        delete pMediaCap;
        delete pMedia;
		delete pPtz;
        return FALSE;
    }

    m_continuousMove.setProfileToken(strToken);
    m_stop.setProfileToken(strToken);
    m_ptzInited = TRUE;
}

 BOOL Device::PtzAction(FPtzAction action, float speed)
{
	if (m_Online == FALSE || m_ptzInited == FALSE)
	{
		VDC_DEBUG( "%s PTZ Camera is Offline\n",__FUNCTION__);
		return TRUE;
	}
	switch (action)
	{
		case F_PTZ_UP:	
		{
			m_continuousMove.setPanTiltX(0);
			m_continuousMove.setPanTiltY(speed);
			m_continuousMove.setZoomX(0);
			m_continuousMove.setTimeout("PT0H0M3.600S");
			m_ptz->continuousMove(&m_continuousMove);
			break;
		}
		case F_PTZ_DOWN:	
		{
			m_continuousMove.setPanTiltX(0);
			m_continuousMove.setPanTiltY(0 - speed);
			m_continuousMove.setZoomX(0);
			m_continuousMove.setTimeout("PT0H0M3.600S");
			m_ptz->continuousMove(&m_continuousMove);
			break;
		}
		case F_PTZ_LEFT:	
		{
			m_continuousMove.setPanTiltX(0 - speed);
			m_continuousMove.setPanTiltY(0);
			m_continuousMove.setZoomX(0);
			m_continuousMove.setTimeout("PT0H0M3.600S");
			m_ptz->continuousMove(&m_continuousMove);
			break;
		}
		case F_PTZ_RIGHT:	
		{
			m_continuousMove.setPanTiltX(speed);
			m_continuousMove.setPanTiltY(0);
			m_continuousMove.setZoomX(0);
			m_continuousMove.setTimeout("PT0H0M3.600S");
			m_ptz->continuousMove(&m_continuousMove);
			break;
		}
		case F_PTZ_ZOOM_IN:	
		{
			m_continuousMove.setPanTiltX(0);
			m_continuousMove.setPanTiltY(0);
			m_continuousMove.setZoomX(speed);
			m_continuousMove.setTimeout("PT0H0M3.600S");
			m_ptz->continuousMove(&m_continuousMove);
			break;
		}
		case F_PTZ_ZOOM_OUT:	
		{
			m_continuousMove.setPanTiltX(0);
			m_continuousMove.setPanTiltY(0);
			m_continuousMove.setZoomX(0 - speed);
			m_continuousMove.setTimeout("PT0H0M3.600S");
			m_ptz->continuousMove(&m_continuousMove);
			break;
		}
		case F_PTZ_STOP:	
		{
			m_stop.setPanTilt(true);
			m_stop.setZoom(false);
			m_ptz->stop(&m_stop);
			break;
		}
		default:
			break;
	}
}

BOOL Device::GetInfoFrame(InfoFrame &pFrame)
{
	if (m_bGotInfoData == TRUE)
	{
		memcpy(&pFrame, &m_infoData, sizeof(InfoFrame));
		return TRUE;
	}else
	{
		return FALSE;
	}
}
BOOL Device::GetSubInfoFrame(InfoFrame &pFrame)
{
	if (m_bGotInfoSubData == TRUE)
	{
		memcpy(&pFrame, &m_infoSubData, sizeof(InfoFrame));
		return TRUE;
	}else
	{
		return FALSE;
	}
}

 BOOL Device::RegDataCallback(DeviceDataCallbackFunctionPtr pCallback, void * pParam)
{
	Lock();
	m_DataMap[pParam] = pCallback;
	UnLock();
	StartData();
	return TRUE;
}

 BOOL Device::UnRegDataCallback(void * pParam)
{
	Lock();
	m_DataMap.erase(pParam);
	UnLock();
	StopData();
	return TRUE;
}

 BOOL Device::RegSubDataCallback(DeviceDataCallbackFunctionPtr pCallback, void * pParam)
{
	Lock();
	m_SubDataMap[pParam] = pCallback;
	UnLock();
	StartSubData();
	return TRUE;
}

 BOOL Device::UnRegSubDataCallback(void * pParam)
{
	Lock();
	m_SubDataMap.erase(pParam);
	UnLock();
	StopSubData();
	return TRUE;
}

BOOL Device::RegRawCallback(DeviceRawCallbackFunctionPtr pCallback, void * pParam)
{
	Lock();
	m_RawMap[pParam] = pCallback;
	UnLock();
	StartRaw();
	return TRUE;
}

BOOL Device::UnRegRawCallback(void * pParam)
{
	Lock();
	m_RawMap.erase(pParam);
	UnLock();
	StopRaw();
	return TRUE;
}

BOOL Device::RegSubRawCallback(DeviceRawCallbackFunctionPtr pCallback, void * pParam)
{
	Lock();
	m_SubRawMap[pParam] = pCallback;
	UnLock();
	StartSubRaw();
	return TRUE;
}

BOOL Device::UnRegSubRawCallback(void * pParam)
{
	Lock();
	m_SubRawMap.erase(pParam);
	UnLock();
	StopSubRaw();
	return TRUE;
}


BOOL Device::StartData()
{
	Lock();
	if (m_nDataRef == 0)
	{
		m_vPlay.StartGetData(this, (VPlayDataHandler)Device::DataHandler);
	}
	m_nDataRef ++;
	UnLock();
	return TRUE;
}
 BOOL Device::StopData()
{
	Lock();
	m_nDataRef --;
	if (m_nDataRef <= 0)
	{
		m_nDataRef = 0;
		m_vPlay.StopGetData();
	}

	UnLock();
	return TRUE;
}

 BOOL Device::StartSubData()
{
	Lock();
	if (m_nSubDataRef == 0)
	{
		m_vPlaySubStream.StartGetData(this, (VPlayDataHandler)Device::SubDataHandler);
	}
	m_nSubDataRef ++;
	UnLock();
	return TRUE;
}
 BOOL Device::StopSubData()
{
	Lock();
	m_nSubDataRef --;
	if (m_nSubDataRef <= 0)
	{
		m_nSubDataRef = 0;
		m_vPlaySubStream.StopGetData();
	}

	UnLock();
	return TRUE;
}

BOOL Device::StartRaw()
{
	Lock();
	if (m_nRawRef == 0)
	{
		m_vPlay.StartGetRawFrame(this, 
					(VPlayRawFrameHandler)Device::RawHandler);
	}
	m_nRawRef ++;
	UnLock();
	return TRUE;
}
 BOOL Device::StopRaw()
{
	Lock();
	m_nRawRef --;
	if (m_nRawRef <= 0)
	{
		m_nRawRef = 0;
		m_vPlay.StopGetRawFrame();
	}

	UnLock();
	return TRUE;
}

 BOOL Device::StartSubRaw()
{
	Lock();
	if (m_nSubRawRef == 0)
	{
		m_vPlaySubStream.StartGetRawFrame(this, 
			(VPlayRawFrameHandler)Device::SubRawHandler);
	}
	m_nSubRawRef ++;
	UnLock();
	return TRUE;
}
 BOOL Device::StopSubRaw()
{
	Lock();
	m_nSubRawRef --;
	if (m_nSubRawRef <= 0)
	{
		m_nSubRawRef = 0;
		m_vPlaySubStream.StopGetRawFrame();
	}

	UnLock();
	return TRUE;
}


 BOOL Device::SetRecord(BOOL bRecording)
{
    if (bRecording == TRUE)
    {
        m_param.m_Conf.data.conf.Recording = 1;
    }else
    {
        m_param.m_Conf.data.conf.Recording = 0;
    }

    return TRUE;
}

 BOOL Device::SetHdfsRecord(BOOL bRecording)
{
    if (bRecording == TRUE)
    {
        m_param.m_Conf.data.conf.HdfsRecording = 1;
    }else
    {
        m_param.m_Conf.data.conf.HdfsRecording = 0;
    }

    return TRUE;
}


 BOOL Device::StartRecord()
{
	if (m_param.m_Conf.data.conf.Recording == 0)
	{
	    return FALSE;
	}
	VDC_DEBUG( "%s Start Record\n",__FUNCTION__);
	StartData();

    return TRUE;
}
 BOOL Device::StopRecord()
{
	if (m_param.m_Conf.data.conf.Recording == 1)
	{
	    return FALSE;
	}
	VDC_DEBUG( "%s Stop Record\n",__FUNCTION__);
	StopData();
	Lock();
	if (m_pRecord)
	{
	    u32 endTime = m_pRecord->GetEndTime();
	    if (endTime != 0)
	    {
	    	 m_pVdb.FinishRecord(m_pRecord);
	    }
	    delete m_pRecord;
	    m_pRecord = NULL;
	}
	UnLock();

    return TRUE;
}

  BOOL Device::StartHdfsRecord()
{
    if (m_param.m_Conf.data.conf.HdfsRecording == 0)
    {
        return FALSE;
    }
    VDC_DEBUG( "%s Start Record\n",__FUNCTION__);
    StartData();

    return TRUE;
}
 BOOL Device::StopHdfsRecord()
{
	if (m_param.m_Conf.data.conf.HdfsRecording == 1)
	{
	    return FALSE;
	}

	VDC_DEBUG( "%s Stop Record\n",__FUNCTION__);
	StopData();
	Lock();
	if (m_pHdfsRecord)
	{
		m_pVHdfsdb.FinishRecord(m_pHdfsRecord);
		delete m_pHdfsRecord;
		m_pHdfsRecord = NULL;
	}
	UnLock();
    	return TRUE;
}

BOOL Device::DataHandler(void* pData, VideoFrame& frame)
{
    int dummy = errno;
    LPDevice pThread = (LPDevice)pData;

    if (pThread)
    {
        return pThread->DataHandler1(frame);
    }
}

BOOL Device::DataHandler1(VideoFrame& frame)
{
	Lock();
	/* Frist cache the info frame */
	if (frame.streamType == VIDEO_STREAM_INFO)
	{
		memcpy(&m_infoData, frame.dataBuf, sizeof(InfoFrame));
		m_bGotInfoData = TRUE;
	}
	
	/* 1. Send to network client */
	DeviceDataCallbackMap::iterator it = m_DataMap.begin();

	for(; it!=m_DataMap.end(); ++it)
	{
	    void *pParam = (*it).first;
	    DeviceDataCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc)
	    {
	        pFunc(frame, pParam);
	    }
	}

	/* 2. Send to Record */
	if (m_param.m_Conf.data.conf.Recording == 1)
	{
		if (m_pRecord == NULL)
		{
		    m_pRecord = m_pVdb.StartRecord(m_param.m_Conf.data.conf.nId, (int)(frame.secs), R_MANUAL);
		}
		
		//VDC_DEBUG("Recording Size %d stream %d frame %d (%d, %d)\n", frame.dataLen,      
		// 	frame.streamType, frame.frameType, frame.secs, frame.msecs);
		
		/* Just skip the info stream for recording */
		if (m_pRecord != NULL 
		&& frame.streamType != VIDEO_STREAM_INFO && m_pRecord->PushAFrame(&frame) == MF_WRTIE_REACH_END)
		{
			u32 endTime = m_pRecord->GetEndTime();
			if (endTime != 0)
			{
				m_pVdb.FinishRecord(m_pRecord);
			}
		    	delete m_pRecord;
		    	m_pRecord = m_pVdb.StartRecord(m_param.m_Conf.data.conf.nId, (int)(frame.secs), 1);
			if (m_pRecord != NULL)
			{
			       m_pRecord->PushAFrame(&frame);	 
			}
		}
	}
	
	/* 2. Send to Hdfs Record */
	if (m_param.m_Conf.data.conf.HdfsRecording == 1)
	{
		if (m_pHdfsRecord == NULL)
		{
			m_pHdfsRecord = m_pVHdfsdb.StartRecord(
				m_param.m_Conf.data.conf.nId, m_param.m_Conf.data.conf.Name);
		}
		if (m_pHdfsRecord != NULL)
		{
			m_pHdfsRecord->PushAFrame(&frame);
		}
	}
	/* Call the Sub DataHandler if there has no sub stream */
	if (m_param.m_bHasSubStream == FALSE)
	{
		SubDataHandler1(frame);
	}
	UnLock();
	return TRUE;
}

BOOL Device::SubDataHandler(void* pData, VideoFrame& frame)
{
    int dummy = errno;
    LPDevice pThread = (LPDevice)pData;

    if (pThread)
    {
        return pThread->SubDataHandler1(frame);
    }
}

BOOL Device::SubDataHandler1(VideoFrame& frame)
{
	SubLock();
	/* Frist cache the info frame */
	if (frame.streamType == VIDEO_STREAM_INFO)
	{
		memcpy(&m_infoSubData, frame.dataBuf, sizeof(InfoFrame));
		m_bGotInfoSubData = TRUE;
	}
	
	/* 1. Send to client */
	DeviceDataCallbackMap::iterator it = m_SubDataMap.begin();

	for(; it!=m_SubDataMap.end(); ++it)
	{
	    void *pParam = (*it).first;
	    DeviceDataCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc)
	    {
	        pFunc(frame, pParam);
	    }
	}

	SubUnLock();
	return TRUE;
}

BOOL Device::RawHandler(void* pData, RawFrame& frame)
{
    int dummy = errno;
    LPDevice pThread = (LPDevice)pData;

    if (pThread)
    {
        return pThread->RawHandler1(frame);
    }
}

BOOL Device::RawHandler1(RawFrame& frame)
{
	SubLock();
	
	/* 1. Send to client */
	DeviceRawCallbackMap::iterator it = m_RawMap.begin();

	for(; it!=m_SubRawMap.end(); ++it)
	{
	    void *pParam = (*it).first;
	    DeviceRawCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc)
	    {
	        pFunc(frame, pParam);
	    }
	}
	/* Call the Sub DataHandler if there has no sub stream */
	if (m_param.m_bHasSubStream == FALSE)
	{
		SubRawHandler1(frame);
	}

	SubUnLock();
	return TRUE;
}

BOOL Device::SubRawHandler(void* pData, RawFrame& frame)
{
    int dummy = errno;
    LPDevice pThread = (LPDevice)pData;

    if (pThread)
    {
        return pThread->SubRawHandler1(frame);
    }
}

BOOL Device::SubRawHandler1(RawFrame& frame)
{
	SubLock();
	
	/* 1. Send to client */
	DeviceRawCallbackMap::iterator it = m_SubRawMap.begin();

	for(; it!=m_SubRawMap.end(); ++it)
	{
	    void *pParam = (*it).first;
	    DeviceRawCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc)
	    {
	        pFunc(frame, pParam);
	    }
	}

	SubUnLock();
	return TRUE;
}

 BOOL Device::Cleanup()
{
#if 0
    VDC_DEBUG( "%s Callback begin\n",__FUNCTION__);
    /* Call the callbacks for this device */
    DeviceDeleteCallbackMap::iterator it = m_DeleteMap.begin();

    for(; it!=m_DeleteMap.end(); ++it)
    {
        void *pParam = (*it).first;
        DeviceDeleteCallbackFunctionPtr pFunc = (*it).second;
        if (pFunc)
        {
            pFunc(m_param.m_Conf.data.conf.nId, pParam);
        }
    }
    VDC_DEBUG( "%s Callback end\n",__FUNCTION__);
#endif

    return TRUE;
}

BOOL Device::GetDeviceOnline()
{
    BOOL online = true;
    return m_Online;
}

BOOL Device::GetUrl(std::string &url)
{
    url = m_param.m_strUrl;
    return TRUE;
}
BOOL Device::GetDeviceRtspUrl(astring & strUrl)
{
	if (m_OnlineUrl == FALSE)
	{
		return FALSE;
	}
	
	strUrl = m_param.m_strUrl;
	return TRUE;
}


#endif // __VSC_DEVICE_H_
