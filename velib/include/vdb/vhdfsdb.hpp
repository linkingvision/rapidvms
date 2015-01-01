#ifndef __V_HDFS_DB_HPP__
#define __V_HDFS_DB_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"

#include "cppkit/ck_memory.h"
#include "cppkit/ck_command_queue.h"
#include "avkit/av_muxer.h"

using namespace std;
using namespace cppkit;
using namespace avkit;

using namespace UtilityLib;

class HdfsRecSession;
class VE_LIBRARY_API VHdfsDB
{
public:
	typedef std::map<s32, HdfsRecSession*> _MapSession;
public:

	VHdfsDB(astring & strPath);
	~VHdfsDB();
public:
	void Lock();
	void UnLock();

public:
	/* Start and Stop Record */
	BOOL StartRecord(s32 deviceId);
	BOOL FinishRecord(s32 deviceId);
	MFStatus PushAFrame(s32 deviceId, VideoFrame *pFrame);

private:
    	fast_mutex m_Lock;
	_MapSession m_MapSess;
};
#endif /* __V_HDFS_DB_HPP__ */
