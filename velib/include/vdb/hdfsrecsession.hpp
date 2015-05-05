#ifndef __HDFS_RECORD_SESSION_HPP__
#define __HDFS_RECORD_SESSION_HPP__
#include "utility.hpp"

#include "Poco/Buffer.h"

typedef Poco::Buffer<u8> VEBuffer;

using namespace std;

typedef struct __VideoSeqFrame
{
	VideoSeqType seqType;
 	u32 start;/*Start time of the video*/
	u32 end;/*End time of the video*/
	VEBuffer  * dataBuf;
}VideoSeqFrame;

typedef BOOL (*HDFSDataHandler)(void* pData, VideoSeqFrame& frame);

class VHdfsDB;
class HdfsRecSessionData;
class VE_LIBRARY_API HdfsRecSession
{
public:
	HdfsRecSession(s32 deviceId, astring strName, VHdfsDB &pDB);
	~HdfsRecSession();
public:
	MFStatus PushAFrame(VideoFrame *pFrame);

	BOOL RegSeqCallback(HDFSDataHandler pCallback, void * pParam);
	BOOL UnRegSeqCallback(void * pParam);
private:
    	HdfsRecSessionData * m_data;
};

#endif /* __HDFS_RECORD_SESSION_HPP__ */
