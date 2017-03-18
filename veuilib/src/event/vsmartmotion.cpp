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
#include "vsmartmotion.h"
#include "Poco/UUIDGenerator.h"

SmartMotionCam::SmartMotionCam(string strId, int nStreamId, astring strName,  astring strMotReg,
		Factory &pFactory, VEventServer &pEventServer)
:m_strId(strId), m_nStreamId(nStreamId), m_pFactory(pFactory), 
m_MotAlgo(false), m_pEventServer(pEventServer), m_nLastMotTime(0), m_strName(strName), 
m_motReg(VVidMotReg::CreateAll())
{
	/* Setup mot region */
	if (VVidMotReg::CheckStringForReg(strMotReg) == true)
	{
		VVidMotReg reg(strMotReg);
		m_motReg = reg;
	}
	
	m_vPlay.Init(false, "dummy", "dummy", "dummy", false, VSC_CONNECT_TCP);
	m_vPlay.StartGetRawFrame(this,
		(VPlayRawFrameHandler)SmartMotionCam::RawHandler);
	if (m_nStreamId == 1)
	{
		m_pFactory.RegDataCallback(m_strId,
		(CameraDataCallbackFunctionPtr)SmartMotionCam::DataHandler, 
			(void *)this);
	}
	else if (m_nStreamId == 2)
	{
		m_pFactory.RegSubDataCallback(m_strId,
		(CameraDataCallbackFunctionPtr)SmartMotionCam::DataHandler, 
			(void *)this);
	}else 
	{
		m_pFactory.RegDataCallback(m_strId,
		(CameraDataCallbackFunctionPtr)SmartMotionCam::DataHandler, 
			(void *)this);
	}
}
SmartMotionCam::~SmartMotionCam()
{
	if (m_nStreamId == 1)
	{
		m_pFactory.UnRegDataCallback(m_strId, (void *)this);
	}
	else if (m_nStreamId == 2)
	{
		m_pFactory.UnRegSubDataCallback(m_strId, (void *)this);
	}else 
	{
		m_pFactory.UnRegDataCallback(m_strId, (void *)this);
	}
}

void SmartMotionCam::DataHandler1(VideoFrame& frame)
{
	/* Process the video frame for motion */
	m_vPlay.PutRawData(frame);
}

void SmartMotionCam::DataHandler(VideoFrame& frame, void * pParam)
{
    SmartMotionCam *pObject = static_cast<SmartMotionCam *> (pParam);
    
    return pObject->DataHandler1(frame);
}

void SmartMotionCam::RawHandler1(RawFrame& frame)
{
	s64 nCurrent = time(NULL);
	/* Call the  mtion algo*/
	//VDC_DEBUG( "Smart Motion process frame\n");

	m_MotAlgo.ProcessFrame(frame);

	VBlobVec output;
	int w;
	int h;

	m_MotAlgo.GetOutput(output, w, h);
	//VDC_DEBUG( "Smart Motion process Output size %d\n", output.size());
	/* <10s motion is ignored */
	if (output.size() > 5 && abs(nCurrent - m_nLastMotTime) > 10)
	{	
		VDC_DEBUG( "Smart Motion Got\n");
		astring strEvtTime;
		strEvtTime = Time2String(nCurrent);		
		VEventData vEvent;
		Poco::UUIDGenerator uuidCreator;
		vEvent.strId = uuidCreator.createRandom().toString();
		vEvent.bMetaData = false;
		vEvent.strType = "SMART_MOTION";
		vEvent.strDevice = m_strId;
		vEvent.strDeviceName = m_strName;
		vEvent.strEvttime = strEvtTime;
		vEvent.nTime = nCurrent;
		vEvent.strDesc = "Server Motion Detection";

		m_pEventServer.PushEvent(vEvent);
		m_nLastMotTime = nCurrent;
	}
	
}
void SmartMotionCam::RawHandler(void* pData, RawFrame& frame)
{
    SmartMotionCam *pObject = static_cast<SmartMotionCam *> (pData);
    
    return pObject->RawHandler1(frame);
}


bool VSmartMotionThread::DeviceChangeCallbackFunc(void* pData, 
								FactoryCameraChangeData change)
{
	if (pData)
	{
		VSmartMotionThread * pthread = (VSmartMotionThread *)pData;
		pthread->DeviceChangeCallbackFunc1(change);
	}
	return true;
}
bool VSmartMotionThread::DeviceChangeCallbackFunc1(FactoryCameraChangeData change)
{
	VDC_DEBUG( "Smart Motion Device Change Callback %s type %d Begin\n", change.id.c_str(), change.type);
	
	if (change.type == FACTORY_CAMERA_OFFLINE)
	{
		DeleteOneDevice(change.id);
	}
	if (change.type == FACTORY_CAMERA_ONLINE)
	{
		Lock();
		VidCamera  pParam;
		if (m_pFactory.GetConfDB().GetCameraConf(change.id, pParam) == true)
		{
			if (pParam.bservermotion() == true)
			{
				InitOneDevice(pParam);
			}
		}
		UnLock();
	}
	VDC_DEBUG( "Smart Motion Change Callback %s type %d End \n", change.id.c_str(), change.type);

	return true;
}

void VSmartMotionThread::run()
{
	int ret;
	InitDevices();
	m_pFactory.RegCameraChangeNotify((void *)this, 
				(FactoryCameraChangeNotify)(VSmartMotionThread::DeviceChangeCallbackFunc));

	//Loop to recevie the event
	while(1)
	{
		ve_sleep(1000);
	}
	return;
}

bool VSmartMotionThread::InitDevices()
{
	Lock();
	
	VidCameraList pCameraList;

	int cameraSize = pCameraList.cvidcamera_size();

	for (s32 i = 0; i < pCameraList.cvidcamera_size(); i ++)
	{
		VidCamera cam = pCameraList.cvidcamera(i);
		InitOneDevice(cam);
	}
	UnLock();
	return true;
}


bool VSmartMotionThread::DeleteOneDevice(astring strId)
{
	Lock();
	SmartMotionCamMap::iterator it = m_SmartMotionMap.begin();
	for(; it!=m_SmartMotionMap.end(); ++it)
	{
		if ((*it).first == strId)
		{
			delete (*it).second;
			m_SmartMotionMap.erase(it);
			break;
		}
	}
	UnLock();
	return true;
}

bool VSmartMotionThread::InitOneDevice(VidCamera  pParam)
{
#if 0
	if (pParam.ntype() != VID_ONVIF_S)
	{
		return true;
	}

	astring IP = pParam.strip();
	astring Port = pParam.strport();
	astring User = pParam.struser();
	astring Password = pParam.strpasswd();
	astring OnvifAddress = pParam.stronvifaddress();
#endif

	SmartMotionCam *pMotCam = new SmartMotionCam(pParam.strid(), 
		pParam.nservermotionstream(), pParam.strname(), pParam.strmotreg(),
								m_pFactory, m_pEventServer);

	m_SmartMotionMap[pParam.strid()] = pMotCam;

	return true;
}
