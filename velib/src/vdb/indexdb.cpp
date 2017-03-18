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
#include "indexdb.hpp"
#include "Poco/UUIDGenerator.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Column.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/SharedPtr.h"
#include "Poco/DateTime.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/SQLite/Connector.h"
#include "debug.hpp"
#include "mediafile.hpp"

/* 
1. All the records for all the channel 
2. All the video file block list for cycle rewrite, and reserve for channel
3. All the HDD list
4. All the scheds
*/

using Poco::Data::Statement;
using Poco::Data::RecordSet;
using namespace Poco::Data;
using namespace Poco;
using namespace Poco::Data::Keywords;

using Poco::Data::Session;

/* Default pre record time is 60s */
#define VDB_DEFAULT_PRE_RECORD_TIME 60

IndexDB::IndexDB(astring & strPath)
: m_strIndexPath(strPath), m_DB(NULL), m_diskStatusInit(false)
{
	Init();
}
IndexDB::~IndexDB()
{
	DeInit();
}

BOOL IndexDB::CreateTablesIfNecessary()
{
	if (m_DB)
	{
		/* Records, ext is for uuid extension table */
		*m_DB << "CREATE TABLE IF NOT EXISTS records (id INTEGER PRIMARY KEY, device TEXT, "
				"blockpath TEXT, type INTEGER, start INTEGER, end INTEGER, "
				"startstr DATE, endstr DATE, ref INTEGER, writing INTEGER, "
				"processed INTEGER, protected INTEGER, ext TEXT)", now;
		/* blocks */
		*m_DB << "CREATE TABLE IF NOT EXISTS blocks (id INTEGER PRIMARY KEY, hdd TEXT, "
				"path TEXT, uuid TEXT, used INTEGER)", now;
		/* HDDs */ //the size is used by the recording, the limit is the max of the recording
		*m_DB << "CREATE TABLE IF NOT EXISTS hdds (id INTEGER PRIMARY KEY, hdd TEXT, "
				"path TEXT, sizelimit INTEGER, size INTEGER)", now;
		/* All the sched item include triggered alarm */
		*m_DB << "CREATE TABLE IF NOT EXISTS scheds (id INTEGER PRIMARY KEY, device TEXT, "
				"type INTEGER, start INTEGER, end INTEGER, startstr DATE, endstr DATE)", now;
		
	}
	return TRUE;
}

BOOL IndexDB::Init()
{
	Poco::Path path(m_strIndexPath);
	path.append("idb");
	Poco::File file(path);
	file.createDirectories();
	SQLite::Connector::registerConnector();
	VDC_DEBUG("Create the DB path %s\n", path.toString().c_str());
	
	Poco::Path filePath("idb.vi");
	path.append(filePath);
	path.makeFile();
	VDC_DEBUG("Create the DB file %s\n", path.toString().c_str());

	m_DB = new Poco::Data::Session("SQLite", path.toString());
	
	CreateTablesIfNecessary();
	ClearAllBlockLock();
	ClearAllBlockWriting();
	CorrectAllTheRecord();
	InitDiskMap();
	
	return TRUE;
}

 BOOL IndexDB::DeInit()
{
	if (m_DB)
	{
		delete m_DB;
	}
	return TRUE;
}

BOOL IndexDB::ClearAllBlockLock()
{
	s64 recordIdLast = 0;
	std::vector<s64> recordId;
	s64 id = 0;
	while (1)
	{
		Statement stmt = (*m_DB << "SELECT id FROM records WHERE ref >0 AND id >:id", 
		           into(recordId), use(recordIdLast), limit(1)); 
		stmt.execute();


		if (recordId.size() > 0)
		{
			std::vector<s64>::iterator it = recordId.begin();
			id = *it;
			recordIdLast = id;
			*m_DB << "UPDATE records SET ref=0 WHERE id=:id", use(id), now;
		}else
		{
			VDC_DEBUG("No ref > 0\n");
			return TRUE;
		}
               
	}
	return TRUE;
}

