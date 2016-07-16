//------------------------------------------------------------------------------
// File: oapiimpl.hpp
//
// Desc: OpenCVR API - OpenCVR API.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPIS_IMPL_H_
#define __VSC_OAPIS_IMPL_H_

bool OAPIConverter::Converter(VidCamera &from, oapi::OAPICamera &to)
{
	to.strId = from.strid();
	to.strName = from.strname();
	to.nType = from.ntype();
	
	to.strIP = from.strip();
	to.strPort = from.strport();
	to.strUser = from.struser();
	to.strPasswd = from.strpasswd();
	
	to.strONVIFAddress = from.stronvifaddress();
	to.bProfileToken = from.bprofiletoken();
	to.strProfileToken1 = from.strprofiletoken1();
	to.strProfileToken2 = from.strprofiletoken2();

	
	to.strFile = from.strfile();
	to.strRTSPUrl = from.strrtspurl();

	to.bHWaccel = from.bhwaccel();
	to.bServerMotion = from.bservermotion();

	to.nConnectType = from.nconnecttype();

	int cameraSize = from.crecsched_size();

	to.cRecSchedList.clear();

	for (s32 i = 0; i < from.crecsched_size(); i ++)
	{
		astring strid = from.crecsched(i);

		to.cRecSchedList.push_back(strid);
	}
	
	return TRUE;
}

bool OAPIConverter::Converter(oapi::OAPICamera &from, VidCamera &to)
{
	to.set_strid(from.strId);
	to.set_strname(from.strName);
	to.set_ntype((VidConf::CameraType)from.nType);

	to.set_strip(from.strIP);
	to.set_strport(from.strPort);
	to.set_struser(from.strUser);
	to.set_strpasswd(from.strPasswd);

	to.set_stronvifaddress(from.strONVIFAddress);
	to.set_bprofiletoken(from.bProfileToken);
	to.set_strprofiletoken1(from.strProfileToken1);
	to.set_strprofiletoken2(from.strProfileToken2);

	to.set_strfile(from.strFile);
	to.set_strrtspurl(from.strRTSPUrl);

	to.set_bhwaccel(from.bHWaccel);
	to.set_bservermotion(from.bServerMotion);

	to.set_nconnecttype((VidConf::CameraConnectType)from.nConnectType);

	to.clear_crecsched();

	for (s32 i = 0; i < from.cRecSchedList.size(); i ++)
	{
		astring strid = from.cRecSchedList[i];
		astring *pNewSched = to.add_crecsched();
		*pNewSched = strid;
	}
	return TRUE;
}

bool OAPIConverter::Converter(oapi::OAPIDisk &from, VidDisk &to)
{
	to.set_strid(from.strId);
	to.set_strpath(from.strPath);
	to.set_ntotalsize(from.nTotalSize);
	to.set_nfreesize(from.nFreeSize);
	to.set_nstorlimit(from.nStorLimit);
	to.set_nstorused(from.nStorUsed);

	return true;
}
bool OAPIConverter::Converter(VidDisk &from, oapi::OAPIDisk &to)
{
	to.strId = from.strid();
	to.strPath = from.strpath();
	to.nTotalSize = from.ntotalsize();
	to.nFreeSize = from.nfreesize();
	to.nStorLimit = from.nstorlimit();
	to.nStorUsed = from.nstorused();
	
	return true;
}

bool OAPIConverter::Converter(oapi::OAPIStreamListRsp &from, VidStreamList &to)
{
	for (s32 i = 0; i < from.list.size(); i ++)
	{
		VidStream *pNewStream = to.add_cvidstream();
		pNewStream->set_strname(from.list[i].strName);
		pNewStream->set_strtoken(from.list[i].strToken);
	}

	return true;
}

bool OAPIConverter::Converter(VidStreamList &from, oapi::OAPIStreamListRsp &to)
{
	for (s32 i = 0; i < from.cvidstream_size(); i ++)
	{
		oapi::OAPIStreamProfile profile;
		VidStream stream = from.cvidstream(i);
		
		profile.strName = stream.strname();
		profile.strToken = stream.strtoken();
		to.list.push_back(profile);
	}

	return true;
}

 OAPIServerCamSearch::OAPIServerCamSearch(OAPICamSearchInterface &pCamInf, 
	Factory &pFactory)
:m_pFactory(pFactory), m_pCamInf(pCamInf), m_bQuit(false), m_Timer(NULL)
{
	/* auto delete when the thread exit */
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
	m_Timer = new QTimer(this);
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(SlotTimeout()));
	
}
 OAPIServerCamSearch::~OAPIServerCamSearch()
{

}

 bool OAPIServerCamSearch::ClearCamSearcherMap()
{
	DeviceSearcherMap::iterator it = m_CamSearcherMap.begin(); 
	for(; it!=m_CamSearcherMap.end(); ++it)
	{
		if ((*it).second)
		{
    			disconnect((*it).second, SIGNAL(receiveData(const QHash<QString, QString> &) ),
					this, SLOT(SlotSearchReceiveData(const QHash<QString, QString> &)));
			delete (*it).second;
		}
	}

	m_CamSearcherMap.clear();

	return true;
}

