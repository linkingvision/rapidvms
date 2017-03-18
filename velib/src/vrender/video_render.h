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
#ifndef __VIDEO_RENDER_H__
#define __VIDEO_RENDER_H__
#ifdef WIN32
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif
#else
#define MAKEFOURCC(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))
#endif
#include "utility/videotype.hpp"

#define FOURCC_PIX_FMT_YUV420P MAKEFOURCC('I','4','2','0') /* 16  YVU420 planar */
#define FOURCC_PIX_FMT_IYUV    MAKEFOURCC('I','Y','U','V') /* 16  IYUV   planar */
#define FOURCC_PIX_FMT_YVU410  MAKEFOURCC('Y','V','U','9') /*  9  YVU 4:1:0     */
#define FOURCC_PIX_FMT_YVU420  MAKEFOURCC('Y','V','1','2') /* 12  YVU 4:2:0     */
#define FOURCC_PIX_FMT_YUY2    MAKEFOURCC('Y','U','Y','2') /* 16  YUYV 4:2:2 */
#define FOURCC_PIX_FMT_UYVY    MAKEFOURCC('U','Y','V','Y') /* 16  UYVY 4:2:2 */
#define FOURCC_PIX_FMT_YVYU    MAKEFOURCC('Y','V','Y','U') /* 16  UYVY 4:2:2 */

#define FOURCC_PIX_FMT_RGB332  MAKEFOURCC('R','G','B','1') /*  8  RGB-3-3-2     */
#define FOURCC_PIX_FMT_RGB555  MAKEFOURCC('R','G','B','O') /* 16  RGB-5-5-5     */
#define FOURCC_PIX_FMT_RGB565  MAKEFOURCC('R','G','B','P') /* 16  RGB-5-6-5     */
#define FOURCC_PIX_FMT_RGB555X MAKEFOURCC('R','G','B','Q') /* 16  RGB-5-5-5 BE  */
#define FOURCC_PIX_FMT_RGB565X MAKEFOURCC('R','G','B','R') /* 16  RGB-5-6-5 BE  */
#define FOURCC_PIX_FMT_BGR15   MAKEFOURCC('B','G','R',15)  /* 15  BGR-5-5-5     */
#define FOURCC_PIX_FMT_RGB15   MAKEFOURCC('R','G','B',15)  /* 15  RGB-5-5-5     */
#define FOURCC_PIX_FMT_BGR16   MAKEFOURCC('B','G','R',16)  /* 32  BGR-5-6-5     */
#define FOURCC_PIX_FMT_RGB16   MAKEFOURCC('R','G','B',16)  /* 32  RGB-5-6-5     */
#define FOURCC_PIX_FMT_BGR24   MAKEFOURCC('B','G','R',24)  /* 24  BGR-8-8-8     */
#define FOURCC_PIX_FMT_RGB24   MAKEFOURCC('R','G','B',24)  /* 24  RGB-8-8-8     */
#define FOURCC_PIX_FMT_BGR32   MAKEFOURCC('B','G','R',32)  /* 32  BGR-8-8-8-8   */
#define FOURCC_PIX_FMT_RGB32   MAKEFOURCC('R','G','B',32)  /* 32  RGB-8-8-8-8   */
#define FOURCC_PIX_FMT_BGR     (('B'<<24)|('G'<<16)|('R'<<8))
#define FOURCC_PIX_FMT_BGR8    (FOURCC_PIX_FMT_BGR|8)
#define RenderFrame RawFrame
#if 0
typedef struct __RenderFrame {
#define NUM_DATA_POINTERS 8
	char *data[NUM_DATA_POINTERS];
	int linesize[NUM_DATA_POINTERS];
	int width, height;
	int format;
	bool hwData;
	__RenderFrame()
	{
		hwData = false;
	}
} RenderFrame;
#endif

class video_render
{
public:
   video_render() {}
   virtual ~video_render() {}

public:

   virtual bool init_render(void* ctx, int w, int h, int pix_fmt) = 0;


   virtual bool render_one_frame(RenderFrame* data, int pix_fmt) = 0;

   virtual void re_size(int width, int height) = 0;

   virtual void aspect_ratio(int srcw, int srch, bool enable_aspect) = 0;

   virtual bool use_overlay() { return false; }

   virtual void destory_render() = 0;

   virtual void enable_mot(bool enable, std::string strConf){return;}
   
   virtual void show_alarm(){return;}
};

#endif // __VIDEO_RENDER_H__
