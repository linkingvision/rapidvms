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

       /* Get the old record */
	BOOL GetAOldRecord(astring & strPath);
	BOOL GetRecordFilePath(VdbRecordItem &pItem, astring & strPath);
	BOOL RecordRLock(astring & strPath);
	BOOL RecordRUnLock(astring & strPath);
	BOOL RecordWLock(astring & strPath);
	BOOL RecordWUnLock(astring & strPath);
	BOOL ClearAllBlockLock();
	BOOL ClearAllBlockWriting();
	BOOL CorrectAllTheRecord();
	
	
	/* The return is the ID of the record */
	s64 AddRecord(astring deviceId, u32 recordType, u32 startTime, astring & strPathBlock);
	BOOL UpdateRecordEndtime(s64 recordId, u32 endTime);
	
	
	/* Search Interface */
	BOOL SearchAItem(astring deviceId, u32 Time, VdbRecordItem &pItem);
	BOOL SearchAItemNear(astring deviceId, u32 Time, VdbRecordItem &pItem);
	BOOL SearchNextItem(astring deviceId, s64 LastId, VdbRecordItem &pItem);
	BOOL SearchItems(astring deviceId, u32 startTime, u32 endTime, u32 recordType, 
				RecordItemMap & pMap);
	BOOL SearchHasItems(astring deviceId, u32 startTime, u32 endTime, u32 recordType);

public:
	/* The Task will merge type with real items, and pre record is based on the file */
	BOOL AddSchedItem(astring deviceId, u32 startTime, u32 endTime, RecordingType type);
	bool SetPrePostRecordTime(s32 nPreTime, s32 nPostTime);
	/* The return vaule is sleep time of the thread */
	u32 MergeSingleStep();
	bool ProcessOneRecord(VdbRecordItem &pItem);
	bool RemoveOldScheds();
private:
	astring m_strIndexPath;
	Poco::Data::Session *m_DB;
	VDBDiskMap m_diskMap;
	VDBDiskStatus m_diskStatus;
	bool m_diskStatusInit;
};
#endif /* __INDEX_DB_HPP__ */
