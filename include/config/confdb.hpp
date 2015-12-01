

#ifndef _CONF_DB_H_
#define _CONF_DB_H_

#include "utility.hpp"
#include "leveldb/db.h"
#include "conf.hpp"
#include "debug.hpp"

#include "config/vidconf.pb.h"

using namespace VidConf;

using namespace UtilityLib;

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
#if 0
	bool CameraRecordSet(astring strCameraId, bool bOn);
	bool CameraHDFSRecordSet(astring strCameraId, bool bOn);
#endif
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

#if 0

	s32 GetSysData(VSCConfData &pSysData);
	s32 UpdateSysData(VSCConfData &pSysData);

	s32 AddDevice(VSCDeviceData &pData, u32 nId);
	s32 DelDevice(u32 nId);

	s32 AddVIPC(VSCVIPCData &pData, u32 nId);
	s32 DelVIPC(u32 nId);

	s32 GetVmsData(VSCVmsData &pVmsData);
	s32 UpdateVmsData(VSCVmsData &pVmsData);

	/* IPC View */
	s32 GetViewData(VSCViewData &pViewData);
	s32 UpdateViewData(VSCViewData &pViewData);

	/* IPC Group */
	s32 GetVGroupData(VSCVGroupData &pVGroupData);
	s32 UpdateVGroupData(VSCVGroupData &pVGroupData);

	/* Hdfs Record */
	s32 GetHdfsRecordData(VSCHdfsRecordData &pData);
	s32 UpdateHdfsRecordData(VSCHdfsRecordData &pData);

	/* User */
	s32 GetUserData(VSCUserData &pData);
	s32 UpdateUserData(VSCUserData &pData);

	/* Emap */
	s32 GetEmapData(VSCEmapData &pData);
	s32 UpdateEmapData(VSCEmapData &pData);
	BOOL GetEmapFile(astring &strFile);
	BOOL SetEmapFile(astring &strFile);

	/* Tour */
	s32 GetTourData(VSCTourData &pData);
	s32 UpdateTourData(VSCTourData &pData);


public:
	BOOL GetSystemConf(VSCConfData &pSys);
	BOOL GetVmsConf(VSCVmsData &pVms);
	BOOL GetViewConf(VSCViewData &pView);
	
	BOOL GetVGroupConf(VSCVGroupData &pGroup);


	BOOL GetUserConf(VSCUserData &pData);

	BOOL GetEmapConf(VSCEmapData &pData);

	BOOL GetTourConf(VSCTourData &pData);
#endif	

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

typedef ConfDB* LPConfDB;


#include "confdbimpl.hpp"


#endif /* _CONF_DB_H_ */