static void GetIPAndPort(astring &str, astring & ip, astring & port, astring & onvifAddr)
{
       astring strItem = "http://";
	size_t pos = 0;
	while (1)
	{
	       strItem = "http://";
		size_t p1 = str.find(strItem, pos);
		if (p1 == string::npos) return;
		size_t posIP = p1 + strItem.length();
		strItem = "/";
		size_t p2 = str.find(strItem,posIP);
		if (p2 == string::npos) return;
		string strGetIP = str.substr(posIP, p2 - posIP);
		if (strGetIP.find(".") == string::npos || 
			strGetIP.find("169") == 0)
		{
			/* IPV6 address */
			pos = p2;
			continue;
		}
		string strGetOnvifAddr;
		strItem = "http://";
		if (str.find(strItem, p2) == string::npos)
		{
			strGetOnvifAddr = str .substr(p2);
		}else
		{
			strItem = " ";
			size_t p3 = str.find(strItem, p2);
			if (p3 == string::npos) return;
			strGetOnvifAddr = str.substr(p2, p3 - p2);
		}

	       string strPort = "80";
		size_t posPort = strGetIP.find_first_of(":");
		if (posPort != std::string::npos) 
		{
			strPort = strGetIP.substr(posPort + 1);
			string strIPTemp = strGetIP.substr(0, posPort);
			strGetIP = strIPTemp;
		}
		ip = strGetIP;
		port = strPort;
		onvifAddr = strGetOnvifAddr;
		strItem = "http://";
		if (str.find(strItem, p2) == string::npos)
		{
			break;
		}else
		{
			pos = p2;
			continue;			
		}
	}
	
}

static void GetHardwareModel(astring &str, astring & hdModel)
{
	size_t pos1= 0;
	size_t pos2 = 0;	
	std::string strItem;
	strItem = "hardware/";
	pos2 = str.find(strItem);

	string sHardware = "unknown";
	if (pos2 != string::npos)
	{
		size_t posHardware =  pos2 + strItem.length();
		strItem = "onvif";
		pos1 = str.find(strItem,pos2);
		
		if (pos1 != string::npos)
		{
			sHardware =  str.substr(posHardware,pos1 - posHardware);
			transform(sHardware.begin(), sHardware.end(),sHardware.begin(),(int(*)(int))toupper);
		}
		
	}
	hdModel = sHardware;
}

 void OAPIServerCamSearch::SlotSearchReceiveData(const QHash<QString, QString> &data)
{
	astring ip = "192.168.0.1";
	astring port = "80";
	astring hdModel = "unknown";
	astring OnvifAddr = "/onvif/device_service";
	astring type = data["types"].toStdString();
	astring strFilter = data["device_ip"].toStdString();
	transform(type.begin(), type.end(),type.begin(),(int(*)(int))tolower);
	transform(strFilter.begin(), strFilter.end(),strFilter.begin(),(int(*)(int))tolower);
	if (strFilter.find("network_video_storage") != string::npos)
	{
	    VDC_DEBUG("[ONVIF]: it is a NVS device\n");
	    return;
	} 
	size_t pos1 = type.find("networkvideotransmitter");
	VDC_DEBUG( "[ONVIF]: Searched ip %s\n", data["device_service_address"].toStdString().c_str());
	if (pos1 == string::npos)
	{
	    return;
	}
	astring strServerAddr = data["device_service_address"].toStdString();
	GetIPAndPort(strServerAddr,  ip, port, OnvifAddr);
	astring strDeviceIp = data["device_ip"].toStdString();
	GetHardwareModel(strDeviceIp, hdModel);
	m_pCamInf.NewCam(ip, port, hdModel, OnvifAddr);
	
	return;
}

 bool OAPIServerCamSearch::CamSearcherStart()
{
	QList<QHostAddress> hostAddr = DeviceSearcher::getHostAddress();
	QList<QHostAddress>::iterator i;
	for(i=hostAddr.begin();i!=hostAddr.end();i++)
	{
		astring strHostAddr = (*i).toString().toStdString();
		QHostAddress host((*i).toString());
		m_CamSearcherMap[strHostAddr]  = DeviceSearcher::instance(host);
    		connect(m_CamSearcherMap[strHostAddr], SIGNAL(receiveData(const QHash<QString, QString> &) ), 
			this, SLOT(SlotSearchReceiveData(const QHash<QString, QString> &)), Qt::DirectConnection);
    		m_CamSearcherMap[strHostAddr]->sendSearchMsg();

	}

	return true;
}

 bool OAPIServerCamSearch::QuitSearch()
{
	XGuard guard(m_cMutex);
	m_bQuit = true;
	return true;
}

 void OAPIServerCamSearch::SlotTimeout()
{
	VDC_DEBUG("[ONVIF]: SlotTimeout\n");
	XGuard guard(m_cMutex);
	if (m_bQuit == true)
	{
		this->exit(0);
	}
}

 void OAPIServerCamSearch::run()
{
	m_Timer->start(300);
	ClearCamSearcherMap();
	CamSearcherStart();
	exec();
}

OAPIServerPlayback::OAPIServerPlayback(OAPIPlaybackInterface &pPbInf, 
	Factory &pFactory, astring strId, u32 nPlaytime)
