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
#ifndef __CLIENT_CONF_DB_H__
#define __CLIENT_CONF_DB_H__

#include "utility.hpp"
#include "leveldb/db.h"
#include "config/conf.hpp"
#include "debug.hpp"
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "simplecrypt.hpp"
#include "config/vidconf.pb.h"

using namespace UtilityLib;
using namespace XSDK;

class VE_LIBRARY_API ClientConfDB
{
public:
    ClientConfDB()
    {

    }
    ~ClientConfDB()
    {

    }

    s32 Open(astring & pPath);
public:
	BOOL GetCmnParam(astring &strKey, astring &strParam);
	BOOL SetCmnParam(astring &strKey, astring &strParam);
	bool GetLicense(astring &strLicense);
	bool SetLicense(astring &strLicense);
	/* Client */
	bool GetClientConf(VidClientConf &pData);
	bool SetClientConf(VidClientConf &pData);
public:
	/* Stor */
	bool FindStor(astring strStorId);
	bool DeleteStor(astring strStorId);
	bool AddStor(VidStor &pStor);
	bool GetStorConf(astring strId, VidStor &pStor);
	/* Stor All */
	BOOL GetStorListConf(VidStorList &pData);
	BOOL UpdateStorListConf(VidStorList &pData);

public:
	/* View */
	bool FindView(astring strViewId);
	bool DeleteView(astring strViewId);
	bool AddView(VidView &pView);
	bool GetViewConf(astring strId, VidView &pView);
	/* View All */
	BOOL GetViewListConf(VidViewList &pData);
	BOOL UpdateViewListConf(VidViewList &pData);

public:
	/* Tour */
	bool FindTour(astring strTourId);
	bool DeleteTour(astring strTourId);
	bool AddTour(VidTour &pTour);
	bool GetTourConf(astring strId, VidTour &pTour);
	/* Group All */
	BOOL GetTourListConf(VidTourList &pData);
	BOOL UpdateTourListConf(VidTourList &pData);
public:
	/* Group */
	bool FindGroup(astring strGroupId);
	bool DeleteGroup(astring strGroupId);
	bool AddGroup(VidGroup &pGroup);
	bool GetGroupConf(astring strId, VidGroup &pGroup);
	/* Group All */
	BOOL GetGroupListConf(VidGroupList &pData);
	BOOL UpdateGroupListConf(VidGroupList &pData);
public:
	/* Emap */
	bool FindEmap(astring strEmapId);
	bool DeleteEmap(astring strEmapId);
	bool AddEmap(VidEmap &pEmap);
	bool GetEmapConf(astring strId, VidEmap &pEmap);
	/* Emap All */
	BOOL GetEmapListConf(VidEmapList &pData);
	BOOL UpdateEmapListConf(VidEmapList &pData);

private:
    XMutex m_cMutex;

private:
    leveldb::DB* m_pDb;
    leveldb::Options m_Options;
};

typedef ClientConfDB* LPClientConfDB;

#endif /* __CLIENT_CONF_DB_H__ */
