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
#include "rapidmedia/rapidmediadata.hpp"
#include "cppkit/ck_memory.h"
#include "rapidrtsp.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XBlockingQueue.h"
#include "rapidmedia/rapiddecsink.hpp"
#include "rapidrtsp/rtspdef.h"
#include "rapidrtsp/rapidrtsplog.h"

#include "ffkit/fflocky.h"

using namespace ffkit;

using namespace cppkit;
using namespace XSDK;

class RapidMediaInitGlobal
{
public:
	RapidMediaInitGlobal()
	{
		locky::register_ffmpeg();	
		return ;
	}
};

RapidMediaInitGlobal gRapidMediaInitGlobal;

RapidMediaData::RapidMediaData(RapidMedia &pMedia)
:m_Media(pMedia), m_pRTSP(NULL), m_nAttachedRTSP(0), m_nAttached(0), m_nAttachedRaw(0), 
m_dataHandler(NULL), m_pDataContext(NULL), m_rawVideoHandler(NULL), 
m_pRawVideoContext(NULL), m_InitBlock(FALSE), m_InitBlockRaw(FALSE), 
m_pThreadData(NULL), m_pThreadRaw(NULL), m_bExitData(FALSE), 
m_bExitRaw(FALSE), m_bRealStream(FALSE), m_QueueDataLost(FALSE), 
m_QueueRawLost(FALSE)
{
	return;
}
RapidMediaData::~RapidMediaData()
{
	StopDataHandlerThread();
	StopRawHandlerThread();
	if (m_pRTSP)
	{
		delete m_pRTSP;
		m_pRTSP = NULL;
	}
}

BOOL RapidMediaData::InitBlockQueue()
{
	if (m_InitBlock == TRUE)
	{
		return TRUE;
	}
	
	for (int i = 0; i < RAPID_BLOCK_QUEUE_MAX; i ++)
	{
		VideoFrame * pFrame = (VideoFrame *)malloc(sizeof(VideoFrame));
		memset(pFrame, 0, sizeof(VideoFrame));
		pFrame->dataBuf = (u8 *)malloc(RAPID_BLOCK_SIZE);
		pFrame->bufLen = RAPID_BLOCK_SIZE;

		m_Queue4DataFree.Push(pFrame);
	}

	m_InitBlock = TRUE;

	return TRUE;
}


BOOL RapidMediaData::InitBlockQueueRaw()
{
	if (m_InitBlockRaw == TRUE)
	{
		return TRUE;
	}
	
	for (int i = 0; i < RAPID_BLOCK_QUEUE_MAX; i ++)
	{
		VideoFrame * pFrame = (VideoFrame *)malloc(sizeof(VideoFrame));
		memset(pFrame, 0, sizeof(VideoFrame));
		pFrame->dataBuf = (u8 *)malloc(RAPID_BLOCK_SIZE);
		pFrame->bufLen = RAPID_BLOCK_SIZE;

		m_Queue4RawFree.Push(pFrame);
	}

	m_InitBlockRaw = TRUE;

	return TRUE;
}

BOOL RapidMediaData::UnInitBlockQueue()
{
	if (m_InitBlock == FALSE)
	{
		return TRUE;
	}

	/* The free queue */
	while(m_Queue4DataFree.NonBlockingPeek() == true)
	{
		VideoFrame * pFrame = m_Queue4DataFree.Pop();
		if (pFrame)
		{
			if (pFrame->dataBuf)
			{
				free(pFrame->dataBuf);
				pFrame->dataBuf = NULL;
			}
			free(pFrame);
			pFrame = NULL;
		}
	}

	/* The used queue */
	while(m_Queue4Data.NonBlockingPeek() == true)
	{
		VideoFrame * pFrame = m_Queue4Data.Pop();
		if (pFrame)
		{
			if (pFrame->dataBuf)
			{
				free(pFrame->dataBuf);
				pFrame->dataBuf = NULL;
			}
			free(pFrame);
			pFrame = NULL;
		}
	}


	m_InitBlock = FALSE;

	return TRUE;
}
BOOL RapidMediaData::UnInitBlockQueueRaw()
{
	if (m_InitBlockRaw == FALSE)
	{
		return TRUE;
	}
	/* The free queue */
	while(m_Queue4RawFree.NonBlockingPeek() == true)
	{
		VideoFrame * pFrame = m_Queue4RawFree.Pop();
		if (pFrame)
		{
			if (pFrame->dataBuf)
			{
				free(pFrame->dataBuf);
				pFrame->dataBuf = NULL;
			}
			free(pFrame);
			pFrame = NULL;
		}
	}

	/* The used queue */
	while(m_Queue4Raw.NonBlockingPeek() == true)
	{
		VideoFrame * pFrame = m_Queue4Raw.Pop();
		if (pFrame)
		{
			if (pFrame->dataBuf)
			{
				free(pFrame->dataBuf);
				pFrame->dataBuf = NULL;
			}
			free(pFrame);
			pFrame = NULL;
		}
	}

	m_InitBlockRaw = FALSE;	

	return TRUE;
}

