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
#include "sdl_render.h"
#include "ffkit/h264_decoder.h"
#include "ffkit/jpeg_encoder.h"
#include "ffkit/ffoptions.h"
#include "ffkit/fflocky.h"
#include "cppkit/ck_memory.h"
#include "libyuv.h"

using namespace cppkit;
using namespace ffkit;
using namespace libyuv;

class SDLInitGlobal
{
public:
	SDLInitGlobal()
	{
		locky::register_ffmpeg();
		if (SDL_Init(SDL_INIT_VIDEO) < 0) 
		{
			puts("# error initializing SDL");
			puts(SDL_GetError());
			return ;
		}		
		return ;
	}
};

SDLInitGlobal gSDLInitGlobal;
fast_mutex gRenderLock;

sdl_render::sdl_render()
{
	m_hWnd = NULL; 
	m_SdlWin = NULL;
	m_SdlRender = NULL;
	m_pTex = NULL;
	m_bInited = false;
	m_bufSize = 1080 * 720 * 3;
	m_buf = (char *)malloc(m_bufSize);

	/* Buffer size 2 is for NV12 of intel media sdk */
	m_bufSize2 = 1080 * 720 * 3;
	m_buf2 = (char *)malloc(m_bufSize2);
	
	m_alarmTime = 0;
	m_alarmEnable = false;
#if 1
	m_motEnable = false;
	m_MotAlgo = NULL;
#else
	m_MotAlgo = new VSMotAlgo(false);
	m_motEnable = true;
#endif

}
sdl_render::~sdl_render()
{
	if (m_buf != NULL)
	{
		free(m_buf);
		m_buf = NULL;
	}
	if (m_MotAlgo)
	{
		delete m_MotAlgo;
		m_MotAlgo = NULL;
	}
}

void sdl_render::enable_mot(bool enable, astring strConf)
{
	m_Lock.lock();
	if (enable == true && m_motEnable == false)
	{
		m_MotAlgo = new VSMotAlgo(true);
		/* Setup mot region */
		if (VVidMotReg::CheckStringForReg(strConf) == true)
		{
			VVidMotReg reg(strConf);
			m_MotAlgo->SetMotReg(reg);
		}
		m_motEnable = true;
	}else if (enable == false && m_motEnable == true)
	{
		delete m_MotAlgo;
		m_MotAlgo = NULL;
		m_motEnable = false;
	}

	m_Lock.unlock();
}

void sdl_render::show_alarm()
{
	m_Lock.lock();
	m_alarmTime = time(NULL) + 5;
	m_alarmEnable = true;
	m_Lock.unlock();
}

bool sdl_render::render_one_frame(RenderFrame * data, int pix_fmt)
{
	//RenderFrame dataOut;
	
	//m_videoDraw.ProcessFrame(*data, dataOut);
	//data = &dataOut;
	m_Lock.lock();
	void * pixels = NULL;
	VBlobVec blobOutput;
	s32 blobW = 0;
	s32 blobH = 0;;
#if 0
    if (data->width != m_image_width || data->height != m_image_height)
    {
    	m_image_width = data->width;
	m_image_height = data->height;
    	Init(data->width, data->height);
    }
#endif
	
	//VDC_DEBUG( "%s Frame (%d,%d) (0x%p,%d) (0x%p,%d) (0x%p,%d)\n",__FUNCTION__, 
	//		data->width, data->height, data->data[0], data->linesize[0], 
	//		data->data[1], data->linesize[1],
	//		data->data[2], data->linesize[2]);
	if (m_bufSize < m_image_width * m_image_height *3)
	{
		free(m_buf );
		m_bufSize = m_image_width * m_image_height *3;
		m_buf = (char *)malloc(m_bufSize);
	}

	if (m_MotAlgo)
	{	
		RawFrame frame;
		frame.data[0] = data->data[0];
		frame.data[1] = data->data[1];
		frame.data[2] = data->data[2];

		frame.linesize[0] = data->linesize[0];
		frame.linesize[1] = data->linesize[1];
		frame.linesize[2] = data->linesize[2];
		frame.format = data->format;
		frame.width = data->width;
		frame.height = data->height;
		
		m_MotAlgo->ProcessFrame(frame);
		m_MotAlgo->GetOutput(blobOutput, blobW, blobH);
	}
	{
#ifndef USING_LIBYUV
		m_Scale.ScaleYUV420((unsigned char *)data->data[0], data->linesize[0], 
				(unsigned char *)data->data[2], data->linesize[2], 
				(unsigned char *)data->data[1], data->linesize[1], 
				data->width, data->height, data->width * 3, (unsigned char *)m_buf, 
				m_image_width, m_image_height, m_image_width * 3);
#else
		I420Scale((unsigned char *)data->data[0], data->linesize[0], 
				(unsigned char *)data->data[2], data->linesize[2], 
				(unsigned char *)data->data[1], data->linesize[1],
					data->width, data->height, 
					(unsigned char *)m_buf, m_image_width, 
					(unsigned char *)m_buf + m_image_width * m_image_height, 
					m_image_width /2, 
					(unsigned char *)m_buf + (m_image_width * m_image_height * 5) /4 , 
					m_image_width /2,
					m_image_width, m_image_height, kFilterNone);
#endif

	}

	SDL_Rect rect;
	char * ptr = m_buf;
	int i = 0;
	rect.x = 0;
	rect.y = 0;
	rect.w = m_image_width;
	rect.h = m_image_height;
	int pitch;

	pitch = m_image_width * SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_YV12);


	SDL_Rect rectDraw;
	rectDraw.x = m_image_width/2;
	rectDraw.y = m_image_height/2;
	rectDraw.w = 20;
	rectDraw.h = 20;
