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
#include "vhdfsdb.hpp"
#include "hdfsrecsession.hpp"
#include "hdfswrapper.hpp"

#include "utility.hpp"
#include "debug.hpp"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_command_queue.h"
#include "ffkit/av_muxer.h"

#include "Poco/Path.h"

using namespace std;
using namespace cppkit;
using namespace ffkit;
using namespace Poco;


 std::string HdfsRecTime2String(time_t nTime) 
{
    time_t     now = nTime;
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M-%S", &tstruct);

    return buf;
}

 std::string HdfsRecTime2Minute(time_t nTime) 
{
    time_t     now = nTime;
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M", &tstruct);

    return buf;
}

  std::string HdfsRecTime2Hour(time_t nTime) 
{
    time_t     now = nTime;
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d-%H", &tstruct);

    return buf;
}

 std::string HdfsRecTime2Day(time_t nTime) 
{
    time_t     now = nTime;
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

    return buf;
}

class HdfsRecSessionData
{
public:
	HdfsRecSessionData(s32 deviceId, astring strName, VHdfsDB &pDB)
		:m_pDB(pDB), m_DeviceId(deviceId), m_strName(strName), m_bGotInfoData(FALSE), 
		m_pMuxer(NULL), m_currStartTime(0), m_pHdfsRec(pDB.GetHdfsRecWrapper()), 
		m_currFrameCnt(0), m_inerval(60), m_handler(NULL), 
		m_pParam(NULL)
	{
		m_options = get_fast_h264_decoder_options();
	}
	~HdfsRecSessionData()
	{
		FinishCurrRec(time(NULL));
	}
	MFStatus PushAFrame(VideoFrame *pFrame);
	BOOL StartNewRec(s32 currTime);
	BOOL FinishCurrRec(s32 currTime);
	BOOL HandleAFrame(VideoFrame *pFrame);
	BOOL RegSeqCallback(HDFSDataHandler pCallback, void * pParam);
	BOOL UnRegSeqCallback(void * pParam);

public:
	s32 m_DeviceId;
	VHdfsDB &m_pDB;
	astring m_strName;
	codec_options m_options;
	InfoFrame m_infoData;
	BOOL m_bGotInfoData;
	av_muxer *m_pMuxer;
	s32 m_currStartTime;
	HdfsRecWrapper &m_pHdfsRec;/* If a file is OK, post to here */
	s32 m_currFrameCnt;/* If > 15(min) * 60(s) * 60(fps) */
	s32 m_inerval;/* (s) */

	HDFSDataHandler m_handler;
	void * m_pParam;
};

BOOL HdfsRecSessionData::RegSeqCallback(HDFSDataHandler pCallback, 
				void * pParam)
{
	m_handler = pCallback;
	m_pParam = pParam;

	return TRUE;
}
BOOL HdfsRecSessionData::UnRegSeqCallback(void * pParam)
{
	m_handler = NULL;
	m_pParam = NULL;

	return TRUE;
}

/* Start a New record */
BOOL HdfsRecSessionData::StartNewRec(s32 currTime)
{
	ck_string fileName = "fake.mp4";
	m_options.width = m_infoData.vWidth;
	m_options.height = m_infoData.vHeight;
	m_options.bit_rate = 4000000;
	m_options.time_base_den = 25;
	m_options.time_base_num = 1;
	m_options.gop_size = 50;
	m_pMuxer = new av_muxer(m_options, fileName, av_muxer::OUTPUT_LOCATION_BUFFER);
	m_currStartTime = currTime;
	m_currFrameCnt = 0;
	m_inerval = m_pDB.GetInerval();
	if (m_inerval >= 60)
	{
		m_inerval = 60;
	}

	VDC_DEBUG("%s Start a New Rec Start Time:%d inerval %d\n", __FUNCTION__, 
		m_currStartTime, m_inerval);
	return TRUE;
}

/* Finish current record */
BOOL HdfsRecSessionData::FinishCurrRec(s32 currTime)
{
	VDC_DEBUG("%s Finish Rec Start Time:%d inerval %d current Time %d\n", __FUNCTION__, 
		m_currStartTime, m_inerval, currTime);
	VideoSeqFrame seqFrame;
	seqFrame.seqType = VIDEO_SEQ_MP4;
	
	if (m_pMuxer == NULL)
	{
		return FALSE;
	}
	ck_memory   *pData = new ck_memory();
	m_pMuxer->finalize_buffer(pData);
	if (pData->size() <= 0)
	{
		VDC_DEBUG("%s ck_memory is 0\n", __FUNCTION__);
		delete m_pMuxer;
		m_pMuxer = NULL;
		delete pData;
		pData = NULL;
		return FALSE;
	}

	seqFrame.start = m_currStartTime;
	seqFrame.end = currTime;

	Poco::Path path("video");
	path.append(m_strName);
	astring strChannel1 = ck_string::from_int(m_DeviceId);
	astring strChannel = "C" + strChannel1;
	//path.append(strChannel);
	astring strDay = HdfsRecTime2Day(m_currStartTime);
	path.append(strDay);
	
	astring strHour = HdfsRecTime2Hour(m_currStartTime);
	path.append(strHour);
	
	astring strMinute = HdfsRecTime2Minute(m_currStartTime);
	path.append(strMinute);
	
	astring strStart = HdfsRecTime2String(m_currStartTime);
	astring strEnd = HdfsRecTime2String(currTime);
	
	astring strFileName = strChannel + "-" + strStart + "-" +  strEnd + ".mp4";
	
	astring strPath = path.toString(Path::PATH_UNIX);
	VDC_DEBUG("%s path %s  file %s size %d\n", __FUNCTION__, strPath.c_str(), strFileName.c_str(), pData->size());
	HdfsCmd cmd;
	
	cmd.cmd = HDFS_CMD_FILE;
	strcpy(cmd.path, strPath.c_str());
	strcpy(cmd.file, strFileName.c_str());
	cmd.pData = pData;

	if (m_handler != NULL)
	{
		ck_byte_ptr ptr = cmd.pData->map();
		VEBuffer seqBuff((u8 *)(ptr.get_ptr()), ptr.length());

		seqFrame.dataBuf = &seqBuff;
		/* Callback the seq callback */
		m_handler(m_pParam, seqFrame);
	}
	m_pHdfsRec.Post(cmd);

	delete m_pMuxer;
	m_pMuxer = NULL;

	return TRUE;
}