BOOL RapidMediaData::StartDataHandlerThread()
{
	if (m_pThreadData != NULL)
	{
		return TRUE;
	}
	InitBlockQueue();
	m_bExitData = FALSE;
	/* start the data handler thread */
	this->m_pThreadData = new tthread::thread(RapidMediaData::RunData, (void *)this);

	
	return TRUE;
}
BOOL RapidMediaData::StopDataHandlerThread()
{
	if (m_pThreadData == NULL)
	{
		return TRUE;
	}

	/* stop the data handler thread */
	m_bExitData = TRUE;

	/* push the exit command to the thread */
	VideoFrame * pFrame = (VideoFrame *)malloc(sizeof(VideoFrame));
	memset(pFrame, 0, sizeof(VideoFrame));
	pFrame->streamType = VIDEO_STREAM_END;
	m_Queue4Data.Push(pFrame);
	
	m_pThreadData->join();
	delete m_pThreadData;
	m_pThreadData = NULL;
	
	UnInitBlockQueue();

	
	return TRUE;
}
BOOL RapidMediaData::StartRawHandlerThread()
{
	if (m_pThreadRaw != NULL)
	{
		return TRUE;
	}
	InitBlockQueueRaw();
	m_bExitRaw = FALSE;
	/* start the data handler thread */
	this->m_pThreadRaw = new tthread::thread(RapidMediaData::RunRaw, 
					(void *)this);

	return TRUE;
}
BOOL RapidMediaData::StopRawHandlerThread()
{
	if (m_pThreadRaw == NULL)
	{
		return TRUE;
	}

	/* stop the raw handler thread */
	m_bExitRaw = TRUE;

	/* push the exit command to the thread */
	VideoFrame * pFrame = (VideoFrame *)malloc(sizeof(VideoFrame));
	memset(pFrame, 0, sizeof(VideoFrame));
	pFrame->streamType = VIDEO_STREAM_END;
	m_Queue4Raw.Push(pFrame);
	
	m_pThreadRaw->join();
	delete m_pThreadRaw;
	m_pThreadRaw = NULL;
	
	UnInitBlockQueueRaw();

	return TRUE;
}

void RapidMediaData::RapidRTSPEventHandle(int eventType, int lParam, void *rParam,
			void *context)
{
	if (context)
	{
		RapidMediaData *pData = (RapidMediaData *)context;
		return pData->RapidRTSPEventHandle1(eventType, 
				 lParam, rParam);
	}
}

void RapidMediaData::RapidRTSPDataHandle(void *pdata, uint32_t dataSize, 
	uint32_t sec, uint32_t msec , int dataType, VideoStreamType streamType, 
	VideoFrameType frameType, unsigned int flag, CRapidRTSPAVInfo &pAVInfo, void *context)
{
	if (context)
	{
		RapidMediaData *pData = (RapidMediaData *)context;
		return pData->RapidRTSPDataHandle1(pdata, dataSize, 
				 sec, msec, dataType, streamType, frameType, flag, pAVInfo);
	}
}

/*
	The dataType is CodecType in videotype.hpp
 */
