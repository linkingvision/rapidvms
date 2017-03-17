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
#ifndef __RAPID_DEC_SINK_HPP__
#define __RAPID_DEC_SINK_HPP__

#include "rapidmedia/rapidmedia.hpp"
#include "rapidmedia/rapidmediadata.hpp"
#include "cppkit/ck_memory.h"
#include "rapidrtsp.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XBlockingQueue.h"
#include "rapidmedia/rapidaudiodec.hpp"
#include "rapidmedia/rapidffmpegdec.hpp"

#include "ffkit/fflocky.h"

using namespace ffkit;

using namespace cppkit;
using namespace XSDK;


class RapidDecSink
{
public:
	RapidDecSink(RMRawVideoHandler rawVideoHandler, 
		void *pRawVideoContext, BOOL HWAccel);
	~RapidDecSink() ;

public:
	BOOL DecodeFrame(VideoFrame & pFrame);
	BOOL DecodeAFrame(VideoFrame & pFrame);
	BOOL DecodeVFrame(VideoFrame & pFrame);
private:
	RMRawVideoHandler m_rawVideoHandler;
	void *m_pRawVideoContext;

	RapidDec *m_pVideoDec;
	RapidAudioDec *m_pAudioDec;
	CodecType m_AudioCodec;
	CodecType m_VideoCodec;
	BOOL m_HWAccel;

	InfoFrameI m_FrameI;
	BOOL m_bGotFrameI;
};

#endif /* __RAPID_DEC_SINK_HPP__ */
