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
#include "vsmotalgo.hpp"
#include "render_wrapper.hpp"
#include "ffkit/av_muxer.h"
#include "ffkit/av_demuxer.h"
#include "ffkit/h264_decoder.h"
#include "ffkit/fflocky.h"
#include "ffkit/ffoptions.h"
#include "cppkit/ck_memory.h"
#include "cppkit/os/ck_large_files.h"
#include "rapidmedia/rapidmedia.hpp"
#include "curl/curl.h"
#include "libyuv.h"
#include "XSDK/TimeUtils.h"

using namespace UtilityLib;
using namespace std;
using namespace cppkit;
using namespace ffkit;
using namespace XSDK;

class VE_LIBRARY_API VSMotAlgoData
{
public:

	VSMotAlgoData(bool bFullFrame);
	~VSMotAlgoData();
public:
	bool ProcessFrame(RawFrame& frame);
	bool GetOutput(VBlobVec &pOutput, s32 &w, s32 &h);
	bool SetMotReg(VVidMotReg &motReg);

private:
	s32 m_ProcWidth;
	s32 m_ProcHeight;
	bool m_bFullFrame;
	char * m_buf;
	u8 * m_mot;
	struct timeval m_lastTime;
	VVidMotReg m_motReg;

};

/*  Default size is 320 * 240 and the blob size is 8x8*/

VSMotAlgoData::VSMotAlgoData(bool bFullFrame)
:m_bFullFrame(bFullFrame),m_ProcWidth(VVID_MOT_PROC_W), m_ProcHeight(VVID_MOT_PROC_H), 
m_buf(NULL)
{
	m_buf = (char *)malloc(m_ProcWidth * m_ProcHeight * 3);
	m_mot = (u8 *)malloc(m_ProcWidth * m_ProcHeight);
	m_lastTime.tv_sec = 0;
	m_lastTime.tv_usec = 0;
}
VSMotAlgoData::~VSMotAlgoData()
{
	delete m_buf;
	delete m_mot;
}

bool VSMotAlgoData::SetMotReg(VVidMotReg &motReg)
{
	m_motReg = motReg;
	return true;
}

bool VSMotAlgoData::ProcessFrame(RawFrame& frame)
{
	if (frame.format != AV_PIX_FMT_YUV420P)
	{
		return false;
	}

	struct timeval curr;
	gettimeofday(&curr, NULL);
	long result;
	timerdiff(&curr, &m_lastTime, result);
	if (m_bFullFrame != true)
	{
		/* 100ms 10fps */
		if (result > 0 && result < 100)
		{
			return true;
		}else
		{
			timercopy(&m_lastTime, &curr);
		}
	}
	
	I420Scale((unsigned char *)frame.data[0], frame.linesize[0], 
			(unsigned char *)frame.data[2], frame.linesize[2], 
			(unsigned char *)frame.data[1], frame.linesize[1],
				frame.width, frame.height, 
				(unsigned char *)m_buf, m_ProcWidth, 
				(unsigned char *)m_buf + m_ProcWidth * m_ProcHeight, 
				m_ProcWidth /2, 
				(unsigned char *)m_buf + (m_ProcWidth * m_ProcHeight * 5) /4 , 
				m_ProcWidth /2,
				m_ProcWidth, m_ProcHeight, kFilterNone);	

	//TODO add opencv for motion detect
	
	return true;
	
}

bool VSMotAlgoData::GetOutput(VBlobVec &pOutput, s32 &w, s32 &h)
{
	w = m_ProcWidth;
	h = m_ProcHeight;
	//TODO
	return true;
}

VSMotAlgo::VSMotAlgo(bool bFullFrame)
{
	m_data = new VSMotAlgoData(bFullFrame);
}
VSMotAlgo::~VSMotAlgo()
{
	if (m_data)
	{
		delete m_data;
	}
}
bool VSMotAlgo::ProcessFrame(RawFrame& frame)
{
	return m_data->ProcessFrame(frame);
	
}

bool VSMotAlgo::SetMotReg(VVidMotReg &motReg)
{
	return m_data->SetMotReg(motReg);
}

bool VSMotAlgo::GetOutput(VBlobVec &pOutput, s32 &w, s32 &h)
{
	return m_data->GetOutput(pOutput, w, h);
}

