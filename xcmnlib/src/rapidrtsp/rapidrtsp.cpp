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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility/utility.hpp"

#include "rapidrtsp/rapidrtsp.h"
#include "rapidrtsplog.h"
#include "ros.h"
#include "utility/videotype.hpp"
#include "XSDK/TimeUtils.h"
#include "Poco/URI.h"
#include "Poco/String.h"
#include "rtspdef.h"
#include "utility/utilitytimer.hpp"


using namespace UtilityLib;
using namespace XSDK;

static char *h264GetStartcode(char *start, char *end) 
{
	for (char *p = start; p < end - 4; p ++)
	{
		if(p[0] == 0x00 && p[1] == 0x00 && p[2] == 0x00 && p[3] == 0x01)
		{
			return p;
		}
	}

	return NULL;
}

static void fRapidRTSP_DEFAULT_DATA_HANDLE(void *pdata, unsigned int dataSize, 
	unsigned int sec, unsigned int msec,
	int dataType, VideoStreamType streamType, 
	VideoFrameType frameType, unsigned int flag,CRapidRTSPAVInfo &pAVInfo, void *context)
{
	printf("RapidRTSP Got %d data (size:%u timestamp:%u)  ===> \n", dataType, dataSize, sec);
	//DumpHexData((unsigned char *)pdata, 20);
	printf("info frame header I %d P %d\n", sizeof(InfoFrameI), sizeof(InfoFrameP));
}

static char *_get_stream_name(int dataType)
{
	switch(dataType)
	{
		case CODEC_H264:
			return "H264";
		case CODEC_H265:
			return "H265";
		case CODEC_AAC:
			return "AAC";
		case CODEC_PCMA:
			return "PCMA";
		case CODEC_PCMU:
			return "PCMU";
		default:
			return NULL;			
	}
}

CRapidRTSP::CRapidRTSP(std::string streamUrl, int transport, 
	std::string userName, std::string userPwd, 
	bool bEnableAudio)
: m_bEnableAudio(bEnableAudio), m_dataHandle(NULL), m_dataContext(NULL),
  m_userName(userName), m_userPwd(userPwd), m_streamUrl(streamUrl)
{
	Poco::URI rtspUrl(m_streamUrl);
	astring strRtsp;
	if (rtspUrl.empty() != true)
	{
		/* The user or the password is empty */
		if (m_userName.length() <= 0 || m_userPwd.length() <= 0)
		{
			strRtsp = rtspUrl.getScheme() + "://" + rtspUrl.getHost() + ":" + std::to_string(rtspUrl.getPort()) + rtspUrl.getPathAndQuery();
		}else
		{
			strRtsp = rtspUrl.getScheme() + "://" + m_userName + ":" + m_userPwd + "@" 
				+ rtspUrl.getHost() + ":" + std::to_string(rtspUrl.getPort()) 
				+ rtspUrl.getPathAndQuery();

		}
	}

	m_streamFullUrl = strRtsp;

	return;
}

CRapidRTSP::~CRapidRTSP()
{
	m_dataContext = NULL;
	m_dataHandle = NULL;
}

void CRapidRTSP::set_data_handle(fRapidRTSP_DATA_HANDLE handle, void *context)
{
	if (1) {
		m_dataContext = context;
				
		m_dataHandle = handle;
	}
}


CRapidRTSPFFMPEG::CRapidRTSPFFMPEG(std::string streamUrl, int transport, 
	std::string userName, std::string userPwd, 
	bool bEnableAudio)
: CRapidRTSP(streamUrl, transport, userName, userPwd, bEnableAudio), 
  m_bExit(false),  m_pContext(NULL), m_nState(RapidRTSP_INIT),m_nStartConnectTime(0), 
  m_nLastGetDataTime(0), m_LastVidPts(0), m_VidFlag(0), m_ConnectingStart(0)
{
	memset(&m_currVidTime, 0, sizeof(m_currVidTime));

	return;
}

CRapidRTSPFFMPEG::~CRapidRTSPFFMPEG()
{
	m_bExit = true;
	
	
	m_pThread->join();
	if (m_pContext)
	{
		avformat_free_context(m_pContext);
	}

	delete m_pThread;
	m_pThread = NULL;
}

