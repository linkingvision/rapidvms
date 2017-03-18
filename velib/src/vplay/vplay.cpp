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
#include "vplay.hpp"
#include "license.hpp"
#include "render_wrapper.hpp"
#include "ffkit/av_muxer.h"
#include "ffkit/av_demuxer.h"
#include "ffkit/h264_decoder.h"
#include "ffkit/fflocky.h"
#include "ffkit/ffoptions.h"
#include "cppkit/ck_memory.h"
#include "cppkit/os/ck_large_files.h"
#include "rapidmedia/rapidmedia.hpp"
#include "curl/curl.h"
#include "AVKit/JPEGEncoder.h"

#define ENABLE_SDL_RENDER 1

using namespace UtilityLib;
using namespace std;
using namespace cppkit;
using namespace ffkit;

fast_mutex gRenderNoVideoLock;
av_packet *gRenderNoVideoPic = NULL;

#define STATE_FRAME 1
#define STATE_MAIN_HEADER 2
#define STATE_FRAME_HEADER 3
#define HTTP_JPEG_HEADER_SIZE 1024

class VPlayData
{
public:
	/* First check the bMJPEG, if the bMJPEG is false then check the bFile */
	VPlayData(VPlay &pPlay, bool bFile, bool bMJPEG, astring strFile, string strUser, 
		string strPass,  BOOL bHWAccel)
	:m_vPlay(pPlay), m_bFile(bFile), m_strFile(strFile), m_bMJPEG(bMJPEG), m_strUser(strUser), 
	 m_strPass(strPass), m_bHWAccel(bHWAccel)
	{
		this->m_nAttachedPlayer = 0;
		this->pData = NULL;
		this->dataHandler = NULL;
		this->m_pThread = NULL;

		this->m_render = new RenderWrapper(m_bHWAccel);
		m_pktMJPEG.bufLen = 0;
		m_pktMJPEG.dataBuf = NULL;

		m_jpeg_buffer = NULL;
		m_jpeg_first_frame = true;
		m_bSingleJpeg = false;

		this->m_bExit = false;
		if (m_bFile == true && m_bMJPEG == false)
		{
			this->m_pThread = new tthread::thread(VPlayData::Run, (void *)this);
		}else if (m_bMJPEG == true)
		{
			this->m_pThread = new tthread::thread(VPlayData::RunMJPEG, (void *)this);
		}

		return;
	}
	~VPlayData()
	{
		m_avWrapper.StopRaw();
		m_avWrapper.Stop();
	  	if (m_bMJPEG == true)
	   	{
			m_bExit = true;
			m_pThread->join();
			delete m_pThread;
			m_pThread = NULL;
	   	}
		if (m_bMJPEG != true && m_bFile == true)
	   	{
			m_bExit = true;
			m_pThread->join();
			delete m_pThread;
			m_pThread = NULL;
	   	}
		if (this->m_render)
		{
			delete this->m_render;
		}
		if (m_jpeg_buffer)
		{
			free(m_jpeg_buffer);
			m_jpeg_buffer = NULL;
		}
		if (m_pktMJPEG.dataBuf)
		{
			free(m_pktMJPEG.dataBuf);
			m_pktMJPEG.dataBuf = NULL;
		}
	}
public:
	static void Run(void * pParam);
	void Run1();

	static void RunMJPEG(void * pParam);
	void RunMJPEG1();
	static size_t MJPEGWriteData(void *ptr, size_t size, size_t nmemb, void *userdata);
	size_t MJPEGWriteData1(void *ptr, size_t size, size_t nmemb);
	static int CurlXferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow);
	int CurlXferinfo1(curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow);
	void on_headerline(char *buf);
	void on_frame(unsigned char *ptr, int len);
