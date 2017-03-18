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
#ifndef __MEDIA_FILE_HPP__
#define __MEDIA_FILE_HPP__
#include "utility.hpp"
#include "vdbfile.hpp"
#include "utility/videotype.hpp"

typedef enum
{
    MEDIA_SESSION_RECORD = 1,
    MEDIA_SESSION_PLAYBACK,
    MEDIA_SESSION_LAST
} MediaSessionType;

using namespace UtilityLib;

class MediaFileData;
class VE_LIBRARY_API MediaFile
{
public:
	MediaFile(astring & strPath, u32 startTime, MediaSessionType type, 
					u32 maxLength, BOOL bSeekIFrame = FALSE);
	~MediaFile();
	
public:
	MFStatus PushAFrame(VideoFrame *pFrame);
	MFStatus GetAFrame(VideoFrame &pFrame, s32 &waiting);
	BOOL PauseForRead();
	BOOL ReadSeek(u32 startTime);
	BOOL ReadSeekByType(u32 startTime, s32 type, BOOL playMode);

	BOOL ReadFrame(void * pkt, u32 len, u32 time);
	BOOL ReadFrameNext(void * pkt, u32 len, u32 time, s32 type);
	BOOL ReadFramePrev(void * pkt, u32 len, u32 time, s32 type);
	BOOL SeekToTime(u32 seekTime);
	BOOL SeekToNextIFrame();
	BOOL SeekToPreIFrame();
	BOOL GetStartAndEndTime(u32  &startTime, u32 &endTime);
	u32 GetEndTime();
private:
	BOOL Init();
	BOOL InitWrite();
	BOOL InitRead();
	
private:
	MediaFileData * m_data;
};

#endif /* __MEDIA_FILE_HPP__ */