:m_pFactory(pFactory), m_pPbInf(pPbInf), m_strId(strId), m_nPlaytime(nPlaytime), 
m_pbWrapper(pFactory.GetVdb(), strId, nPlaytime, 
(PBCallbackFunctionPtr)(OAPIServerPlayback::DataHandler), 
false, this), m_bQuit(false)
{
	/* auto delete when the thread exit */
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}
OAPIServerPlayback::~OAPIServerPlayback()
{

}

void OAPIServerPlayback::run()
{
	m_pbWrapper.run();
	while(m_bQuit != true)
	{
		ve_sleep(300);
	}
}

bool OAPIServerPlayback::SeekToTime(u32 seekTime)
{
	m_pbWrapper.SeekToTime(seekTime);
	return true;
}
bool OAPIServerPlayback::StartPlay()
{
	m_pbWrapper.StartPlay();
	return true;
}
bool OAPIServerPlayback::PausePlay()
{
	m_pbWrapper.StopPlay();
	return true;
}
bool OAPIServerPlayback::QuitPlay()
{
	XGuard guard(m_cMutex);
	m_bQuit = true;
	m_pbWrapper.QuitPlay();
	return true;
}

OAPIServer::OAPIServer(XRef<XSocket> pSocket, Factory &pFactory, VEventServer &pEvent)
:m_pFactory(pFactory), m_pSocket(pSocket), m_cnt(0), 
m_bLogin(FALSE), m_bStreaming(false), m_bRegNotify(false), m_pPlayback(NULL), 
m_pCamSearch(NULL), m_bRealEvent(false), m_bSearchEvent(false), 
m_pEvent(pEvent)
{
	UUIDGenerator uuidCreator;
	
	m_seesionId  = uuidCreator.createRandom().toString();
}

bool OAPIServer::CallChange(void* pParam, FactoryCameraChangeData data)
{
    int dummy = errno;
    OAPIServer * pObject = (OAPIServer * )pParam;

    if (pObject)
    {
        return pObject->CallChange1(data);
    }
}
bool OAPIServer::CallChange1(FactoryCameraChangeData data)
{
	/* Streaming channel do not call the camera change */
	if (m_bStreaming == true)
	{
		return true;
	}

	switch(data.type)
	{
		case FACTORY_CAMERA_ADD:
			return NotifyCamAdd(data);
			break;
		case FACTORY_CAMERA_DEL:
			return NotifyCamDel(data);
			break;
		case FACTORY_CAMERA_ONLINE:
			return NotifyCamOnline(data);
			break;
		case FACTORY_CAMERA_OFFLINE:
			return NotifyCamOffline(data);
			break;
		case FACTORY_CAMERA_REC_ON:
			return NotifyCamRecOn(data);
			break;
		case FACTORY_CAMERA_REC_OFF:
			return NotifyCamRecOff(data);
			break;
		default:
			break;		
	}
}

bool OAPIServer::NotifyCamAdd(FactoryCameraChangeData data)
{
	OAPIHeader header;
	oapi::OAPICamAddNotify sCam;
	VidCamera cCam;
	if (m_pFactory.GetConfDB().GetCameraConf(data.id, cCam) == false)
	{
		return false;
	}
	OAPIConverter::Converter(cCam, sCam.cam);

	
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sCam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_NOTIFY_DEVICE_ADD);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	
	return true;
}
bool OAPIServer::NotifyCamDel(FactoryCameraChangeData data)
{
	OAPIHeader header;
	oapi::OAPICamNotifyId sCam;

	sCam.strId = data.id;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sCam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_NOTIFY_DEVICE_DEL);
	header.length = htonl(nJsonLen + 1);


	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	
	return true;
}
bool OAPIServer::NotifyCamOnline(FactoryCameraChangeData data)
{
	OAPIHeader header;
	oapi::OAPICamNotifyId sCam;

	sCam.strId = data.id;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sCam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_NOTIFY_DEVICE_ONLINE);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	
	return true;
}
bool OAPIServer::NotifyCamOffline(FactoryCameraChangeData data)
{
	OAPIHeader header;
	oapi::OAPICamNotifyId sCam;

	sCam.strId = data.id;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sCam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_NOTIFY_DEVICE_OFFLINE);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	
	return true;
}
bool OAPIServer::NotifyCamRecOn(FactoryCameraChangeData data)
{
	OAPIHeader header;
	oapi::OAPICamNotifyId sCam;

	sCam.strId = data.id;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sCam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_NOTIFY_DEVICE_REC_ON);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	
	return true;
}
bool OAPIServer::NotifyCamRecOff(FactoryCameraChangeData data)
{
	OAPIHeader header;
	oapi::OAPICamNotifyId sCam;

	sCam.strId = data.id;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(sCam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	header.cmd = htonl(OAPI_NOTIFY_DEVICE_REC_OFF);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	return true;
}

OAPIServer::~OAPIServer()
{
	if (m_bStreaming == true)
	{
		if (m_nStream == 1)
		{
			m_pFactory.UnRegDataCallback(m_strLiveviewId, (void *)this);
		}
		else if (m_nStream == 2)
		{
			m_pFactory.UnRegSubDataCallback(m_strLiveviewId, (void *)this);
		}else 
		{
			m_pFactory.UnRegDataCallback(m_strLiveviewId, (void *)this);
		}

	}

	if (m_bRegNotify)
	{
		m_pFactory.UnRegCameraChangeNotify((void *)this);
	}

	if (m_pPlayback)
	{
		m_pPlayback->QuitPlay();
		m_pPlayback = NULL;
	}

	if (m_bSearchEvent == true)
	{
		m_pEvent.UnRegSearchEventNotify((void *)this);
	}
	if (m_bRealEvent == true)
	{
		m_pEvent.UnRegEventNotify((void *)this);
	}
}

BOOL OAPIServer::ProcessStartLive(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIStartLiveViewReq liveview;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, liveview, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}

	m_strLiveviewId = liveview.strId;
	m_nStream = liveview.nStream;

	if (liveview.nStream == 1)
	{
		m_pFactory.RegDataCallback(liveview.strId,
		(CameraDataCallbackFunctionPtr)OAPIServer::DataHandler, 
			(void *)this);
	}
	else if (liveview.nStream == 2)
	{
		m_pFactory.RegSubDataCallback(liveview.strId,
		(CameraDataCallbackFunctionPtr)OAPIServer::DataHandler, 
			(void *)this);
	}else 
	{
		m_pFactory.RegDataCallback(liveview.strId,
		(CameraDataCallbackFunctionPtr)OAPIServer::DataHandler, 
			(void *)this);
	}

	m_bStreaming = true;

	delete [] pRecv;
	SendCmnRetRsp(OAPI_START_LIVE_RSP, true);

	return TRUE;
}