#ifndef WIN32 //Linux need big lock
	gRenderLock.lock();
#endif
	SDL_UpdateTexture(m_pTex,NULL,m_buf,pitch);
	SDL_SetRenderDrawColor(m_SdlRender,0,0,0,0);
	SDL_RenderClear(m_SdlRender);
	SDL_SetRenderDrawColor(m_SdlRender,255,0,0,0);
	SDL_RenderCopy(m_SdlRender, m_pTex,NULL,&rect);

	SDL_Rect rectMotDraw;
	if (m_MotAlgo)
	{
		VBlobVec::iterator iter;
		double radioW = (double)m_image_width/(double)blobW;
		double radioH = (double)m_image_height/(double)blobH;
	
		for (iter=blobOutput.begin();iter!=blobOutput.end();iter++)  
		{  

			//printf("Motion %d %d\n", (*iter).x1 - (*iter).x0, (*iter).y1 - (*iter).y0);
			rectMotDraw.x = (double)((*iter).x0) * radioW;
			rectMotDraw.y = (double)((*iter).y0) * radioH;
			rectMotDraw.w =(double)( (*iter).x1 - (*iter).x0)* radioW;
			rectMotDraw.h =(double) ( (*iter).y1 - (*iter).y0) * radioH;
			SDL_RenderDrawRect(m_SdlRender, &rectMotDraw);
		} 
	}

	if (m_alarmEnable == true)
	{
		int currentTime = time(NULL);
		if (currentTime > m_alarmTime)
		{
			m_alarmEnable = false;
			m_alarmTime = 0;
		}else
		{
			if (currentTime % 2 == 0)
			{
				SDL_Rect rectDraw;
				rectDraw.x = 0;
				rectDraw.y = 0;
				rectDraw.w = m_image_width;
				rectDraw.h = m_image_height;
				SDL_SetRenderDrawColor(m_SdlRender,255,255,0,0);
				SDL_RenderDrawRect(m_SdlRender, &rectDraw);
			}
		}
	}
	//SDL_RenderDrawRect(m_SdlRender, &rectDraw);

	SDL_RenderPresent(m_SdlRender);
#ifndef WIN32
	gRenderLock.unlock();
#endif

    	m_Lock.unlock();
    	return true;
}

void
sdl_render::re_size(int w, int h)
{
#if 1
	m_Lock.lock();
       m_image_height = h;
       m_image_width = w;
#if 0
	if (m_image_width % 4 != 0)
	{
		m_image_width = (m_image_width/4) * 4;
	}
	if (m_image_height % 4 != 0)
	{
		m_image_height = (m_image_height/4) * 4;
	}
#endif
	VDC_DEBUG( "%s (%d, %d)\n",__FUNCTION__, m_image_width, m_image_height);
	Init(m_image_width, m_image_height);
#ifndef WIN32
       //usleep(1000 * 50);
#endif
	m_Lock.unlock();
#endif
}

bool
sdl_render::init_render(void *ctx, int w, int h, int pix_fmt)
{
	m_hWnd = ctx;
	m_pix_fmt = pix_fmt;

	//if (g_UseHardwareCodec == 1)
	//{
	//	m_Scale.SetAttribute(SWS_PF_NV12, SWS_PF_YUV420P, SWS_SA_FAST_BILINEAR);
	//}else
#ifndef USING_LIBYUV
	{
		m_Scale.SetAttribute(SWS_PF_YUV420P, SWS_PF_YUV420P, SWS_SA_FAST_BILINEAR);
	}
#endif
	re_size(w, h);
	return true;
}

void
sdl_render::aspect_ratio(int srcw, int srch, bool enable_aspect)
{

}

void
sdl_render::destory_render()
{
	m_Lock.lock();
	Close();
	m_Lock.unlock();

}

bool
sdl_render::use_overlay()
{
    return true;
}

bool sdl_render::Init(int width, int height)
{
	if (m_bInited == true)
	{
		Close();
	}
#ifndef WIN32 //Linux need big lock
	gRenderLock.lock();
#endif	
	if (m_SdlWin == NULL)
	{
		m_SdlWin = SDL_CreateWindowFrom((void *)m_hWnd);
		SDL_ShowWindow(m_SdlWin);
	}
	
	m_SdlRender = SDL_CreateRenderer(m_SdlWin, 0, SDL_RENDERER_ACCELERATED);
	//if (g_UseHardwareCodec == 1)
	//{
	//    	m_pTex = SDL_CreateTexture(m_SdlRender, SDL_PIXELFORMAT_IYUV, 
	//        	SDL_TEXTUREACCESS_STREAMING, width, height);
	//}else
	{
	    	m_pTex = SDL_CreateTexture(m_SdlRender, SDL_PIXELFORMAT_YV12, 
	        	SDL_TEXTUREACCESS_STREAMING, width, height);		
	}
#ifndef WIN32 //Linux need big lock
	gRenderLock.unlock();
#endif
	m_bInited = true;
	
	return true;
}

bool sdl_render::Close()
{
	if (m_pTex)
	{
		SDL_DestroyTexture(m_pTex);
		m_pTex = NULL;
	}
	if (m_SdlRender)
	{
		SDL_DestroyRenderer(m_SdlRender);
		m_SdlRender = NULL;
	}
	
#if 1
	if (m_SdlWin)
	{
		SDL_DestroyWindow(m_SdlWin);
		m_SdlWin = NULL;
	}
#endif

	m_bInited = false;
	return true;
}