BOOL IndexDB::ClearAllBlockWriting()
{
	s64 recordIdLast = 0;
	std::vector<s64> recordId;
	s64 id = 0;
	while (1)
	{
		Statement stmt = (*m_DB << "SELECT id FROM records WHERE writing >0 AND id >:id", 
		           into(recordId), use(recordIdLast), limit(1)); 
		stmt.execute();


		if (recordId.size() > 0)
		{
			std::vector<s64>::iterator it = recordId.begin();
			id = *it;
			recordIdLast = id;
			*m_DB << "UPDATE records SET writing=0 WHERE id=:id", use(id), now;
		}else
		{
			VDC_DEBUG("No ref > 0\n");
			return TRUE;
		}
               
	}
	return TRUE;
}

BOOL IndexDB::CorrectAllTheRecord()
{
	s64 recordIdLast = 0;
       std::vector<s64> recordId;
       s64 id = 0;
       astring strPath;

       while (1)
       {
               Statement stmt = (*m_DB << "SELECT id FROM records WHERE start == end AND id >:recordid", 
                           into(recordId), use(recordIdLast), limit(1)); 
               stmt.execute();
               
               
               if (recordId.size() > 0)
               {
               	std::vector<s64>::iterator it = recordId.begin();
               	id = *it;
			recordIdLast = id;
			*m_DB << "SELECT blockpath FROM records WHERE id=:id", use(id), into(strPath), now;
			MediaFile * VFile = new MediaFile(strPath, 0, MEDIA_SESSION_PLAYBACK, 
			                            MF_FILE_MAX_LENGTH); 
			u32 endTime;
			u32 startTime;
			if (strPath.length() == 0)
			{
				endTime = 0;
				startTime = 0;
	               	astring start;
				start = Time2String(startTime);
				VDC_DEBUG("Update records ID %lld End\n", id);
				*m_DB << "UPDATE records SET start=:startdate WHERE id=:id", 
				              use(startTime), use(id), now;
				*m_DB << "UPDATE records SET startstr=:startdate WHERE id=:id", 
				              use(start), use(id), now;
			}else
			{	
                      	VFile->GetStartAndEndTime(startTime, endTime);
			}
               	astring end;
	               end = Time2String(endTime);
	               VDC_DEBUG("Update records ID %lld End\n", id);
	               *m_DB << "UPDATE records SET end=:enddate WHERE id=:id", 
                                  use(endTime), use(id), now;
	               *m_DB << "UPDATE records SET endstr=:enddate WHERE id=:id", 
                                  use(end), use(id), now;
			  /* set writing to 0 */
	               *m_DB << "UPDATE records SET writing=0 WHERE id=:id", 
                                  use(end), use(id), now;
                      delete VFile;
               }else
               {
               	VDC_DEBUG("No ref > 0\n");
               	return TRUE;
               }
               
       }

	return TRUE;
}

BOOL IndexDB::InitDiskMap()
{
	std::vector<astring> hdd;
	VdbDiskItem diskItem;
	/* Clear all the data */
	m_diskMap.clear();
	s64 diskLimit;
	
	Statement stmt = (*m_DB << "SELECT hdd FROM hdds", into(hdd), limit(64)); 
	stmt.execute();
	if (hdd.size() > 0)
	{
		std::vector<astring>::iterator it;
		for (it = hdd.begin(); it != hdd.end(); ++it)	
		{
			diskItem.hdd = *it;
                      *m_DB << "SELECT path FROM hdds WHERE hdd=:hdd", use(diskItem.hdd), 
                   into(diskItem.path), now;
                      *m_DB << "SELECT sizelimit FROM hdds WHERE hdd=:hdd", use(diskItem.hdd), 
                   into(diskItem.limit), now;
                      *m_DB << "SELECT size FROM hdds WHERE hdd=:hdd", use(diskItem.hdd), 
				   into(diskItem.used), now;
			diskItem.loading = 0;
			m_diskMap[*it] = diskItem;
		}
	}
	return TRUE;
}

