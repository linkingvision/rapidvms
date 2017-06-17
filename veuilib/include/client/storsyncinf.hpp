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
#ifndef __VSC_STOR_SYNC_INF_H__
#define __VSC_STOR_SYNC_INF_H__
#include "config/confdb.hpp"
#include "server/camera.hpp"
#include "vdb.hpp"
#include "vhdfsdb.hpp"
#include "vplay.hpp"
#include "config/sysdb.hpp"
#include "server/hdddevice.hpp"
#include <QThread>
#include <qdebug.h>
#include "Poco/Path.h"
#include "Poco/File.h"
#include "config/vidconf.pb.h"
#include "server/camera.hpp"


#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "oapi/oapic.hpp"
#include "oapi/oapis.hpp"
#include "client/storwsclient.hpp"

using namespace cppkit;
using namespace std;

class VE_LIBRARY_API StorSyncInf: public StorWebSocketClient
{
public:
	StorSyncInf(VidStor &stor, int nTimeoutMillis = 5000);
	~StorSyncInf();
public:
	virtual bool ProcessRecvMsg(char *data, size_t data_len);
	virtual bool ProcessOffline();
	virtual bool ProcessOnline();
	virtual bool IsKeep();
	bool ProcessCamListResp(Link::LinkCmd &cmd);

public:
	VidCameraList GetVidCameraList();
	bool GetVidCamera(astring strId, VidCamera &pCam);
	VidDiskList GetVidDiskList();
	VidDiskList GetSysVidDiskList();
	bool AddVidDisk(VidDisk &pDisk);
	bool DeleteVidDisk(astring strId);
	bool UpdateVidDiskLimit(astring strId, s64 nLimit);
	bool ConfAdminPasswd(astring strOldPasswd, astring strNewPasswd);
	bool GetLic(astring &pLic, astring &strHostId, 
							int &ch, astring &type, astring &startTime, astring &expireTime);
	bool GetVer(astring &pVer, astring &strInfo);
	bool ConfLic(astring &pLic);
	bool AddCam(VidCamera &pParam);
	bool SetCamSched(VidCamera &pParam);
	bool DeleteCam(astring strId);
	/* Cam Search */
	bool CamSearchStart();
	bool CamSearchStop();
	bool CamSearchGet(astring &strIP, astring &strPort, astring &strModel, 
					astring &strOnvifAddr);
	
	/* Video Search */
	bool SearchRec(astring strId, u32 nStart, u32 nEnd, u32 nType, RecordItemMap &pMap);
	bool SearchHasRec(astring strId, HasRecordItemMap &pMap);

	/* Stream List */
	bool GetStreamList(astring strId, VidStreamList &pList);

private:
	bool SyncRecv(OAPIHeader &header);
	bool GetRespMsg(long long lastId, Link::LinkCmd & respCmd);

private:
	XMutex m_cMutex;
	
private:
	astring m_strId;
	VidStor m_stor;
	Link::LinkCmd m_lastCmd;
};

typedef StorSyncInf* LPStorSyncInf;

#endif