public:
	VPlay &m_vPlay;
	RapidMedia m_avWrapper;
	s32 m_nAttachedPlayer;
	void* pData;
	VPlayDataHandler dataHandler;
	RenderWrapper *m_render;
	bool m_bHWAccel;

	bool m_bFile;
	bool m_bMJPEG;
	astring m_strUser;
	astring m_strPass;
	astring m_strFile;
	tthread::thread *m_pThread;
	struct timeval m_lastTime;
	bool m_bExit;
	VideoFrame m_pktMJPEG;

	/* HTTP JPEG */
	char m_headerline[HTTP_JPEG_HEADER_SIZE];
	int m_jpeg_frame_position;
	int m_jpeg_num_headers;
	int m_jpeg_frame_size;
	bool m_jpeg_first_frame;
	bool m_bSingleJpeg;
	int m_g_state;	
	unsigned char *m_jpeg_buffer;
	int m_jpeg_buffer_len;
};

static RenderType UpdateRenderType(bool bHWAccel, RenderType current)
{
	if (bHWAccel == false)
	{
		return current;
	}

#ifdef _WIN32
	return RENDER_TYPE_D3D;
#else
#ifdef __APPLE__
	/* Current MacOS do not support HW decode */
	return RENDER_TYPE_SDL;
#endif
	return RENDER_TYPE_VAAPI;
#endif
	
}

 void VPlayData::Run(void * pParam)
{
	int dummy = errno;
	VPlayData * pThread = (VPlayData *)pParam;

	if (pThread)
	{
	    pThread->Run1();
	}
}
//#define VPLAY_DUMP_RAW
 
void VPlayData::Run1()
{
	struct timeval current;
	VideoFrame packet;
	packet.bufLen = 1024 * 16;
	packet.dataBuf = (u8 *)malloc(packet.bufLen);
#ifdef VPLAY_DUMP_RAW
       FILE* fSink;    
       fopen_s(&fSink, "raw.264", "wb");
	BOOL bGotIFrame = FALSE;
#endif
	while (m_bExit != true)
	{
		try {
			VDC_DEBUG( "%s  Read File Source %s\n",__FUNCTION__, m_strFile.c_str());
			ve_sleep(1000);	
			struct stream_info info;
			shared_ptr<av_demuxer> deMuxer = make_shared<av_demuxer>( m_strFile );

			int videoStreamIndex = deMuxer->get_video_stream_index();
			deMuxer->get_stream_types(videoStreamIndex, info);
			VDC_DEBUG( "%s  Read File Codec %d (%d, %d)\n",__FUNCTION__, m_strFile.c_str(), 
				info.codec_id, info.width, info.height);
			packet.streamType = VIDEO_STREAM_VIDEO;

			int index = 0;
			int i = 0;
			int streamIndex = 0;
			double videoTimeDiff1 = deMuxer->get_seconds_between_frames(videoStreamIndex);
			int videoTimeDiff = videoTimeDiff1 * 1000;

			if (videoTimeDiff < 20)
			{
				videoTimeDiff = 20;
			}
			VDC_DEBUG( "%s  time diff %f %d\n",__FUNCTION__, videoTimeDiff1, videoTimeDiff);
			while(m_bExit != true && deMuxer->read_frame( streamIndex ))
			{
				if ( streamIndex != videoStreamIndex )
				{
					continue;
				}
				gettimeofday(&current, NULL);
				//VDC_DEBUG( "%s  current (%d, %d), videoTimeDiff %d\n",__FUNCTION__, 
				//		current.tv_sec, current.tv_usec, videoTimeDiff);


				shared_ptr<av_packet> frame = deMuxer->get();
#ifdef VPLAY_DUMP_RAW
				if (bGotIFrame == TRUE || deMuxer->is_key())
				{
					fwrite(frame->map(), 1, frame->get_data_size(), fSink);
					if (deMuxer->is_key())
					{	
						bGotIFrame = TRUE;
					}
				}
#endif
				if (packet.bufLen < (sizeof(InfoFrameI) + frame->get_data_size()))
				{
					free(packet.dataBuf);
					packet.bufLen = (sizeof(InfoFrameI) + frame->get_data_size());
					packet.dataBuf = (u8 *)malloc(packet.bufLen);
				}
				if (deMuxer->is_key() == true)
				{
					packet.frameType = VIDEO_FRM_I;
					memcpy(packet.dataBuf + sizeof(InfoFrameI), 
						frame->map(), frame->get_data_size());
					InfoFrameI *pI = (InfoFrameI *)packet.dataBuf;
					packet.dataLen = frame->get_data_size() + sizeof(InfoFrameI);
					pI->audio = CODEC_NONE;
					if (info.codec_id == AV_CODEC_ID_HEVC)
					{
						pI->video = CODEC_H265;
					}else if (info.codec_id == AV_CODEC_ID_H264)
					{
						pI->video = CODEC_H264;
					}else if (info.codec_id == AV_CODEC_ID_MJPEG)
					{
						pI->video = CODEC_MJPEG;
					}else if (info.codec_id == AV_CODEC_ID_MPEG4)
					{
						pI->video = CODEC_MPEG4;
					}else
					{
						pI->video = CODEC_H264;
					}

					pI->vWidth = info.width;
					pI->vHeight = info.height;
				}else
				{
					packet.frameType = VIDEO_FRM_P;
					memcpy(packet.dataBuf + sizeof(InfoFrameP), 
						frame->map(), frame->get_data_size());
					InfoFrameP *pP = (InfoFrameP *)packet.dataBuf;
					pP->audio = CODEC_NONE;
					packet.dataLen = frame->get_data_size() + sizeof(InfoFrameP);
					if (info.codec_id == AV_CODEC_ID_HEVC)
					{
						pP->video = CODEC_H265;
					}else if (info.codec_id == AV_CODEC_ID_H264)
					{
						pP->video = CODEC_H264;
					}else if (info.codec_id == AV_CODEC_ID_MJPEG)
					{
						pP->video = CODEC_MJPEG;
					}else if (info.codec_id == AV_CODEC_ID_MPEG4)
					{
						pP->video = CODEC_MPEG4;
					}else
					{
						pP->video = CODEC_H264;
					}
					
				}
				
				packet.secs = current.tv_sec;
				packet.msecs = current.tv_usec/1000;
				
				m_vPlay.PutRawData(packet);
				
				if (dataHandler)
				{
					dataHandler(pData, packet);
				}
				if (videoTimeDiff> 0)
				{
					ve_sleep(videoTimeDiff);
				}
				
			}
		}catch (...)
		{
		}
	}
	free(packet.dataBuf);

	return;
	
}