BOOL  IndexDB::SelectADisk(astring &strHdd)
{
	s64 loading = -1;
  	/* loop to find a good disk */
	for (VDBDiskMap::iterator it = m_diskMap.begin(); it != m_diskMap.end(); it++)
	{
		VDBDiskStatus::iterator it1 = m_diskStatus.find((*it).first), ite1 = m_diskStatus.end();
		if (it1 == ite1)
		{
			VDC_DEBUG("HDD %s do not have status\n", ((*it).first).c_str());
			continue;
		}

		if (m_diskStatus[(*it).first].status == HDD_DISK_OK && m_diskStatus[(*it).first].freeSize >= 512 * 1024
			&& (*it).second.used < (*it).second.limit)
		{
			if (loading == -1)
			{
				strHdd = (*it).second.hdd;
				loading = (*it).second.loading;
			}else
			{	
				/* The current disk has low loading */
				if ((*it).second.loading < loading)
				{
					strHdd = (*it).second.hdd;
					loading = (*it).second.loading;
				}
			}
		}	

	}
	if (loading == -1)
	{
		VDC_DEBUG("Can not find HDD\n");
		return false;
	}
	/* increase the loading */
	m_diskMap[strHdd].loading++;
	return true;
}


BOOL IndexDB::GetDiskMap(VDBDiskMap &pMap)
{
	pMap = m_diskMap;

	return TRUE;

}

BOOL IndexDB::GetDiskStatus(VDBDiskStatus &pStatus)
{
	pStatus = m_diskStatus;

	return TRUE;

}

BOOL IndexDB::UpdateDiskStatusMap(VDBDiskStatus &pMap)
{
	if (pMap.size() <= 0)
	{
		return false;
	}
	if (m_diskStatusInit == false)
	{
		m_diskStatusInit = true;
	}
	m_diskStatus = pMap;
	return true;
}

/* nSize is in MBytes */
BOOL IndexDB::AddHdd(astring &strHdd, astring & strPath, s64 nSize)
{
	std::vector<astring> hdd;
	VdbDiskItem diskItem;
	Statement stmt = (*m_DB << "SELECT hdd FROM hdds", into(hdd), limit(64)); 
	stmt.execute();
	
	/* Check is the HDD is in the hdds tables */
	if (hdd.size() > 0)
	{
		std::vector<astring>::iterator it;
		it = find(hdd.begin(), hdd.end(), strHdd);
		if (it != hdd.end())
		{
			VDC_DEBUG("find HDD %s in the hdds\n", strHdd.c_str());
			return TRUE;
		}

	}
	
	/* The HDD table is empty, Just insert */
	VDC_DEBUG("Add A new HDD %s path %s size %d\n", strHdd.c_str(), strPath.c_str(), nSize);
	//std::vector<std::string> insert;
	//insert.push_back(strPath);
	
	*m_DB << "INSERT INTO hdds VALUES(NULL, :hdd, :path, :sizelimit, 0)", 
					use(strHdd), use(strPath), use(nSize), now;
	//CreateBlocksIfNecessary(strPath, nSize);
	
	diskItem.limit = nSize;
	diskItem.path = strPath;
	diskItem.hdd = strHdd;
	diskItem.used = 0;
	diskItem.loading = 0;

	m_diskMap[strHdd] = diskItem;
	
	return TRUE;
}

BOOL IndexDB::DelHdd(astring & strHdd)
{
    VDBDiskMap::iterator it1 = m_diskMap.find(strHdd), ite1 = m_diskMap.end();
    if (it1 == ite1)
    {
    	VDC_DEBUG("Can not find the hdd\n", strHdd.c_str());
    	return false;
    }
    s64 id = 0;
    *m_DB << "SELECT id FROM hdds WHERE hdd=:hdd", use(strHdd), 
        into(id), now;
    VDC_DEBUG("Hdd id %lld\n", id);
    *m_DB << "DELETE FROM hdds WHERE hdd=:hdd", use(strHdd), now;
    m_diskMap.erase(strHdd);
	return true;
}