void RapidMediaData::RapidRTSPDataHandle1(void *pdata, uint32_t dataSize, 
	uint32_t sec, uint32_t msec , int dataType, VideoStreamType streamType, 
	VideoFrameType frameType, unsigned int flag, CRapidRTSPAVInfo &pAVInfo)
{
	int padding = 0;
	char pad[RTP_MAX_PAD_SIZE];
#ifdef RAPID_PERF_DEBUG
	RRLOG(RRLOG_INFO, "%s data %d size:%u timestamp (%u, %d) frametype %d flag 0x%x", __FUNCTION__, 
					dataType, dataSize, sec, msec, frameType, flag);
#endif
	//printf("%s data %d (size:%u) (%d, %d) frametype %d flag 0x%x\n", __FUNCTION__, 
	//				dataType, dataSize, sec, msec, frameType, flag);
	if (streamType != VIDEO_STREAM_VIDEO)
	{
		return;//TODO just return if not video
	}
	if (dataType  == CODEC_H264 || dataType  == CODEC_H265)
	{
		if ((flag & RTP_FLAG_HAS_SPS_PPS) == RTP_FLAG_HAS_SPS_PPS)
		{
			padding = 0;
		}else
		{
			RRLOG(RRLOG_INFO, "Padding for H264/H265 SPS and PPS or VPS");
			padding = pAVInfo.video.video.extradataSize;
			if (padding > RTP_MAX_PAD_SIZE || padding <= 0)
			{
				padding = 0;
			}else
			{
				memcpy(pad, pAVInfo.video.video.extradata, pAVInfo.video.video.extradataSize);
			}
		}
	}

	BOOL skipData = FALSE;
	BOOL skipRaw = FALSE;

	if (m_QueueDataLost == TRUE)
	{
		if (streamType == VIDEO_STREAM_VIDEO && frameType == VIDEO_FRM_I)
		{
			m_QueueDataLost = FALSE;
		}else
		{
			skipData = TRUE;
		}
	}
	
	if (m_QueueRawLost == TRUE)
	{
		if (streamType == VIDEO_STREAM_VIDEO && frameType == VIDEO_FRM_I)
		{
			m_QueueRawLost = FALSE;
		}else
		{
			skipRaw = TRUE;
		}
	}
	/* get the  */
	if (skipData != TRUE && m_pThreadData)
	{
		if(m_Queue4DataFree.NonBlockingPeek() == true)
		{
			VideoFrame * pFrame = m_Queue4DataFree.Pop();
			if (pFrame)
			{
				if (pFrame->bufLen < (dataSize + sizeof(InfoFrameI) + padding))
				{
					free(pFrame->dataBuf);
					pFrame->dataBuf = (u8 *)malloc(dataSize + sizeof(InfoFrameI) + padding);
				}
				if (frameType == VIDEO_FRM_I)
				{
					memcpy(pFrame->dataBuf + sizeof(InfoFrameI) + padding, 
								pdata, dataSize);
					if (padding > 0)
					{
						memcpy(pFrame->dataBuf + sizeof(InfoFrameI), pad, 
							padding);
					}
					InfoFrameI *pI = (InfoFrameI *)pFrame->dataBuf;
					/* Init I frame header here */
					pI->audio = pAVInfo.audio.codec;
					pI->video = pAVInfo.video.codec;
					pI->aSampleRate = pAVInfo.audio.audio.sampleRate;
					pI->aSampleSize = pAVInfo.audio.audio.sampleSize;
					pI->aChannels = pAVInfo.audio.audio.channel;
					pI->vWidth = pAVInfo.audio.video.width;
					pI->vHeight = pAVInfo.audio.video.height;
					pI->vFps = pAVInfo.audio.video.fps;
					pFrame->dataLen = dataSize + sizeof(InfoFrameI);
				}else
				{
					memcpy(pFrame->dataBuf + sizeof(InfoFrameP), 
								pdata, dataSize);
					InfoFrameP *pP = (InfoFrameP *)pFrame->dataBuf;
					/* Init P frame header here */
					pP->audio = pAVInfo.audio.codec;
					pP->video = pAVInfo.video.codec;
					pFrame->dataLen = dataSize + sizeof(InfoFrameP);
				}
				/* Init frame header here */
				pFrame->secs = sec;
				pFrame->msecs = msec;
				pFrame->streamType = streamType;
				pFrame->frameType = frameType;
				m_Queue4Data.Push(pFrame);
				
			}else
			{
				RRLOG(RRLOG_ERROR, "[%s] data buff free none \n", __FUNCTION__);
			}
		}else
		{
			RRLOG(RRLOG_ERROR, "[%s] data buff free none \n", __FUNCTION__);
			m_QueueDataLost = TRUE;
		}
	}

	if (skipRaw != TRUE && m_pThreadRaw)
	{
		if(m_Queue4RawFree.NonBlockingPeek() == true)
		{
			VideoFrame * pFrame = m_Queue4RawFree.Pop();
			if (pFrame)
			{
				if (pFrame->bufLen < dataSize + sizeof(InfoFrameI) + padding)
				{
					free(pFrame->dataBuf);
					pFrame->dataBuf = (u8 *)malloc(dataSize + sizeof(InfoFrameI) + padding);
				}
				if (frameType == VIDEO_FRM_I)
				{
					memcpy(pFrame->dataBuf + sizeof(InfoFrameI) + padding, 
								pdata, dataSize);
					if (padding > 0)
					{
						memcpy(pFrame->dataBuf + sizeof(InfoFrameI), pad, 
							padding);
					}
					InfoFrameI *pI = (InfoFrameI *)pFrame->dataBuf;
					/* Init I frame header here */
					pI->audio = pAVInfo.audio.codec;
					pI->video = pAVInfo.video.codec;
					pI->aSampleRate = pAVInfo.audio.audio.sampleRate;
					pI->aSampleSize = pAVInfo.audio.audio.sampleSize;
					pI->aChannels = pAVInfo.audio.audio.channel;
					pI->vWidth = pAVInfo.audio.video.width;
					pI->vHeight = pAVInfo.audio.video.height;
					pI->vFps = pAVInfo.audio.video.fps;
					pFrame->dataLen = dataSize + sizeof(InfoFrameI);
				}else
				{
					memcpy(pFrame->dataBuf + sizeof(InfoFrameP), 
								pdata, dataSize);
					InfoFrameP *pP = (InfoFrameP *)pFrame->dataBuf;
					/* Init P frame header here */
					pP->audio = pAVInfo.audio.codec;
					pP->video = pAVInfo.video.codec;
					pFrame->dataLen = dataSize + sizeof(InfoFrameP);
				}
				/* Init frame header here */
				pFrame->secs = sec;
				pFrame->msecs = msec;
				pFrame->streamType = streamType;
				pFrame->frameType = frameType;
				m_Queue4Raw.Push(pFrame);
				
			}else
			{
				RRLOG(RRLOG_ERROR, "[%s] data buff free none \n", __FUNCTION__);
			}
		}else
		{
			RRLOG(RRLOG_ERROR, "[%s] data buff free none \n", __FUNCTION__);
			m_QueueRawLost = TRUE;
		}
	}
	
	
}

