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
#include "rapidmedia/rapiddecsink.hpp"
#ifdef _WIN32
#include "rapidmedia/rapiddxva2dec.hpp"
#endif

static void DumpHexData(unsigned char *pNuf, int nLen)
{
    int i = 0;
    for (i = 0; i < nLen; i ++)
    {
		printf(" 0x%x", pNuf[i]);
    }
	printf("\n");
}

RapidDecSink::RapidDecSink(RMRawVideoHandler rawVideoHandler, 
		void *pRawVideoContext, BOOL HWAccel)
:m_rawVideoHandler(rawVideoHandler), m_pRawVideoContext(pRawVideoContext), 
m_pVideoDec(NULL), m_pAudioDec(NULL), 
m_AudioCodec(CODEC_NONE), m_VideoCodec(CODEC_NONE), 
m_HWAccel(HWAccel), m_bGotFrameI(FALSE)
{

}
RapidDecSink::~RapidDecSink()
{
		if (m_pVideoDec)
		{
			delete m_pVideoDec;
			m_pVideoDec = NULL;
		}
		if (m_pAudioDec)
		{
			delete m_pAudioDec;
			m_pAudioDec = NULL;			
		}
}

BOOL RapidDecSink::DecodeAFrame(VideoFrame & pFrame)
{
	CodecType current;
	//DumpHexData((uint8_t*)(pFrame.dataBuf), 100);
	if (m_bGotFrameI == FALSE)
	{
#ifdef RAPID_PERF_DEBUG
		RRLOG(RRLOG_INFO, "I Frame not got, can decode audio\n");
#endif
		return TRUE;
	}
	current = (CodecType)(m_FrameI.audio);
	
	if ((m_pAudioDec == NULL && m_AudioCodec == CODEC_NONE)
	 	|| (m_AudioCodec != current) )
	{
		if (m_AudioCodec != CODEC_NONE && m_pAudioDec)
		{
			delete m_pAudioDec;
			m_pAudioDec = NULL;
		}
		m_AudioCodec = current;
		switch (m_AudioCodec)
		{
			case CODEC_PCMU:
			{
				m_pAudioDec = new RapidAudioDec(CODEC_PCMU, m_FrameI.aSampleRate, 
					m_FrameI.aSampleSize, m_FrameI.aChannels, 
							get_fast_decoder_options());
				break;
			}
			case CODEC_PCMA:
			{
				break;
			}
			default:
			   	return FALSE;
		};
	
	}


	switch (m_AudioCodec)
	{
		case CODEC_PCMU:
		{
			RawFrame  pDecFrame;
			pDecFrame.type = VIDEO_RAW_AUDIO;
			pDecFrame.codec = CODEC_PCMU;
			//DumpHexData((uint8_t*)(dataBuf), 100);
			BOOL ret = m_pAudioDec->Decode((uint8_t*)pFrame.dataBuf, 
							pFrame.bufLen, pDecFrame);
			if (ret == TRUE && m_rawVideoHandler)
			{
				m_rawVideoHandler(m_pRawVideoContext, pDecFrame);
			}
#ifdef RAPID_PERF_DEBUG
			RRLOG(RRLOG_INFO, "Decode ret Audio %d size %d\n", ret, pDecFrame.linesize[0]);
#endif
			break;
		}
		default:
		   	return FALSE;
	};	
	

	return TRUE;
}
BOOL RapidDecSink::DecodeVFrame(VideoFrame & pFrame)
{
	CodecType current;

	u8  *dataBuf = NULL;
	u32 dataSize = 0;

	VideoFrameType type = pFrame.frameType;

	switch (pFrame.frameType)
	{
		case VIDEO_FRM_I:
		{
			dataBuf = pFrame.dataBuf + sizeof(InfoFrameI);
			dataSize = pFrame.dataLen - sizeof(InfoFrameI);
			InfoFrameI *pI = (InfoFrameI *)pFrame.dataBuf;
			/* Cache I frame for audio decoder */
			memcpy(&m_FrameI, pI, sizeof(InfoFrameI));
			m_bGotFrameI = TRUE;
			current = (CodecType)pI->video;

			if (current == CODEC_NONE)
			{
				return FALSE;
			}
			break;
		}
		case VIDEO_FRM_P:
		{
			dataBuf = pFrame.dataBuf + sizeof(InfoFrameP);
			dataSize = pFrame.dataLen - sizeof(InfoFrameP);
			InfoFrameP *pP = (InfoFrameP *)pFrame.dataBuf;
			current = (CodecType)pP->video;

			if (current == CODEC_NONE)
			{
				return FALSE;
			}
			break;
		}
		default:
		   	return FALSE;
	};

	if ((m_pVideoDec == NULL && m_VideoCodec == CODEC_NONE)
	 	|| (m_VideoCodec != current) )
	{
		if (m_VideoCodec != CODEC_NONE && m_pVideoDec)
		{
			delete m_pVideoDec;
			m_pVideoDec = NULL;
		}
		m_VideoCodec = current;
		switch (m_VideoCodec)
		{
			case CODEC_H264:
			{
				if (m_HWAccel == FALSE)
				{
					m_pVideoDec = new RapidFFMPEGDec(get_fast_h264_decoder_options(), AV_CODEC_ID_H264);
				}else
				{
	#ifdef _WIN32       
					/* Windows use DXVA2 */
					m_pVideoDec = new RapidDXVA2Dec(m_rawVideoHandler, 
							m_pRawVideoContext, get_fast_h264_decoder_options(), AV_CODEC_ID_H264);
	#else
					/* Linux use  Yami*/
	#endif
				}
				break;
			}
			case CODEC_H265:
			{
				if (m_HWAccel == FALSE)
				{
					m_pVideoDec = new RapidFFMPEGDec(get_fast_h264_decoder_options(), AV_CODEC_ID_HEVC);
				}else
				{
	#ifdef _WIN32       
					/* Windows use DXVA2 */
					m_pVideoDec = new RapidDXVA2Dec(m_rawVideoHandler, m_pRawVideoContext, 
							get_fast_h264_decoder_options(), AV_CODEC_ID_HEVC);
	#else
					/* Linux use  Yami*/
	#endif
				}
				break;
			}
			case CODEC_MJPEG:
			{
				m_pVideoDec = new RapidFFMPEGDec(get_fast_h264_decoder_options(), AV_CODEC_ID_MJPEG);
				break;
			}
			case CODEC_MPEG4:
			{
				m_pVideoDec = new RapidFFMPEGDec(get_fast_h264_decoder_options(), AV_CODEC_ID_MPEG4);
				break;
			}
			default:
			   	return FALSE;
		};
	
	}

	switch (m_VideoCodec)
	{
		case CODEC_H264:
		{
			RawFrame  pDecFrame;
			pDecFrame.type = VIDEO_RAW_VIDEO;
			pDecFrame.codec = CODEC_H264;
			//RapidH264Dec * pH264Dec = (RapidH264Dec *)m_pVideoDec;
			//DumpHexData((uint8_t*)(dataBuf), 100);
			BOOL ret = m_pVideoDec->Decode((uint8_t*)dataBuf, dataSize, pDecFrame);
			if (ret == TRUE && m_rawVideoHandler)
			{
				m_rawVideoHandler(m_pRawVideoContext, pDecFrame);
			}
#ifdef RAPID_PERF_DEBUG
			RRLOG(RRLOG_INFO, "Decode ret H264 %d (%d, %d)\n", ret, pDecFrame.width, pDecFrame.height);
#endif
			break;
		}
		case CODEC_H265:
		{
			RawFrame  pDecFrame;
			pDecFrame.type = VIDEO_RAW_VIDEO;
			pDecFrame.codec = CODEC_H265;
			//RapidH265Dec * pH265Dec = (RapidH265Dec *)m_pVideoDec;
			//DumpHexData((uint8_t*)(dataBuf), 100);
			BOOL ret = m_pVideoDec->Decode((uint8_t*)dataBuf, dataSize, pDecFrame);
			if (ret == TRUE && m_rawVideoHandler)
			{
				m_rawVideoHandler(m_pRawVideoContext, pDecFrame);
			}
#ifdef RAPID_PERF_DEBUG
			RRLOG(RRLOG_INFO, "Decode ret H265 %d (%d, %d)\n", ret, pDecFrame.width, pDecFrame.height);
#endif
			break;
		}
		case CODEC_MJPEG:
		{
			RawFrame  pDecFrame;
			pDecFrame.type = VIDEO_RAW_VIDEO;
			pDecFrame.codec = CODEC_MJPEG;
			//RapidMJPEGDec * pMJPEGDec = (RapidMJPEGDec *)m_pVideoDec;
			//DumpHexData((uint8_t*)(dataBuf), 100);
			BOOL ret = m_pVideoDec->Decode((uint8_t*)dataBuf, dataSize, pDecFrame);
			if (ret == TRUE && m_rawVideoHandler)
			{
				m_rawVideoHandler(m_pRawVideoContext, pDecFrame);
			}
#ifdef RAPID_PERF_DEBUG
			RRLOG(RRLOG_INFO, "Decode ret JPEG %d (%d, %d)\n", ret, pDecFrame.width, pDecFrame.height);
#endif
			break;
		}
		case CODEC_MPEG4:
		{
			RawFrame  pDecFrame;
			pDecFrame.type = VIDEO_RAW_VIDEO;
			pDecFrame.codec = CODEC_MPEG4;
			//RapidMPEG4Dec * pMPEG4Dec = (RapidMPEG4Dec *)m_pVideoDec;
			//DumpHexData((uint8_t*)(dataBuf), 100);
			BOOL ret = m_pVideoDec->Decode((uint8_t*)dataBuf, dataSize, pDecFrame);
			if (ret == TRUE && m_rawVideoHandler)
			{
				m_rawVideoHandler(m_pRawVideoContext, pDecFrame);
			}
#ifdef RAPID_PERF_DEBUG
			RRLOG(RRLOG_INFO, "Decode ret MPEG4 %d (%d, %d)\n", ret, pDecFrame.width, pDecFrame.height);
#endif
			break;
			break;
		}
		default:
		   	return FALSE;
	};	
	


	return TRUE;
}

BOOL RapidDecSink::DecodeFrame(VideoFrame & pFrame)
{

	switch (pFrame.streamType)
	{
		case VIDEO_STREAM_VIDEO:
			return DecodeVFrame(pFrame);
		case VIDEO_STREAM_AUDIO:
			return DecodeAFrame(pFrame);
		default:
		   	return FALSE;
	};	


	return FALSE;
}



