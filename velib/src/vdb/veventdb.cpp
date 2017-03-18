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
#include "veventdb.hpp"
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

/* 
1. The event table 
CREATE TABLE events (id INTEGER PRIMARY KEY, device INTEGER, 
value TEXT, numa INTEGER, numb INTEGER, start DATE, end DATE, type TEXT)
*/

using Poco::Data::Statement;
using Poco::Data::RecordSet;
using namespace Poco::Data;
using namespace Poco;
using namespace Poco::Data::Keywords;

using Poco::Data::Session;

VEventDB::VEventDB(astring & strPath)
: m_strIndexPath(strPath), m_DB(NULL)
{
	Init();
}
VEventDB::~VEventDB()
{
	DeInit();
}

BOOL VEventDB::CreateTablesIfNecessary()
{
	if (m_DB)
	{										
		/* events  */
		*m_DB << "CREATE TABLE IF NOT EXISTS events (id INTEGER PRIMARY KEY, device TEXT, "
		"devname TEXT, type TEXT, evttime INTEGER, evttimestr DATE, desc TEXT)", now;
	}
	return TRUE;
}

BOOL VEventDB::Init()
{
	Poco::Path path(m_strIndexPath);
	path.append("idb");
	Poco::File file(path);
	file.createDirectories();
	SQLite::Connector::registerConnector();
	VDC_DEBUG("Create the DB path %s\n", path.toString().c_str());
	
	Poco::Path filePath("eventdb.vi");
	path.append(filePath);
	path.makeFile();
	VDC_DEBUG("Create the DB file %s\n", path.toString().c_str());

	m_DB = new Poco::Data::Session("SQLite", path.toString());
	
    CreateTablesIfNecessary();
	
	return TRUE;
}

BOOL VEventDB::NewEvent(astring strDev, astring strDevName, astring strType, s64 nEvtTime, astring strDesc)
{
	XGuard guard(m_cMutex);
	astring strEvtTime;
	s64 recordId;
	strEvtTime = Time2String(nEvtTime);
	*m_DB << "INSERT INTO records VALUES(NULL, :device, :devname, :type, :evttime, :evttimestr, :desc)", 
	use(strDev), use(strDevName), use(strType), 
	use(nEvtTime), use(strEvtTime), use(strDesc), now;	
	
	return true;
}

BOOL VEventDB::DeInit()
{
	if (m_DB)
	{
		delete m_DB;
	}
	return TRUE;
}