BOOL OAPIServer::ProcessStopLive(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIStopLiveViewReq liveview;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, liveview, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}
	/* Just use the id and stream when started */
	if (m_bStreaming == true)
	{
		if (m_nStream == 1)
		{
			m_pFactory.UnRegDataCallback(m_strLiveviewId, (void *)this);
		}
		else if (m_nStream == 2)
		{
			m_pFactory.UnRegSubDataCallback(m_strLiveviewId, (void *)this);
		}else 
		{
			m_pFactory.UnRegDataCallback(m_strLiveviewId, (void *)this);
		}

	}

	m_bStreaming = false;

	delete [] pRecv;

	SendCmnRetRsp(OAPI_STOP_LIVE_RSP, true);

	return TRUE;
}

bool OAPIServer::ProcessAddCam(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIAddCameraReq cam;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, cam, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	VidCamera addCam;

	OAPIConverter::Converter(cam.cam, addCam);

	CameraParam addParam(addCam);

	m_pFactory.AddCamera(addParam);

	delete [] pRecv;

	SendCmnRetRsp(OAPI_ADD_CAM_RSP, true);

	return TRUE;
}

bool OAPIServer::ProcessSetCamSched(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPICameraUpdateSchedReq cam;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, cam, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	VidCamera updateCam;

	OAPIConverter::Converter(cam.cam, updateCam);

	m_pFactory.UpdateRecSched(updateCam.strid(), updateCam);

	delete [] pRecv;

	SendCmnRetRsp(OAPI_SET_CAM_SCHED_RSP, true);

	return TRUE;
}

bool OAPIServer::ProcessDeleteCam(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIDeleteCameraReq cam;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, cam, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	m_pFactory.DelCamera(cam.strId);

	delete [] pRecv;

	SendCmnRetRsp(OAPI_DEL_CAM_RSP, true);

	return TRUE;
}

bool OAPIServer::ProcessPtzCmd(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIPtzCmd cmd;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, cmd, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	m_pFactory.PtzAction(cmd.strId, (FPtzAction)(cmd.nPtzCmd), 
					(float)cmd.nParam);
}

bool OAPIServer::ProcessAddDisk(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIAddDiskReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	m_pFactory.AddHdd(req.disk.strId, req.disk.strPath, req.disk.nStorLimit);

	delete [] pRecv;

	SendCmnRetRsp(OAPI_ADD_DISK_RSP, true);

	return TRUE;
}

bool OAPIServer::ProcessDeleteDisk(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIDelDiskReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	m_pFactory.DelHdd(req.strId);

	delete [] pRecv;

	SendCmnRetRsp(OAPI_DEL_DISK_RSP, true);

	return TRUE;
}

bool OAPIServer::ProcessUpdateDiskLimit(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIUpdateDiskLimitReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	m_pFactory.HddUpdateSize(req.disk.strId, req.disk.nStorLimit);

	delete [] pRecv;

	SendCmnRetRsp(OAPI_UPDATE_DISK_LIMIT_RSP, true);

	return TRUE;
}


bool OAPIServer::ProcessRegNotify(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIRegNotifyReq notify;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, notify, result))
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	delete [] pRecv;
	m_pFactory.RegCameraChangeNotify(this,
		(FactoryCameraChangeNotify)(OAPIServer::CallChange));
	m_bRegNotify = true;
	SendCmnRetRsp(OAPI_REG_NOTIFY_RSP, true);

	return TRUE;
}

inline BOOL OAPIServer::ProcessLogin(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::LoginReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}
	delete [] pRecv;
	pRecv = NULL;

	/* current only support admin */
	std::string UserAdmin = "admin";

	if (req.User != UserAdmin)
	{
		return FALSE;
	}
