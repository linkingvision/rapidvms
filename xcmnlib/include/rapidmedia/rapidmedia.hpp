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
#ifndef __V_RAPID_MEDIA_HPP__
#define __V_RAPID_MEDIA_HPP__
#include "cppkit/os/ck_exports.h"
#include "utility/type.hpp"
#include "utility/videotype.hpp"

using namespace std;

class RapidMediaData;
class VE_LIBRARY_API RapidMedia
{
public:
	RapidMedia();
	~RapidMedia();
public:
	/* The Decode one i frame  */
	static bool DecodeiFrame(VideoFrame& packet, RawFrame& pRaw, void* &pDecoder);
	static bool DecodeiFrameFreeDec(void* pDec);
public:
	BOOL Init(BOOL bRealStream, string strUrl, string strUser, 
				string strPass, BOOL bHWAccel = FALSE, VSCConnectType connectType = VSC_CONNECT_TCP);
	BOOL PutData(VideoFrame& packet);
	void	SetDataHandler(RMDataHandler handle, void *pContext);
	void	SetRawDataHandler(RMRawVideoHandler handle, void *pContext);

	BOOL Start();
	BOOL Stop();
	BOOL StartRaw();
	BOOL StopRaw();
public:
	static std::string GetCodecName(CodecType codec);

public:
	RapidMediaData * m_data;
};
#endif /* __V_RAPID_MEDIA_HPP__ */
