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

#ifndef _SYS_DB_H_
#define _SYS_DB_H_

#include "utility.hpp"
#include "leveldb/db.h"
#include "config/conf.hpp"
#include "debug.hpp"

using namespace UtilityLib;

class VE_LIBRARY_API SysDB
{
public:
    SysDB()
    {

    }
    ~SysDB()
    {

    }

    s32 Open(astring & pPath);

public:
    BOOL GetSystemPath(astring &strPath);
    BOOL SetSystemPath(astring &strPath);

public:
    void Lock()
    {
        m_Lock.lock();
    }
    void UnLock()
    {
        m_Lock.unlock();
    }


private:
    fast_mutex m_Lock;

private:
    leveldb::DB* m_pDb;
    leveldb::Options m_Options;
};

typedef SysDB* LPSysDB;


using namespace std;

inline BOOL SysDB::GetSystemPath(astring &strPath)
{
    leveldb::Slice key("videoSystemPath");


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }else
    {
        VDC_DEBUG( "System Config is not init\n");
        return FALSE;
    }

    if (sysValue.size() == 0)
    {
        VDC_DEBUG( "System Config is not init\n");
        delete it;
        return FALSE;
    }

    strPath = sysValue.ToString();

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return TRUE;
}

inline BOOL SysDB::SetSystemPath(astring &strPath)
{
    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey("videoSystemPath");
    leveldb::Slice sysValue(strPath);

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return TRUE;
}


inline s32 SysDB::Open(astring & pPath)
{
    m_Options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(m_Options, pPath, &m_pDb);
    if (false == status.ok())
    {
        cerr << "Unable to open/create test database "<< pPath << endl;
        cerr << status.ToString() << endl;
        VDC_DEBUG( "Unable to open/create sys database %s\n", pPath.c_str());

        return FALSE;
    }
    return TRUE;
}

#endif /* _SYS_DB_H_ */
