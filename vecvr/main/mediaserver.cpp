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


#include "Rtsp/UDPServer.h"
#include "Rtsp/RtspSession.h"
#include "Rtmp/RtmpSession.h"
#include "Http/HttpSession.h"

#ifdef ENABLE_OPENSSL
#include "Util/SSLBox.h"
#include "Http/HttpsSession.h"
#endif//ENABLE_OPENSSL

#include "Util/logger.h"
#include "Util/onceToken.h"
#include "Util/File.h"
#include "Network/TcpServer.h"
#include "Poller/EventPoller.h"
#include "Thread/WorkThreadPool.h"
#include "Device/PlayerProxy.h"
#include "Shell/ShellSession.h"
#include "Common/config.h"
#include <map>

#include "mediaserver.hpp"
#include "h264_parser.h"
using namespace std;
using namespace ZL::Util;
using namespace ZL::Http;
using namespace ZL::Rtsp;
using namespace ZL::Rtmp;
using namespace ZL::Shell;
using namespace ZL::Thread;
using namespace ZL::Network;
using namespace ZL::DEV;
using namespace media;



class VEMediaChannel : public DevChannel
{
public:
	VEMediaChannel(Factory &pFactory, std::string strId, int nStreamId);
	~VEMediaChannel();
	
public:
	static bool InitMediaServer();
	static void MediaServerThread();

public:
	void DataHandler1(VideoFrame& frame);
	static void DataHandler(VideoFrame& frame, void * pParam);	
		
private:
	std::string m_strId;
	int m_nStreamId;
	SmoothTicker m_Time;
	bool m_bFirstFrame;

	Factory &m_pFactory;
	H264Parser m_H264Parser;
	
	static TcpServer<RtspSession> * m_rtspSrv;
	static TcpServer<RtmpSession> * m_rtmpSrv;
	static TcpServer<HttpSession> * m_httpSrv;
	static TcpServer<ShellSession> * m_shellSrv;
	static std::thread * m_pThread;
};

TcpServer<RtspSession>* VEMediaChannel::m_rtspSrv = NULL;
TcpServer<RtmpSession>* VEMediaChannel::m_rtmpSrv = NULL;
TcpServer<HttpSession>* VEMediaChannel::m_httpSrv = NULL;
TcpServer<ShellSession>* VEMediaChannel::m_shellSrv = NULL;
std::thread * VEMediaChannel::m_pThread = NULL;

bool VEMediaChannel::InitMediaServer()
{
	Logger::Instance().add(std::make_shared<ConsoleChannel>("stdout", LTrace));
	Config::loaIniConfig();
	
	m_rtspSrv = new TcpServer<RtspSession>();
	m_rtmpSrv = new TcpServer<RtmpSession>();
	m_httpSrv = new TcpServer<HttpSession>();
	m_shellSrv = new TcpServer<ShellSession>();
	
	m_rtspSrv->start(mINI::Instance()[Config::Rtsp::kPort]);
	m_rtmpSrv->start(mINI::Instance()[Config::Rtmp::kPort]);
	m_httpSrv->start(mINI::Instance()[Config::Http::kPort]);
	
	//TODO add HTTPS support
	
	m_pThread = new std::thread(VEMediaChannel::MediaServerThread);
	
	return true;
	
}

void VEMediaChannel::MediaServerThread()
{
	
	/* Loop for media server */
	EventPoller::Instance().runLoop();

	delete m_rtspSrv;
	delete m_rtmpSrv;
	delete m_httpSrv;
	delete m_shellSrv;

	//TODO HTTPS support

	UDPServer::Destory();
	WorkThreadPool::Destory();
	EventPoller::Destory();
	Logger::Destory();	
	return;
}	