BOOL IndexDB::HddUpdateSize(astring &strHdd, s64 nSize)
{
    VDBDiskMap::iterator it1 = m_diskMap.find(strHdd), ite1 = m_diskMap.end();
    if (it1 == ite1)
    {
    	VDC_DEBUG("Can not find the hdd\n", strHdd.c_str());
    	return false;
    }
    m_diskMap[strHdd].limit = nSize;
    *m_DB << "UPDATE hdds SET sizelimit=:sizelimit WHERE hdd=:hdd", use(nSize),
	 	use(strHdd), now;
	
	return true;
}

BOOL IndexDB::CreateBlocksIfNecessary(astring & strPath, s32 nSize)
{
	UUIDGenerator uuidCreator;
	s64 nTotalSize = nSize * 1024;
	s64 nBlocks = nTotalSize / (MF_FILE_MAX_LENGTH / (1024 * 1024));
	//Just for test insert 200 block 
	for (int i = 0; i < nBlocks; i ++)
	{
		Poco::Path path(strPath);
		VDC_DEBUG("Create the DB path %s\n", path.toString().c_str());
		path.append("mdb");
		
		char buff[256];
		sprintf(buff, "%d", i/64);
		path.append(buff);
		Poco::File file1(path);
		file1.createDirectories();
		astring id = uuidCreator.createRandom().toString();
		astring fileName = id + ".vm";
		path.append(fileName);
		
		path.makeFile();
		Poco::File file2(path);
		file2.createFile();
		file2.setSize(MF_FILE_MAX_LENGTH);//TODO set the block size
		VDC_DEBUG("Create the Block file %s\n", path.toString().c_str());
		astring tempPath = path.toString(); 
		*m_DB << "INSERT INTO blocks VALUES(NULL, :device, :path, :uuid, 0)", use(strPath), 
			use(tempPath), use(id), now;
	}
	
	return TRUE;
	
}

BOOL IndexDB::CreateABlock(astring & strHdd, astring &strBlockPath)
{
	astring tempPath = "path";
	s64 split = 0;
	astring strPath = m_diskMap[strHdd].path;
	UUIDGenerator uuidCreator;
	
	astring id = uuidCreator.createRandom().toString();
	*m_DB << "INSERT INTO blocks VALUES(NULL, :hdd, :path, :uuid, 0)", use(strHdd), 
		use(tempPath), use(id), now;
    	*m_DB << "SELECT id FROM blocks WHERE uuid=:uuid", 
			use(id), into(split), now;
	
	Poco::Path path(strPath);
	VDC_DEBUG("Create the DB path %s\n", path.toString().c_str());
#ifndef _WIN64
	path.append("vidstor");
#else
	path.append("vidstor64");
#endif
	path.append("mdb");

	char buff[256];
	sprintf(buff, "%d", split/4096);
	path.append(buff);
	Poco::File file1(path);
	file1.createDirectories();
	
	astring fileName = id + ".vm";
	path.append(fileName);

	path.makeFile();
	Poco::File file2(path);
	file2.createFile();
	file2.setSize(MF_FILE_MAX_LENGTH);
	VDC_DEBUG("Create the Block file %s\n", path.toString().c_str());
	tempPath = path.toString(); 
	 *m_DB << "UPDATE blocks SET path=:path WHERE uuid=:uuid", use(tempPath),
	 	use(id), now;
	strBlockPath = tempPath;
	m_diskMap[strHdd].used = m_diskMap[strHdd].used + MF_FILE_MAX_LENGTH/(1024 * 1024);

	*m_DB << "UPDATE hdds SET size=:size WHERE hdd=:hdd", use(m_diskMap[strHdd].used),
	 	use(strHdd), now;
	
	return true;
}