int CRapidRTSPFFMPEG::start()
{
	m_pThread = new std::thread(CRapidRTSPFFMPEG::proc, this);

	return true;
}

void CRapidRTSPFFMPEG::proc(void *param)
{
	if (param != NULL)
	{
		CRapidRTSPFFMPEG * pRapidRTSP = (CRapidRTSPFFMPEG *)param;
		pRapidRTSP->proc1();
	}
}

int CRapidRTSPFFMPEG::CheckInterruptCallback(void * param)
{
	if (param != NULL)
	{
		CRapidRTSPFFMPEG * pRapidRTSP = (CRapidRTSPFFMPEG *)param;
		return pRapidRTSP->CheckInterruptCallback1();
	}

	return 0;
}

int CRapidRTSPFFMPEG::CheckInterruptCallback1()
{	
	/* Check if the thread exit */
	if (m_bExit == true)
	{
		return 1;
	}
	
	if (m_pContext == NULL)
	{
		/* Error, just return */
		return 1;
	}
	if (m_nState == RapidRTSP_CONNECTED)
	{
		double currCnt = GetTickCount();
		if (currCnt - m_nLastGetDataTime > 30 * 1000)
		{
			RRLOG(RRLOG_INFO, "%s av_read_frame Timeout", __FUNCTION__);
			return 1;
		}
		return 0;
	}
	/* Check is connect time out */
	if (m_nState == RapidRTSP_CONNECTING)
	{
		double currTime = GetTickCount();
		if (currTime - m_ConnectingStart > 30 * 1000)
		{
			RRLOG(RRLOG_INFO, "%s Connecting Timeout", __FUNCTION__);
			return 1;
		}
		
		if (m_pContext->nb_streams > 0)
		{
			for (int i = 0; i < m_pContext->nb_streams; i++) 
			{
				if (m_pContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) 
				{
					if (m_pContext->streams[i]->codec_info_nb_frames >= 1)
					{
						
						return 1;
					}
				}
			}
			return 0;
		}
	}
	return 0;
}

