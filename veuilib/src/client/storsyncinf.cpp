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
#include "client/storsyncinf.hpp"

StorSyncInf::StorSyncInf(VidStor &stor, int nTimeoutMillis)
:m_stor(stor), StorWebSocketClient(stor.strip(), stor.strport(), LINK_PROTO_WS_PATH)
{
	/* Just log when there has command */
	//Login(stor.struser(), stor.strpasswd(), "Nonce");
	StartKeepThread();
}
StorSyncInf::~StorSyncInf()
{
	Disconnect();
}

bool StorSyncInf::IsKeep()
{
	return false;
}

bool StorSyncInf::ProcessRecvMsg(char *data, size_t data_len)
{
	/* lock guard */
	std::lock_guard<std::mutex> guard(m_lock);
	std::string strMsg(data, data_len);
	Link::LinkCmd cmd;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::JsonStringToMessage(strMsg, &cmd);
	if (!status.ok())
	{
		return false;
	}

	switch (cmd.type())
	{
		case Link::LINK_CMD_LOGIN_RESP:
		{
			return ProcessLoginResp(cmd, m_stor.struser(), m_stor.strpasswd());
			break;
		}
		case Link::LINK_CMD_CAM_SEARCHED_NOTIFY:
		{
			return ProcessCamSearchedNotify(cmd);
			break;
		}

		default:
		   	break;
	};
	m_msgId ++;
	m_lastCmd = cmd;
	return true;
}

bool StorSyncInf::ProcessCamSearchedNotify(Link::LinkCmd &cmd)
{
	if (!cmd.has_camsearchednotify())
	{
		return false;
	}

	const LinkCamSearchedNotify& pResp =  cmd.camsearchednotify();
	
	XGuard guard(m_cMutex);
	m_searchedCam.push_back(pResp);

	return true;
}


bool StorSyncInf::ProcessCamListResp(Link::LinkCmd &cmd)
{
	if (!cmd.has_camlistresp())
	{
		return false;
	}
	
	const LinkListCamResp& pResp =  cmd.camlistresp();

	
	
	
	return true;
}

bool StorSyncInf::ProcessOffline()
{	
	
	return true;
}

bool StorSyncInf::ProcessOnline()
{
	Login(m_stor.struser(), m_stor.strpasswd(), "Nonce");
	return true;
}


VidCameraList StorSyncInf::GetVidCameraList()
{
	
	
	VidCameraList empty;
	if (Connect() == false)
	{
		return empty;
	}


	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_CAM_LIST_REQ);
	LinkListCamReq * req = new LinkListCamReq;
	req->set_ball(true);

	cmd.set_allocated_camlistreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return empty;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return empty;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return empty;
	}

	if (!respCmd.has_camlistresp())
	{
		return empty;
	}

	const LinkListCamResp& pResp =  respCmd.camlistresp();
	
	return pResp.clist();
}

bool StorSyncInf::GetVidCamera(astring strId, VidCamera &pCam)
{
	
	
	VidCameraList empty;
	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_CAM_REQ);
	LinkCamReq * req = new LinkCamReq;
	req->set_strid(strId);

	cmd.set_allocated_camreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_camresp())
	{
		return false;
	}

	const LinkCamResp& pResp =  respCmd.camresp();
	pCam = pResp.ccam();
	
	return true;
	
}

	
VidDiskList StorSyncInf::GetVidDiskList()
{
	
	
	VidDiskList empty;
	if (Connect() == false)
	{
		return empty;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_DISK_LIST_REQ);
	LinkVidDiskListReq * req = new LinkVidDiskListReq;

	cmd.set_allocated_disklistreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return empty;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return empty;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return empty;
	}

	if (!respCmd.has_disklistresp())
	{
		return empty;
	}

	const LinkVidDiskListResp &pResp =  respCmd.disklistresp();
	empty = pResp.cdisk();

	return empty;	
}

VidDiskList StorSyncInf::GetSysVidDiskList()
{
	
	
	VidDiskList empty;
	if (Connect() == false)
	{
		return empty;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_SYS_DISK_LIST_REQ);
	LinkSysVidDiskListReq * req = new LinkSysVidDiskListReq;

	cmd.set_allocated_sysdisklistreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return empty;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return empty;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return empty;
	}

	if (!respCmd.has_sysdisklistresp())
	{
		return empty;
	}

	const LinkSysVidDiskListResp &pResp =  respCmd.sysdisklistresp();
	empty = pResp.cdisk();

	return empty;		
}

bool StorSyncInf::AddVidDisk(VidDisk &pDisk)
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_ADD_DISK_REQ);
	LinkAddDiskReq * req = new LinkAddDiskReq;
	VidDisk *newDisk = new VidDisk;
	*newDisk = pDisk;
	req->set_allocated_cdisk(newDisk);

	cmd.set_allocated_adddiskreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_adddiskresp())
	{
		return false;
	}

	const LinkAddDiskResp &pResp =  respCmd.adddiskresp();

	return pResp.bsuccess();	
}

