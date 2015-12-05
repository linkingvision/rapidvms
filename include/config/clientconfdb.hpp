#ifndef __CLIENT_CONF_DB_H__
#define __CLIENT_CONF_DB_H__

#include "utility.hpp"
#include "leveldb/db.h"
#include "conf.hpp"
#include "debug.hpp"

using namespace UtilityLib;

class ClientConfDB
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
	
	BOOL GetStorListConf(VidStorList &pData);
	BOOL UpdateStorListConf(VidStorList &pData);
	
	
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

typedef ClientConfDB* LPClientConfDB;


#include "clientconfdbimpl.hpp"


#endif /* __CLIENT_CONF_DB_H__ */