/* Push the data  */
BOOL HdfsRecSessionData::HandleAFrame(VideoFrame *pFrame)
{
	u8  *dataBuf = NULL;
	u32 dataSize = 0;
	
	if (m_pMuxer == NULL)
	{
		return FALSE;
	}
	/* Skip info frame */
	if (pFrame->streamType == VIDEO_STREAM_INFO)
	{
		return TRUE;
	}
	m_currFrameCnt ++;
	bool keyFrame = false;

	if (pFrame->frameType == VIDEO_FRM_I)
	{
		
		dataBuf = pFrame->dataBuf + sizeof(InfoFrameI);
		dataSize = pFrame->dataLen - sizeof(InfoFrameI);
		keyFrame  = true;
	}
	else if (pFrame->frameType == VIDEO_FRM_P)
	{
		dataBuf = pFrame->dataBuf + sizeof(InfoFrameP);
		dataSize = pFrame->dataLen - sizeof(InfoFrameP);
	}else 
	{
		return TRUE;
	}

	av_packet input(dataBuf, dataSize, false);
	m_pMuxer->write_video_packet(&input, keyFrame);

	return TRUE;
}

MFStatus HdfsRecSessionData::PushAFrame(VideoFrame *pFrame)
{
	u8  *dataBuf = NULL;
	u32 dataSize = 0;
	s32 currTime = time(NULL);
	//VDC_DEBUG("HDFS Recording Size %d stream %d frame %d (%d, %d)\n", pFrame->dataLen,      
	//	pFrame->streamType, pFrame->frameType, pFrame->secs, pFrame->msecs);
	if (pFrame->frameType == VIDEO_FRM_I)
	{
		
		dataBuf = pFrame->dataBuf + sizeof(InfoFrameI);
		dataSize = pFrame->dataLen - sizeof(InfoFrameI);
		InfoFrameI *pI = (InfoFrameI *)pFrame->dataBuf;
		/* Cache I frame for audio decoder */
		memcpy(&m_infoData, pI, sizeof(InfoFrameI));
		m_bGotInfoData = TRUE;
	}

	if (m_bGotInfoData != TRUE 
		|| m_infoData.video != CODEC_H264
		|| pFrame->streamType != VIDEO_STREAM_VIDEO)
	{
		//VDC_DEBUG("The Frame is not H264 or not Video data\n");
		return MF_OK;
	}
	/* This is first time */
	if (m_currStartTime == 0 && m_pMuxer == NULL)
	{
		if (m_bGotInfoData == FALSE)
		{
			VDC_DEBUG("No InfoData\n");
			return MF_OK;
		}
		StartNewRec(currTime);
	}

	if (currTime < m_currStartTime 
		|| m_currFrameCnt > 15 * 60 * 60
		|| currTime >= (m_currStartTime + m_inerval))
	{
		/* I Frame split file */
		if (pFrame->frameType == VIDEO_FRM_I)
		{
			FinishCurrRec(currTime);
			StartNewRec(currTime);
		}
	}
	
	HandleAFrame(pFrame);
	
	return MF_OK;
}

HdfsRecSession::HdfsRecSession(s32 deviceId, astring strName, VHdfsDB &pDB)
: m_data(new HdfsRecSessionData(deviceId, strName, pDB))
{

}
HdfsRecSession::~HdfsRecSession()
{
	if (m_data)
	{
		delete m_data;
	}
}

BOOL HdfsRecSession::RegSeqCallback(HDFSDataHandler pCallback, 
		void * pParam)
{
	return m_data->RegSeqCallback(pCallback, pParam);
}
BOOL HdfsRecSession::UnRegSeqCallback(void * pParam)
{
	return m_data->UnRegSeqCallback(pParam);
}

MFStatus HdfsRecSession::PushAFrame(VideoFrame *pFrame)
{
	//VDC_DEBUG("HDFS Recording Size %d stream %d frame %d (%d, %d)\n", pFrame->dataLen,      
	//	pFrame->streamType, pFrame->frameType, pFrame->secs, pFrame->msecs);

	return m_data->PushAFrame(pFrame);
}