void CRapidRTSPFFMPEG::proc1()
{
	RRLOG(RRLOG_INFO, "%s CRapidRTSPFFMPEG Start", __FUNCTION__);
	int ret;
	AVRational timeBase;
	timeBase.num = 1;
	timeBase.den = 1000;

	/* Enable ffmpeg log level to get more information */
	av_log_set_level(8);

	while (m_bExit != true)
	{
		int nVidIdx = -1;
		int nAudIdx = -1;
		AVPacket packet;
		bool bGetVidinfo = false;
		if (m_pContext)
		{
			avformat_free_context(m_pContext);
		}

		m_nState = RapidRTSP_CONNECTING;
		m_ConnectingStart = GetTickCount();

		memset(&m_AVinfo, 0, sizeof(m_AVinfo));
		memset(&m_currVidTime, 0, sizeof(m_currVidTime));
		RRLOG(RRLOG_INFO, "%s Begin to connect URL '%s'\n", __FUNCTION__, 
				m_streamFullUrl.c_str());

		m_pContext = avformat_alloc_context();
		/* TODO support HTTP or HTTPS */
		AVDictionary *opts = 0;
		ret = av_dict_set(&opts, "rtsp_transport", "tcp", 0);

		m_pContext->interrupt_callback.callback = CRapidRTSPFFMPEG::CheckInterruptCallback;
		m_pContext->interrupt_callback.opaque = this;
		if ((ret = avformat_open_input(&m_pContext, m_streamFullUrl.c_str(), 0, &opts)) < 0) 
		{
			RRLOG(RRLOG_INFO, "%s Could not open URL '%s'\n", __FUNCTION__, 
				m_streamFullUrl.c_str());
			continue;
		}

		if ((ret = avformat_find_stream_info(m_pContext, 0)) < 0) 
		{
			RRLOG(RRLOG_INFO, "%s Could not get stream information '%s'\n", __FUNCTION__, 
				m_streamFullUrl.c_str());
			continue;
		}

		m_nState = RapidRTSP_CONNECTED;

		av_dump_format(m_pContext, 0, m_streamFullUrl.c_str(), 0);

		for (int i = 0; i < m_pContext->nb_streams; i++) 
		{
			if (m_pContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) 
			{
				nVidIdx = i;
				RRLOG(RRLOG_INFO, "%s Video index %d\n", __FUNCTION__, 
						nVidIdx);
				/* Update video codec info */
				switch (m_pContext->streams[i]->codec->codec_id)
				{
					case AV_CODEC_ID_H264:
					{
						m_AVinfo.video.codec = CODEC_H264;
						break;
					}
					case AV_CODEC_ID_HEVC:
					{
						m_AVinfo.video.codec = CODEC_H265;
						break;
					}
					case AV_CODEC_ID_MJPEG:
					{
						m_AVinfo.video.codec = CODEC_MJPEG;
						break;
					}
					case AV_CODEC_ID_MPEG4:
					{
						m_AVinfo.video.codec = CODEC_MPEG4;
						break;
					}
					default:
					{
						/* The codec don't support now, just return */
						RRLOG(RRLOG_ERROR, "%s codec do not support %d\n", __FUNCTION__, 
								m_pContext->streams[i]->codec->codec_id);
						return;
					}
					   	
				};

				if (m_pContext->streams[i]->codec->extradata_size > 0
					&& m_pContext->streams[i]->codec->extradata_size < RAPID_MAX_EXTRA_SIZE)
				{
					m_AVinfo.video.video.extradataSize = m_pContext->streams[i]->codec->extradata_size;
					memcpy(m_AVinfo.video.video.extradata, 
						m_pContext->streams[i]->codec->extradata, m_pContext->streams[i]->codec->extradata_size);
				}
				m_AVinfo.video.video.width = m_pContext->streams[i]->codec->width;
				m_AVinfo.video.video.height = m_pContext->streams[i]->codec->height;
				bGetVidinfo = true;
				
			} else if (m_pContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) 
			{
				nAudIdx = i;
				RRLOG(RRLOG_INFO, "%s Audio index %d\n", __FUNCTION__, 
						nAudIdx);
			}
		}
	

		/* Loop to read the video */
		m_nLastGetDataTime = GetTickCount();
		while (m_bExit != true)
		{
			av_init_packet(&packet); 
			ret = av_read_frame(m_pContext, &packet); 
			double currCnt = GetTickCount();
			if (ret >= 0)
			{
				RRLOG(RRLOG_INFO, "%s av_read_frame %d\n", __FUNCTION__, 
							packet.size);
			}else
			{
				RRLOG(RRLOG_INFO, "%s av_read_frame failed %d\n", __FUNCTION__, 
							ret);
				if (currCnt - m_ConnectingStart > 30 * 1000)
				{
					RRLOG(RRLOG_INFO, "%s Connecting Timeout", __FUNCTION__);
					break;
				}
				ve_sleep(1);
				continue;
			}
			m_nLastGetDataTime = currCnt;
			
			if (packet.stream_index == nAudIdx) 
			{
				/* process audio data */
			} 
			else if (packet.stream_index == nVidIdx) 
			{
				/* process video data */
	  			s64 pts = av_rescale_q(packet.pts, 
	  					m_pContext->streams[packet.stream_index]->time_base, timeBase); 

				VideoFrameType frameType = VIDEO_FRM_P;
				if (packet.flags & AV_PKT_FLAG_KEY)
				{
					frameType = VIDEO_FRM_I;
				}

				if (m_currVidTime.tv_sec == 0)
				{
					gettimeofday(&m_currVidTime, NULL);
				}else
				{
					
					s64 diff = pts - m_LastVidPts;
					if (diff < 0)
					{
						diff = 0;
					}
					struct timeval timeAdd;
					timeAdd.tv_sec = 0;
					timeAdd.tv_usec = diff;
					struct timeval timeNew;
					struct timeval timeCurr;
					gettimeofday(&timeCurr, NULL);

					timeradd(&m_currVidTime, &timeAdd, &timeNew);
					if (abs(timeNew.tv_sec - timeCurr.tv_sec) > 2)
					{
						memcpy(&m_currVidTime, &timeCurr, sizeof(m_currVidTime));
					}else
					{
						memcpy(&m_currVidTime, &timeNew, sizeof(m_currVidTime));
					}
				}
				RRLOG(RRLOG_INFO, "%s av_read_frame size %d ts %lld key %d (%d, %d)\n", __FUNCTION__, 
							packet.size, pts, frameType, m_currVidTime.tv_sec, m_currVidTime.tv_usec);
				m_LastVidPts = pts;
				if (m_AVinfo.video.codec == CODEC_H264)
				{
					if (packet.size < 5)
					{
						continue;
					}

					unsigned char nal_type = packet.data[4]&0x1f;
					if (nal_type == H264_SPS || nal_type == H264_PPS)
					{
						m_VidFlag = RTP_FLAG_HAS_SPS_PPS;
					}
				}
				if (m_dataHandle)
				{
					m_dataHandle(packet.data, packet.size, m_currVidTime.tv_sec, m_currVidTime.tv_usec/1000, 
						m_AVinfo.video.codec, VIDEO_STREAM_VIDEO, frameType, 
						m_VidFlag, m_AVinfo, m_dataContext);
				}
			}
			else 
			{
				av_packet_unref(&packet);
				continue;
			}
		}
		
	}
	return;
}


