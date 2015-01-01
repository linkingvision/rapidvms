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
class VHdfsDBData;
class HdfsRecWrapper;
class VE_LIBRARY_API VHdfsDB
{
public:
	typedef std::map<s32, HdfsRecSession*> _MapSession;
public:

	VHdfsDB(astring &pNameNode, astring &pPort, 
		astring &pUser);
	~VHdfsDB();
public:
	void Lock();
	void UnLock();
public:
	BOOL Config(astring &pNameNode, astring &pPort, 
		astring &pUser);
public:
	/* Start and Stop Record */
	HdfsRecSession * StartRecord(s32 deviceId, astring strName);
	BOOL FinishRecord(HdfsRecSession *pSess);
	MFStatus PushAFrame(s32 deviceId, VideoFrame *pFrame);
public:
	HdfsRecWrapper & GetHdfsRecWrapper();
	BOOL ReleaseHdfsRecWrapper();

private:
    	fast_mutex m_Lock;
	_MapSession m_MapSess;
    	VHdfsDBData * m_data;
	astring m_pNameNode;
	astring m_pPort; 
	astring m_pUser;
};
#endif /* __V_HDFS_DB_HPP__ */