BOOL IndexDB::RequestABlockFile(astring & strPath)
{
	/* Disk status not init, can not recording */
	if (m_diskStatusInit == false)
	{
		return false;
	}
	
	/* create a block */
	astring strHdd;
	if (SelectADisk(strHdd) == false)
	{
		return false;
	}
	if (CreateABlock(strHdd, strPath) == false)
	{
		return false;
	}
	
	return true;
}

BOOL IndexDB::GetAOldRecord(astring & strPath)
{
	/* Disk status not init, can not recording */
	if (m_diskStatusInit == false)
	{
		return false;
	}
    s64 recordId;
    *m_DB << "SELECT MIN(Id) FROM records", into(recordId), now;

    *m_DB << "SELECT blockpath FROM records WHERE id=:recordid", use(recordId), into(strPath), now;

    //TODO find if the block is in locked
    VDC_DEBUG("Find a used block %s\n", strPath.c_str());
    *m_DB << "UPDATE blocks SET used=1 WHERE path=:paths", use(strPath), now;

    *m_DB << "DELETE FROM records WHERE id=:recordid", use(recordId), now;
    return TRUE;
}

s64 IndexDB::AddRecord(astring deviceId, u32 recordType, u32 startTime, astring & strPathBlock)
{
	astring start;
	s64 recordId;
	astring ext = " ";
	start = Time2String(startTime);
	*m_DB << "INSERT INTO records VALUES(NULL, :device, :path, :type, :start, :end, "
		":startstr, :endstr, 0, 0, 0, 0, :ext)", 
	use(deviceId), use(strPathBlock), use(recordType), 
	use(startTime), use(startTime), use(start), use(start), use(ext), now;
	
	*m_DB << "SELECT id FROM records WHERE blockpath=:paths", use(strPathBlock), into(recordId), 
	now;
	VDC_DEBUG("Add record ID %lld\n", recordId);
	
	return recordId;
}


BOOL IndexDB::SearchAItem(astring deviceId, u32 Time, VdbRecordItem &pItem)
{
    s64 recordId = -1;
    std::vector<s64> recordIds;
    Statement stmt = (*m_DB << "SELECT id FROM records WHERE device=:device AND start<=:time AND end >=:time", 
        use(deviceId), use(Time), use(Time), into(recordIds), limit(1)); 
    stmt.execute();
	
    if (recordIds.size() > 0)
    {
    	std::vector<s64>::iterator it = recordIds.begin();
    	recordId = *it;
    	//VDC_DEBUG("Find a record %d\n", recordId);
    }else
    {
        return FALSE;
    }

    if (recordId == -1)
    {
        return FALSE;
    }
    pItem.id = recordId;
    *m_DB << "SELECT start FROM records WHERE id=:id", use(recordId), 
        into(pItem.start), now;
    *m_DB << "SELECT end FROM records WHERE id=:id", use(recordId), 
        into(pItem.end), now;   
    return TRUE;
}

BOOL IndexDB::SearchAItemNear(astring deviceId, u32 Time, VdbRecordItem &pItem)
{
    s64 recordId = -1;
    std::vector<s64> recordIds;
    Statement stmt = (*m_DB << "SELECT id FROM records WHERE device=:device AND start>=:time", 
        use(deviceId), use(Time), into(recordIds), limit(1)); 
    stmt.execute();
	
    if (recordIds.size() > 0)
    {
    	std::vector<s64>::iterator it = recordIds.begin();
    	recordId = *it;
    	//VDC_DEBUG("Find a record %d\n", recordId);
    }else
    {
        return FALSE;
    }

    if (recordId == -1)
    {
        return FALSE;
    }
    pItem.id = recordId;
    *m_DB << "SELECT start FROM records WHERE id=:id", use(recordId), 
        into(pItem.start), now;
    *m_DB << "SELECT end FROM records WHERE id=:id", use(recordId), 
        into(pItem.end), now;   
    return TRUE;
}