inline void VPlayData::on_headerline(char *buf) {
    // printf("HEADER: %s\n", buf);
    if (strncmp(buf, "Content-Length:", 15) == 0) {
        m_jpeg_frame_size = atoi(buf + 16);
	 //printf("HEADER: Got content length = %d bytes\n", m_jpeg_frame_size);
    }
    if (strncmp(buf, "Content-Type:", 13) == 0) {
        //printf("HEADER: Got content type = %s\n", buf + 14);
    }
}


inline void VPlayData::on_frame(unsigned char *ptr, int len) 
{
	struct timeval current;
	//VDC_DEBUG("%s read %d\n", __FUNCTION__, len);
	gettimeofday(&current, NULL);

	if (m_pktMJPEG.bufLen < (sizeof(InfoFrameI) + len))
	{
		free(m_pktMJPEG.dataBuf);
		m_pktMJPEG.bufLen = (sizeof(InfoFrameI) + len);
		m_pktMJPEG.dataBuf = (u8 *)malloc(m_pktMJPEG.bufLen);
	}
#if 0
	static int iName = 0;
	char strName[64];
	sprintf(strName, "%d.jpg", iName ++);
	
	image_write_raw((unsigned char *)ptr, len, strName);
	DumpHexData((unsigned char *)ptr, 20);
#endif
	
	m_pktMJPEG.streamType = VIDEO_STREAM_VIDEO;
	m_pktMJPEG.frameType = VIDEO_FRM_I;
	u8 * pHeader = m_pktMJPEG.dataBuf + sizeof(InfoFrameI);
	memcpy(pHeader, ptr, len);
	InfoFrameI *pI = (InfoFrameI *)m_pktMJPEG.dataBuf;
	m_pktMJPEG.dataLen = len + sizeof(InfoFrameI);

	pI->video = CODEC_MJPEG;
	pI->audio = CODEC_NONE;

	pI->vWidth = 640;
	pI->vHeight = 480;
	m_pktMJPEG.secs = current.tv_sec;
	m_pktMJPEG.msecs = current.tv_usec/1000;
	
	m_vPlay.PutRawData(m_pktMJPEG);
	
	if (dataHandler)
	{
		dataHandler(pData, m_pktMJPEG);
	}
}