void RapidMediaData::RapidRTSPEventHandle1(int eventType, 
				int lParam, void *rParam)
{

}

BOOL RapidMediaData::Init(BOOL bRealStream, string strUrl, string strUser, 
	string strPass, BOOL bHWAccel, VSCConnectType connectType)
{
	m_bRealStream = bRealStream;

	m_strUrl = strUrl;
	m_strUser = strUser;
	m_strPass=strPass;
	m_bHWAccel = bHWAccel;

	m_connectType = connectType;
	
	if (bRealStream == FALSE)
	{
		return TRUE;
	}

	return TRUE;
}

void	RapidMediaData::SetDataHandler(RMDataHandler handle, void *pContext)
{
	Lock();
	//set the handler to the callback thread
	m_dataHandler = handle;
	m_pDataContext = pContext;
	UnLock();
}
void	RapidMediaData::SetRawDataHandler(RMRawVideoHandler handle, void *pContext)
{
	//set the handler to the decode thread
	Lock();
	m_rawVideoHandler = handle;
	m_pRawVideoContext = pContext;
	UnLock();
}

BOOL RapidMediaData::Start()
{
	Lock();
	if (m_nAttached == 0)
	{
		StartDataHandlerThread();
	}
	if (m_bRealStream == TRUE)
	{
		if (m_nAttachedRTSP == 0)
		{
			StartRapidRTSP();
		}
		m_nAttachedRTSP ++;
	}
	
	m_nAttached ++;
	UnLock();
	return TRUE;
}
BOOL RapidMediaData::Stop()
{
	Lock();
	m_nAttached --;

	if (m_nAttached < 0)
	{
		m_nAttached = 0;
	}
	
	if (m_nAttached == 0)
	{
		StopDataHandlerThread();
	}
	if (m_bRealStream == TRUE)
	{
		m_nAttachedRTSP --;
		if (m_nAttachedRTSP < 0)
		{
			m_nAttachedRTSP = 0;
		}
		if (m_nAttachedRTSP == 0)
		{
			StopRapidRTSP();
		}
	}
	UnLock();
	return TRUE;
}

BOOL RapidMediaData::StartRaw()
{
	Lock();
	if (m_nAttachedRaw == 0)
	{
		StartRawHandlerThread();
	}

	if (m_bRealStream == TRUE)
	{
		if (m_nAttachedRTSP == 0)
		{
			StartRapidRTSP();
		}
		m_nAttachedRTSP ++;
	}

	m_nAttachedRaw ++;
	UnLock();

	return TRUE;
}
BOOL RapidMediaData::StopRaw()
{
	Lock();
	m_nAttachedRaw --;
	if (m_nAttachedRaw < 0)
	{
		m_nAttachedRaw = 0;
	}
	
	if (m_nAttachedRaw == 0)
	{
		StopRawHandlerThread();
	}
	if (m_bRealStream == TRUE)
	{
		m_nAttachedRTSP --;
		if (m_nAttachedRTSP < 0)
		{
			m_nAttachedRTSP = 0;
		}
		if (m_nAttachedRTSP == 0)
		{
			StopRapidRTSP();
		}
	}
	UnLock();
	return TRUE;
}

