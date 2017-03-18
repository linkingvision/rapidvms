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
#include "vvideodraw.hpp"
#include "render_wrapper.hpp"
#include "XSDK/TimeUtils.h"

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32 //Windows
#define snprintf _snprintf
#endif

using namespace UtilityLib;
using namespace std;
using namespace XSDK;


VVideoDraw::VVideoDraw()
:m_width(0), m_height(0),m_format(0),
m_filter_graph(NULL), m_bInit(false)
{

}
VVideoDraw::~VVideoDraw()
{

}

bool VVideoDraw::Init(int w, int h)
{
	int ret;
	m_width = w;
	m_height = h;
	//const char *filter_descr = "lutyuv='u=128:v=128'";
	//const char *filter_descr = "boxblur";
	//const char *filter_descr = "hflip";
	//const char *filter_descr = "hue='h=60:s=-3'";
	const char *filter_descr = "crop=320:240";
	//const char *filter_descr = "drawbox=x=100:y=100:w=100:h=100:color=pink@0.5";
	//const char *filter_descr = "drawtext=fontfile=arial.ttf:fontcolor=white:fontsize=30:text='OpenCVR'";
	//const char *filter_descr = "drawtext=fontfile=arial.ttf:fontcolor=white:fontsize=30:text='OpenCVR'";

	char args[512];
	m_buffersrc  = avfilter_get_by_name("buffer");
	m_buffersink = avfilter_get_by_name("buffersink");
	m_outputs = avfilter_inout_alloc();
	m_inputs  = avfilter_inout_alloc();
	enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE };

	m_filter_graph = avfilter_graph_alloc();

	/* buffer video source: the decoded frames from the decoder will be inserted here. */
	snprintf(args, sizeof(args),
		"video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
		m_width,m_height,AV_PIX_FMT_YUV420P,
		1, 100,1,1);

	ret = avfilter_graph_create_filter(&m_buffersrc_ctx, m_buffersrc, "in",
		args, NULL, m_filter_graph);
	if (ret < 0) {
		printf("Cannot create buffer source\n");
		return false;
	}

	/* buffer video sink: to terminate the filter chain. */
	m_buffersink_params = av_buffersink_params_alloc();
	m_buffersink_params->pixel_fmts = pix_fmts;
	ret = avfilter_graph_create_filter(&m_buffersink_ctx, m_buffersink, "out",
		NULL, m_buffersink_params, m_filter_graph);
	av_free(m_buffersink_params);
	if (ret < 0) {
		printf("Cannot create buffer sink\n");
		return false;
	}

	/* Endpoints for the filter graph. */
	m_outputs->name       = av_strdup("in");
	m_outputs->filter_ctx = m_buffersrc_ctx;
	m_outputs->pad_idx    = 0;
	m_outputs->next       = NULL;

	m_inputs->name       = av_strdup("out");
	m_inputs->filter_ctx = m_buffersink_ctx;
	m_inputs->pad_idx    = 0;
	m_inputs->next       = NULL;

	if ((ret = avfilter_graph_parse_ptr(m_filter_graph, filter_descr,
		&m_inputs, &m_outputs, NULL)) < 0)
		return false;

	if ((ret = avfilter_graph_config(m_filter_graph, NULL)) < 0)
		return false;

	m_frame_in=av_frame_alloc();
	m_frame_buffer_in=(unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, m_width,m_height,1));
	av_image_fill_arrays(m_frame_in->data, m_frame_in->linesize,m_frame_buffer_in,
		AV_PIX_FMT_YUV420P,m_width, m_height,1);
	
	m_frame_out=av_frame_alloc();
	m_frame_buffer_out=(unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, m_width,m_height,1));
	av_image_fill_arrays(m_frame_out->data, m_frame_out->linesize,m_frame_buffer_out,
		AV_PIX_FMT_YUV420P,m_width, m_height,1);
	fp_out=fopen("output.yuv","wb+");

	m_bInit = true;

	return true;
}
bool VVideoDraw::UnInit()
{
	if (m_bInit != true)
	{
		return false;
	}
	avfilter_graph_free(&m_filter_graph);
	m_filter_graph = NULL;
	m_bInit = false;
	return true;
}