size_t VPlayData::MJPEGWriteData(void *ptr, size_t size, size_t nmemb, void *userdata) 
{
	int dummy = errno;
	VPlayData * pThread = (VPlayData *)userdata;

	if (pThread)
	{
	    pThread->MJPEGWriteData1(ptr, size, nmemb);
	}

	return nmemb;
}


size_t VPlayData::MJPEGWriteData1(void *ptr, size_t size, size_t nmemb) 
{
	size_t written;

	unsigned char *bptr = (unsigned char *)ptr;
	long nbytes = size * nmemb;
	
	if (nbytes < 2)
	{
		return nbytes;
	}

	VDC_DEBUG("got %d bytes (%d, %d)...\n", nbytes, size, nmemb);
	//DumpTxtData((unsigned char *)ptr, 200);

	if (m_jpeg_first_frame == true)
	{
		VDC_DEBUG("got %d bytes (%d, %d) %c %c...\n", nbytes, size, nmemb, bptr[0], bptr[1]);
		if (strncmp((char *)bptr, "--", 2) != 0)
		{
			m_bSingleJpeg = true;
		}
		m_jpeg_first_frame = false;
	}

	if (m_bSingleJpeg == true)
	{
		if (m_jpeg_frame_position + nbytes > m_jpeg_buffer_len)
		{
			u8 * tempbuf = (u8*)malloc((m_jpeg_frame_position + nbytes) * 2);
			memcpy(tempbuf, m_jpeg_buffer, m_jpeg_buffer_len);
			m_jpeg_buffer_len = (m_jpeg_frame_position + nbytes) * 2;
			free(m_jpeg_buffer);
			m_jpeg_buffer = tempbuf;
		}

		memcpy(m_jpeg_buffer + m_jpeg_frame_position, bptr, nbytes);
		m_jpeg_frame_position = m_jpeg_frame_position + nbytes;
		m_jpeg_frame_size = m_jpeg_frame_size + nbytes;
		return nbytes;
	}


	for(int i=0; i<nbytes; i++) {
	    unsigned char b = bptr[i];
	    // printf("byte #%d.%d %02x (%c)\n", block_counter, i, b, (b > 32 && b < 128) ? b : '?');
	    // printf("%c", (b > 32 && b < 128) ? b : '?');
	    if (m_g_state == STATE_MAIN_HEADER || m_g_state == STATE_FRAME_HEADER) {
	        int p = strlen(m_headerline);
	        if (p < 1000)
	            m_headerline[p] = b;
	        if (b == '\n') {
	            if (m_headerline[p-1] == '\r') {
	                m_headerline[p] = 0;
	                m_headerline[p-1] = 0;
	                // printf("got header newline after \"%s\".\n", headerline);
	                on_headerline(m_headerline);
	                if (strncmp(m_headerline, "--", 2) == 0) {
	                    // printf("got boundary.\n");
	                    m_g_state = STATE_FRAME_HEADER;
	                }
	                if (strlen(m_headerline) == 0 && m_jpeg_num_headers > 0) {
	                    // did we get an empty line, switch state.
	                    // printf("empty header, switch state.\n");
	                    if (m_g_state == STATE_FRAME_HEADER) {
	                        // printf("starting new jpeg frame.\n");
	                        m_g_state = STATE_FRAME;
	                        m_jpeg_frame_position = 0;
	                    }
	                }
	                memset(m_headerline, 0, 1000);
	                m_jpeg_num_headers ++;
	            }
	        }
	    }
	    else if (m_g_state == STATE_FRAME) {
	        if (m_jpeg_frame_position < m_jpeg_buffer_len) {
	            m_jpeg_buffer[m_jpeg_frame_position] = b;
	        }else
	        {
	        	u8 * tempbuf = (u8*)malloc(m_jpeg_buffer_len * 2);
			memcpy(tempbuf, m_jpeg_buffer, m_jpeg_buffer_len);
			m_jpeg_buffer_len = m_jpeg_buffer_len * 2;
			free(m_jpeg_buffer);
			m_jpeg_buffer = tempbuf;
			m_jpeg_buffer[m_jpeg_frame_position] = b;
	        }
	        m_jpeg_frame_position ++;
	        if (m_jpeg_frame_position >= m_jpeg_frame_size) {
	            //printf("position %d / %d\n", m_jpeg_frame_position, m_jpeg_frame_size);
	            //printf("end of frame.\n");
	            on_frame(m_jpeg_buffer, m_jpeg_frame_size);
	            m_g_state = STATE_FRAME_HEADER;
	            memset(m_headerline, 0, 1000);
	            m_jpeg_frame_position = 0;
	            m_jpeg_num_headers = 0;
	        }
	    }
	}

	return nbytes;
}

