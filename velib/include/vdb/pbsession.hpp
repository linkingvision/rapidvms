#ifndef __PB_SESSION_HPP__
#define __PB_SESSION_HPP__
#include "utility.hpp"
#include "mediafile.hpp"

class VE_LIBRARY_API PlaybackSession
{
public:
    PlaybackSession(s32 deviceId, astring &strBlockPath);
    ~PlaybackSession();
public:
	MFStatus GetAFrame(VideoFrame &pFrame, s32 &waiting);
	BOOL SeekToTime(u32 seekTime);
	
private:
    s32 m_DeviceId;
    astring m_StrBlockPath;
    u32 m_startTime;
    MediaFile m_MediaFile;
};

#endif /* __RECORD_SESSION_HPP__ */