BOOL RapidMediaData::StartRapidRTSP()
{
	if (m_bRealStream == FALSE || m_pRTSP != NULL)
	{
		return TRUE;
	}
	
	m_pRTSP = new CRapidRTSPLive555(m_strUrl, (int)m_connectType, 
					m_strUser, m_strPass, true);
	m_pRTSP->set_data_handle(RapidMediaData::RapidRTSPDataHandle, 
			(void *)this);

	m_pRTSP->start();
	
	return TRUE;
}
BOOL RapidMediaData::StopRapidRTSP()
{
	if (m_pRTSP == NULL)
	{
		return TRUE;
	}

	delete m_pRTSP;

	m_pRTSP = NULL;

	return TRUE;
}

void RapidMediaData::RunData(void * pParam)
{
	int dummy = errno;
	RapidMediaData * pThread = (RapidMediaData*)pParam;

	if (pThread)
	{
	    pThread->RunData1();
	}
}
void RapidMediaData::RunData1()
{
	int emptyCnt = 1;
	/* Loop to peek the data list */
	while (m_bExitData != TRUE)
	{
		m_Queue4Data.BlockingPeek();

		VideoFrame * pFrame = m_Queue4Data.Pop();
		/* this is the end of the thread */
		if (pFrame->streamType == VIDEO_STREAM_END)
		{
			RRLOG(RRLOG_INFO, "%s Got a End Frame", __FUNCTION__);
			m_Queue4DataFree.Push(pFrame);
			break;
		}
#ifdef RAPID_PERF_DEBUG
		RRLOG(RRLOG_INFO, "%s Got a New Frame", __FUNCTION__);
#endif
		if (m_dataHandler)
		{
			m_dataHandler(m_pDataContext, *pFrame);
		}
		/* send to free list */
		m_Queue4DataFree.Push(pFrame);
	}
}
void RapidMediaData::RunRaw(void * pParam)
{
	int dummy = errno;
	RapidMediaData * pThread = (RapidMediaData*)pParam;

	if (pThread)
	{
	    pThread->RunRaw1();
	}
}
void RapidMediaData::RunRaw1()
{
	int emptyCnt = 1;

	RapidDecSink decSink(m_rawVideoHandler, m_pRawVideoContext, m_bHWAccel);
	/* Loop to peek the data list */
	while (m_bExitRaw != TRUE)
	{
		m_Queue4Raw.BlockingPeek();

		VideoFrame * pFrame = m_Queue4Raw.Pop();
		/* this is the end of the thread */
		if (pFrame->streamType == VIDEO_STREAM_END)
		{
			RRLOG(RRLOG_INFO, "%s Got a End Frame", __FUNCTION__);
			m_Queue4RawFree.Push(pFrame);
			break;
		}
#ifdef RAPID_PERF_DEBUG
		RRLOG(RRLOG_INFO, "%s Got a send to  Decoder", __FUNCTION__);
#endif
		/* send to the decoder  */
		decSink.DecodeFrame(*pFrame);
		
		/* send to free list */
		m_Queue4RawFree.Push(pFrame);
	}
}

BOOL RapidMediaData::PutData(VideoFrame& packet)
{
	if (m_pThreadRaw)
	{
		if(m_Queue4RawFree.BlockingPeek() == true)
		{
			VideoFrame * pFrame = m_Queue4RawFree.Pop();
			if (pFrame)
			{
				if (pFrame->bufLen < packet.dataLen)
				{
					free(pFrame->dataBuf);
					pFrame->dataBuf = (u8 *)malloc(packet.dataLen);
				}

				memcpy(pFrame->dataBuf, packet.dataBuf, packet.dataLen);
				
				pFrame->dataLen = packet.dataLen;
				/* Init frame header here */
				pFrame->secs = packet.secs;
				pFrame->msecs = packet.msecs;
				pFrame->streamType = packet.streamType;
				pFrame->frameType = packet.frameType;
				m_Queue4Raw.Push(pFrame);
				
			}else
			{
				RRLOG(RRLOG_ERROR, "[%s] data buff free none \n", __FUNCTION__);
			}
		}else
		{
			RRLOG(RRLOG_ERROR, "[%s] data buff free none \n", __FUNCTION__);
		}
	}
	
	return TRUE;
}