#if 0
	/* Get user data */
	VSCUserData user;
	m_pFactory.GetUserData(user);
	astring realPasswd = user.data.conf.Passwd;

	/* calc the md5 and compare */
	std::string pass = m_seesionId + realPasswd;

	XMD5 md5Check;
	md5Check.Update((const uint8_t*)(pass.c_str()), pass.length());

	md5Check.Finalize();
	std::string md5Output = md5Check.GetAsString().c_str();
	

	oapi::LoginRsp rsp;

	if (md5Output == req.Password)
	{
		rsp.bRet = true;
		m_bLogin = TRUE;
	}else
	{
		rsp.bRetNonce = true;
		rsp.bRet = false;
	}

	rsp.Nonce = m_seesionId;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(rsp);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}

	OAPIHeader header;
	header.cmd = htonl(OAPI_LOGIN_RSP);
	header.length = htonl(nJsonLen + 1);

	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
#else
	astring realPasswd = "admin";

	/* calc the md5 and compare */
	std::string pass = m_seesionId + realPasswd;

	XMD5 md5Check;
	md5Check.Update((const uint8_t*)(pass.c_str()), pass.length());

	md5Check.Finalize();
	std::string md5Output = md5Check.GetAsString().c_str();
	

	oapi::LoginRsp rsp;

	if (md5Output == req.Password)
	{
		rsp.bRet = true;
		m_bLogin = TRUE;
	}else
	{
		rsp.bRetNonce = true;
		rsp.bRet = false;
	}

	rsp.Nonce = m_seesionId;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(rsp);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}

	OAPIHeader header;
	header.cmd = htonl(OAPI_LOGIN_RSP);
	header.length = htonl(nJsonLen + 1);


	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
#endif
	return TRUE;
	
}

BOOL OAPIServer::ProcessGetLic(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIGetLicReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}

	oapi::OAPIGetLicRsp rsp;

	m_pFactory.GetLicense(rsp.strLic, rsp.strHostId, rsp.nCh, rsp.strType,
					rsp.strExpireTime);

	std::string strJson = autojsoncxx::to_pretty_json_string(rsp);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	OAPIHeader header;
	header.cmd = htonl(OAPI_GET_LIC_RSP);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

inline BOOL OAPIServer::ProcessGetVer(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIGetVerReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}

	oapi::OAPIGetVerRsp rsp;

	rsp.strVer = VE_VERSION;
	rsp.strInfo = VE_INFO;

	std::string strJson = autojsoncxx::to_pretty_json_string(rsp);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	OAPIHeader header;
	header.cmd = htonl(OAPI_GET_VER_RSP);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

BOOL OAPIServer::ProcessConfAdmin(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIConfAdminReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}
	bool nRet = false;
	astring strCurrPasswd;
	m_pFactory.GetAdminPasswd(strCurrPasswd);
	if (strCurrPasswd == req.strOldPasswd)
	{
		m_pFactory.SetAdminPasswd(req.strNewPasswd);
		nRet = true;
	}

	delete [] pRecv;

	SendCmnRetRsp(OAPI_CONF_ADMIN_RSP, nRet);

	return TRUE;
}

BOOL OAPIServer::ProcessConfLic(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIConfLicReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	m_pFactory.SetLicense(req.strLic);

	delete [] pRecv;

	SendCmnRetRsp(OAPI_CONF_LIC_RSP, true);

	return TRUE;
}

BOOL OAPIServer::ProcessGetDevice(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPICameraListReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}

	oapi::OAPICameraListRsp dataList;
	dataList.Num = 0;

	CameraOnlineMap pCameraOnlineMap;
	CameraRecMap pCameraRecMap;

	m_pFactory.GetCameraOnlineMap(pCameraOnlineMap);
	m_pFactory.GetCameraRecMap(pCameraRecMap);

	VidCameraList pCameraList;
	m_pFactory.GetCameraList(pCameraList);

	for (s32 i = 0; i < pCameraList.cvidcamera_size(); i ++)
	{
		const VidCamera &cam = pCameraList.cvidcamera(i);
		oapi::OAPICamera data;
		OAPIConverter::Converter((VidCamera &)cam, data);
		data.bOnline = pCameraOnlineMap[cam.strid()];
		data.bRec = pCameraRecMap[cam.strid()];
		dataList.list.push_back(data);
		dataList.Num ++;
	}

	//autojsoncxx::to_json_string

	std::string strJson = autojsoncxx::to_pretty_json_string(dataList);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	OAPIHeader header;
	header.cmd = htonl(OAPI_CAM_LIST_RSP);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

BOOL OAPIServer::ProcessGetDisk(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIDiskListReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
	}

	oapi::OAPIDiskListRsp dataList;
	VDBDiskMap diskMap;
	m_pFactory.GetDiskMap(diskMap);

       VDBDiskMap::iterator it = diskMap.begin(); 
       for(; it!=diskMap.end(); ++it)
	{
		oapi::OAPIDisk oapiDisk;
		oapiDisk.strId = (*it).second.hdd;
		oapiDisk.strPath = (*it).second.path;
		oapiDisk.nTotalSize = 0;
		oapiDisk.nFreeSize = 0;
		oapiDisk.nStorLimit = (*it).second.limit;
		oapiDisk.nStorUsed = (*it).second.used;
		dataList.list.push_back(oapiDisk);
	}

	std::string strJson = autojsoncxx::to_pretty_json_string(dataList);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	
	OAPIHeader header;
	header.cmd = htonl(OAPI_DISK_LIST_RSP);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