BOOL IndexDB::SearchItems(astring deviceId, u32 startTime, u32 endTime, u32 recordType, 
					RecordItemMap & pMap)
{
    s64 recordId = 0;
    s64 recordLastId = 0;

    VdbRecordItem Item;
    int searchTime = startTime;
    int i = 0;

    while (1)
    {
        std::vector<s64> recordIds;
        
        Statement stmt = (*m_DB << "SELECT id FROM records WHERE device=:device AND id >:recordid AND \
 (end>=:starttime AND  start<=:endtime)", 
            use(deviceId), use(recordLastId), use(searchTime), use(endTime), use(searchTime), use(endTime), into(recordIds), limit(1)); 
        stmt.execute();
    	
        if (recordIds.size() > 0)
        {
        	std::vector<s64>::iterator it = recordIds.begin();
        	recordId = *it;
               Item.id = recordId;
               *m_DB << "SELECT start FROM records WHERE id=:id", use(recordId), 
                   into(Item.start), now;
               *m_DB << "SELECT end FROM records WHERE id=:id", use(recordId), 
                   into(Item.end), now;
               *m_DB << "SELECT type FROM records WHERE id=:id", use(recordId), 
                   into(Item.type), now;
                 VDC_DEBUG("Find a record %lld (%d,  %d), input (%d,  %d) type %d\n", recordId, Item.start, 
                                 Item.end, startTime, endTime, Item.type);
                 recordLastId = recordId;
                 pMap[i] = Item;
                 i ++;
        }else
        {
            return FALSE;
        }
    }
    VDC_DEBUG("%s Find record %d input (%d,  %d)\n", deviceId.c_str(), i, startTime, endTime);
    return TRUE;
}

BOOL IndexDB::SearchHasItems(astring deviceId, u32 startTime, u32 endTime, u32 recordType)
{
    s64 recordId = 0;
    s64 recordLastId = 0;

    VdbRecordItem Item;
    int searchTime = startTime;
    int i = 0;

    while (1)
    {
        std::vector<s64> recordIds;
        
        Statement stmt = (*m_DB << "SELECT id FROM records WHERE device=:device AND id >:recordid AND \
 (end>=:starttime AND  start<=:endtime)", 
            use(deviceId), use(recordLastId), use(searchTime), use(endTime), use(searchTime), use(endTime), into(recordIds), limit(1)); 
        stmt.execute();
	 VDC_DEBUG("%s Find has record %d input (%d,  %d)\n", deviceId.c_str(), recordIds.size(), startTime, endTime);
        if (recordIds.size() > 0)
        {
        	return TRUE;
        }else
        {
            return FALSE;
        }
    }

    return TRUE;
}

 BOOL IndexDB::SearchNextItem(astring deviceId, s64 LastId, VdbRecordItem &pItem)
{
    s64 recordId = -1;

    std::vector<s64> recordIds;
    Statement stmt = (*m_DB << "SELECT id FROM records WHERE device=:device AND id>:id ", 
        use(deviceId), use(LastId), into(recordIds), limit(1)); 
    stmt.execute();
	
    if (recordIds.size() > 0)
    {
    	std::vector<s64>::iterator it = recordIds.begin();
    	recordId = *it;
    	VDC_DEBUG("Find a record %d\n", recordId);
    }else
    {
        return FALSE;
    }

    VDC_DEBUG("Search Next Item  deivce %s, lastid %lld recordid %lld\n", deviceId.c_str(), LastId, 
                                recordId);
    if (recordId == -1)
    {
        return FALSE;
    }
    pItem.id = recordId;
    *m_DB << "SELECT start FROM records WHERE id=:id", use(recordId), 
        into(pItem.start), now;
    *m_DB << "SELECT end FROM records WHERE id=:id", use(recordId), 
        into(pItem.end), now;   
    return TRUE;
}

 BOOL IndexDB::UpdateRecordEndtime(s64 recordId, u32 endTime)
{
	astring end;
	end = Time2String(endTime);
	VDC_DEBUG("Update record ID %lld End\n", recordId);
	
	*m_DB << "UPDATE records SET end=:enddate WHERE id=:id", use(endTime), use(recordId), now;
	*m_DB << "UPDATE records SET endstr=:enddate WHERE id=:id", use(end), use(recordId), now;
	return TRUE;
}

