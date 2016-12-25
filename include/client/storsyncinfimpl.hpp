
#ifndef __VSC_STOR_SYNC_INF_IMPL_H__
#define __VSC_STOR_SYNC_INF_IMPL_H__

inline StorSyncInf::StorSyncInf(VidStor &stor, int nTimeoutMillis)
:m_pSocket(new XSocket), m_stor(stor), m_nTimeoutMillis(nTimeoutMillis), 
m_pRecv(NULL), m_nRecvLen(0), m_bConnected(false)
{

}
inline StorSyncInf::~StorSyncInf()
{
	if (m_pRecv)
	{
		delete [] m_pRecv;
		m_pRecv = NULL;
		m_nRecvLen = 0;
	}
}

inline VidCameraList StorSyncInf::GetVidCameraList()
{
	VidCameraList empty;

	if (m_bConnected == false)
	{
		return empty;
	}

	XGuard guard(m_cMutex);

	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	/* Send add cam command  */
	pClient.SendDeviceListRequest();

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_CAM_LIST_RSP)
	{
		oapi::OAPICameraListRsp list;
		pClient.ParseDeviceList(m_pRecv, header.length, list);
		return UpdateVidCameraList(list);
	}

	return empty;
	
}

inline bool StorSyncInf::GetVidCamera(astring strId, VidCamera &pCam)
{

	if (m_bConnected == false)
	{
		return false;
	}

	XGuard guard(m_cMutex);

	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	/* Send add cam command  */
	pClient.GetCam(strId);

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_GET_CAM_RSP)
	{
		oapi::OAPICameraGetRsp cam;
		pClient.ParseGetCam(m_pRecv, header.length, cam);
		OAPIConverter::Converter(cam.cam, pCam);
		return true;
	}

	return false;
	
}

inline VidCameraList StorSyncInf::UpdateVidCameraList(oapi::OAPICameraListRsp list)
{
	
	XGuard guard(m_cMutex);
	/* Clean the list */
	VidCameraList camList;
	
	//list push_back
	std::vector<oapi::OAPICamera>::iterator it;
	for (it = list.list.begin(); it != list.list.end(); it ++)
	{
		VidCamera *pCam = camList.add_cvidcamera();
		OAPIConverter::Converter(*it, *pCam);
	}

	return camList;
}

	
inline VidDiskList StorSyncInf::GetVidDiskList()
{
	VidDiskList empty;

	if (m_bConnected == false)
	{
		return empty;
	}

	XGuard guard(m_cMutex);

	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	/* Send add cam command  */
	pClient.SendDiskListRequest();

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_DISK_LIST_RSP)
	{
		oapi::OAPIDiskListRsp list;
		VidDiskList diskList;
		pClient.ParseDiskList(m_pRecv, header.length, list);
		std::vector<oapi::OAPIDisk>::iterator it;
		for (it = list.list.begin(); it != list.list.end(); it ++)
		{
			VidDisk *pDisk = diskList.add_cviddisk();
			OAPIConverter::Converter(*it, *pDisk);
		}
		return diskList;
	}

	return empty;	
}

inline VidDiskList StorSyncInf::GetSysVidDiskList()
{
	VidDiskList empty;

	if (m_bConnected == false)
	{
		return empty;
	}

	XGuard guard(m_cMutex);

	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	/* Send add cam command  */
	pClient.SendSysDiskListRequest();

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_SYS_DISK_LIST_RSP)
	{
		oapi::OAPISysDiskListRsp list;
		VidDiskList diskList;
		pClient.ParseSysDiskList(m_pRecv, header.length, list);
		std::vector<oapi::OAPIDisk>::iterator it;
		for (it = list.list.begin(); it != list.list.end(); it ++)
		{
			VidDisk *pDisk = diskList.add_cviddisk();
			OAPIConverter::Converter(*it, *pDisk);
		}
		return diskList;
	}

	return empty;	
}

