#ifndef __HDFS_RECORD_SESSION_HPP__
#define __HDFS_RECORD_SESSION_HPP__
#include "utility.hpp"

#include "cppkit/ck_memory.h"
#include "cppkit/ck_command_queue.h"
#include "avkit/av_muxer.h"

using namespace std;
using namespace cppkit;
using namespace avkit;

class VHdfsDB;
class HdfsRecSession
{
public:
    HdfsRecSession(s32 deviceId, astring strName, VHdfsDB &pDB);
    ~HdfsRecSession();
public:
	MFStatus PushAFrame(VideoFrame *pFrame);
private:
    s32 m_DeviceId;
	VHdfsDB &m_pDB;
};

#endif /* __HDFS_RECORD_SESSION_HPP__ */