int VPlayData::CurlXferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow,
                curl_off_t ultotal, curl_off_t ulnow)
{
	int dummy = errno;
	VPlayData * pThread = (VPlayData *)p;

	if (pThread)
	{
	    return pThread->CurlXferinfo1(dltotal, dlnow, ultotal, ulnow);
	}

	return 1;
}
int VPlayData::CurlXferinfo1(curl_off_t dltotal, curl_off_t dlnow,
                curl_off_t ultotal, curl_off_t ulnow)
{
	if (m_bExit == true)
	{
		VDC_DEBUG( "%s  CurlXferinfo1 exit\n",__FUNCTION__);
		return 1;
	}else
	{
		return 0;
	}
}

void VPlayData::RunMJPEG(void * pParam)
{
	int dummy = errno;
	VPlayData * pThread = (VPlayData *)pParam;

	if (pThread)
	{
	    pThread->RunMJPEG1();
	}
}
//#define VPLAY_DUMP_RAW
 
void VPlayData::RunMJPEG1()
{
	m_pktMJPEG.bufLen = 1024 * 16;
	m_pktMJPEG.dataBuf = (u8 *)malloc(m_pktMJPEG.bufLen);
	m_jpeg_buffer_len = 2 * 1024 * 1024;
	m_jpeg_buffer = (unsigned char *)malloc(m_jpeg_buffer_len);
#ifdef VPLAY_DUMP_RAW
       FILE* fSink;    
       fopen_s(&fSink, "raw.264", "wb");
	BOOL bGotIFrame = FALSE;
#endif
	while (m_bExit != true)
	{
		try {
			VDC_DEBUG( "%s  Read MJPEG Source %s\n",__FUNCTION__, m_strFile.c_str());
			
			/* HTTP JPEG */
			memset(m_headerline, 0, HTTP_JPEG_HEADER_SIZE);
			m_g_state = STATE_MAIN_HEADER;
			m_jpeg_frame_position = 0;
			m_jpeg_num_headers = 0;
			m_jpeg_frame_size = 0;
			m_jpeg_first_frame = true;
			m_bSingleJpeg = false;
			

			CURL *curl;
			CURLcode res;
			curl = curl_easy_init();
			if (curl)
			{
				curl_easy_setopt(curl, CURLOPT_USERNAME, m_strUser.c_str());
				curl_easy_setopt(curl, CURLOPT_PASSWORD, m_strPass.c_str());
				curl_easy_setopt(curl, CURLOPT_URL, m_strFile.c_str());
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, VPlayData::MJPEGWriteData);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)this);

				/*  https://curl.haxx.se/libcurl/c/progressfunc.html */
				curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, VPlayData::CurlXferinfo);
				curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
				res = curl_easy_perform(curl);
				ve_sleep(1000);
				if (m_bSingleJpeg == true)
				{
					on_frame(m_jpeg_buffer, m_jpeg_frame_size);
				}
				/* always cleanup */
				curl_easy_cleanup(curl);				
			}
		}catch (...)
		{
		}
	}

	return;
	
}