bool StorSyncInf::DeleteVidDisk(astring strId)
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_DEL_DISK_REQ);
	LinkDelDiskReq * req = new LinkDelDiskReq;
	req->set_strid(strId);

	cmd.set_allocated_deldiskreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_deldiskresp())
	{
		return false;
	}

	const LinkDelDiskResp &pResp =  respCmd.deldiskresp();

	return pResp.bsuccess();	
}


bool StorSyncInf::UpdateVidDiskLimit(astring strId, s64 nLimit)
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_UPDATE_DISK_LIMIT_REQ);
	LinkUpdateDiskLimitReq * req = new LinkUpdateDiskLimitReq;
	req->set_strid(strId);
	req->set_nlimit(nLimit);

	cmd.set_allocated_disklimitreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_disklimitresp())
	{
		return false;
	}

	const LinkUpdateDiskLimitResp &pResp =  respCmd.disklimitresp();

	return pResp.bsuccess();		
}

bool StorSyncInf::ConfAdminPasswd(astring strOldPasswd, astring strNewPasswd)
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_ADD_USER_REQ);
	LinkAddUserReq * req = new LinkAddUserReq;
	req->set_struser("admin");
	req->set_strpasswd(strNewPasswd);
	cmd.set_allocated_adduserreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_adduserresp())
	{
		return false;
	}

	const LinkAddUserResp &pResp =  respCmd.adduserresp();
	
	
	return pResp.bsuccess();	
}
bool StorSyncInf::GetLic(astring &pLic, astring &strHostId, 
							int &ch, astring &type, astring &startTime, astring &expireTime)
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_GET_LIC_REQ);
	LinkGetLicReq * req = new LinkGetLicReq;

	cmd.set_allocated_licreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_licresp())
	{
		return false;
	}

	const LinkGetLicResp &pResp =  respCmd.licresp();
	pLic = pResp.strlic();
	strHostId = pResp.strhostid();
	ch = pResp.nch();
	type = pResp.strtype();
	if (pResp.strstarttime().length() > 0)
		startTime = pResp.strstarttime();
	if (pResp.strexpiretime().length() > 0)
		expireTime = pResp.strexpiretime();
	
	return true;	
}

bool StorSyncInf::GetVer(astring &pVer, astring &strInfo)
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_GET_VER_REQ);
	LinkGetVerReq * req = new LinkGetVerReq;

	cmd.set_allocated_verreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_verresp())
	{
		return false;
	}

	const LinkGetVerResp &pResp =  respCmd.verresp();
	pVer = pResp.strver();
	strInfo = pResp.strinfo();

	return true;	
}

bool StorSyncInf::ConfLic(astring &pLic)
{
	if (pLic.length() <= 0)
	{
		return false;
	}
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_CONF_LIC_REQ);
	LinkConfLicReq * req = new LinkConfLicReq;
	req->set_strlic(pLic);

	cmd.set_allocated_conflicreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_conflicresp())
	{
		return false;
	}

	const LinkConfLicResp &pResp =  respCmd.conflicresp();
	
	
	return pResp.bsuccess();
}

bool StorSyncInf::AddCam(VidCamera &pParam)
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_ADD_CAM_REQ);
	LinkAddCamReq * req = new LinkAddCamReq;
	VidCamera *newCam = new VidCamera;
	*newCam = pParam;
	req->set_allocated_ccam(newCam);

	cmd.set_allocated_addcamreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_addcamresp())
	{
		return false;
	}

	const LinkAddCamResp &pResp =  respCmd.addcamresp();

	return pResp.bsuccess();
}

bool StorSyncInf::SetCamSched(VidCamera &pParam)
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_SET_CAM_SCHED_REQ);
	LinkSetCamSchedReq * req = new LinkSetCamSchedReq;
	VidCamera *newCam = new VidCamera;
	*newCam = pParam;
	req->set_allocated_ccam(newCam);

	cmd.set_allocated_setcamschedreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_setcamschedresp())
	{
		return false;
	}

	const LinkSetCamSchedResp &pResp =  respCmd.setcamschedresp();

	return pResp.bsuccess();
}
bool StorSyncInf::DeleteCam(astring strId)
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_DEL_CAM_REQ);
	LinkDelCamReq * req = new LinkDelCamReq;
	req->set_strid(strId);

	cmd.set_allocated_delcamreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_delcamresp())
	{
		return false;
	}

	const LinkDelCamResp &pResp =  respCmd.delcamresp();
	
	
	return pResp.bsuccess();
}

