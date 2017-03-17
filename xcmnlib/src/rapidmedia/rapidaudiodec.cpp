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
#include "rapidmedia/rapidaudiodec.hpp"
#include "cppkit/ck_exception.h"

extern "C"
{
#include "libavutil/opt.h"
}


static const size_t DEFAULT_PADDING = 16;

RapidAudioDec::RapidAudioDec(CodecType codec,  int bit_rate, int sample_rate, 
	int channels, 
	const struct codec_options& options, int decodeAttempts) 
:m_codecType(codec), m_codec(NULL), m_context(NULL), m_frame(NULL)
{
	m_options = options;
	m_decodeAttempts = decodeAttempts;
	m_codec = avcodec_find_decoder(AV_CODEC_ID_PCM_ALAW);
	if( !m_codec )
	{
		printf("Failed to find Audio decoder.\n");
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


	m_context->debug_mv = 0;
	m_context->debug = 0;
	m_context->workaround_bugs = 1;
	m_context->lowres = 0;
	m_context->idct_algo = FF_IDCT_AUTO;
	m_context->skip_frame = AVDISCARD_DEFAULT;
	m_context->skip_idct = AVDISCARD_DEFAULT;
	m_context->skip_loop_filter = AVDISCARD_DEFAULT;
	m_context->error_concealment = 3;
	
	m_context->bit_rate = bit_rate;
	m_context->sample_rate = sample_rate;
	m_context->channels = channels;
	m_context->sample_fmt = AV_SAMPLE_FMT_S16;
	m_context->codec_id = AV_CODEC_ID_PCM_ALAW;
	m_context->codec_type = AVMEDIA_TYPE_AUDIO;

	if( avcodec_open2( m_context, m_codec, NULL) < 0 )
	{
		printf("Unable to open Audio decoder.\n");
		return;
	}

	m_Init = TRUE;
}

RapidAudioDec::~RapidAudioDec()
{
    if( m_frame )
        av_free( m_frame );

    if( m_context )
    {
        avcodec_close( m_context );

        av_free( m_context );
    }
}

BOOL RapidAudioDec::Decode(uint8_t* pBuf, int nSize, RawFrame & pFrame)
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
	    ret = avcodec_decode_audio4( m_context,
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
		memcpy(pFrame.data, m_frame->data, sizeof(uint8_t*) * 4);
		memcpy(pFrame.linesize, m_frame->linesize, sizeof(int) * 4);
		//printf("Decoding returned size : %d\n", pFrame.linesize[0]);

		pFrame.format = m_frame->format;

		return TRUE;
	}

	return FALSE;
}

