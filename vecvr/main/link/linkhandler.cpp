#include "link/linkhandler.hpp"
#include "config/vidconf.pb.h"
#include <QtCore/QStorageInfo>
#include <QtCore/QFileInfoList>
#include <QtCore/QDir>
#include "devicesearcher.h"
#include <QHostAddress>
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "pbwrapper.hpp"
#include <QTimer>

/*
Loop list for protobuf
	VidCameraList cameraList;
	GetCameraListConf(cameraList);
	int cameraSize = cameraList.cvidcamera_size();

	for (s32 i = 0; i < cameraList.cvidcamera_size(); i ++)
	{
		const VidCamera &cam = cameraList.cvidcamera(i);
		if (cam.strid() == strCameraId)
		{
			pCam = cam;
			return true;
		}
	}

*/

/*
Add for protobuf
	VidCamera *pCam = cameraListNew.add_cvidcamera();

*/

LinkHandler::LinkHandler(Factory &pFactory)
:m_pFactory(pFactory), m_bLogin(false), m_bRegNotify(false), m_server(NULL), 
m_conn(NULL)
{
	UUIDGenerator uuidCreator;
	
	m_seesionId  = uuidCreator.createRandom().toString();
}
LinkHandler::~LinkHandler()
{
	if (m_bRegNotify == true)
	{
		m_pFactory.UnRegCameraChangeNotify((void *)this);
	}
}

bool LinkHandler::CallChange(void* pParam, FactoryCameraChangeData data)
{
    int dummy = errno;
   LinkHandler * pObject = (LinkHandler * )pParam;

    if (pObject)
    {
        return pObject->CallChange1(data);
    }
}
bool LinkHandler::CallChange1(FactoryCameraChangeData data)
{
	/* have not register */
	if (m_server == NULL || m_conn == NULL || m_bRegNotify == false)
	{
		return true;
	}
		
	switch(data.type)
	{
		case FACTORY_CAMERA_ADD:
			return NotifyCamAdd(data);
			break;
		case FACTORY_CAMERA_DEL:
			return NotifyCamDel(data);
			break;
		case FACTORY_CAMERA_ONLINE:
			return NotifyCamOnline(data);
			break;
		case FACTORY_CAMERA_OFFLINE:
			return NotifyCamOffline(data);
			break;
		case FACTORY_CAMERA_REC_ON:
			return NotifyCamRecOn(data);
			break;
		case FACTORY_CAMERA_REC_OFF:
			return NotifyCamRecOff(data);
			break;
		default:
			break;		
	}
}

bool LinkHandler::NotifyCamAdd(FactoryCameraChangeData data)
{
	Link::LinkCmd cmdResp;

	cmdResp.set_type(Link::LINK_CMD_CAM_ADD_NOTIFY);
	LinkCamAddNotify * resp = new LinkCamAddNotify;

	VidCamera *pCam = new VidCamera;
	if (m_pFactory.GetConfDB().GetCameraConf(data.id, *pCam) == false)
	{
		return false;
	}

	resp->set_allocated_ccam(pCam);

	cmdResp.set_allocated_camaddnotify(resp);

	SendRespMsg(cmdResp, m_server, m_conn);

	return true;
}
bool LinkHandler::NotifyCamDel(FactoryCameraChangeData data)
{
	Link::LinkCmd cmdResp;

	cmdResp.set_type(Link::LINK_CMD_CAM_DEL_NOTIFY);
	LinkCamIdNotify * resp = new LinkCamIdNotify;
	resp->set_strid(data.id);

	cmdResp.set_allocated_camidnotify(resp);

	SendRespMsg(cmdResp, m_server, m_conn);

	return true;
}
bool LinkHandler::NotifyCamOnline(FactoryCameraChangeData data)
{
	Link::LinkCmd cmdResp;

	cmdResp.set_type(Link::LINK_CMD_CAM_ONLINE_NOTIFY);
	LinkCamIdNotify * resp = new LinkCamIdNotify;
	resp->set_strid(data.id);

	cmdResp.set_allocated_camidnotify(resp);

	SendRespMsg(cmdResp, m_server, m_conn);

	return true;
}
bool LinkHandler::NotifyCamOffline(FactoryCameraChangeData data)
{
	Link::LinkCmd cmdResp;

	cmdResp.set_type(Link::LINK_CMD_CAM_OFFLINE_NOTIFY);
	LinkCamIdNotify * resp = new LinkCamIdNotify;
	resp->set_strid(data.id);

	cmdResp.set_allocated_camidnotify(resp);

	SendRespMsg(cmdResp, m_server, m_conn);

	return true;
}
bool LinkHandler::NotifyCamRecOn(FactoryCameraChangeData data)
{
	Link::LinkCmd cmdResp;

	cmdResp.set_type(Link::LINK_CMD_CAM_REC_ON_NOTIFY);
	LinkCamIdNotify * resp = new LinkCamIdNotify;
	resp->set_strid(data.id);

	cmdResp.set_allocated_camidnotify(resp);

	SendRespMsg(cmdResp, m_server, m_conn);

	return true;
}
bool LinkHandler::NotifyCamRecOff(FactoryCameraChangeData data)
{
	Link::LinkCmd cmdResp;

	cmdResp.set_type(Link::LINK_CMD_CAM_REC_OFF_NOTIFY);
	LinkCamIdNotify * resp = new LinkCamIdNotify;
	resp->set_strid(data.id);

	cmdResp.set_allocated_camidnotify(resp);

	SendRespMsg(cmdResp, m_server, m_conn);

	return true;
}