BOOL IndexDB::GetRecordFilePath(VdbRecordItem &pItem, astring & strPath)
{
    *m_DB << "SELECT blockpath FROM records WHERE id=:id", use(pItem.id), 
        into(strPath), now;
     return TRUE;
}

BOOL IndexDB::RecordRLock(astring & strPath)
{
	s32 ref = 0;
	*m_DB << "SELECT ref FROM records WHERE blockpath=:path", 
		use(strPath), into(ref), now;
	VDC_DEBUG("%s Lock ref %d\n", strPath.c_str(), ref);
	ref ++;

	*m_DB << "UPDATE records SET ref=:ref WHERE blockpath=:path", use(ref),
		use(strPath), now;
	return TRUE;
}
BOOL IndexDB::RecordRUnLock(astring & strPath)
{
	s32 ref = 0;
	*m_DB << "SELECT ref FROM records WHERE blockpath=:path", 
		use(strPath), into(ref), now;
	 VDC_DEBUG("%s Lock ref %d\n", strPath.c_str(), ref);
	 if (ref >= 1)
	 {
	     ref --;
	 }

	*m_DB << "UPDATE records SET ref=:ref WHERE blockpath=:path", use(ref),
		use(strPath), now;
	return TRUE;
}
 BOOL IndexDB::RecordWLock(astring & strPath)
{
	*m_DB << "UPDATE records SET writing=1 WHERE blockpath=:path", use(strPath), now;
	return TRUE;
}
 BOOL IndexDB::RecordWUnLock(astring & strPath)
{
	*m_DB << "UPDATE records SET writing=0 WHERE blockpath=:path", use(strPath), now;
	return TRUE;
}

	/* The Task will merge type with real items, and pre record is based on the file */
BOOL IndexDB::AddSchedItem(astring deviceId, u32 startTime, u32 endTime, RecordingType recordType)
{
	astring start;
	astring end;
	s32 nType = recordType;

	start = Time2String(startTime);
	end = Time2String(endTime);

	/*First search if need merge*/
	s64 recordId = -1;
	std::vector<s64> schedIds;
	Statement stmt = (*m_DB << "SELECT id FROM scheds WHERE device=:device "
					"AND end>=:stime AND end<=:etime AND type=:type", 
					use(deviceId), use(startTime), use(endTime), use(nType), into(schedIds), limit(1));
	stmt.execute();

	if (schedIds.size() > 0)
	{
		std::vector<s64>::iterator it = schedIds.begin();
		recordId = *it;
		VDC_DEBUG("Find a sched %d just update it\n", recordId);
		*m_DB << "UPDATE scheds SET end=:enddate WHERE id=:id", use(endTime), use(recordId), now;
		*m_DB << "UPDATE scheds SET endstr=:enddate WHERE id=:id", use(end), use(recordId), now;
	}else
	{
		*m_DB << "INSERT INTO scheds VALUES(NULL, :device, :type, :start, :end, :startstr, :endstr)", 
		use(deviceId), use(recordType), 
		use(startTime), use(endTime), use(start), use(end), now;
	}

	return TRUE;
}

