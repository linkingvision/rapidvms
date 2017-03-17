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
#include "rapidmedia/rapidmedia.hpp"
#include "cppkit/ck_memory.h"
#include "rapidrtsp.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XBlockingQueue.h"
#include "rapidmedia/rapidmediadata.hpp"
#include "rapidmedia/rapidaudiodec.hpp"
#include "rapidmedia/rapidffmpegdec.hpp"

using namespace cppkit;
using namespace XSDK;

RapidMedia::RapidMedia()
{
	m_data = new RapidMediaData(*this);
}
RapidMedia::~RapidMedia()
{
	if (m_data)
	{
		delete m_data;
		m_data = NULL;
	}
}

BOOL RapidMedia::Init(BOOL bRealStream, string strUrl, string strUser, 
		string strPass, BOOL bHWAccel, VSCConnectType connectType)
{
	return m_data->Init(bRealStream, strUrl, strUser, strPass, bHWAccel, connectType);
}

BOOL RapidMedia::PutData(VideoFrame& packet)
{
	return m_data->PutData(packet);
}
void	RapidMedia::SetDataHandler(RMDataHandler handle, void *pContext)
{
	//set the handler to the callback thread
	return m_data->SetDataHandler(handle, pContext);
}
void	RapidMedia::SetRawDataHandler(RMRawVideoHandler handle, void *pContext)
{
	//set the handler to the decode thread
	return m_data->SetRawDataHandler(handle, pContext);
}

BOOL RapidMedia::Start()
{
	return m_data->Start();
}
BOOL RapidMedia::Stop()
{
	return m_data->Stop();
}

BOOL RapidMedia::StartRaw()
{
	return m_data->StartRaw();
}
BOOL RapidMedia::StopRaw()
{
	return m_data->StopRaw();
}

std::string RapidMedia::GetCodecName(CodecType codec)
{
	switch (codec)
	{
		case CODEC_PCMU:
		{
			return "PCMU";
			break;
		}
		case CODEC_PCMA:
		{
			return "PCMU";
			break;
		}
		case CODEC_G711A:
		{
			return "G711A";
			break;
		}
		case CODEC_G711U:
		{
			return "G711U";
			break;
		}
		case CODEC_H264:
		{
			return "H264";
			break;
		}
		case CODEC_H265:
		{
			return "H265";
			break;
		}
		case CODEC_MPEG4:
		{
			return "MPEG4";
			break;
		}
		case CODEC_MJPEG:
		{
			return "MJPEG";
			break;
		}
		case CODEC_AAC:
		{
			return "AAC";
			break;
		}
		default:
		   	return "LOADING";
	};
}

bool RapidMedia::DecodeiFrame(VideoFrame& packet, RawFrame& pRaw, void* &pDecoder)
{
	if (packet.frameType != VIDEO_FRM_I  
		|| packet.dataBuf == NULL || packet.dataLen == 0)
	{
		return false;
	}

	CodecType current;

	u8  *dataBuf = NULL;
	u32 dataSize = 0;

	VideoFrameType type = packet.frameType;
	RapidDec *pDec;

	switch (packet.frameType)
	{
		case VIDEO_FRM_I:
		{
			dataBuf = packet.dataBuf + sizeof(InfoFrameI);
			dataSize = packet.dataLen - sizeof(InfoFrameI);
			InfoFrameI *pI = (InfoFrameI *)packet.dataBuf;
			
			current = (CodecType)pI->video;

			if (current == CODEC_NONE)
			{
				return FALSE;
			}
			break;
		}
		default:
		   	return FALSE;
	};

	switch (current)
	{
		case CODEC_H264:
		{
			pDec = new RapidFFMPEGDec(get_fast_h264_decoder_options(), AV_CODEC_ID_H264);
			break;
		}
		case CODEC_H265:
		{
			pDec = new RapidFFMPEGDec(get_fast_h264_decoder_options(), AV_CODEC_ID_HEVC);
			break;
		}
		case CODEC_MJPEG:
		{
			pDec = new RapidFFMPEGDec(get_fast_h264_decoder_options(), AV_CODEC_ID_MJPEG);
			break;
		}
		case CODEC_MPEG4:
		{
			pDec = new RapidFFMPEGDec(get_fast_h264_decoder_options(), AV_CODEC_ID_MPEG4);
			break;
		}
		default:
		   	return FALSE;
	};
	

	RawFrame  pDecFrame;
	BOOL ret = FALSE;
	switch (current)
	{
		case CODEC_H264:
		{
			
			pDecFrame.type = VIDEO_RAW_VIDEO;
			pDecFrame.codec = CODEC_H264;
			
			//RapidH264Dec * pH264Dec = (RapidH264Dec *)pDec;
			//DumpHexData((uint8_t*)(dataBuf), 100);
			ret = pDec->Decode((uint8_t*)dataBuf, dataSize, pDecFrame);

			break;
		}
		case CODEC_H265:
		{
			pDecFrame.type = VIDEO_RAW_VIDEO;
			pDecFrame.codec = CODEC_H265;
			//RapidH265Dec * pH265Dec = (RapidH265Dec *)pDec;
			//DumpHexData((uint8_t*)(dataBuf), 100);
			BOOL ret = pDec->Decode((uint8_t*)dataBuf, dataSize, pDecFrame);
			break;
		}
		case CODEC_MJPEG:
		{
			pDecFrame.type = VIDEO_RAW_VIDEO;
			pDecFrame.codec = CODEC_MJPEG;
			//RapidMJPEGDec * pMJPEGDec = (RapidMJPEGDec *)pDec;
			//DumpHexData((uint8_t*)(dataBuf), 100);
			BOOL ret = pDec->Decode((uint8_t*)dataBuf, dataSize, pDecFrame);

			break;
		}
		case CODEC_MPEG4:
		{
			pDecFrame.type = VIDEO_RAW_VIDEO;
			pDecFrame.codec = CODEC_MPEG4;
			//RapidMPEG4Dec * pMPEG4Dec = (RapidMPEG4Dec *)pDec;
			//DumpHexData((uint8_t*)(dataBuf), 100);
			BOOL ret = pDec->Decode((uint8_t*)dataBuf, dataSize, pDecFrame);

			break;
		}
		default:
			delete pDec;
		   	return FALSE;
	};	

	memset(&pRaw, 0, sizeof(RawFrame));

	pRaw.type = pDecFrame.type;
	pRaw.codec = pDecFrame.codec;
	pRaw.width = pDecFrame.width;
	pRaw.height = pDecFrame.height;
	pRaw.format = pDecFrame.format;
	pRaw.secs = pDecFrame.msecs;
	
	pRaw.linesize[0] = pDecFrame.linesize[0];
	pRaw.linesize[1] = pDecFrame.linesize[1];
	pRaw.linesize[2] = pDecFrame.linesize[2];

	pRaw.data[0] = pDecFrame.data[0];
	pRaw.data[1] = pDecFrame.data[1];
	pRaw.data[2] = pDecFrame.data[2];
	

	pDecoder = pDec;

	return true;
}

bool RapidMedia::DecodeiFrameFreeDec(void* pDec)
{
	if (pDec)
	{
		delete pDec;
		pDec = NULL;
	}

	return true;
}