bool LinkHandler::ProcessLoginReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_loginreq())
	{
		return false;
	}
	
	
	const LinkLoginReq& pLogin =  req.loginreq();

	cmdResp.set_type(Link::LINK_CMD_LOGIN_RESP);
	LinkLoginResp * resp = new LinkLoginResp;

	astring realPasswd = "admin";

	SimpleCrypt crypt;
	QString strDePasswd = m_pFactory.GetAdminPasswd(realPasswd);

	//realPasswd = crypt.decryptToString(strDePasswd).toStdString();
	realPasswd = "admin";

	/* calc the md5 and compare */
	std::string pass = m_seesionId + realPasswd;

	XMD5 md5Check;
	md5Check.Update((const uint8_t*)(pass.c_str()), pass.length());

	md5Check.Finalize();
	std::string md5Output = md5Check.GetAsString().c_str();
	

	if (md5Output == pLogin.strpasswd())
	{
		resp->set_bret(true);
		m_bLogin = true;
		resp->set_bretnonce(false);
	}else
	{
		resp->set_strnonce(m_seesionId);
		resp->set_bretnonce(true);
	}

	cmdResp.set_allocated_loginresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessRegNotifyReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_regnotifyreq())
	{
		return false;
	}

	
	const LinkRegNotifyReq& pReq =  req.regnotifyreq();
	m_bRegNotify = true;
	m_server = server;
	m_conn = conn;
	m_pFactory.RegCameraChangeNotify(this,
		(FactoryCameraChangeNotify)(LinkHandler::CallChange));


	cmdResp.set_type(Link::LINK_CMD_REG_NOTIFY_RESP);
	LinkRegNotifyResp * resp = new LinkRegNotifyResp;

	resp->set_bsuccess(nRet);

	cmdResp.set_allocated_regnotifyresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}
	                        

bool LinkHandler::ProcessCamListReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_camlistreq())
	{
		return false;
	}

	CameraOnlineMap pCameraOnlineMap;
	CameraRecMap pCameraRecMap;

	m_pFactory.GetCameraOnlineMap(pCameraOnlineMap);
	m_pFactory.GetCameraRecMap(pCameraRecMap);
	
	const LinkListCamReq& pList =  req.camlistreq();

	cmdResp.set_type(Link::LINK_CMD_CAM_LIST_RESP);
	LinkListCamResp * resp = new LinkListCamResp;
	VidCameraList * clist = new VidCameraList;

	m_pFactory.GetCameraList(*clist);

	for (s32 i = 0; i < clist->cvidcamera_size(); i ++)
	{
		VidCamera &cam = (VidCamera &)(clist->cvidcamera(i));
		cam.set_bonline(pCameraOnlineMap[cam.strid()]);
		cam.set_brec(pCameraRecMap[cam.strid()]);	
	}
	resp->set_allocated_clist(clist);

	cmdResp.set_allocated_camlistresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessCamReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_camreq())
	{
		return false;
	}
	
	const LinkCamReq& pCamReq =  req.camreq();

	cmdResp.set_type(Link::LINK_CMD_CAM_RESP);
	LinkCamResp * resp = new LinkCamResp;
	VidCamera * pCam = new VidCamera;
	m_pFactory.GetCamera(pCamReq.strid(), *pCam);

	resp->set_allocated_ccam(pCam);

	cmdResp.set_allocated_camresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessAddCamReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_addcamreq())
	{
		return false;
	}
	
	const LinkAddCamReq& pCamReq =  req.addcamreq();
	CameraParam addParam((VidCamera &)(pCamReq.ccam()));
	nRet = true;
	m_pFactory.AddCamera(addParam);

	cmdResp.set_type(Link::LINK_CMD_ADD_CAM_RESP);
	LinkAddCamResp * resp = new LinkAddCamResp;

	resp->set_bsuccess(nRet);

	cmdResp.set_allocated_addcamresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}