VEMediaChannel::VEMediaChannel(Factory &pFactory, std::string strId, int nStreamId)
:m_pFactory(pFactory), m_strId(strId), m_nStreamId(nStreamId), m_bFirstFrame(true),
DevChannel("live", strId.c_str())
{	
	printf("Device ID %s  Stream %d\n", strId.c_str(), m_nStreamId);
	if (m_nStreamId == 1)
	{
		m_pFactory.RegDataCallback(m_strId,
		(CameraDataCallbackFunctionPtr)VEMediaChannel::DataHandler, 
			(void *)this);
	}
	else if (m_nStreamId == 2)
	{
		m_pFactory.RegSubDataCallback(m_strId,
		(CameraDataCallbackFunctionPtr)VEMediaChannel::DataHandler, 
			(void *)this);
	}else 
	{
		m_pFactory.RegDataCallback(m_strId,
		(CameraDataCallbackFunctionPtr)VEMediaChannel::DataHandler, 
			(void *)this);
	}
}
VEMediaChannel::~VEMediaChannel()
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

void VEMediaChannel::DataHandler1(VideoFrame& frame)
{
	//printf("%s---%d\n", __FILE__, __LINE__);
	/* Process the video frame for media server */
	switch (frame.streamType)
	{
		case VIDEO_STREAM_VIDEO:
			break;
		case VIDEO_STREAM_AUDIO:
			return ;
		default:
		   	return;
	};	
	//printf("%s---%d\n", __FILE__, __LINE__);

	CodecType current;
	bool bIsKeyFrame = false;

	u8  *dataBuf = NULL;
	u32 dataSize = 0;

	VideoFrameType type = frame.frameType;
	//printf("%s---%d\n", __FILE__, __LINE__);

	switch (frame.frameType)
	{
		case VIDEO_FRM_I:
		{
			dataBuf = frame.dataBuf + sizeof(InfoFrameI);
			dataSize = frame.dataLen - sizeof(InfoFrameI);
			InfoFrameI *pI = (InfoFrameI *)frame.dataBuf;

			current = (CodecType)pI->video;

			if (current == CODEC_NONE)
			{
				return;
			}
			/* Current only support H264 */
			if (current != CODEC_H264)
			{
				return;
			}
			bIsKeyFrame = true;
			break;
		}
		case VIDEO_FRM_P:
		{
			dataBuf = frame.dataBuf + sizeof(InfoFrameP);
			dataSize = frame.dataLen - sizeof(InfoFrameP);
			InfoFrameP *pP = (InfoFrameP *)frame.dataBuf;
			current = (CodecType)pP->video;

			if (current == CODEC_NONE)
			{
				return;
			}
			/* Current only support H264 */
			if (current != CODEC_H264)
			{
				return;
			}
			break;
		}
		default:
		   	return;
	};
	//printf("%s---%d\n", __FILE__, __LINE__);
	if (m_bFirstFrame == true)
	{
		m_Time.resetTime();
	}
	
	m_H264Parser.Reset();
	m_H264Parser.SetStream(dataBuf, dataSize);
	int sps_id = 0;

	H264NALU nalu;
	std::vector<H264NALU> nalus;
	int pps_id;

	if (m_bFirstFrame == true)
	{
		if (bIsKeyFrame == false)
		{
			std::cout << "Have not get I frame skip" << std::endl;
			return;
		}
	}
	H264NALU nal_sps;
	H264NALU nal_pps;

	while (m_H264Parser.AdvanceToNextNALU(&nalu) != H264Parser::kEOStream) {
		switch (nalu.nal_unit_type) {
		case H264NALU::Type::kSPS:
			nal_sps = nalu;
			//Live555DumpHex((unsigned char *)nalu.data, nalu.size);
			m_H264Parser.ParseSPS(&sps_id);
			//std::cout << "get sps id:" << sps_id << std::endl;
			break;
		case H264NALU::Type::kPPS:
			nal_pps = nalu;
			m_H264Parser.ParsePPS(&pps_id);
			//std::cout << "get pps id:" << pps_id << std::endl;
			break;
		case H264NALU::Type::kSEIMessage:
			/* todo */
			break;
		case H264NALU::Type::kSliceDataA:
		case H264NALU::Type::kSliceDataB:
		case H264NALU::Type::kSliceDataC:
		case H264NALU::Type::kIDRSlice: 
		{
			break;
		}
		default:
			//std::cout << "NAL none" << std::endl;
			break;
		}
		nalus.push_back(nalu);
		
	}

	if (m_bFirstFrame && bIsKeyFrame == true)
	{
		const H264SPS* pSPS = m_H264Parser.GetSPS(sps_id);
		const H264PPS* pPPS = m_H264Parser.GetPPS(pps_id);
		VideoInfo info;
		info.iWidth  = (pSPS->pic_width_in_mbs_minus1 + 1) * 16;
		info.iHeight = (pSPS->pic_height_in_map_units_minus1 + 1) * 16;
		info.iFrameRate = 25;

		initVideo(info);
		m_bFirstFrame = false;
	}
	
	uint32_t uiStamp = m_Time.elapsedTime();

	/* loop to put frame */
	for (auto nalu : nalus) 
	{
		//if (nalu.nal_unit_type == H264NALU::Type::kIDRSlice || nalu.nal_unit_type == H264NALU::Type::kNonIDRSlice)
		{
			unsigned int *p = (unsigned int *) (nalu.data - 4);
			int psize = nalu.size + 4;
			if (psize > 20)
			{
				psize = 20;
			}
			//DumpHexData((unsigned char*)p,psize);
			//printf("%s---%d size %d\n", __FILE__, __LINE__, nalu.size + 4);
			inputH264((char *)p, nalu.size + 4, uiStamp);
		}
	}
	
	return;
}

