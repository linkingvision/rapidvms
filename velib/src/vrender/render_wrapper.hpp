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
#ifndef __RENDER_WRAPPER_H_
#define __RENDER_WRAPPER_H_

#include "utility.hpp"
#include "debug.hpp"
#include "vplay.hpp"

#ifndef ARM_LINUX
#include "SDL.h"
#include "sdl_render.h"
#endif

#include "video_render.h"
#ifdef WIN32
#include "d3d_render.h"
//#include <comdef.h>
#endif

#undef main

#include <map>

#include "ffkit/h264_decoder.h"
#include "ffkit/jpeg_decoder.h"
#include "ffkit/ffoptions.h"
#include "ffkit/fflocky.h"
#include "cppkit/ck_memory.h"
#include "libyuv.h"

extern "C"
{
#include "libavformat/avformat.h"
};

#include "vehwcmd.hpp"

using namespace UtilityLib;
using namespace ffkit;
using namespace cppkit;
using namespace libyuv;

typedef std::map<HWND, video_render*> RenderMap;
extern unsigned char novideo_jpg[];
extern unsigned int novideo_jpg_len;
extern fast_mutex gRenderNoVideoLock;
extern av_packet *gRenderNoVideoPic;

class RenderWrapper
{
public:
    RenderWrapper(BOOL bHWAccel)
	: m_bInited(FALSE), m_pData(NULL), 
	m_rawFrameHandler(NULL), m_bHWAccel(bHWAccel)
    {
    	gRenderNoVideoLock.lock();
	if (gRenderNoVideoPic == NULL)
	{
		shared_ptr<jpeg_decoder> decoder = make_shared<jpeg_decoder>( 
							get_jpeg_options(640, 480, 1, 1, 255) );
		shared_ptr<av_packet> pkt = make_shared<av_packet>(novideo_jpg, novideo_jpg_len, false );
		decoder->decode(pkt);

		decoder->set_output_width(640 );
		decoder->set_output_height(480 );

		shared_ptr<av_packet> pic;
		pic = decoder->get();
		gRenderNoVideoPic = new av_packet(*pic);
	}
	gRenderNoVideoLock.unlock();

	memset(&m_frameCache, 0, sizeof(m_frameCache));
	memset(&m_frameHWAccel, 0, sizeof(m_frameHWAccel));
	m_HWAccelSize = 0;

	
	m_frameCache.width = 640;
	m_frameCache.height = 480;
	m_frameCache.data[0] = (char *)malloc (m_frameCache.width * m_frameCache.height);
	memcpy(m_frameCache.data[0] , gRenderNoVideoPic->map(), m_frameCache.width * m_frameCache.height);
	m_frameCache.linesize[0] = m_frameCache.width;

	m_frameCache.data[1] = (char *)malloc ((m_frameCache.width * m_frameCache.height)/4);
	m_frameCache.linesize[1] = m_frameCache.width /2;
	memcpy(m_frameCache.data[1] , gRenderNoVideoPic->map() + m_frameCache.width * m_frameCache.height, 
		(m_frameCache.width * m_frameCache.height)/4);

	m_frameCache.data[2] = (char *)malloc ((m_frameCache.width * m_frameCache.height)/4);
	m_frameCache.linesize[2] = m_frameCache.width /2;
	memcpy(m_frameCache.data[2] , gRenderNoVideoPic->map() + ((m_frameCache.width * m_frameCache.height)* 5/4), 
		(m_frameCache.width * m_frameCache.height)/4);
	m_bExit = false;

	memset(&m_streamInfo, 0, sizeof(m_streamInfo));
	m_streamInfo.codec = CODEC_NONE;
#ifndef DISABLE_COVER
	CopyInitCache(&m_pCache, 8192);
#endif
	m_pThread = new tthread::thread(RenderWrapper::Run, (void *)this);
	
    }
    ~RenderWrapper()
    {
	m_bExit = true;
	m_pThread->join();
	delete m_pThread;
	m_pThread = NULL;
	RenderMap::iterator it = m_RenderMap.begin(); 
	for(; it!=m_RenderMap.end(); ++it)
	{
		video_render *pRender = (*it).second;
		if (pRender)
		{
			pRender->destory_render();
			delete pRender;
			pRender = NULL;
		}
	}
	for (int i = 0; i < 3; i ++)
	{
		if (m_frameHWAccel.data[i])
		{
			free(m_frameHWAccel.data[i]);
			m_frameHWAccel.data[i] = NULL;
		}
		if (m_frameCache.data[i])
		{
			free(m_frameCache.data[i]);
			m_frameCache.data[i] = NULL;
		}
	}
    }

public:

