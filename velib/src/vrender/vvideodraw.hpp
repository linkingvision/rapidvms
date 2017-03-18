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
#ifndef __V_VIDEO_DRAW_HPP__
#define __V_VIDEO_DRAW_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "utility/videotype.hpp"
#include "video_render.h"

extern "C"
{
#include "libavfilter/avfiltergraph.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
};

using namespace UtilityLib;

class VVideoDraw
{
public:

	VVideoDraw();
	~VVideoDraw();
public:
	bool ProcessFrame(RenderFrame& frame, RenderFrame& outFrame);
	bool ReleaseFrame();
	bool Init(int w, int h);
	bool UnInit();
private:
	int m_width; 
	int m_height;
	int m_format;
	astring m_strText;
	bool m_bInit;

	AVFrame *m_frame_in;
	AVFrame *m_frame_out;
	unsigned char *m_frame_buffer_in;
	unsigned char *m_frame_buffer_out;

	AVFilterContext *m_buffersink_ctx;
	AVFilterContext *m_buffersrc_ctx;
	AVFilterGraph *m_filter_graph;
	AVFilter *m_buffersrc;
	AVFilter *m_buffersink;
	AVFilterInOut *m_outputs;
	AVFilterInOut *m_inputs;
	AVBufferSinkParams *m_buffersink_params;
	FILE *fp_out;
	
	
	
};
#endif /* __V_VIDEO_DRAW_HPP__ */