bool LinkHandler::ProcessDelCamReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool bRet = false;
	if (!req.has_delcamreq())
	{
		return false;
	}
	
	const LinkDelCamReq& pCamReq =  req.delcamreq();

	bRet = m_pFactory.DelCamera(pCamReq.strid());

	cmdResp.set_type(Link::LINK_CMD_DEL_CAM_RESP);
	LinkDelCamResp * resp = new LinkDelCamResp;

	resp->set_bsuccess(bRet);

	cmdResp.set_allocated_delcamresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}
bool LinkHandler::ProcessSetCamSchedReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_setcamschedreq())
	{
		return false;
	}
	
	const LinkSetCamSchedReq& pCamReq =  req.setcamschedreq();
	const VidCamera & pCam = pCamReq.ccam();

	nRet = m_pFactory.UpdateRecSched(pCam.strid(), (VidCamera &)pCam);

	cmdResp.set_type(Link::LINK_CMD_SET_CAM_SCHED_RESP);
	LinkSetCamSchedResp * resp = new LinkSetCamSchedResp;

	resp->set_bsuccess(nRet);

	cmdResp.set_allocated_setcamschedresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessGetStreamListReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_getstreamlistreq())
	{
		return false;
	}
	
	const LinkGetStreamListReq& pReq =  req.getstreamlistreq();

	VidStreamList *pList = new VidStreamList;
	nRet = m_pFactory.GetCamStreamList(pReq.strid(), *pList);

	cmdResp.set_type(Link::LINK_CMD_GET_STREAM_LIST_RESP);
	LinkGetStreamListResp * resp = new LinkGetStreamListResp;

	resp->set_allocated_clist(pList);

	cmdResp.set_allocated_getstreamlistresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessDiskListReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_disklistreq())
	{
		return false;
	}

	cmdResp.set_type(Link::LINK_CMD_DISK_LIST_RESP);
	LinkVidDiskListResp * resp = new LinkVidDiskListResp;


	/* Get disk list from factory */
	VDBDiskMap diskMap;
	m_pFactory.GetDiskMap(diskMap);
	VidDiskList *diskList = new VidDiskList;

	VDBDiskMap::iterator it = diskMap.begin(); 
	for(; it!=diskMap.end(); ++it)
	{
		VidDisk *pDisk = diskList->add_cviddisk();
		pDisk->set_strid((*it).second.hdd);
		pDisk->set_strpath((*it).second.path);
		pDisk->set_ntotalsize(0);
		pDisk->set_nfreesize(0);
		pDisk->set_nstorused((*it).second.used);
		pDisk->set_nstorlimit((*it).second.limit);
	}

	resp->set_allocated_cdisk(diskList);

	cmdResp.set_allocated_disklistresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}
bool LinkHandler::ProcessSysDiskListReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_sysdisklistreq())
	{
		return false;
	}

	cmdResp.set_type(Link::LINK_CMD_SYS_DISK_LIST_RESP);
	LinkSysVidDiskListResp * resp = new LinkSysVidDiskListResp;

	VidDiskList *diskList = new VidDiskList;

	/* Get Disk list from system */
	QList<QStorageInfo> hdd = QStorageInfo::mountedVolumes();
	QListIterator<QStorageInfo> it(hdd);
	while(it.hasNext())
	{
		QStorageInfo  disk = it.next();
		astring strHdd = disk.rootPath().toStdString();
		
		R_LOG(logRINFO,"%s strHdd %s \n",__FUNCTION__, strHdd.c_str());
		s64 totalSize = disk.bytesTotal() / (1024 * 1024);
		s64 leftSize = disk.bytesFree()/ (1024 * 1024);
		
		R_LOG(logRINFO,"%s Total %lld M Left %lld M \n",__FUNCTION__, totalSize, leftSize);  
		QString strQtHdd = disk.rootPath();
		HddDriveType diskType = HddGetDriveType(strQtHdd);
		
		R_LOG(logRINFO,"%s Type %d \n",__FUNCTION__,  diskType);
		
		if (totalSize/1024 < 4 || leftSize/1024 < 2) /* In G */
		{
			continue;
		}
		
		if (diskType == HddInternalDrive 
			|| diskType == HddRemovableDrive || diskType == HddRemoteDrive)
		{
			VidDisk *pDisk = diskList->add_cviddisk();
			pDisk->set_strid(disk.device().toStdString());
			pDisk->set_strpath(disk.rootPath().toStdString());
			pDisk->set_ntotalsize(totalSize);
			pDisk->set_nfreesize(leftSize);
			pDisk->set_nstorused(0);
			pDisk->set_nstorlimit(0);
		}
	}

	resp->set_allocated_cdisk(diskList);

	cmdResp.set_allocated_sysdisklistresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;	
}


