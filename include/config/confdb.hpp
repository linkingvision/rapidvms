

#ifndef _CONF_DB_H_
#define _CONF_DB_H_

#include "utility.hpp"
#include "leveldb/db.h"
#include "conf.hpp"
#include "debug.hpp"

#include "config/vidconf.pb.h"

#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "simplecrypt.hpp"
#include "recordwrapper.hpp"

using namespace UtilityLib;
using namespace XSDK;

using namespace VidConf;


class ConfDB
{
public:
	ConfDB()
	{

	}
	~ConfDB()
	{

	}

	s32 Open(astring & pPath);
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


#include "confdbimpl.hpp"


#endif /* _CONF_DB_H_ */