static BOOL VplayRMDataHandler(void* pContext, VideoFrame& packet)
{
	int dummy = errno;
	VPlay * pVPlay = (VPlay *)pContext;
	if (pVPlay)
	{
		if (pVPlay->m_data->dataHandler)
		{
			pVPlay->m_data->dataHandler(pVPlay->m_data->pData, packet);
		}
	}
	
	return TRUE;
}

VPlay::VPlay()
{
	m_data = new VPlayData(*this, false, false, "fake", "fake", "fake",FALSE);
}
VPlay::~VPlay()
{
	if (m_data)
	{
		delete m_data;
	}
}

/* File video source */
BOOL VPlay::Init(string strFile, bool bMJPEG, string strUser, 
		string strPass, BOOL bHWAccel)
{
	if (m_data)
	{
		delete m_data;
		m_data = NULL;
	}

	//TOD check if the license support HW decode
	
	m_data = new VPlayData(*this, true, bMJPEG, strFile, strUser, strPass, bHWAccel);
	m_data->m_avWrapper.Init(FALSE," "," "," ", bHWAccel);
	m_data->m_avWrapper.SetRawDataHandler((RMRawVideoHandler)(RenderWrapper::RenderRMRawVideoHandler),
						m_data->m_render);
	
	return TRUE;
}
BOOL VPlay::Init(BOOL bRealStream, string strUrl, string strUser, 
		string strPass, BOOL bHWAccel, VSCConnectType connectType)
{
	if (m_data)
	{
		delete m_data;
		m_data = NULL;
	}

	//TOD check if the license support HW decode
	
	m_data = new VPlayData(*this, false, false, "false", strUser, strPass, bHWAccel);

	m_data->m_avWrapper.Init(bRealStream, strUrl,  
				strUser,strPass, bHWAccel, connectType);
	m_data->m_avWrapper.SetRawDataHandler(RMRawVideoHandler(RenderWrapper::RenderRMRawVideoHandler),
						m_data->m_render);
	return TRUE;
}

BOOL VPlay::GetStreamInfo(VideoStreamInfo &pInfo)
{
	m_data->m_render->GetStreamInfo(pInfo);

	return TRUE;
}

BOOL VPlay::AttachWidget(HWND hWnd, int w, int h, RenderType render)
{
	if (LicPlayRef() == false)
	{
		VDC_DEBUG("Need License !\n");
		m_data->m_nAttachedPlayer ++;
		return FALSE;
	}
	
	m_data->m_render->AttachPlayer(hWnd, w, h, UpdateRenderType(m_data->m_bHWAccel, render));
	if (m_data->m_nAttachedPlayer == 0)
	{
		m_data->m_avWrapper.StartRaw();
	}
	m_data->m_nAttachedPlayer ++;
	return TRUE;
}

BOOL VPlay::UpdateWidget(HWND hWnd, int w, int h)
{
#if ENABLE_SDL_RENDER
	m_data->m_render->ResizePlayer(hWnd, w, h);
#endif
	return TRUE;
}

BOOL VPlay::EnableMot(HWND hWnd, bool enable, astring strConf)
{
#if ENABLE_SDL_RENDER
	m_data->m_render->EnableMot(hWnd, enable, strConf);
#endif
	return TRUE;
}

BOOL VPlay::ShowAlarm(HWND hWnd)
{
#if ENABLE_SDL_RENDER
	m_data->m_render->ShowAlarm(hWnd);
#endif
	return TRUE;
}


BOOL VPlay::SetPbTimeCallback(HWND hWnd, void * pData, VPlayPBTimeCallback callback)
{
	return TRUE;
}

BOOL VPlay::Control(VPlayCmd cmd, VPlayCmdParam param)
{
	return TRUE;
}