bool IndexDB::ProcessOneRecord(VdbRecordItem &pItem)
{
	/* Check if there need keep the record  and update the record type */
	s64 schedId = 0;
	s64 schedLastId = 0;
	bool bFound = false;
	s32 recordType = 0;

	while (1)
	{
	    std::vector<s64> schedIds;
	    
	    Statement stmt = (*m_DB << "SELECT id FROM scheds "
			"WHERE start<=:rend  AND end>=:rstart AND id >:lastid", use(pItem.end), 
						use(pItem.start), use(schedLastId), into(schedIds), limit(1)); 
	    stmt.execute();
		
	    if (schedIds.size() > 0)
	    {
		std::vector<s64>::iterator it = schedIds.begin();
		schedId = *it;
		bFound = true;
		s32 tmpType = 0;

		*m_DB << "SELECT type FROM scheds WHERE id=:id", use(schedId), 
		   into(tmpType), now;
		recordType = tmpType | recordType;

		 VDC_DEBUG("Find a sched %lld (%d,  %d) type %d\n", schedId, pItem.start, 
		                 pItem.end, tmpType);

		 schedLastId = schedId;
	    }else
	    {
	        break;
	    }
	}

	/* Check if we find */
	if (bFound == true)
	{
		/* update type */
		*m_DB << "UPDATE records SET type=:newType WHERE id=:recordid", use(recordType), 
				use(pItem.id), now;
		*m_DB << "UPDATE records SET processed=1 WHERE id=:recordid",  
				use(pItem.id), now;
	}else
	{
		/* delete the record */
		//TODO update the id+1 to the key frame time
		*m_DB << "DELETE FROM records WHERE id=:recordid", use(pItem.id), now;
	}

	return true;
}

bool IndexDB::RemoveOldScheds()
{
	std::time_t delTime = time(NULL) - 24 * 3600 * 2; //delete all the 2 day before
	if (delTime <= 0)
	{
		delTime = 0;
	}
	s64 schedId = 0;
	s64 schedLastId = 0;

	while (1)
	{
	    std::vector<s64> schedIds;
	    
	    Statement stmt = (*m_DB << "SELECT id FROM scheds "
			"WHERE end<=:curr  AND id >:lastid", use(delTime), 
				use(schedLastId), into(schedIds),limit(1)); 
	    stmt.execute();
		
	    if (schedIds.size() > 0)
	    {
		std::vector<s64>::iterator it = schedIds.begin();
		schedId = *it;
		*m_DB << "DELETE FROM scheds WHERE id=:schedId", use(schedId), now;
		 VDC_DEBUG("Del one %lld from scheds\n", schedId);
	    }else
	    {
	        break;
	    }
	}

	return true;
}

/* The return vaule is sleep time of the thread */
u32 IndexDB::MergeSingleStep()
{
	s64 recordId = 0;
	s64 recordLastId = 0;

	int i = 0;
	VdbRecordItem item;


	while (1)
	{
	    std::vector<s64> recordIds;
	    
	    Statement stmt = (*m_DB << "SELECT id FROM records "
			"WHERE processed<=0 AND ref<=0 "
			"AND writing<=0 AND protected<=0 AND id>:lastid", use(recordLastId), into(recordIds), limit(1)); 
	    stmt.execute();
		
	    if (recordIds.size() > 0)
	    {
		std::vector<s64>::iterator it = recordIds.begin();
		recordId = *it;
		item.id = recordId;

		*m_DB << "SELECT start FROM records WHERE id=:id", use(recordId), 
		   into(item.start), now;
		*m_DB << "SELECT end FROM records WHERE id=:id", use(recordId), 
		   into(item.end), now;
		*m_DB << "SELECT type FROM records WHERE id=:id", use(recordId), 
		   into(item.type), now;

		 VDC_DEBUG("Find a record %lld (%d,  %d)\n", recordId, item.start, 
		                 item.end);
		 ProcessOneRecord(item);
		 recordLastId = recordId;
	    }else
	    {
	        RemoveOldScheds();
	        return 60 * 1000;//if there has record need to be processed, sleep 60
	    }
	}

    return 10;
}