inline bool StorSyncInf::AddVidDisk(VidDisk &pDisk)
{
	if (m_bConnected == false)
	{
		return false;
	}
	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	oapi::OAPIAddDiskReq sDisk;
	OAPIConverter::Converter(pDisk, sDisk.disk);

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	pClient.AddDisk(sDisk);

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_ADD_DISK_RSP)
	{
		return true;
	}

	return false;	
}
inline bool StorSyncInf::DeleteVidDisk(astring strId)
{
	if (m_bConnected == false)
	{
		return false;
	}
	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	XGuard guard(m_cMutex);
	/* Send add disk command  */
	pClient.DelDisk(strId);

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_DEL_DISK_RSP)
	{
		return true;
	}

	return false;		
}


inline bool StorSyncInf::UpdateVidDiskLimit(astring strId, s64 nLimit)
{
	if (m_bConnected == false)
	{
		return false;
	}
	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	XGuard guard(m_cMutex);
	/* Send add disk command  */
	pClient.UpdateDiskLimit(strId, nLimit);

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_UPDATE_DISK_LIMIT_RSP)
	{
		return true;
	}

	return false;		
}

inline bool StorSyncInf::ConfAdminPasswd(astring strOldPasswd, astring strNewPasswd)
{
	if (m_bConnected == false)
	{
		return false;
	}
	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	XGuard guard(m_cMutex);
	/* Send add disk command  */
	pClient.ConfAdminPasswd(strOldPasswd, strNewPasswd);

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_CONF_ADMIN_RSP)
	{
		return true;
	}

	return false;		
}
inline bool StorSyncInf::GetLic(astring &pLic, astring &strHostId, 
							int &ch, astring &type, astring &startTime, astring &expireTime)
{
	if (m_bConnected == false)
	{
		return false;
	}
	VidDiskList empty;
	XGuard guard(m_cMutex);

	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	/* Send Get lic command  */
	pClient.GetLic();

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_GET_LIC_RSP)
	{

		pClient.ParseLic(m_pRecv, header.length, pLic, strHostId, ch, type, startTime, expireTime);

		return true;
	}

	return false;	
}

inline bool StorSyncInf::GetVer(astring &pVer, astring &strInfo)
{
	if (m_bConnected == false)
	{
		return false;
	}
	VidDiskList empty;
	XGuard guard(m_cMutex);

	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	/* Send Get lic command  */
	pClient.GetVer();

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_GET_VER_RSP)
	{

		pClient.ParseVer(m_pRecv, header.length, pVer, strInfo);

		return true;
	}

	return false;
}

inline bool StorSyncInf::ConfLic(astring &pLic)
{
	if (m_bConnected == false)
	{
		return false;
	}
	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	XGuard guard(m_cMutex);
	/* Send add disk command  */
	pClient.ConfLic(pLic);

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_CONF_LIC_RSP)
	{
		return true;
	}

	return false;		
}

inline bool StorSyncInf::AddCam(VidCamera &pParam)
{
	if (m_bConnected == false)
	{
		return false;
	}
	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	oapi::OAPIAddCameraReq sCam;
	OAPIConverter::Converter(pParam, sCam.cam);

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	pClient.AddCam(sCam);

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_ADD_CAM_RSP)
	{
		return true;
	}

	return false;
}

inline bool StorSyncInf::SetCamSched(VidCamera &pParam)
{
	if (m_bConnected == false)
	{
		return false;
	}
	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	oapi::OAPICameraUpdateSchedReq sCam;
	OAPIConverter::Converter(pParam, sCam.cam);

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	pClient.SetCamSched(sCam);

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_SET_CAM_SCHED_RSP)
	{
		return true;
	}

	return false;
}
inline bool StorSyncInf::DeleteCam(astring strId)
{
	if (m_bConnected == false)
	{
		return false;
	}
	XGuard guard(m_cMutex);

	/* Send del cam command */
	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	/* Send add cam command  */
	pClient.DeleteCam(strId);

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_DEL_CAM_RSP)
	{
		return true;
	}

	return false;
}

