#ifndef __VDB_HPP__
#define __VDB_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "utility/videotype.hpp"
#include "recordsession.hpp"
#include "pbsession.hpp"
#include "indexdb.hpp"

using namespace UtilityLib;

class VE_LIBRARY_API VDB
{
public:
	typedef std::map<s32, RecordSession*> _MapSession;
public:

	VDB(astring & strPath);
	~VDB();
public:
	BOOL SetRecordingMode(RecordingMode mode);

	void Lock();
	void UnLock();

	BOOL AddHdd(astring &strHdd, astring & strPath, s64 nSize);
	BOOL HddUpdateSize(astring &strHdd, s64 nSize);
	BOOL DelHdd(astring &strHdd);

	/* Disk map API */
	BOOL GetDiskMap(VDBDiskMap &pMap);
	BOOL GetDiskStatus(VDBDiskStatus &pStatus);
	BOOL UpdateDiskStatusMap(VDBDiskStatus &pMap);

	
public:
	/* Start and Stop Record */
	RecordSession * StartRecord(astring deviceId, u32 startTime, u32 recordType);
	BOOL FinishRecord(RecordSession * pRs);
		
public:
	/* video search function, if startTime or endTime is 0, it mean all */
	BOOL SearchItems(astring deviceId, u32 startTime, u32 endTime, u32 recordType, 
					RecordItemMap & pMap);
	BOOL SearchHasItems(astring deviceId, u32 startTime, u32 endTime, u32 recordType);
	BOOL SearchAItem(astring deviceId, u32 Time, VdbRecordItem &pItem);
	BOOL SearchAItemNear(astring deviceId, u32 Time, VdbRecordItem &pItem);
    	BOOL SearchNextItem(astring deviceId, s64 LastId, VdbRecordItem &pItem);
	BOOL RequestAMFRead(VdbRecordItem &pItem, astring & strPath);
	BOOL FinishedAMFRead(VdbRecordItem &pItem, astring & strPath);
	
	
public:
	/* Event log function, video record and event record has a link */

private:
    IndexDB m_IndexDB;
    RecordingMode m_Mode;
    fast_mutex m_Lock;
};
#endif /* __VDB_HPP__ */