BOOL OAPIServer::ProcessGetCam(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPICameraGetReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
	}

	oapi::OAPICameraGetRsp rsp;

	VidCamera pCam;
	m_pFactory.GetCamera(req.strId, pCam);

	OAPIConverter::Converter((VidCamera &)pCam, rsp.cam);

	std::string strJson = autojsoncxx::to_pretty_json_string(rsp);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	
	OAPIHeader header;
	header.cmd = htonl(OAPI_GET_CAM_RSP);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

BOOL OAPIServer::ProcessGetSysDisk(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPISysDiskListReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
	}

	oapi::OAPISysDiskListRsp dataList;
	QList<QStorageInfo> hdd = QStorageInfo::mountedVolumes();
	QListIterator<QStorageInfo> it(hdd);
	while(it.hasNext())
	{
		QStorageInfo  disk = it.next();
		astring strHdd = disk.rootPath().toStdString();
		
		VDC_DEBUG( "%s strHdd %s \n",__FUNCTION__, strHdd.c_str());
		s64 totalSize = disk.bytesTotal() / (1024 * 1024);
		s64 leftSize = disk.bytesFree()/ (1024 * 1024);
		
		VDC_DEBUG( "%s Total %lld M Left %lld M \n",__FUNCTION__, totalSize, leftSize);  
		QString strQtHdd = disk.rootPath();
		HddDriveType diskType = HddGetDriveType(strQtHdd);
		
		VDC_DEBUG( "%s Type %d \n",__FUNCTION__,  diskType);
		
		if (totalSize/1024 < 4 || leftSize/1024 < 2) /* In G */
		{
			continue;
		}
		
		if (diskType == HddInternalDrive 
			|| diskType == HddRemovableDrive || diskType == HddRemoteDrive)
		{
			oapi::OAPIDisk oapiDisk;
			oapiDisk.strId = disk.device().toStdString();
			oapiDisk.strPath = disk.rootPath().toStdString();
			oapiDisk.nTotalSize = totalSize;
			oapiDisk.nFreeSize = leftSize;
			oapiDisk.nStorLimit = 0;
			oapiDisk.nStorUsed = 0;
			dataList.list.push_back(oapiDisk);
		}
	}

	std::string strJson = autojsoncxx::to_pretty_json_string(dataList);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	
	OAPIHeader header;
	header.cmd = htonl(OAPI_SYS_DISK_LIST_RSP);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

inline bool OAPIServer::ProcessCamSearchStart(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPISearchCamStartReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	m_pCamSearch = new OAPIServerCamSearch(*this, m_pFactory);
	
	delete [] pRecv;
	SendCmnRetRsp(OAPI_CAM_SEARCH_START_RSP, true);
	m_pCamSearch->start();

	return true;	
}

inline bool OAPIServer::ProcessCamSearchStop(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPISearchCamStopReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}
	if (m_pCamSearch)
	{
		m_pCamSearch->QuitSearch();
		m_pCamSearch = NULL;
	}
	delete [] pRecv;
	SendCmnRetRsp(OAPI_CAM_SEARCH_STOP_RSP, true);

	return true;	
}

bool OAPIServer::ProcessGetStreamList(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIStreamListReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
	}

	oapi::OAPIStreamListRsp rsp;

	VidStreamList pList;
	m_pFactory.GetCamStreamList(req.strId, pList);

	OAPIConverter::Converter((VidStreamList &)pList, rsp);

	std::string strJson = autojsoncxx::to_pretty_json_string(rsp);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	
	OAPIHeader header;
	header.cmd = htonl(OAPI_STREAM_LIST_RSP);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}


bool OAPIServer::ProcessSearchRec(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPISearchRecordReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
	}

	RecordItemMap recMap;

	m_pFactory.SearchItems(req.strId, req.nStart, req.nEnd, req.nType, recMap);

	oapi::OAPISearchRecordRsp recList;
	recList.Num = 0;
	RecordItemMap::iterator it = recMap.begin(); 
	recList.strId = req.strId;
	for(; it!=recMap.end(); ++it)
	{	
		oapi::OAPIRecordItem recItem;
		recItem.nId = (*it).second.id;
		recItem.nStart = (*it).second.start;
		recItem.nEnd = (*it).second.end;
		recItem.nType = (*it).second.type;

		recList.record.push_back(recItem);
		recList.Num ++;
	}

	std::string strJson = autojsoncxx::to_pretty_json_string(recList);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	
	OAPIHeader header;
	header.cmd = htonl(OAPI_SEARCH_REC_RSP);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

bool OAPIServer::ProcessSearchHasRec(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPISearchHasRecordReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
	}

	oapi::OAPISearchHasRecordRsp rsp;

	rsp.strId = req.strId;
	rsp.Num = 0;
	std::vector<oapi::OAPIHasRecordItem>::iterator it;
	for (it = req.record.begin(); it != req.record.end(); it ++)
	{
		(*it).bHas = m_pFactory.SearchHasItems(req.strId, (*it).nStart, (*it).nEnd, (*it).nType);
		rsp.hasRecord.push_back(*it);
		rsp.Num ++;
	}
	
	std::string strJson = autojsoncxx::to_pretty_json_string(rsp);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	
	OAPIHeader header;
	header.cmd = htonl(OAPI_SEARCH_HAS_REC_RSP);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