void VEMediaChannel::DataHandler(VideoFrame& frame, void * pParam)
{
    VEMediaChannel *pObject = static_cast<VEMediaChannel *> (pParam);
    
    return pObject->DataHandler1(frame);
}



VEMediaServer::VEMediaServer(Factory &pFactory)
:m_pFactory(pFactory)
{
	InitDevices();
}
VEMediaServer::~VEMediaServer()
{
	
}

bool VEMediaServer::DeviceChangeCallbackFunc(void* pData, 
								FactoryCameraChangeData change)
{
	if (pData)
	{
		VEMediaServer * pthread = (VEMediaServer *)pData;
		pthread->DeviceChangeCallbackFunc1(change);
	}
	return true;
}
bool VEMediaServer::DeviceChangeCallbackFunc1(FactoryCameraChangeData change)
{
	VDC_DEBUG( "Media Server Device Change Callback %s type %d Begin\n", change.id.c_str(), change.type);
	
	if (change.type == FACTORY_CAMERA_OFFLINE)
	{
		DeleteOneDevice(change.id);
	}
	if (change.type == FACTORY_CAMERA_ONLINE)
	{
		VidCamera  pParam;
		if (m_pFactory.GetConfDB().GetCameraConf(change.id, pParam) == true)
		{
			if (pParam.bservermotion() == true)
			{
				InitOneDevice(pParam);
			}
		}
	}
	VDC_DEBUG( "Media Server Change Callback %s type %d End \n", change.id.c_str(), change.type);

	return true;
}

bool VEMediaServer::InitDevices()
{	
	VidCameraList pCameraList;
	
	//TODO Check if online
	m_pFactory.GetCameraList(pCameraList);

	int cameraSize = pCameraList.cvidcamera_size();

	for (s32 i = 0; i < pCameraList.cvidcamera_size(); i ++)
	{
		VidCamera cam = pCameraList.cvidcamera(i);
		InitOneDevice(cam);
	}
	return true;
}


bool VEMediaServer::DeleteOneDevice(astring strId)
{
	VEMediaChannelMap::iterator it = m_ChannelMap.begin();
	for(; it!=m_ChannelMap.end(); ++it)
	{
		if ((*it).first == strId)
		{
			delete (*it).second;
			m_ChannelMap.erase(it);
			break;
		}
	}
	return true;
}

bool VEMediaServer::InitOneDevice(VidCamera  pParam)
{
	VEMediaChannel *pChn = new VEMediaChannel(m_pFactory, pParam.strid(), 0);

	m_ChannelMap[pParam.strid()] = pChn;

	return true;
}


bool VEMediaServer::InitMediaServer()
{
	return VEMediaChannel::InitMediaServer();
}


