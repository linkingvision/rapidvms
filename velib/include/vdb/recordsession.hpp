#ifndef __RECORD_SESSION_HPP__
#define __RECORD_SESSION_HPP__
#include "utility.hpp"
#include "mediafile.hpp"

class VE_LIBRARY_API RecordSession
{
public:
    RecordSession(s32 deviceId, astring &strBlockPath, u32 startTime, s64 recordId);
    ~RecordSession();
public:
	MFStatus PushAFrame(VideoFrame *pFrame);
	u32 GetEndTime();
	s64 GetRecordId();
	astring & GetBlockPath();

private:
    s32 m_DeviceId;
    astring m_StrBlockPath;
    u32 m_startTime;
    MediaFile m_MediaFile;
    s64 m_RecordId;
};

#endif /* __RECORD_SESSION_HPP__ */