bool LinkHandler::ProcessAddDiskReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_adddiskreq())
	{
		return false;
	}

	const VidDisk& pDisk =  req.adddiskreq().cdisk();

	bool bRet = m_pFactory.AddHdd(pDisk.strid(), pDisk.strpath(), pDisk.nstorlimit());
	
	cmdResp.set_type(Link::LINK_CMD_ADD_DISK_RESP);
	LinkAddDiskResp * resp = new LinkAddDiskResp;
	resp->set_bsuccess(bRet);

	cmdResp.set_allocated_adddiskresp(resp);

	SendRespMsg(cmdResp, server, conn);

}

bool LinkHandler::ProcessDelDiskReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_deldiskreq())
	{
		return false;
	}

	const LinkDelDiskReq& pReq =  req.deldiskreq();

	

	bool bRet = m_pFactory.DelHdd(pReq.strid());
	
	cmdResp.set_type(Link::LINK_CMD_DEL_DISK_RESP);
	LinkDelDiskResp * resp = new LinkDelDiskResp;
	resp->set_bsuccess(bRet);

	cmdResp.set_allocated_deldiskresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessUpdateDiskLimitReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_disklimitreq())
	{
		return false;
	}

	const LinkUpdateDiskLimitReq& pReq =  req.disklimitreq();

	

	bool bRet = m_pFactory.HddUpdateSize(pReq.strid(), pReq.nlimit());
	
	cmdResp.set_type(Link::LINK_CMD_UPDATE_DISK_LIMIT_RESP);
	LinkUpdateDiskLimitResp * resp = new LinkUpdateDiskLimitResp;
	resp->set_bsuccess(bRet);

	cmdResp.set_allocated_disklimitresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

/* License & Version */
bool LinkHandler::ProcessGetLicReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_licreq())
	{
		return false;
	}

	const LinkGetLicReq& pReq =  req.licreq();

	
	
	cmdResp.set_type(Link::LINK_CMD_GET_LIC_RESP);
	LinkGetLicResp * resp = new LinkGetLicResp;
	std::string strLic;
	std::string strHostId;
	int nCh;
	std::string strType;
	std::string strStartTime;
	std::string strExpireTime;	
	m_pFactory.GetLicense(strLic, strHostId, nCh, strType,
					strStartTime, strExpireTime);
	resp->set_strlic(strLic);
	resp->set_strhostid(strHostId);
	resp->set_strtype(strType);
	resp->set_strstarttime(strStartTime);
	resp->set_strexpiretime(strExpireTime);

	cmdResp.set_allocated_licresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}
bool LinkHandler::ProcessGetVerReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_verreq())
	{
		return false;
	}

	const LinkGetVerReq& pReq =  req.verreq();

	cmdResp.set_type(Link::LINK_CMD_GET_VER_RESP);
	LinkGetVerResp * resp = new LinkGetVerResp;

	resp->set_strver(VE_VERSION);
	resp->set_strinfo(VE_INFO);

	cmdResp.set_allocated_verresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessConfLicReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_conflicreq())
	{
		return false;
	}

	const LinkConfLicReq& pReq =  req.conflicreq();

	astring strLic = pReq.strlic();
	m_pFactory.SetLicense(strLic);

	cmdResp.set_type(Link::LINK_CMD_CONF_LIC_RESP);
	LinkConfLicResp * resp = new LinkConfLicResp;

	resp->set_bsuccess(true);

	cmdResp.set_allocated_conflicresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;	
}

bool LinkHandler::ProcessAddUserReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_adduserreq())
	{
		return false;
	}

	const LinkAddUserReq& pReq =  req.adduserreq();

	/* Change passwd of admin */
	if (pReq.struser() == "admin")
	{
		nRet = m_pFactory.SetAdminPasswd(pReq.strpasswd());
	}else/* normal user */
	{
	}

	cmdResp.set_type(Link::LINK_CMD_ADD_USER_RESP);
	LinkAddUserResp * resp = new LinkAddUserResp;

	resp->set_bsuccess(nRet);

	cmdResp.set_allocated_adduserresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;	
}

