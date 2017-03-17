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
#include "rapidmedia/rapiddxva2dec.hpp"
#include "cppkit/ck_exception.h"

extern "C"
{
#include "libavutil/opt.h"
}


static const size_t DEFAULT_PADDING = 16;

RapidDXVA2Dec::RapidDXVA2Dec(RMRawVideoHandler pHandler,  void * pVideoContext, 
	const struct codec_options& options, 
   enum AVCodecID id, int decodeAttempts) 
   :m_id(id), m_codec(NULL), m_context(NULL), m_frame(NULL), 
   RapidDec(pHandler, pVideoContext)
{
	m_options = options;
	m_decodeAttempts = decodeAttempts;
	{
		m_codec = avcodec_find_decoder(m_id);
	}
	if( !m_codec )
	{
		printf("Failed to find %s decoder.\n", m_id);
		return;
	}

	m_context = avcodec_alloc_context3(m_codec);
	if( !m_context )
	{
		printf("Failed to allocate decoder context.\n" );
		return;
	}
	m_frame = av_frame_alloc();
	if( !m_frame)
	{
		printf("Failed to allocate frame.\n");
		return;
	}

	m_inputStream.hwaccel_id = HWACCEL_DXVA2;	
	m_inputStream.active_hwaccel_id = HWACCEL_DXVA2;	
	m_inputStream.hwaccel_device = 0;	
	m_inputStream.hwaccel_ctx = NULL;	
	m_inputStream.hwaccel_get_buffer = NULL;	
	m_inputStream.hwaccel_retrieve_data = NULL;	
	m_inputStream.hwaccel_uninit = NULL;
	m_inputStream.hwaccel_cb_ctx = (void *)this;
	m_inputStream.hwaccel_get_d3d_dev = RapidDXVA2Dec::HWGetD3DDev;
	

	m_context->opaque = &m_inputStream;
	m_context->get_format = get_format;
	m_context->get_buffer2 = get_buffer;
	m_context->thread_safe_callbacks = 1;

	int res = avcodec_open2(m_context, m_codec, NULL);
	if (res < 0) {
		printf("error %x in avcodec_open2\n", res);
		return;
	}
	
	m_Init = TRUE;
	
}

RapidDXVA2Dec::~RapidDXVA2Dec()
{
    if( m_frame )
        av_free( m_frame );

    if( m_context )
    {
        avcodec_close( m_context );

        av_free( m_context );
    }
}

void * RapidDXVA2Dec::HWGetD3DDev(AVCodecContext *s, void * pCtx)
{
	int dummy = errno;
	RapidDXVA2Dec *pDec = (RapidDXVA2Dec*)pCtx;

	if (pDec)
	{
		return pDec->HWGetD3DDev1(s);
	}
	return NULL;
}

void * RapidDXVA2Dec::HWGetD3DDev1(AVCodecContext *s)
{
	RawFrame frame;
	if (m_rawVideoHandler)
	{
		frame.hwCmd = VIDEO_HW_GET_D3D_DEV;
		m_rawVideoHandler(m_pRawVideoContext, frame);
		return frame.hwD3DDev;
	}
	return NULL;
}

BOOL RapidDXVA2Dec::Decode(uint8_t* pBuf, int nSize, RawFrame & pFrame)
{
	if (pBuf == NULL || nSize == 0 || m_Init == FALSE)
	{
		return FALSE;
	}
	AVPacket inputPacket;
	av_init_packet( &inputPacket );
	inputPacket.data = pBuf;
	inputPacket.size = nSize;

	int gotPicture = 0;
	int ret = 0;
	int attempts = 0;

	do
	{
	    ret = avcodec_decode_video2( m_context,
	                                 m_frame,
	                                 &gotPicture,
	                                 &inputPacket );

	    attempts++;

	} while( (gotPicture == 0) && (attempts < m_decodeAttempts) );

	if( attempts >= m_decodeAttempts )
	{
	    //printf( "Unable to decode frame %s\n." __FUNCTION__);
	}

	if( ret < 0 )
	{
	    //printf("Decoding returned error: %d\n", ret );
	}
	if (gotPicture >= 1)
	{
		//int err = m_inputStream.hwaccel_get_buffer(m_context, m_frame, 0);
		//if (err < 0)
		{
		//	printf("dxva2 decode failed\n");
		}
	
		pFrame.hwCmd = VIDEO_HW_DATA;
		memcpy(pFrame.data, m_frame->data, sizeof(uint8_t*) * 4);
		memcpy(pFrame.linesize, m_frame->linesize, sizeof(int) * 4);

		pFrame.width = m_context->width;
		pFrame.height = m_context->height;

		pFrame.format = m_frame->format;

		return TRUE;
	}

	return FALSE;
}

