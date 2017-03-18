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
#ifndef SDL_RENDER_H
#define SDL_RENDER_H

#include "video_render.h"
#include "utility.hpp"
#include "debug.hpp"
#include "ffmpeg_scale.hpp"
#include "SDL.h"
#include "vplay/vsmotalgo.hpp"
#include "vvideodraw.hpp"

#ifdef WIN32
#define USING_LIBYUV 1
#endif
using namespace UtilityLib;

class sdl_render : public video_render
{
public:
	virtual bool init_render(void* ctx, int w, int h, int pix_fmt);

	virtual bool render_one_frame(RenderFrame* data, int pix_fmt);

	virtual void re_size(int width, int height);

	virtual void aspect_ratio(int srcw, int srch, bool enable_aspect);

	virtual void destory_render();
	
	virtual bool use_overlay();

	virtual void enable_mot(bool enable, astring strConf);
	
	virtual void show_alarm();
	
public:
	bool Close();
	bool Init(int width, int height);
	sdl_render();
	~sdl_render();
private:
    fast_mutex m_Lock;
#ifndef USING_LIBYUV
	FFmpegScale m_Scale;
#endif
	void * m_hWnd;
	bool m_bInited; 
	SDL_Window *m_SdlWin;
	SDL_Renderer *m_SdlRender;
	SDL_Texture *m_pTex;
	HWND m_hwnd;
	int m_image_width;
	int m_image_height;

	bool m_keep_aspect;

	float m_window_aspect;	
	int m_pix_fmt;
	char * m_buf;
	int m_bufSize;

	char * m_buf2;
	int m_bufSize2;

	bool m_motEnable;
	int m_alarmTime;
	bool m_alarmEnable;
	VSMotAlgo *m_MotAlgo;
	//VVideoDraw m_videoDraw;
};

#endif // SDL_RENDER_H
