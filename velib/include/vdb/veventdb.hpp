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
#ifndef __VEVENT_DB_HPP__
#define __VEVENT_DB_HPP__
#include "utility.hpp"
#include "Poco/Data/Session.h"
#include "utility/videotype.hpp"
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"

using namespace UtilityLib;
using namespace XSDK;

/*
Event Time:
MotionStart
MotionEnd

*/

class VE_LIBRARY_API VEventDB
{
public:
    VEventDB(astring & strPath);
    ~VEventDB();
	
public:
	BOOL Init();
	BOOL CreateTablesIfNecessary();
	BOOL DeInit();
public:
	BOOL NewEvent(astring strDev, astring strDevName, astring strType, s64 nEvtTime, astring strDesc);
	
private:
	XMutex m_cMutex;
    astring m_strIndexPath;
    Poco::Data::Session *m_DB;
};
#endif /* __VEVENT_DB_HPP__ */
