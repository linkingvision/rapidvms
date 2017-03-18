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
#include "pbsession.hpp"
#include "videotype.hpp"

PlaybackSession::PlaybackSession(astring deviceId, astring &strBlockPath,  BOOL bSeekIFrame)
: m_DeviceId(deviceId),m_StrBlockPath(strBlockPath), 
m_MediaFile(strBlockPath, 0, MEDIA_SESSION_PLAYBACK, MF_FILE_MAX_LENGTH, bSeekIFrame)
{

}

PlaybackSession::~PlaybackSession()
{

}

/* waiting is in ms */
MFStatus PlaybackSession::GetAFrame(VideoFrame &pFrame, s32 &waiting)
{
	return m_MediaFile.GetAFrame(pFrame, waiting);
}

BOOL PlaybackSession::SeekToTime(u32 seekTime)
{
	return m_MediaFile.SeekToTime(seekTime);
}

BOOL PlaybackSession::Pause()
{
	return m_MediaFile.PauseForRead();
}