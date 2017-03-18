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
#include "vscpbexport.h"
#include "pbwrapper.hpp"

 static std::string VSCPbExportTaskTime2String(time_t nTime) 
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

VSCPbExportTask::VSCPbExportTask(astring strExportPath, astring strCamName, 
	VidStor &pStor, astring strId, u32 nPlaytime, 
	u32 nEndTime)
	:m_StorStream(new StorStream(pStor, strId, 1, true, nPlaytime)), m_pStor(pStor), 
	m_strId(strId), m_strExportPath(strExportPath), m_strCamName(strCamName), m_nPlaytime(nPlaytime), m_nEndTime(nEndTime), 
	m_isEol(FALSE), m_pMuxer(NULL), m_bQuit(false)
{
	m_StorStream->RegDataCallback(VSCPbExportTask::PBCallbackHandler, this);
	m_qStartTime.setTime_t(nPlaytime);
	m_qEndTime.setTime_t(nEndTime);
	m_currentTime = nPlaytime;


	astring strFileName =m_strCamName + " " + VSCPbExportTaskTime2String(nPlaytime) + 
						"-" + VSCPbExportTaskTime2String(nEndTime) + ".mp4";

	Poco::Path path(strExportPath);
	path.append("video");
	Poco::File file1(path);
	file1.createDirectories();
	
	path.append(strFileName);

	m_fileName = path.toString();

	m_options = get_fast_h264_decoder_options();

	GetTaskName();

	start();
}

VSCPbExportTask::~VSCPbExportTask()
{

}

void VSCPbExportTask::PBCallbackHandler(VideoFrame& frame,  void * pParam)
{
    int dummy = errno;
    VSCPbExportTask* pThread = (VSCPbExportTask*)pParam;

    if (pThread)
    {
        return pThread->PBCallbackHandler1(frame);
    }
}

void VSCPbExportTask::PBCallbackHandler1(VideoFrame& frame)
{
	u8  *dataBuf = NULL;
	u32 dataSize = 0;

	//XGuard guard(m_cMutex);
	
	/* write frame to file */
	//VDC_DEBUG("VSCPbExportTask Size %d stream %d frame %d (%d, %d)\n", frame.dataLen,      
	//	 	frame.streamType, frame.frameType, frame.secs, frame.msecs);
	m_currentTime = frame.secs;
	VDC_DEBUG("VSCPbExportTask Time %d \n", m_currentTime);
	if (m_currentTime > m_nEndTime)
	{
		m_bQuit = true;
		m_isEol = TRUE;
		return;
	}
	if (m_bQuit == true || m_isEol == true)
	{
		return;
	}

	if (frame.frameType == VIDEO_FRM_I)
	{
		
		dataBuf = frame.dataBuf + sizeof(InfoFrameI);
		dataSize = frame.dataLen - sizeof(InfoFrameI);
		InfoFrameI *pI = (InfoFrameI *)frame.dataBuf;
		/* Cache I frame for audio decoder */
		memcpy(&m_infoData, pI, sizeof(InfoFrameI));
		m_bGotInfoData = TRUE;
	}

	if (m_bGotInfoData != TRUE 
		|| m_infoData.video != CODEC_H264
		|| frame.streamType != VIDEO_STREAM_VIDEO)
	{
		//VDC_DEBUG("The Frame is not H264 or not Video data\n");
		return;
	}

	if (frame.frameType == VIDEO_FRM_P)
	{
			dataBuf = frame.dataBuf + sizeof(InfoFrameP);
			dataSize = frame.dataLen - sizeof(InfoFrameP);
	}

	/* This is first time */
	if (m_pMuxer == NULL)
	{
		if (m_bGotInfoData == FALSE)
		{
			VDC_DEBUG("No InfoData\n");
			return ;
		}
		if (m_infoData.vWidth == 0 || m_infoData.vHeight == 0)
		{
			m_infoData.vWidth = 640;
			m_infoData.vHeight = 480;
		}

		m_options.width = m_infoData.vWidth;
		m_options.height = m_infoData.vHeight;

		m_options.bit_rate = 4000000;
		m_options.time_base_den = 25;
		m_options.time_base_num = 1;
		m_options.gop_size = 50;
		m_pMuxer = new av_muxer(m_options, m_fileName, av_muxer::OUTPUT_LOCATION_FILE);
	}

	if (m_pMuxer == NULL)
	{
		VDC_DEBUG("m_pMuxer is NULL\n");
		return ;
	}

	/* Skip info frame */
	if (frame.streamType == VIDEO_STREAM_INFO)
	{
		return;
	}

	bool keyFrame = false;
	if (frame.frameType == VIDEO_FRM_I)
	{
		keyFrame  = true;
	}

	av_packet input(dataBuf, dataSize, false);
	m_pMuxer->write_video_packet(&input, keyFrame);

}

void VSCPbExportTask::run()
{
	/* start export */
	m_StorStream->StartStorStream();

	while(m_bQuit == false)
	{
		ve_sleep(400);
	}

	m_StorStream->UnRegDataCallback();

	m_StorStream->StopStorStream();
	
	if (m_pMuxer)
	{
		m_pMuxer->finalize_file();
	}
	
	m_StorStream = NULL;
	if (m_pMuxer)
	{
		delete m_pMuxer;
		m_pMuxer = NULL;
	}
}


int VSCPbExportTask::GetPercentage()
{
	int percent = ((m_currentTime - m_nPlaytime) * 100 ) / (m_nEndTime - m_nPlaytime);
	if (percent <= 0)
	{
		percent = 0;
	}
	if (m_isEol == true)
	{
		percent = 100;
	}
	return percent;
}
bool VSCPbExportTask::HasOpenResult()
{
	return TRUE;
}
bool VSCPbExportTask::OpenResult()
{
	return false;
}
bool VSCPbExportTask::StopTask()
{
	m_bQuit = true;
	return TRUE;
}

bool VSCPbExportTask::IsEol()
{
	return m_isEol;
}



std::string VSCPbExportTask::GetTaskName()
{
	QString strName;

	QString strDeviceName;
	strDeviceName = m_strCamName.c_str();
	strName = tr("Record export  ") + strDeviceName + "  ( " + m_qStartTime.toString(Qt::ISODate) + tr(" to ")
		+ m_qEndTime.toString(Qt::ISODate) + " )";
	return strName.toStdString();
}