CRapidRTSPLive555::CRapidRTSPLive555(std::string streamUrl, int transport, 
	std::string userName, std::string userPwd, 
	bool bEnableAudio)
: CRapidRTSP(streamUrl, transport, userName, userPwd, bEnableAudio), 
m_rtsp(streamUrl, userName, userPwd), m_currRecv(0), m_lastRecv(0), m_bExit(false), m_bStarted(false)
{
	/* Register the data callback */
	m_rtsp.RegCallback(this, this);
	m_pWatchThread = new std::thread(CRapidRTSPLive555::WatchThread, this);
	return;
}

CRapidRTSPLive555::~CRapidRTSPLive555()
{
	/* Stop the watch thread */
	m_bExit = true;
	m_pWatchThread->join();
	delete m_pWatchThread;
	
	m_rtsp.Stop();
	m_bStarted = false;
}

bool CRapidRTSPLive555::WatchThread(void* pData)
{
	CRapidRTSPLive555 * pThread = (CRapidRTSPLive555 *)pData;

	if (pThread)
	{
		return pThread->WatchThreadRun();
	}
	return false;
}
bool CRapidRTSPLive555::WatchThreadRun()
{
	int i = 0;
	while(!m_bExit)
	{
		std::chrono::milliseconds dura(500);
		std::this_thread::sleep_for( dura );
		if (i ++ >= 40)
		{
			CheckRTSPClient();
			i = 0;
		}
	}
	return true;
}

bool CRapidRTSPLive555::CheckRTSPClient()
{
	s64 nCurrRecv = 0;

	{
		std::lock_guard<std::mutex> guard(m_MutexData);
		nCurrRecv = m_currRecv;
	}

	std::lock_guard<std::mutex> guard(m_Mutex);
	if (m_bStarted == false)
	{
		return true;
	}

	if (nCurrRecv <= m_lastRecv)
	{
		
		m_rtsp.Stop();
		m_rtsp.Start();
	}
	m_lastRecv = nCurrRecv;
	return true;
}

int CRapidRTSPLive555::start()
{
	std::lock_guard<std::mutex> guard(m_Mutex);
	m_rtsp.Start();
	m_bStarted = true;

	return true;
}


bool   CRapidRTSPLive555::onH5SData(unsigned char* buffer, int size, unsigned long long secs, 
	unsigned long long msecs, H5SCodecType codec, H5SStreamType stream, 
	H5SFrameType frm)
{
	/* update receive */
	{
		std::lock_guard<std::mutex> guard(m_MutexData);
		m_currRecv = m_currRecv + size;
	}
	if (m_dataHandle)
	{
		m_dataHandle(buffer, size, secs, msecs, 
			(int)codec, (VideoStreamType)stream, (VideoFrameType)frm, 
			RTP_FLAG_HAS_SPS_PPS, m_AVinfo, m_dataContext);		
	}
	
	return true;
}