bool OAPIServer::ProcessStartPlayback(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIPlaybackReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	if (m_pPlayback != NULL)
	{
		m_pPlayback->QuitPlay();
		/* the playback thread will delete auto */
		m_pPlayback = NULL;
	}

	m_pPlayback = new OAPIServerPlayback(*this, m_pFactory, req.strId, req.nPlayTime);
	m_pPlayback->start();
	delete [] pRecv;

	return TRUE;
}
bool OAPIServer::ProcessPausePlayback(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIPlaybackPauseReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	if (m_pPlayback != NULL)
	{
		m_pPlayback->PausePlay();
	}

	delete [] pRecv;

	return TRUE;
}
bool OAPIServer::ProcessResumePlayback(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIPlaybackResumeReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	if (m_pPlayback != NULL)
	{
		m_pPlayback->StartPlay();
	}

	delete [] pRecv;

	return TRUE;
}
bool OAPIServer::ProcessSeekPlayback(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIPlaybackSeekReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	if (m_pPlayback != NULL)
	{
		m_pPlayback->SeekToTime(req.nPlayTime);
	}

	delete [] pRecv;

	return TRUE;
}
bool OAPIServer::ProcessStopPlayback(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIPlaybackStopReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	if (m_pPlayback != NULL)
	{
		m_pPlayback->QuitPlay();
		/* the playback thread will delete auto */
		m_pPlayback = NULL;
	}

	delete [] pRecv;

	return TRUE;
}

bool OAPIServer::ProcessSearchEvent(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPISearchEventReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	m_pEvent.RegSearchEventNotify(this, (FunctionEventNotify)(OAPIServer::SearchEventHandler));
	m_pEvent.SearchEvent(req.strId, req.nStart, req.nEnd, this);
	m_bSearchEvent = true;

	delete [] pRecv;

	SendCmnRetRsp(OAPI_SEARCH_EVENT_RSP, true);

	return TRUE;

}

bool OAPIServer::ProcessRegEvent(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIRegEventReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	m_pEvent.RegEventNotify(this, OAPIServer::EventHandler);
	m_bRealEvent = true;

	delete [] pRecv;

	SendCmnRetRsp(OAPI_REG_EVENT_REQ, true);

	return TRUE;
}
bool OAPIServer::ProcessUnRegEvent(s32 len)
{
	if (len == 0)
	{
		return false;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::OAPIRegEventReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return false;
		}
		
	}

	m_pEvent.UnRegEventNotify(this);
	m_bRealEvent = false;

	delete [] pRecv;

	SendCmnRetRsp(OAPI_UNREG_EVENT_REQ, true);

	return TRUE;
}

bool OAPIServer::NewFrame(VideoFrame& frame)
{
	DataHandler1(frame);
	return true;
}

bool OAPIServer::NewCam(astring strIP, astring strPort, 
			astring strModel, astring strONVIFAddr)
{
	oapi::OAPICamSearchedNotify cam;

	cam.strIP = strIP;
	cam.strPort = strPort;
	cam.strModel = strModel;
	cam.strONVIFAddress = strONVIFAddr;

	std::string strJson = autojsoncxx::to_pretty_json_string(cam);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	
	OAPIHeader header;
	header.cmd = htonl(OAPI_CAM_SAERCH_PUSH);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return true;
}

