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
#ifndef __RAPID_AUDIO_DEC_HPP__
#define __RAPID_AUDIO_DEC_HPP__

#include "ffkit/ffoptions.h"
#include "ffkit/av_demuxer.h"
#include "ffkit/av_packet.h"
#include "ffkit/av_packet_factory.h"
#include "cppkit/ck_types.h"
#include "cppkit/ck_memory.h"
#include "rapidmedia/rapiddec.hpp"
#include "utility/videotype.hpp"

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

using namespace ffkit;
using namespace cppkit;
using namespace std;

const int AUDIO_DECODE_ATTEMPTS = 1;

class RapidAudioDec :public RapidDec
{
public:
	RapidAudioDec(CodecType codec,  int bit_rate, int sample_rate, 
		int channels, const struct codec_options& options, int decodeAttempts = AUDIO_DECODE_ATTEMPTS );

	virtual ~RapidAudioDec() ;

	BOOL Decode( uint8_t* pBuf, int nSize, RawFrame & pFrame);

private:
	RapidAudioDec( const RapidAudioDec& obj );
	RapidAudioDec& operator = ( const RapidAudioDec& );

	CodecType m_codecType;
	enum AVCodecID m_id;
	AVCodec* m_codec;
	AVCodecContext* m_context;
	struct codec_options m_options;
	AVFrame* m_frame;
	int m_decodeAttempts;
};

#endif /* __RAPID_Audio_DEC_HPP__ */