inline bool StorSyncInf::CamSearchStart()
{
	if (m_bConnected == false)
	{
		return false;
	}
	XGuard guard(m_cMutex);

	/* Send del cam command */
	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	/* Send add cam command  */
	pClient.CamSearchStart();

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_CAM_SEARCH_START_RSP)
	{
		return true;
	}

	return false;
}
inline bool StorSyncInf::CamSearchStop()
{
	if (m_bConnected == false)
	{
		return false;
	}
	XGuard guard(m_cMutex);

	/* Send del cam command */
	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	/* Send add cam command  */
	pClient.CamSearchStop();

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_CAM_SEARCH_STOP_RSP)
	{
		return true;
	}

	return false;
}
inline bool StorSyncInf::CamSearchGet(astring &strIP, astring &strPort, astring &strModel, 
				astring &strOnvifAddr)
{
	XGuard guard(m_cMutex);

	/* Send del cam command */
	OAPIClient pClient(m_pSocket);
	
	OAPIHeader header;
	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_CAM_SAERCH_PUSH)
	{	
		oapi::OAPICamSearchedNotify pCam;
		pClient.ParseSearchNotify(m_pRecv, header.length, pCam);
		strIP = pCam.strIP;
		strPort = pCam.strPort;
		strModel = pCam.strModel;
		strOnvifAddr = pCam.strONVIFAddress;
		return true;
	}

	return false;
}

inline bool StorSyncInf::SearchRec(astring strId, u32 nStart, u32 nEnd, u32 nType, RecordItemMap &pMap)
{
	if (m_bConnected == false)
	{
		return false;
	}
	VidDiskList empty;
	XGuard guard(m_cMutex);

	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	/* Send Get lic command  */
	pClient.SearchRec(strId, nStart, nEnd, nType);
	pMap.clear();

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_SEARCH_REC_RSP)
	{
		oapi::OAPISearchRecordRsp list;
		pClient.ParseSearchRecList(m_pRecv, header.length, list);
		std::vector<oapi::OAPIRecordItem>::iterator it;
		for (it = list.record.begin(); it != list.record.end(); it++)
		{
			VdbRecordItem item;
			item.id = (*it).nId;
			item.start = (*it).nStart;
			item.end = (*it).nEnd;
			item.type = (*it).nType;
			pMap[item.id] = item;
		}
		VDC_DEBUG("SearchRec size %d!\n", pMap.size());
		return true;
	}

	return false;
}
inline bool StorSyncInf::SearchHasRec(astring strId, HasRecordItemMap &pMap)
{

	if (m_bConnected == false)
	{
		return false;
	}
	VidDiskList empty;
	XGuard guard(m_cMutex);

	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	oapi::OAPISearchHasRecordReq req;
	req.Num = 0;
	req.strId = strId;
	HasRecordItemMap::iterator it = pMap.begin(); 
	for(; it!=pMap.end(); ++it)
	{
		oapi::OAPIHasRecordItem item;
		item.nIdx = (*it).first;
		item.nStart = (*it).second.start;
		item.nEnd =  (*it).second.end;
		item.nType =  (*it).second.type;
		req.record.push_back(item);
	}

	/* Send Get lic command  */
	pClient.SearchHasRec(req);
	pMap.clear();

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_SEARCH_HAS_REC_RSP)
	{
		oapi::OAPISearchHasRecordRsp list;
		pClient.ParseSearchHasRecList(m_pRecv, header.length, list);
		std::vector<oapi::OAPIHasRecordItem>::iterator it2;
		for (it2 = list.hasRecord.begin(); it2 != list.hasRecord.end(); it2++)
		{
			VdbHasRecordItem item;
			item.id = (*it2).nIdx;
			item.start = (*it2).nStart;
			item.end = (*it2).nEnd;
			item.type = (*it2).nType;
			item.has = (*it2).bHas;
			pMap[item.id] = item;
		}
		return true;
	}

	return false;

}

