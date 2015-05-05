#ifndef __INDEX_DB_HPP__
#define __INDEX_DB_HPP__
#include "utility.hpp"
#include "Poco/Data/Session.h"
#include "utility/videotype.hpp"


using namespace UtilityLib;
/* The string is the device id of disk
    For example, on Unix filesystems (including OS X), this returns the
    devpath like \c /dev/sda0 for local storages. On Windows, it returns the UNC
    path starting with \c \\\\?\\ for local storages (in other words, the volume GUID).
*/

typedef std::map<astring, VdbDiskItem> VDBDiskMap;
typedef std::map<astring, VdbDiskStatus> VDBDiskStatus;

class VE_LIBRARY_API IndexDB
{
public:
    IndexDB(astring & strPath);
    ~IndexDB();
	
public:
	BOOL Init();
	BOOL UpdateDiskMap();
	BOOL CreateTablesIfNecessary();
	BOOL CreateBlocksIfNecessary(astring & strPath, s32 nSize);
	BOOL CreateABlock(astring & strHdd, astring &strBlockPath);
	BOOL DeInit();
	
public:
	//The size is in M bytes
	/* Disk control API*/
	BOOL AddHdd(astring &strHdd, astring & strPath, s64 nSize);
	BOOL HddUpdateSize(astring &strHdd, s64 nSize);
	BOOL DelHdd(astring &strHdd);

	/* Disk map API */
	BOOL GetDiskMap(VDBDiskMap &pMap);
	BOOL GetDiskStatus(VDBDiskStatus &pStatus);
	BOOL UpdateDiskStatusMap(VDBDiskStatus &pMap);

	/* Internal Disk function */
	BOOL InitDiskMap();
	BOOL  SelectADisk(astring &strHdd);
	
	BOOL RequestABlockFile(astring & strPath);
	BOOL FinishedABlockFile(astring & strPath);
	BOOL RequestABlockFileRead(VdbRecordItem &pItem, astring & strPath);
	BOOL FinishedABlockFileRead(VdbRecordItem &pItem, astring & strPath);
	/* Lock and Unlock for  */
	BOOL BlockFileLock(astring & strPath);
	BOOL BlockFileUnLock(astring & strPath);

       /* Get the old record */
	BOOL GetAOldRecord(astring & strPath);
	BOOL BlockFileRLock(astring & strPath);
	BOOL BlockFileRUnLock(astring & strPath);
	BOOL BlockFileWLock(astring & strPath);
	BOOL BlockFileWUnLock(astring & strPath);
	BOOL ClearAllBlockLock();
	BOOL CorrectAllTheRecord();
	
	
	/* The return is the ID of the record */
	s64 AddRecord(s32 deviceId, u32 recordType, u32 startTime, astring & strPathBlock);
	BOOL UpdateRecordEndtime(s64 recordId, u32 endTime);
	
	
	/* Search Interface */
	BOOL SearchAItem(s32 deviceId, u32 Time, VdbRecordItem &pItem);
	BOOL SearchAItemNear(s32 deviceId, u32 Time, VdbRecordItem &pItem);
	BOOL SearchNextItem(s32 deviceId, s64 LastId, VdbRecordItem &pItem);
	BOOL SearchItems(s32 deviceId, u32 startTime, u32 endTime, u32 recordType, 
				RecordItemMap & pMap);
	BOOL SearchHasItems(s32 deviceId, u32 startTime, u32 endTime, u32 recordType);
	
private:
    astring m_strIndexPath;
    Poco::Data::Session *m_DB;
	VDBDiskMap m_diskMap;
	VDBDiskStatus m_diskStatus;
	bool m_diskStatusInit;
};
#endif /* __INDEX_DB_HPP__ */
