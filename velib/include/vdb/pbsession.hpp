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
#ifndef __PB_SESSION_HPP__
#define __PB_SESSION_HPP__
#include "utility.hpp"
#include "mediafile.hpp"

class VE_LIBRARY_API PlaybackSession
{
public:
    /* bSeekIFrame is for play a new block from seek method  */
    PlaybackSession(astring deviceId, astring &strBlockPath, BOOL bSeekIFrame = FALSE);
    ~PlaybackSession();
public:
	MFStatus GetAFrame(VideoFrame &pFrame, s32 &waiting);
	BOOL SeekToTime(u32 seekTime);
	BOOL Pause();
	astring GetBlockPath()
	{
		return m_StrBlockPath;
	}
	
private:
	astring m_DeviceId;
    astring m_StrBlockPath;
    u32 m_startTime;
    MediaFile m_MediaFile;
};

#endif /* __RECORD_SESSION_HPP__ */