	BOOL GetStreamInfo(VideoStreamInfo &pInfo)
	{
		Lock();

		memcpy(&pInfo, &m_streamInfo, sizeof(VideoStreamInfo));
		UnLock();
		return TRUE;	
	}
	BOOL AttachPlayer(HWND hWnd, int w, int h, RenderType render)
	{
		video_render *pRender = m_RenderMap[hWnd];
		Lock();
		if (pRender)
		{
			pRender->init_render((void *)hWnd, w, h, 0);
		}else
		{
			pRender = CreateRender(render);
			pRender->init_render((void *)hWnd, w, h, 0);
			m_RenderMap[hWnd] = pRender;
		}
		/* Render the cache frame */
		if (m_frameCache.width > 0)
		{
			for (int i = 0; i < 3; i ++)
			{
				pRender->render_one_frame(&m_frameCache, 0);
			}
		}
		
		UnLock();
		return TRUE;
	}
	BOOL ResizePlayer(HWND hWnd, int w, int h)
	{
		Lock();
		video_render *pRender = m_RenderMap[hWnd];
		if (pRender)
		{
			pRender->re_size(w, h);
			if (m_frameCache.width > 0)
			{
				pRender->render_one_frame(&m_frameCache, 0);
			}
		}
		UnLock();
		return TRUE;
	}
	BOOL DetachPlayer(HWND hWnd)
	{
		Lock();
		video_render *pRender = m_RenderMap[hWnd];
		if (pRender)
		{
			pRender->destory_render();
			m_RenderMap.erase(hWnd);
			delete pRender;
			pRender = NULL;
		}
		UnLock();
		return TRUE;
	}

	BOOL EnableMot(HWND hWnd, bool enable, astring strConf)
	{
		Lock();
		video_render *pRender = m_RenderMap[hWnd];
		if (pRender)
		{
			pRender->enable_mot(enable, strConf);
		}
		UnLock();
		return TRUE;
	}
	BOOL ShowAlarm(HWND hWnd)
	{
		Lock();
		video_render *pRender = m_RenderMap[hWnd];
		if (pRender)
		{
			pRender->show_alarm();
		}
		UnLock();
		return TRUE;
	}
public:
	 static void Run(void * pParam)
	{
		int dummy = errno;
		RenderWrapper * pThread = (RenderWrapper *)pParam;

		if (pThread)
		{
		    pThread->Run1();
		}
	}
	void Run1()
	{
		struct timeval current;
		while (m_bExit != true)
		{
			ve_sleep(500);
			gettimeofday(&current, NULL);
			/* Check last render time and then render the cache */
			Lock();
			if (current.tv_sec == m_lastRenderTime.tv_sec)
			{
				if (current.tv_usec - m_lastRenderTime.tv_usec < 100000)
				{
					UnLock();
					continue;
				}
			}
			if (current.tv_sec -  m_lastRenderTime.tv_sec == 1)
			{
				if (current.tv_usec + 1000000 - m_lastRenderTime.tv_usec < 100000)
				{
					UnLock();
					continue;
				}		
			}
			RenderMap::iterator it = m_RenderMap.begin(); 
			for(; it!=m_RenderMap.end(); ++it)
			{
				video_render *pRender = (*it).second;
				if (pRender)
				{
					//pRender->render_one_frame(&m_frameCache, 0);
				}
			}   	

			UnLock();
			
		}
	}
	
public:
	inline BOOL StartGetRawFrame(void * pData, VPlayRawFrameHandler callback);
	inline BOOL StopGetRawFrame();
public:
	static BOOL RenderRMRawVideoHandler(void* pContext, RawFrame& packet)
	{
		if (pContext)
		{
			RenderWrapper *pData = (RenderWrapper *)pContext;
			return pData->RenderRMRawVideoHandler1(packet);
		}
	}

