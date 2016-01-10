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
#include <QtWidgets/QApplication>

using namespace cppkit;
using namespace std;

class StorSyncInf: public QObject
{
	Q_OBJECT
public:
	StorSyncInf(VidStor &stor, int nTimeoutMillis);
	~StorSyncInf();

public:
	bool Connect();
	VidCameraList GetVidCameraList();
	VidDiskList GetVidDiskList();
	bool AddVidDisk(VidDisk &pDisk);
	bool DeleteVidDisk(astring strId);
	bool ConfAdminPasswd(astring strOldPasswd, astring strNewPasswd);
	bool GetLic(astring &pLic);
	bool ConfLic(astring &pLic);
	bool AddCam(VidCamera &pParam);
	bool DeleteCam(astring strId);
	/* Search */

private:
	bool SyncRecv(OAPIHeader &header);
 	VidCameraList UpdateVidCameraList(oapi::OAPICameraListRsp list);
private:
	XMutex m_cMutex;
	
private:
	astring m_strId;
	VidStor m_stor;


	XRef<XSocket> m_pSocket;
	int m_nTimeoutMillis; 
	bool m_bConnected;
	char *m_pRecv;
	int m_nRecvLen;;
};

typedef StorSyncInf* LPStorSyncInf;


#include "storsyncinfimpl.hpp"

#endif