bool StorSyncInf::CamSearchStart()
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_CAM_SEARCH_START_REQ);
	LinkCamSearchStartReq * req = new LinkCamSearchStartReq;

	cmd.set_allocated_camsearchstartreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_camsearchstartresp())
	{
		return false;
	}

	const LinkCamSearchStartResp &pResp =  respCmd.camsearchstartresp();
	
	
	return pResp.bsuccess();
}
bool StorSyncInf::CamSearchStop()
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_CAM_SEARCH_STOP_REQ);
	LinkCamSearchStopReq * req = new LinkCamSearchStopReq;

	cmd.set_allocated_camsearchstopreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_camsearchstopresp())
	{
		return false;
	}

	const LinkCamSearchStopResp &pResp =  respCmd.camsearchstopresp();
	
	
	return pResp.bsuccess();
}
bool StorSyncInf::CamSearchGet(astring &strIP, astring &strPort, astring &strModel, 
				astring &strOnvifAddr)
{
	XGuard guard(m_cMutex);

	if (m_searchedCam.size() <= 0)
	{
		return false;
	}

	LinkCamSearchedNotify cam = m_searchedCam.back();
	m_searchedCam.pop_back();

	strIP = cam.strip();
	strPort = cam.strport();
	strModel = cam.strmodel();
	strOnvifAddr = cam.stronvifaddress();

	return true;
}

bool StorSyncInf::SearchRec(astring strId, u32 nStart, u32 nEnd, u32 nType, RecordItemMap &pMap)
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_SEARCH_RECORD_REQ);
	LinkSearchRecordReq * req = new LinkSearchRecordReq;
	req->set_strid(strId);
	req->set_nstart(nStart);
	req->set_nend(nEnd);
	req->set_ntype(nType);

	cmd.set_allocated_searchrecreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_searchrecresp())
	{
		return false;
	}

	const LinkSearchRecordResp &pResp =  respCmd.searchrecresp();

	const LinkRecordList &pSearchList = pResp.clist();

	int recSize = pSearchList.clist_size();

	for (s32 i = 0; i < pSearchList.clist_size(); i++)
	{
		const LinkRecordItem &itemNew = pSearchList.clist(i);
		VdbRecordItem itemNew1;
		itemNew1.id = itemNew.nid();
		itemNew1.start = itemNew.nstart();
		itemNew1.end = itemNew.nend();
		itemNew1.type = itemNew.ntype();
		pMap[itemNew1.id] = itemNew1;
	}
	
	return true;
}

bool StorSyncInf::SearchHasRec(astring strId, HasRecordItemMap &pMap)
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_HAS_RECORD_REQ);
	LinkHasRecordReq * req = new LinkHasRecordReq;
	LinkHasRecordList *pList = new LinkHasRecordList;
	
	HasRecordItemMap::iterator it = pMap.begin(); 
	for(; it!=pMap.end(); ++it)
	{
		LinkHasRecordItem *pItem = pList->add_chasrec();
		pItem->set_nid((*it).first);
		pItem->set_nstart((*it).second.start);
		pItem->set_nend((*it).second.end);
		pItem->set_ntype((*it).second.type);
	}

	req->set_allocated_clist(pList);
	req->set_strid(strId);

	cmd.set_allocated_hasrecreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_hasrecresp())
	{
		return false;
	}

	pMap.clear();

	const LinkHasRecordResp &pResp =  respCmd.hasrecresp();

	const LinkHasRecordList &pSearchList = pResp.clist();

	int recSize = pSearchList.chasrec_size();

	for (s32 i = 0; i < pSearchList.chasrec_size(); i ++)
	{
		const LinkHasRecordItem &itemNew = pSearchList.chasrec(i);
		VdbHasRecordItem itemNew1;
		itemNew1.id = itemNew.nid();
		itemNew1.start = itemNew.nstart();
		itemNew1.end = itemNew.nend();
		itemNew1.type = itemNew.ntype();
		itemNew1.has = itemNew.bhas();
		pMap[itemNew1.id] = itemNew1;
	}
	
	return true;
}

bool StorSyncInf::GetStreamList(astring strId, VidStreamList &pList)
{
	

	if (Connect() == false)
	{
		return false;
	}

	Link::LinkCmd cmd;
	cmd.set_type(Link::LINK_CMD_GET_STREAM_LIST_REQ);
	LinkGetStreamListReq * req = new LinkGetStreamListReq;
	req->set_strid(strId);

	cmd.set_allocated_getstreamlistreq(req);
	std::string strMsg;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
		return false;
	}
	long long lastMsgId = 0;
	/* only lock here */
	{
		//std::lock_guard<std::mutex> guard(m_lock);
		lastMsgId = m_msgId;
	}
	
	if (SendMsg(strMsg) == false)
	{
		return false;
	}

	Link::LinkCmd respCmd;

	if (GetRespMsg(lastMsgId, respCmd) == false)
	{
		return false;
	}

	if (!respCmd.has_getstreamlistresp())
	{
		return false;
	}

	const LinkGetStreamListResp &pResp =  respCmd.getstreamlistresp();

	pList = pResp.clist();
	
	return true;
	
}

bool StorSyncInf::GetRespMsg(long long lastId, Link::LinkCmd & respCmd)
{
	int i = 30;
	while(i --)
	{
		std::chrono::milliseconds dura(100);
		std::this_thread::sleep_for(dura);

		/* lock the guard */
		std::lock_guard<std::mutex> guard(m_lock);
		if (m_msgId > lastId)
		{
			respCmd = m_lastCmd;
			return true;
		}
		
	}
	return false;
}