BOOL VPlay::DetachWidget(HWND hWnd)
{
	LicPlayUnRef();

	m_data->m_render->DetachPlayer(hWnd);
	m_data->m_nAttachedPlayer --;
	if (m_data->m_nAttachedPlayer < 0)
	{
		m_data->m_nAttachedPlayer = 0;
	}
	if (m_data->m_nAttachedPlayer == 0)
	{
	    m_data->m_avWrapper.StopRaw();
	}
    
    return TRUE;
}

BOOL VPlay::StartGetData(void * pData, VPlayDataHandler callback)
{
	m_data->pData = pData;
	m_data->dataHandler = callback;
	if (m_data->m_bFile != true)
	{
		m_data->m_avWrapper.SetDataHandler((RMDataHandler)VplayRMDataHandler, this);
		m_data->m_avWrapper.Start();
	}
	return TRUE;
}
BOOL VPlay::StopGetData()
{
	m_data->pData = NULL;
	m_data->dataHandler = NULL;
	return TRUE;
}

BOOL VPlay::StartGetRawFrame(void * pData, VPlayRawFrameHandler callback)
{
	BOOL ret = FALSE;
#if ENABLE_SDL_RENDER
	ret = m_data->m_render->StartGetRawFrame(pData, callback);
#endif
	if (m_data->m_nAttachedPlayer == 0)
	{
	    m_data->m_avWrapper.StartRaw();
	}
    	m_data->m_nAttachedPlayer ++;
	
	return ret;
}
BOOL VPlay::StopGetRawFrame()
{
	BOOL ret = FALSE;
	
#if ENABLE_SDL_RENDER
	ret = m_data->m_render->StopGetRawFrame();
#endif
	m_data->m_nAttachedPlayer --;
	if (m_data->m_nAttachedPlayer < 0)
	{
		m_data->m_nAttachedPlayer = 0;
	}
	if (m_data->m_nAttachedPlayer == 0)
	{
	    	m_data->m_avWrapper.StopRaw();
	}
	return ret;
}

BOOL VPlay::PutRawData(VideoFrame& packet)
{
	m_data->m_avWrapper.PutData(packet);

	return TRUE;
}


BOOL VPlay::SetLicense(astring &strLicense)
{
	return LicSetLicense(strLicense);
}
BOOL VPlay::GetLicenseInfo(astring &strHostId, int &ch, astring &type, 
	astring &startTime, astring &expireTime)
{
	return LicGetLicenseInfo(strHostId, ch, type, startTime, expireTime);
}


size_t VPlay::EncodeJPEG(RawFrame& pRaw, int dst_w, int dst_h, u8* output, size_t outputSize)
{
	if (dst_w == 0 || dst_h == 0)
	{
		dst_w = pRaw.width;
		dst_h = pRaw.height;
	}
	AVKit::JPEGEncoder *pEnc = new AVKit::JPEGEncoder( AVKit::GetJPEGOptions( dst_w, 
					dst_h));

	if (pEnc == NULL)
	{
		return -1;
	}

	FFmpegScale m_Scale;
	m_Scale.SetAttribute(SWS_PF_YUV420P, SWS_PF_YUV420P, SWS_SA_FAST_BILINEAR);
	char * m_buf = (char *)av_malloc(dst_w * dst_h * 3);

	m_Scale.ScaleYUV420((unsigned char *)pRaw.data[0], pRaw.linesize[0], 
				(unsigned char *)pRaw.data[1], pRaw.linesize[1], 
				(unsigned char *)pRaw.data[2], pRaw.linesize[2], 
				pRaw.width, pRaw.height, pRaw.width * 3, (unsigned char *)m_buf, 
				dst_w, dst_h, dst_w * 3);
	

	size_t nRet = pEnc->EncodeYUV420P((uint8_t*)m_buf, 
			(uint8_t*)(m_buf + dst_w* dst_h), (uint8_t*) (m_buf + ((dst_w * dst_h)* 5)/4), output, outputSize);

	delete pEnc;

	av_free(m_buf);
	
	return nRet;

}

