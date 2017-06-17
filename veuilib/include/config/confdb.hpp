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

#ifndef _CONF_DB_H_
#define _CONF_DB_H_

#include "utility.hpp"
#include "leveldb/db.h"
#include "config/conf.hpp"
#include "debug.hpp"

#include "config/vidconf.pb.h"

#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "simplecrypt.hpp"
#include "recordwrapper.hpp"

using namespace UtilityLib;
using namespace XSDK;


class VE_LIBRARY_API ConfDB
{
public:
	ConfDB()
	{

	}
	~ConfDB()
	{

	}

	s32 Open(astring  pPath);
public:
	bool CameraRecordTemplSet(astring strCameraId, astring strTempl);
	bool FindCamera(astring strCameraId);
	bool DeleteCamera(astring strCameraId);
	bool AddCamera(VidCamera &pAddCam);
	bool GetCameraConf(astring strCameraId, VidCamera &pCam);
public:

	BOOL GetHdfsRecordConf(VidHDFSConf &pData);
	BOOL UpdateHdfsRecordConf(VidHDFSConf &pData);

	BOOL GetCameraListConf(VidCameraList &pData);
	BOOL UpdateCameraListConf(VidCameraList &pData);

	BOOL GetLicense(astring &strLicense);
	BOOL SetLicense(astring &strLicense);



	BOOL GetCmnParam(astring &strKey, astring &strParam);
	BOOL SetCmnParam(astring &strKey, astring &strParam);

	bool GetStorServerConf(VidStorServerConf &pData);
	bool SetStorServerConf(VidStorServerConf &pData);
public:
	bool GetRecSched(astring strId, RecordSchedWeek &pSched);

private:
    XMutex m_cMutex;

private:
    leveldb::DB* m_pDb;
    leveldb::Options m_Options;
};

typedef ConfDB* LPConfDB;

#endif /* _CONF_DB_H_ */