/* Search */
bool LinkHandler::ProcessHasRecordReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_hasrecreq())
	{
		return false;
	}

	const LinkHasRecordReq& pReq =  req.hasrecreq();
	const LinkHasRecordList &pSearchList = pReq.clist();

	LinkHasRecordList *pNewList = new LinkHasRecordList;
	*pNewList = pSearchList;

	for (s32 i = 0; i < pNewList->chasrec_size(); i ++)
	{
		LinkHasRecordItem &item = (LinkHasRecordItem &)(pNewList->chasrec(i));
		bool bRet = m_pFactory.SearchHasItems(pReq.strid(), item.nstart(),
							item.nend(), item.ntype());
		item.set_bhas(bRet);
	}

	cmdResp.set_type(Link::LINK_CMD_HAS_RECORD_RESP);
	LinkHasRecordResp * resp = new LinkHasRecordResp;
	resp->set_allocated_clist(pNewList);

	cmdResp.set_allocated_hasrecresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;	
}

bool LinkHandler::ProcessSearchRecordReq(Link::LinkCmd &req, CivetServer *server,
                        struct mg_connection *conn)

{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_searchrecreq())
	{
		return false;
	}

	const LinkSearchRecordReq& pReq =  req.searchrecreq();
	RecordItemMap recMap;

	m_pFactory.SearchItems(pReq.strid(), pReq.nstart(), pReq.nend(), 
									pReq.ntype(), recMap);

	cmdResp.set_type(Link::LINK_CMD_SEARCH_RECORD_RESP);
	LinkSearchRecordResp * resp = new LinkSearchRecordResp;

	LinkRecordList *pList = new LinkRecordList;

	resp->set_strid(pReq.strid());
	RecordItemMap::iterator it = recMap.begin(); 
	
	for(; it!=recMap.end(); ++it)
	{	
		LinkRecordItem *pItem = pList->add_clist();

		pItem->set_nid((*it).second.id);
		pItem->set_nstart((*it).second.start);
		pItem->set_nend((*it).second.end);
		pItem->set_ntype((*it).second.type);
	}

	
	resp->set_allocated_clist(pList);
	

	cmdResp.set_allocated_searchrecresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;	
}
	

bool LinkHandler::ProcessMsg(std::string &strMsg, CivetServer *server,
                        struct mg_connection *conn)
{
	Link::LinkCmd cmd;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::JsonStringToMessage(strMsg, &cmd);
	if (!status.ok())
	{
		return false;
	}

	switch (cmd.type())
	{
		case Link::LINK_CMD_LOGIN_REQ:
		{
			return ProcessLoginReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_REG_NOTIFY_REQ:
		{
			return ProcessRegNotifyReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_CAM_LIST_REQ:
		{
			return ProcessCamListReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_CAM_REQ:
		{
			return ProcessCamReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_ADD_CAM_REQ:
		{
			return ProcessAddCamReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_DEL_CAM_REQ:
		{
			return ProcessDelCamReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_SET_CAM_SCHED_REQ:
		{
			return ProcessSetCamSchedReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_GET_STREAM_LIST_REQ:
		{
			return ProcessGetStreamListReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_DISK_LIST_REQ:
		{
			return ProcessDiskListReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_SYS_DISK_LIST_REQ:
		{
			return ProcessSysDiskListReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_ADD_DISK_REQ:
		{
			return ProcessAddDiskReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_DEL_DISK_REQ:
		{
			return ProcessDelDiskReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_UPDATE_DISK_LIMIT_REQ:
		{
			return ProcessUpdateDiskLimitReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_GET_LIC_REQ:
		{
			return ProcessGetLicReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_GET_VER_REQ:
		{
			return ProcessGetVerReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_CONF_LIC_REQ:
		{
			return ProcessConfLicReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_ADD_USER_REQ:
		{
			return ProcessAddUserReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_HAS_RECORD_REQ:
		{
			return ProcessHasRecordReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_SEARCH_RECORD_REQ:
		{
			return ProcessSearchRecordReq(cmd, server, conn);
			break;
		}
		default:
		   	return false;
	};		
	

}

bool LinkHandler::SendRespMsg(Link::LinkCmd &resp, CivetServer *server,
                        struct mg_connection *conn)
{
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(resp, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	int ret = mg_websocket_write(conn, 
		WEBSOCKET_OPCODE_TEXT, strMsg.c_str(), strMsg.length());
	printf("%s %d websocket send %d\n", __FILE__, __LINE__, ret);
	if (ret == strMsg.length())
	{
		return true;
	}
	return false;
}