BOOL OAPIServer::Process(OAPIHeader &header)
{
	header.cmd = ntohl(header.cmd);
	header.length = ntohl(header.length);
	if (m_bLogin == FALSE && header.cmd != OAPI_LOGIN_REQ)
	{
		//return FALSE;
	}

	switch(header.cmd)
	{
		case OAPI_LOGIN_REQ:
			return ProcessLogin(header.length);
			break;
		case OAPI_KEEPALIVE_REQ:
			break;
		case OAPI_CAM_LIST_REQ:
			return ProcessGetDevice(header.length);
			break;
		case OAPI_START_LIVE_REQ:
			return ProcessStartLive(header.length);
			break;
		case OAPI_STOP_LIVE_REQ:
			return ProcessStopLive(header.length);
			break;
		case OAPI_ADD_CAM_REQ:
			return ProcessAddCam(header.length);
			break;
		case OAPI_DEL_CAM_REQ:
			return ProcessDeleteCam(header.length);
			break;
		case OAPI_REG_NOTIFY_REQ:
			return ProcessRegNotify(header.length);
			break;
		case OAPI_GET_LIC_REQ:
			return ProcessGetLic(header.length);
			break;
		case OAPI_CONF_LIC_REQ:
			return ProcessConfLic(header.length);
			break;
		case OAPI_CONF_ADMIN_REQ:
			return ProcessConfAdmin(header.length);
			break;
		case OAPI_DISK_LIST_REQ:
			return ProcessGetDisk(header.length);
			break;
		case OAPI_STREAM_LIST_REQ:
			return ProcessGetStreamList(header.length);
			break;
		case OAPI_SYS_DISK_LIST_REQ:
			return ProcessGetSysDisk(header.length);
			break;
		case OAPI_ADD_DISK_REQ:
			return ProcessAddDisk(header.length);
			break;
		case OAPI_DEL_DISK_REQ:
			return ProcessDeleteDisk(header.length);
			break;
		case OAPI_UPDATE_DISK_LIMIT_REQ:
			return ProcessUpdateDiskLimit(header.length);
			break;
		case OAPI_CAM_SEARCH_START_REQ:
			return ProcessCamSearchStart(header.length);
			break;
		case OAPI_CAM_SEARCH_STOP_REQ:
			return ProcessCamSearchStop(header.length);
			break;
		case OAPI_GET_VER_REQ:
			return ProcessGetVer(header.length);
			break;
		case OAPI_GET_CAM_REQ:
			return ProcessGetCam(header.length);
			break;
		case OAPI_SET_CAM_SCHED_REQ:
			return ProcessSetCamSched(header.length);
			break;
		case OAPI_PTZ_CMD:
			return ProcessPtzCmd(header.length);
			break;
		case OAPI_SEARCH_REC_REQ:
			return ProcessSearchRec(header.length);
			break;
		case OAPI_SEARCH_HAS_REC_REQ:
			return ProcessSearchHasRec(header.length);
			break;
		case OAPI_START_PLAYBACK_REQ:
			return ProcessStartPlayback(header.length);
			break;
		case OAPI_PAUSE_PLAYBACK_REQ:
			return ProcessPausePlayback(header.length);
			break;
		case OAPI_RESUME_PLAYBACK_REQ:
			return ProcessResumePlayback(header.length);
			break;
		case OAPI_SEEK_PLAYBACK_REQ:
			return ProcessSeekPlayback(header.length);
			break;
		case OAPI_STOP_PLAYBACK_REQ:
			return ProcessStopPlayback(header.length);
			break;
		case OAPI_SEARCH_EVENT_REQ:
			return ProcessSearchEvent(header.length);
			break;
		case OAPI_REG_EVENT_REQ:
			return ProcessRegEvent(header.length);
			break;
		case OAPI_UNREG_EVENT_REQ:
			return ProcessUnRegEvent(header.length);
			break;
		default:
			break;		
	}
	return TRUE;
}

inline bool OAPIServer::SendCmnRetRsp(OAPICmd nCmd, 
				bool bRet)
{
	oapi::CmnRetRsp RetRsp;
	RetRsp.bRet = bRet;
	std::string strJson = autojsoncxx::to_pretty_json_string(RetRsp);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return false;
	}
	OAPIHeader header;
	header.cmd = htonl(nCmd);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
}


inline void OAPIServer::DataHandler1(VideoFrame& frame)
{
	VideoFrameHeader frameHeader;
	OAPIHeader header;
	header.cmd = htonl(OAPI_FRAME_PUSH);
	header.length = htonl(sizeof(frameHeader) + frame.dataLen);	

	frameHeader.streamType = htonl(frame.streamType);
	frameHeader.frameType = htonl(frame.frameType);
	frameHeader.secs = htonl(frame.secs);
	frameHeader.msecs = htonl(frame.msecs);
	frameHeader.dataLen = htonl(frame.dataLen);

	//printf("Send a new frame %d  length %d\n", m_cnt++, sizeof(frameHeader) + frame.dataLen);
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)&frameHeader, sizeof(frameHeader));
	m_pSocket->Send((void *)frame.dataBuf, frame.dataLen);
	
}

inline void OAPIServer::DataHandler(VideoFrame& frame, void * pParam)
{
    OAPIServer *pOapi = static_cast<OAPIServer *> (pParam);
    
    return pOapi->DataHandler1(frame);
}

inline void OAPIServer::EventHandler1(VEventData data)
{
	oapi::OAPIEventNotify event;

	event.strId = data.strId;
	event.strDevice = data.strDevice;
	event.strDeviceName = data.strDeviceName;
	event.nTime = data.nTime;
	event.strTime = data.strEvttime;
	event.strType = data.strType;
	event.bSearched = false;

	std::string strJson = autojsoncxx::to_pretty_json_string(event);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return ;
	}
	
	OAPIHeader header;
	header.cmd = htonl(OAPI_EVENT_PUSH);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return;
}
inline void OAPIServer::EventHandler(VEventData data, void* pParam)
{
    OAPIServer *pOapi = static_cast<OAPIServer *> (pParam);
    
    return pOapi->EventHandler1(data);
}
inline void OAPIServer::SearchEventHandler1(VEventData data)
{
	oapi::OAPIEventNotify event;

	event.strId = data.strId;
	event.strDevice = data.strDevice;
	event.strDeviceName = data.strDeviceName;
	event.nTime = data.nTime;
	event.strTime = data.strEvttime;
	event.strType = data.strType;
	event.bSearched = true;

	std::string strJson = autojsoncxx::to_pretty_json_string(event);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return;
	}
	
	OAPIHeader header;
	header.cmd = htonl(OAPI_EVENT_PUSH);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);
	return;
}
inline void OAPIServer::SearchEventHandler(VEventData data, void* pParam)
{
    OAPIServer *pOapi = static_cast<OAPIServer *> (pParam);
    
    return pOapi->EventHandler1(data);
}

#endif