inline bool StorSyncInf::GetStreamList(astring strId, VidStreamList &pList)
{

	if (m_bConnected == false)
	{
		return false;
	}

	XGuard guard(m_cMutex);

	OAPIClient pClient(m_pSocket);
	OAPIHeader header;

	/* Send add cam command  */
	pClient.GetStreamList(strId);

	if (SyncRecv(header) == true 
			&& header.cmd == OAPI_STREAM_LIST_RSP)
	{
		oapi::OAPIStreamListRsp streamlist;
		pClient.ParseStreamList(m_pRecv, header.length, streamlist);
		OAPIConverter::Converter(streamlist, pList);
		return true;
	}

	return false;
	
}

inline bool StorSyncInf::SyncRecv(OAPIHeader &header)
{
	s32 nRet = 0;
	//QCoreApplication::processEvents();
	nRet = m_pSocket->Recv((void *)&header, sizeof(header));
	//QCoreApplication::processEvents();
	if (nRet != sizeof(header))
	{
		if (m_pSocket->Valid() == true)
		{
			m_bConnected = false;
			return false;
		}else
		{
			m_bConnected = false;
			return false;
		}
	}

	header.cmd = ntohl(header.cmd);
	header.length = ntohl(header.length);
	if (header.length > m_nRecvLen)
	{
		if (m_pRecv)
		{
			delete [] m_pRecv;
			m_pRecv = NULL;
		}
		/* Some thing has break */
		if (header.length > 1024 * 1024 * 8)
		{
			m_pSocket->Close();
			return false;
		}
		m_pRecv = new char[header.length + 1];
		m_nRecvLen = header.length + 1;
	}

	s32 nRetBody = m_pSocket->Recv((void *)m_pRecv, header.length);
	if (nRetBody == header.length)
	{
		return true;
	}else
	{
		m_bConnected = false;
		return false;
	}
	m_bConnected = false;
	return false;
}

inline void StorSyncInf::SetRecvTimeout(int nTimeoutMillis)
{
	m_pSocket->SetRecvTimeout(nTimeoutMillis);
	m_nTimeoutMillis = nTimeoutMillis;
}

inline bool StorSyncInf::Connect()
{
	OAPIHeader header;
	int frameCnt =0;

	s32 nRet = 0;
	ck_string ckPort = m_stor.strport();

	u16 Port = ckPort.to_uint16(10);
	int i = 2;

	try
	{
		XSDK::XString host = m_stor.strip();
		QCoreApplication::processEvents();
		m_pSocket->Connect(host, Port, m_nTimeoutMillis);
		QCoreApplication::processEvents();
		oapi::OAPICameraListRsp list;
		OAPIClient pClient(m_pSocket);
		
		pClient.Setup(m_stor.struser(), m_stor.strpasswd(), "Nonce");
		QCoreApplication::processEvents();

		m_pSocket->SetRecvTimeout(m_nTimeoutMillis);
		while(i --)
		{
			QCoreApplication::processEvents();
			nRet = m_pSocket->Recv((void *)&header, sizeof(header));
			if (nRet != sizeof(header))
			{
				if (m_pSocket->Valid() == true)
				{
					return false;
				}else
				{
					return false;
				}
			}

			header.cmd = ntohl(header.cmd);
			header.length = ntohl(header.length);
			if (header.length > m_nRecvLen)
			{
				if (m_pRecv)
				{
					delete [] m_pRecv;
					m_pRecv = NULL;
				}
				m_pRecv = new char[header.length + 1];
				m_nRecvLen = header.length + 1;
			}
			
			s32 nRetBody = m_pSocket->Recv((void *)m_pRecv, header.length);
			if (nRetBody == header.length)
			{
				
				switch(header.cmd)
				{
					case OAPI_LOGIN_RSP:
					{
						oapi::LoginRsp rsp;
						pClient.ParseLogin(m_pRecv, header.length, rsp);
						if (rsp.bRetNonce == true)
						{
							pClient.Setup(m_stor.struser(), 
									m_stor.strpasswd(), rsp.Nonce);
						}
						if (rsp.bRet == true)
						{
							m_bConnected = true;
							return true;
						}
						break;
					}
					default:
						break;		
				}
			}

		}

	}
	catch( XSDK::XException& ex )
	{
		
	}

	return false;

}

#endif /* __VSC_STOR_SYNC_INF_IMPL_H__ */