	inline BOOL RenderRMRawVideoHandler1(RawFrame& packet);

public:
	video_render * CreateRender(RenderType render)
	{
		video_render *pRender = NULL;
		
		switch (render)
		{

		case RENDER_TYPE_SDL: 
#ifndef ARM_LINUX
			pRender = new sdl_render();
#endif
			//pRender = new d3d_render();
			break;
#ifdef WIN32
		case RENDER_TYPE_D3D:   
			pRender = new d3d_render(m_bHWAccel);
			break; 
#endif
		default:
			return NULL;
			break;
		}
		
		return pRender;
	}
public:
	inline bool CacheFrame(RenderFrame * data);

public:
    void Lock()
    {
        m_Lock.lock();
    }
    void UnLock()
    {
        m_Lock.unlock();
    }


private:
	fast_mutex m_Lock;
	HWND m_hWnd;
	BOOL m_bInited; 
	RenderMap m_RenderMap;
	RenderFrame m_frameCache;
	RenderFrame m_frameHWAccel;
	int m_HWAccelSize;
	tthread::thread *m_pThread;
	struct timeval m_lastRenderTime;
	bool m_bExit;

	void* m_pData;
	VPlayRawFrameHandler m_rawFrameHandler;
	VideoStreamInfo m_streamInfo;
#ifndef DISABLE_COVER
	copy_cache_t m_pCache;
#endif
	bool m_bHWAccel;
};

typedef RenderWrapper* LPRenderWrapper;


inline BOOL RenderWrapper::RenderRMRawVideoHandler1(RawFrame& packet)
{
	if (packet.type == VIDEO_RAW_AUDIO)
	{
		return TRUE;//TODO skip the audio
	}
	
	m_streamInfo.codec = packet.codec;
	m_streamInfo.width = packet.width;
	m_streamInfo.height = packet.height;
	
	if (packet.hwCmd != VIDEO_HW_NONE)
	{

		Lock();
		gettimeofday(&m_lastRenderTime, NULL);
		RenderMap::iterator it = m_RenderMap.begin();
		for (; it != m_RenderMap.end(); ++it)
		{
			video_render *pRender = (*it).second;
			if (pRender)
			{
				pRender->render_one_frame(&packet, 0);
			}
		}
		UnLock();
		return TRUE;
	}

	video_render *pRender = NULL;
	RenderFrame data;
	data.data[0] = packet.data[0];
	data.linesize[0] = packet.linesize[0];
	
	data.data[1] = packet.data[1];
	data.linesize[1] = packet.linesize[1];

	data.data[2] = packet.data[2] ;
	data.linesize[2] = packet.linesize[2];

	data.data[3] = packet.data[3] ;
	data.linesize[3] = packet.linesize[3];
	
	data.width = packet.width;
	data.height = packet.height;
	data.format = packet.format;
	
	if (packet.format == AV_PIX_FMT_NV12)
	{
		if (m_HWAccelSize <  packet.width * packet.height)
		{
			for (int i = 0; i < 3; i ++)
			{
				if (m_frameHWAccel.data[i])
				{
					free(m_frameHWAccel.data[i]);
				}
				m_frameHWAccel.data[i] = (char *)malloc( packet.width * packet.height);

			}
			m_HWAccelSize =packet.width * packet.height;
		}
#ifdef DISABLE_COVER
		NV12ToI420((unsigned char *)data.data[0], data.linesize[0], 
					(unsigned char *)data.data[1], data.linesize[1], 
					(unsigned char *)m_frameHWAccel.data[0], data.width, 
					(unsigned char *)m_frameHWAccel.data[1], data.width /2,
					(unsigned char *)m_frameHWAccel.data[2] , data.width /2, 
					data.width, data.height);
#else
			
		cover_SSE_CopyPlane((unsigned char *)m_frameHWAccel.data[0], data.width,
			(unsigned char *)data.data[0], data.linesize[0],
	                  m_pCache.buffer, m_pCache.size,
	                  packet.width, packet.height, VLC_CPU_SSE_ALL);
		cover_SSE_SplitPlanes((unsigned char *)m_frameHWAccel.data[1], data.width / 2,
			(unsigned char *)m_frameHWAccel.data[2], data.width / 2,
			(unsigned char *)data.data[1], data.linesize[1],
	                    m_pCache.buffer, m_pCache.size,
	                    (packet.width+1)/2, (packet.height+1)/2, VLC_CPU_SSE_ALL);
#endif
		m_frameHWAccel.linesize[0] = data.width;
		m_frameHWAccel.linesize[1] = data.width/2;
		m_frameHWAccel.linesize[2] = data.width/2;
		m_frameHWAccel.width = packet.width;
		m_frameHWAccel.height = packet.height;
		m_frameHWAccel.format = AV_PIX_FMT_YUV420P;
		memcpy(&data, &m_frameHWAccel, sizeof(m_frameHWAccel));
		
	}

	Lock();
	gettimeofday(&m_lastRenderTime, NULL);
	RenderMap::iterator it = m_RenderMap.begin(); 
	for(; it!=m_RenderMap.end(); ++it)
	{
		video_render *pRender = (*it).second;
		if (pRender)
		{
			pRender->render_one_frame(&data, 0);
		}
	}  
	if (m_rawFrameHandler)
	{
		m_rawFrameHandler(m_pData, packet);
	}
	/* Cache the frame */
	CacheFrame(&data);

	UnLock();

	
	return TRUE;
}

