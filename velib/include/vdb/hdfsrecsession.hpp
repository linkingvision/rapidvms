#ifndef __HDFS_RECORD_SESSION_HPP__
#define __HDFS_RECORD_SESSION_HPP__
#include "utility.hpp"


using namespace std;

class VHdfsDB;
class HdfsRecSessionData;
class VE_LIBRARY_API HdfsRecSession
{
public:
	HdfsRecSession(s32 deviceId, astring strName, VHdfsDB &pDB);
	~HdfsRecSession();
public:
	MFStatus PushAFrame(VideoFrame *pFrame);
private:
    	HdfsRecSessionData * m_data;
};

#endif /* __HDFS_RECORD_SESSION_HPP__ */