bool VVideoDraw::ProcessFrame(RenderFrame& frame, RenderFrame& outFrame)
{
	int ret = 0;

	if (frame.width != m_width || frame.height != m_height)
	{
		UnInit();
	}
	printf( "%s--%d.\n", __FILE__, __LINE__);
	if (m_bInit == false)
	{
		Init(frame.width, frame.height);
	}
	printf( "%s--%d.\n", __FILE__, __LINE__);

	m_frame_in->width = frame.width;
	m_frame_in->height = frame.height;
	m_frame_in->format = frame.format;

	//m_frame_in->data[0]=m_frame_buffer_in;
	//m_frame_in->data[1]=m_frame_buffer_in+m_width*m_height;
	//m_frame_in->data[2]=m_frame_buffer_in+m_width*m_height*5/4;

	//input Y,U,V
	m_frame_in->data[0] = (uint8_t*)frame.data[0];
	m_frame_in->data[1] = (uint8_t*)frame.data[1];
	m_frame_in->data[2] = (uint8_t*)frame.data[2];

	m_frame_in->linesize[0]=frame.linesize[0];
	m_frame_in->linesize[1]=frame.linesize[1];
	m_frame_in->linesize[2]=frame.linesize[2];
#if 0
	m_frame_out->width = frame.width;
	m_frame_out->height = frame.height;
	m_frame_out->format = AV_PIX_FMT_YUV420P;

	//input Y,U,V
	m_frame_out->data[0] = (uint8_t*)frame.data[0];
	m_frame_out->data[1] = (uint8_t*)frame.data[1];
	m_frame_out->data[2] = (uint8_t*)frame.data[2];

	m_frame_out->linesize[0]=frame.linesize[0];
	m_frame_out->linesize[1]=frame.linesize[1];
	m_frame_out->linesize[2]=frame.linesize[2];
#endif

    	if (av_buffersrc_add_frame(m_buffersrc_ctx, m_frame_in) < 0) {
            	printf( "Error while add frame.\n");
            	return false;
    	}
	printf( "%s--%d.\n", __FILE__, __LINE__);
    /* pull filtered pictures from the filtergraph */
	ret = av_buffersink_get_frame(m_buffersink_ctx, m_frame_out);
	printf( "%s--%d.\n", __FILE__, __LINE__);
#if 0
	memcpy(frame.data[0], m_frame_out->data[0], m_frame_out->linesize[0]);
	memcpy(frame.data[1], m_frame_out->data[1], m_frame_out->linesize[1]);
	memcpy(frame.data[2], m_frame_out->data[2], m_frame_out->linesize[2]);
#else
	//memset(frame.data[0], 0, frame.linesize[0]);
	//memset(frame.data[1], 0, frame.linesize[1]);
	//memset(frame.data[2], 0, frame.linesize[2]);
#endif
	outFrame.format = m_frame_out->format;
#if 1
	outFrame.linesize[0] = m_frame_out->linesize[0];
	outFrame.linesize[1] = m_frame_out->linesize[1];
	outFrame.linesize[2] = m_frame_out->linesize[2];
#else
	outFrame.linesize[0] = 320;
	outFrame.linesize[1] = 160;
	outFrame.linesize[2] = 160;
#endif
	outFrame.width = m_frame_out->width;
	outFrame.height = m_frame_out->height;

	outFrame.data[0] = (char*)m_frame_out->data[0];
	outFrame.data[1] = (char*)m_frame_out->data[1];
	outFrame.data[2] = (char*)m_frame_out->data[2];
	
#if 0
	if(m_frame_out->format==AV_PIX_FMT_YUV420P){
		for(int i=0;i<m_frame_out->height;i++){
			fwrite(m_frame_out->data[0]+m_frame_out->linesize[0]*i,1,m_frame_out->width,fp_out);
		}
		for(int i=0;i<m_frame_out->height/2;i++){
			fwrite(m_frame_out->data[1]+m_frame_out->linesize[1]*i,1,m_frame_out->width/2,fp_out);
		}
		for(int i=0;i<m_frame_out->height/2;i++){
			fwrite(m_frame_out->data[2]+m_frame_out->linesize[2]*i,1,m_frame_out->width/2,fp_out);
		}
	}
#endif

	//av_frame_unref(m_frame_out);
}

bool VVideoDraw::ReleaseFrame()
{
	av_frame_unref(m_frame_out);
	return true;
}

