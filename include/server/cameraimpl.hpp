//------------------------------------------------------------------------------
// File: camera.hpp
//
// Desc: Camera factory - Manage IP Camera.
//
// Copyright (c) 2014-2018 opencvr.com All rights reserved.
//------------------------------------------------------------------------------

#ifndef __VSC_CAMERA_IMPL_H_
#define __VSC_CAMERA_IMPL_H_


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


CameraParam::CameraParam()
{
	Poco::UUIDGenerator uuidCreator;
	
	astring strId  = uuidCreator.createRandom().toString();
	m_Conf.set_ntype(VID_ONVIF_S);
	m_Conf.set_strid(strId);
	m_Conf.set_strname("Camera");

	m_Conf.set_strip("192.168.0.1");
	m_Conf.set_strport("80");
	m_Conf.set_struser("admin");
	m_Conf.set_strpasswd("admin");

	m_Conf.set_strrtspurl("rtsp://192.168.0.1:554/Streaming");

	astring filePath = GetProgramDir() +  "camera.mp4";

	m_Conf.set_strfile(filePath.c_str());

	m_Conf.set_stronvifaddress("/onvif/device_service");

	m_Conf.set_bprofiletoken(false);
	m_Conf.set_bhwaccel(false);

	m_Conf.set_strprofiletoken1("quality_h264");
	m_Conf.set_strprofiletoken1("second_h264");
	m_Conf.set_bhdfsrecord(false);
	astring *pSched = m_Conf.add_crecsched();
	*pSched = REC_SCHED_ALL_DAY;
	
	m_bOnvifUrlGetted = false;
	m_bHasSubStream = false;

	m_Conf.set_nconnecttype(VID_CONNECT_TCP);

	m_Online = false;
	m_OnlineUrl = false;

	astring IP = m_Conf.strip();
	m_strUrl = "rtsp://" + IP + ":" + "554" + "/Streaming";
	m_strUrlSubStream = "rtsp://" + IP + ":" + "554" + "/Streaming";

}

BOOL CameraParam::UpdateDefaultUrl()
{
	astring IP = m_Conf.strip();
	m_strUrl = "rtsp://" + IP + ":" + "554" + "/Streaming";
	m_strUrlSubStream = "rtsp://" + IP + ":" + "554" + "/Streaming";

	return TRUE;
}