bool RenderWrapper::CacheFrame(RenderFrame * data)
{
	if (data->linesize[0] != m_frameCache.linesize[0] 
		|| data->height != m_frameCache.height)
	{
		if (m_frameCache.data[0] != NULL)
		{
			free(m_frameCache.data[0]);
			m_frameCache.data[0] = NULL;
		}
		m_frameCache.linesize[0] = data->linesize[0];
		m_frameCache.data[0] = (char *)malloc(data->linesize[0] * data->height);
	}
	memcpy(m_frameCache.data[0], data->data[0], data->linesize[0] * data->height);
	
	if (data->linesize[1] != m_frameCache.linesize[1] 
		|| data->height != m_frameCache.height)
	{
		if (m_frameCache.data[1] != NULL)
		{
			free(m_frameCache.data[1]);
			m_frameCache.data[1] = NULL;
		}
		m_frameCache.linesize[1] = data->linesize[1];
		m_frameCache.data[1] = (char *)malloc((data->linesize[1] * data->height) /2);
	}
	memcpy(m_frameCache.data[1], data->data[1], (data->linesize[1] * data->height )/ 2);
	
	if (data->linesize[2] != m_frameCache.linesize[2] 
		|| data->height != m_frameCache.height)
	{
		if (m_frameCache.data[2] != NULL)
		{
			free(m_frameCache.data[2]);
			m_frameCache.data[2] = NULL;
		}
		m_frameCache.linesize[2] = data->linesize[2];
		m_frameCache.data[2] = (char *)malloc((data->linesize[2] * data->height) / 2);
	}
	memcpy(m_frameCache.data[2], data->data[2], (data->linesize[2] * data->height) / 2);
	
	m_frameCache.width = data->width;
	m_frameCache.height = data->height;
	
	return true;
}


BOOL RenderWrapper::StartGetRawFrame(void * pData, VPlayRawFrameHandler callback)
{
	Lock();
	m_pData = pData;
	m_rawFrameHandler = callback;
	UnLock();

	return TRUE;
}
BOOL RenderWrapper::StopGetRawFrame()
{
	Lock();
	m_pData = NULL;
	m_rawFrameHandler = NULL;
	UnLock();

	return TRUE;
}


#endif /* _RENDER_WRAPPER_H_ */