CameraParam::CameraParam(VidCamera &pData)
{
	m_Conf = pData;
	
	m_bOnvifUrlGetted = FALSE;
	m_bHasSubStream = FALSE;
	m_Online = FALSE;
	m_OnlineUrl = FALSE;
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

BOOL CameraParam::CheckOnline()
{
    if (m_Conf.ntype()== VID_FILE 
		|| m_Conf.ntype() == VID_RTSP 
		|| m_Conf.ntype() == VID_MJPEG )
    {
    	return TRUE;
    }
    astring IP = m_Conf.strip();
    astring Port = m_Conf.strport();
    astring User = m_Conf.struser();
    astring Password = m_Conf.strpasswd();
    astring OnvifAddress = m_Conf.stronvifaddress();

    astring OnvifCameraService = "http://" + IP + ":" + Port + OnvifAddress;
	astring url = "rtsp://" + IP + ":" + "554" + "/";
     VDC_DEBUG( "%s  TryCheckDevice Begin \n",__FUNCTION__);
     if (TryCheckDevice(IP, Port) == false)
     {
       VDC_DEBUG( "%s TryCheckDevice False \n",__FUNCTION__);    
	return false;
     }
     VDC_DEBUG( "%s  TryCheckDevice End \n",__FUNCTION__);
    DeviceManagement *pDm = new DeviceManagement(OnvifCameraService.c_str(), 
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

BOOL CameraParam::UpdateUrlOnvif()
{
	BOOL bGotUrl = FALSE;
	astring IP = m_Conf.strip();
	astring Port = m_Conf.strport();
	astring User = m_Conf.struser();
	astring Password = m_Conf.strpasswd();
	astring OnvifAddress = m_Conf.stronvifaddress();

	astring OnvifCameraService = "http://" + IP + ":" + Port + OnvifAddress;
	astring url = "rtsp://" + IP + ":" + "554" + "/";
	astring urlSubStream = "rtsp://" + IP + ":" + "554" + "/";

	DeviceManagement *pDm = new DeviceManagement(OnvifCameraService.c_str(), 
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

	if (pProfileS != NULL && pProfileS->m_toKenPro.size() <= 0)
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
		if (m_Conf.bprofiletoken() == true)
		{
			strToken = m_Conf.strprofiletoken1().c_str();
			//Find which token is in the OnvifProfileToken, and then use the token
		}else
		{
			strToken = pProfileS->m_toKenPro[0];
			m_Conf.set_strprofiletoken1(pProfileS->m_toKenPro[0].toStdString().c_str());
		}
		StreamUri *pUri = pMedia->getStreamUri(strToken);
		if (pUri)
		{
			m_strUrl = pUri->uri().toStdString();
			if (m_strUrl.length() <= 0)
			{
				bGotUrl = FALSE;
			}else
			{
				bGotUrl = TRUE;
			}
			delete pUri;
		}
	}
	if (pProfileS->m_toKenPro.size() > 1)
	{
		VDC_DEBUG( "%s m_toKenPro SubStream size %d \n",__FUNCTION__, pProfileS->m_toKenPro.size());
		QString strToken;
		if (m_Conf.bprofiletoken() == true)
		{
			strToken = m_Conf.strprofiletoken2().c_str();
		}else
		{
			strToken = pProfileS->m_toKenPro[1];
			m_Conf.set_strprofiletoken2(pProfileS->m_toKenPro[1].toStdString().c_str());
		}
		StreamUri *pUri = pMedia->getStreamUri(strToken);
		if (pUri)
		{
			m_strUrlSubStream = pUri->uri().toStdString();
			
			if (m_strUrl.length() <= 0)
			{
				bGotUrl = FALSE;
			}else
			{
				m_bHasSubStream = TRUE;
				bGotUrl = TRUE;
			}
			delete pUri;
		}
	}

	/* Cache the stream profile list */
	
	for ( int i=0; i!=pProfileS->m_toKenPro.size(); ++i )
	{
		VidStream *pNewStream = m_cStreamList.add_cvidstream();
		s8 profileDisplay[1024];
		sprintf(profileDisplay, "%s %dx%d %dfps %dbps", 
				pProfileS->m_encodingVec.at(i).toStdString().c_str(), pProfileS->m_widthVec.at(i), 
				pProfileS->m_heightVec.at(i), pProfileS->m_frameRateLimitVec.at(i), 
				pProfileS->m_bitrateLimitVec.at(i));
		pNewStream->set_strname( profileDisplay);
		pNewStream->set_strtoken( pProfileS->m_toKenPro.at(i).toStdString());
	}

	/* rtsp://admin:12345@192.168.1.1:554/Streaming/Channels/1\
	?transportmode=unicast&profile=Profile_1 */
	//astring urlWithUser = "rtsp://" + User + ":" + Password + "@";
	//Replace(strUrl, "rtsp://", urlWithUser.c_str());

	if (bGotUrl == TRUE)
	{
		m_bOnvifUrlGetted = TRUE;
	}

	delete pDm;
	delete pMediaCap;
	delete pMedia;
	delete pProfileS;
	
	return bGotUrl;
}

BOOL CameraParam::UpdateUrl()
{
    //TODO RTSP ONVIF call onvif sdk to get a Stream URL
    if (m_Conf.ntype()== VID_FILE )
    {
        m_strUrl = m_Conf.strfile();
	 m_bHasSubStream = FALSE;
    }

    if (m_Conf.ntype()== VID_RTSP
	|| m_Conf.ntype()== VID_MJPEG)
    {
#if 0
        char url[512];
        sprintf(url, "rtsp://%s:%s%s",
                m_Conf.data.conf.IP,
                m_Conf.data.conf.Port, m_Conf.data.conf.RtspLocation);
#endif
	Poco::URI rtspUrl(m_Conf.strrtspurl());
	astring strRtsp;
	if (rtspUrl.empty() != true)
	{
		strRtsp = rtspUrl.getScheme() + "://" + rtspUrl.getHost() + ":" + std::to_string(rtspUrl.getPort()) + rtspUrl.getPathAndQuery();
	}

	m_strUrl = strRtsp;
	m_bHasSubStream = FALSE;

    }

    if (m_Conf.ntype()== VID_ONVIF_S)
    {
        return UpdateUrlOnvif();
    }
    VDC_DEBUG( "%s url %s\n",__FUNCTION__, m_strUrl.c_str());
    return TRUE;
}

CameraParam::CameraParam(const CameraParam &pParam)
{
	m_Conf = pParam.m_Conf;

	m_bOnvifUrlGetted = pParam.m_bOnvifUrlGetted;
	m_bHasSubStream = pParam.m_bHasSubStream;
	m_Online = pParam.m_Online;
	m_OnlineUrl = pParam.m_OnlineUrl;
}


CameraParam::~CameraParam()
{
}

Camera::Camera(ConfDB &pConfDB, VDB &pVdb, VHdfsDB &pVHdfsdb, 
	const CameraParam &pParam, RecChangeFunctionPtr pCallback, void *pCallbackParam)
:m_bStarted(FALSE), m_param(pParam),
m_pVdb(pVdb), 
m_ptzInited(FALSE), 
m_ptz(NULL), m_bGotInfoData(FALSE), m_nDataRef(0), m_bGotInfoSubData(FALSE),
m_nSubDataRef(0), 
m_pvPlay(new VPlay), m_pvPlaySubStream(new VPlay), 
m_vPlay(*m_pvPlay), m_vPlaySubStream(*m_pvPlaySubStream), 
m_pConfDB(pConfDB),
m_cRecordWrapper(pVdb, pParam.m_Conf.strid(), m_cSchedMap, pCallback, pCallbackParam)

{	
	m_param = pParam;
	m_param.UpdateDefaultUrl();
	UpdateRecSched(m_param.m_Conf);
	return ;
}

Camera::~Camera()
{
	/* stop all the data  */
	Lock();
	SubLock();

	CameraDelCallbackMap::iterator it = m_DelMap.begin();

	for(; it!=m_DelMap.end(); ++it)
	{
	    void *pParam = (*it).first;
	    CameraDelCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc)
	    {
	        pFunc(pParam);
	    }
	}
	SubUnLock();
	UnLock();

	m_vPlay.StopGetData();
	m_vPlaySubStream.StopGetData();

	delete m_pvPlay;
	delete m_pvPlaySubStream;
}

bool Camera::UpdateRecSched(VidCamera &pCam)
{
	m_cSchedMap.clear();

	int cameraSize = pCam.crecsched_size();

	for (s32 i = 0; i < pCam.crecsched_size(); i ++)
	{
		astring strid = pCam.crecsched(i);
		RecordSchedWeek sched;
		if (m_pConfDB.GetRecSched(strid, sched) == true)
		{
			m_cSchedMap[strid] = sched;
		}
	}

	m_cRecordWrapper.UpdateSchedMap(m_cSchedMap);

	return true;
	
}


BOOL Camera::GetCameraParam(CameraParam &pParam)
{
	pParam = m_param;
	return TRUE;
}

CameraStatus Camera::CheckCamera(astring strUrl, astring strUrlSubStream, 
		BOOL bHasSubStream, BOOL bOnline, 
		BOOL bOnlineUrl, VidStreamList cStreamlist)
{
    if (bOnline == TRUE)
    {
        /* Camera from offline to online */
        if (m_param.m_OnlineUrl == FALSE)
        {
        	BOOL HWAccel = FALSE;
		if (m_param.m_Conf.bhwaccel()== TRUE)
		{
			HWAccel = TRUE;
		}
		if (bOnlineUrl == FALSE)
		{
		    return  DEV_NO_CHANGE;
		}
		m_param.m_strUrl = strUrl;
		m_param.m_strUrlSubStream = strUrlSubStream;
		m_param.m_bHasSubStream = bHasSubStream;
		if (m_param.m_Conf.ntype()== VID_FILE)
		{
			m_vPlay.Init(m_param.m_strUrl, false, "fake", "fake", HWAccel);
		}else if (m_param.m_Conf.bmotionjpeg()== true
		  || m_param.m_Conf.ntype()== VID_MJPEG)
		{
			/* Motion JPEG */
			m_vPlay.Init(m_param.m_strUrl, true, m_param.m_Conf.struser(),
				m_param.m_Conf.strpasswd(), HWAccel);
		}
		else
		{
			m_vPlay.Init(TRUE, m_param.m_strUrl, m_param.m_Conf.struser(),
				m_param.m_Conf.strpasswd(), HWAccel, 
				(VSCConnectType)(m_param.m_Conf.nconnecttype()));
			VDC_DEBUG( "%s url %s\n",__FUNCTION__, m_param.m_strUrl.c_str());
			if (m_param.m_bHasSubStream == TRUE)
			{
				m_vPlaySubStream.Init(TRUE, m_param.m_strUrlSubStream, m_param.m_Conf.struser(),
					m_param.m_Conf.strpasswd(), HWAccel,
					(VSCConnectType)(m_param.m_Conf.nconnecttype()));
				VDC_DEBUG( "%s url %s\n",__FUNCTION__, m_param.m_strUrlSubStream.c_str());
			}
		}
		VDC_DEBUG( "%s url %s\n",__FUNCTION__, m_param.m_strUrl.c_str());
		
		m_param.m_OnlineUrl = TRUE;
		UpdatePTZConf();
		m_param.m_cStreamList = cStreamlist;
		/* Always start data */
		StartData();
        }
        if (m_param.m_Online == FALSE)
        {
            m_param.m_Online = TRUE;
            return DEV_OFF2ON;
        }
    }else
    {
        if (m_param.m_Online == TRUE)
        {
            m_param.m_Online = FALSE;
            return DEV_ON2OFF;
        }
    }

    return DEV_NO_CHANGE;
    
}

 BOOL Camera::UpdatePTZConf()
{
	if (m_param.m_Conf.ntype()!= VID_ONVIF_S)
	{
		return TRUE;
	}
	QString strToken;

	astring IP = m_param.m_Conf.strip();
	astring Port = m_param.m_Conf.strport();
	astring User = m_param.m_Conf.struser();
	astring Password = m_param.m_Conf.strpasswd();
	astring OnvifAddress = m_param.m_Conf.stronvifaddress();

	astring OnvifCameraService = "http://" + IP + ":" + Port + OnvifAddress;

	DeviceManagement *pDm = new DeviceManagement(OnvifCameraService.c_str(), 
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
		
		if (m_param.m_Conf.bprofiletoken() == true)
		{
			strToken = m_param.m_Conf.strprofiletoken1().c_str();
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

 BOOL Camera::PtzAction(FPtzAction action, float speed)
{
	if (m_param.m_Online == FALSE || m_ptzInited == FALSE)
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

BOOL Camera::GetInfoFrame(InfoFrame &pFrame)
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
BOOL Camera::GetSubInfoFrame(InfoFrame &pFrame)
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

 BOOL Camera::RegDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam)
{
	Lock();
	m_DataMap[pParam] = pCallback;
	UnLock();
	StartData();
	return TRUE;
}

 BOOL Camera::UnRegDataCallback(void * pParam)
{
	Lock();
	m_DataMap.erase(pParam);
	UnLock();
	StopData();
	return TRUE;
}

 BOOL Camera::RegSubDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam)
{
	Lock();
	m_SubDataMap[pParam] = pCallback;
	UnLock();
	if (m_param.m_bHasSubStream == FALSE)
	{
		StartData();
	}else
	{
		StartSubData();
	}
	return TRUE;
}

 BOOL Camera::UnRegSubDataCallback(void * pParam)
{
	Lock();
	m_SubDataMap.erase(pParam);
	UnLock();
	if (m_param.m_bHasSubStream == FALSE)
	{
		StopData();
	}else
	{
		StopSubData();
	}
	return TRUE;
}

BOOL Camera::RegDelCallback(CameraDelCallbackFunctionPtr pCallback, void * pParam)
{
	Lock();
	m_DelMap[pParam] = pCallback;
	UnLock();
	return TRUE;
}
BOOL Camera::UnRegDelCallback(void * pParam)
{
	Lock();
	m_DelMap.erase(pParam);
	UnLock();
	return TRUE;
}

BOOL Camera::StartData()
{
	Lock();
	if (m_nDataRef == 0)
	{
		m_vPlay.StartGetData(this, (VPlayDataHandler)Camera::DataHandler);
	}
	m_nDataRef ++;
	UnLock();
	return TRUE;
}
 BOOL Camera::StopData()
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

 BOOL Camera::StartSubData()
{
	Lock();
	if (m_nSubDataRef == 0)
	{
		m_vPlaySubStream.StartGetData(this, (VPlayDataHandler)Camera::SubDataHandler);
	}
	m_nSubDataRef ++;
	UnLock();
	return TRUE;
}
 BOOL Camera::StopSubData()
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

BOOL Camera::DataHandler(void* pData, VideoFrame& frame)
{
    int dummy = errno;
    LPCamera pThread = (LPCamera)pData;

    if (pThread)
    {
        return pThread->DataHandler1(frame);
    }
}

BOOL Camera::DataHandler1(VideoFrame& frame)
{
	//VDC_DEBUG( "%s  %d\n",__FUNCTION__, frame.dataLen);
	Lock();
	/* Frist cache the info frame */
	if (frame.streamType == VIDEO_STREAM_INFO)
	{
		memcpy(&m_infoData, frame.dataBuf, sizeof(InfoFrame));
		m_bGotInfoData = TRUE;
	}
	
	/* 1. Send to network client */
	CameraDataCallbackMap::iterator it = m_DataMap.begin();

	for(; it!=m_DataMap.end(); ++it)
	{
	    void *pParam = (*it).first;
	    CameraDataCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc)
	    {
	        pFunc(frame, pParam);
	    }
	}

	m_cRecordWrapper.PushAFrame(frame);

	/* Call the Sub DataHandler if there has no sub stream */
	if (m_param.m_bHasSubStream == FALSE)
	{
		SubDataHandler1(frame);
	}
	UnLock();
	return TRUE;
}

BOOL Camera::SubDataHandler(void* pData, VideoFrame& frame)
{
    int dummy = errno;
    LPCamera pThread = (LPCamera)pData;

    if (pThread)
    {
        return pThread->SubDataHandler1(frame);
    }
}

BOOL Camera::SubDataHandler1(VideoFrame& frame)
{
	SubLock();
	/* Frist cache the info frame */
	if (frame.streamType == VIDEO_STREAM_INFO)
	{
		memcpy(&m_infoSubData, frame.dataBuf, sizeof(InfoFrame));
		m_bGotInfoSubData = TRUE;
	}
	
	/* 1. Send to client */
	CameraDataCallbackMap::iterator it = m_SubDataMap.begin();

	for(; it!=m_SubDataMap.end(); ++it)
	{
	    void *pParam = (*it).first;
	    CameraDataCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc)
	    {
	        pFunc(frame, pParam);
	    }
	}

	SubUnLock();
	return TRUE;
}

BOOL Camera::GetCameraOnline()
{
    BOOL online = true;
	return m_param.m_Online;
}

BOOL Camera::GetStreamInfo(VideoStreamInfo &pInfo)
{
    m_vPlay.GetStreamInfo(pInfo);

    return TRUE;
}


BOOL Camera::GetCamStreamList(VidStreamList &pList)
{
	pList =  m_param.m_cStreamList;
	return TRUE;
}
		


BOOL Camera::AttachPlayer(HWND hWnd, int w, int h)
{
    m_vPlay.AttachWidget(hWnd, w, h);

    return TRUE;
}

BOOL Camera::UpdateWidget(HWND hWnd, int w, int h)
{
    m_vPlay.UpdateWidget(hWnd, w, h);

    return TRUE;
}

BOOL Camera::DetachPlayer(HWND hWnd)
{
    m_vPlay.DetachWidget(hWnd);
    
    return TRUE;
}

BOOL Camera::ShowAlarm(HWND hWnd)
{
	m_vPlay.ShowAlarm(hWnd);
	return TRUE;
}

#endif // __VSC_CAMERA_IMPL_H_
