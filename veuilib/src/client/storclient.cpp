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
#include "client/storclient.hpp"

StorClient::StorClient(VidStor &stor, StorFactoryNotifyInterface &pNotify)
:m_stor(stor), m_pNotify(pNotify),
m_Quit(false), StorWebSocketClient(stor.strip(), stor.strport(), LINK_PROTO_WS_PATH)
{
	Login(stor.struser(), stor.strpasswd(), "Nonce");
}

StorClient::~StorClient()
{

}

bool StorClient::ProcessRecvMsg(char *data, size_t data_len)
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
		case Link::LINK_CMD_CAM_LIST_RESP:
		{
			return ProcessCamListResp(cmd);
			break;
		}
		case Link::LINK_CMD_CAM_ADD_NOTIFY:
		{
			StorFactoryChangeData data;
			if (cmd.has_camaddnotify())
			{
				const LinkCamAddNotify& pCam =  cmd.camaddnotify();
				
				data.cId.set_strcameraid(pCam.ccam().strid());
				data.cCam = pCam.ccam();
				data.type = STOR_FACTORY_CAMERA_ADD;
				m_pNotify.CallChange(data);
			}
			return true;
			break;
		}
		case Link::LINK_CMD_CAM_DEL_NOTIFY:
		{
			StorFactoryChangeData data;
			if (cmd.has_camidnotify())
			{
				const LinkCamIdNotify& pCam =  cmd.camidnotify();
				
				data.cId.set_strcameraid(pCam.strid());
				data.type = STOR_FACTORY_CAMERA_DEL;
				m_pNotify.CallChange(data);
			}
			return true;
			break;
		}
		case Link::LINK_CMD_CAM_ONLINE_NOTIFY:
		{
			StorFactoryChangeData data;
			if (cmd.has_camidnotify())
			{
				const LinkCamIdNotify& pCam =  cmd.camidnotify();
				
				data.cId.set_strcameraid(pCam.strid());
				data.type = STOR_FACTORY_CAMERA_ONLINE;
				m_pNotify.CallChange(data);
			}
			return true;
			break;
		}
		case Link::LINK_CMD_CAM_OFFLINE_NOTIFY:
		{
			StorFactoryChangeData data;
			if (cmd.has_camidnotify())
			{
				const LinkCamIdNotify& pCam =  cmd.camidnotify();
				
				data.cId.set_strcameraid(pCam.strid());
				data.type = STOR_FACTORY_CAMERA_OFFLINE;
				m_pNotify.CallChange(data);
			}
			return true;
			break;
		}
		case Link::LINK_CMD_CAM_REC_ON_NOTIFY:
		{
			StorFactoryChangeData data;
			if (cmd.has_camidnotify())
			{
				const LinkCamIdNotify& pCam =  cmd.camidnotify();
				
				data.cId.set_strcameraid(pCam.strid());
				data.type = STOR_FACTORY_CAMERA_REC_ON;
				m_pNotify.CallChange(data);
			}
			return true;
			break;
		}
		case Link::LINK_CMD_CAM_REC_OFF_NOTIFY:
		{
			StorFactoryChangeData data;
			if (cmd.has_camidnotify())
			{
				const LinkCamIdNotify& pCam =  cmd.camidnotify();
				
				data.cId.set_strcameraid(pCam.strid());
				data.type = STOR_FACTORY_CAMERA_REC_OFF;
				m_pNotify.CallChange(data);
			}
			return true;
			break;
		}

		default:
		   	return true;
	};	
	m_msgId ++;
	m_lastCmd = cmd;
	return true;
}

bool StorClient::ProcessCamListResp(Link::LinkCmd &cmd)
{
	if (!cmd.has_camlistresp())
	{
		return false;
	}
	
	const LinkListCamResp& pResp =  cmd.camlistresp();

	XGuard guard(m_cMutex);
	
	UpdateVidCameraList(pResp.clist());
	StorFactoryChangeData data;
	data.cId.set_strstorid(m_stor.strid());
	data.type = STOR_FACTORY_STOR_ONLINE;
	m_bOnline = true;
	guard.Release();
	m_pNotify.CallChange(data);
	guard.Acquire();
	
	return true;
}

bool StorClient::ProcessOffline()
{
	XGuard guard(m_cMutex);
	
	StorFactoryChangeData data;
	data.cId.set_strstorid(m_stor.strid());
	data.type = STOR_FACTORY_STOR_OFFLINE;
	m_bOnline = true;
	guard.Release();
	m_pNotify.CallChange(data);
	guard.Acquire();

	return true;
}

bool StorClient::ProcessOnline()
{
	Login(m_stor.struser(), m_stor.strpasswd(), "Nonce");
	return true;
}

StorClientOnlineMap StorClient::GetVidCameraOnlineList()
{
	if (m_bOnline == false)
	{
		StorClientOnlineMap empty;
		return empty;
	}

	XGuard guard(m_cMutex);
	return m_CamOnlineList;
}

StorClientRecMap StorClient::GetVidCameraRecList()
{
	if (m_bOnline == false)
	{
		StorClientRecMap empty;
		return empty;
	}

	XGuard guard(m_cMutex);
	return m_CamRecList;
}


bool StorClient::GetCameraConf(astring strCameraId, VidCamera &pCam)
{
	if (m_bOnline == false)
	{
		return false;
	}

	XGuard guard(m_cMutex);

	int cameraSize = m_cCamList.cvidcamera_size();

	for (s32 i = 0; i < m_cCamList.cvidcamera_size(); i ++)
	{
		const VidCamera &cam = m_cCamList.cvidcamera(i);
		if (cam.strid() == strCameraId)
		{
			pCam = cam;
			return true;
		}
	}
	
	return false;
}

VidCameraList StorClient::GetVidCameraList()
{
	if (m_bOnline == false)
	{
		VidCameraList empty;
		return empty;
	}

	XGuard guard(m_cMutex);
	return m_cCamList;
}

astring StorClient::GetVidCameraName(astring strCam)
{
	astring empty;
	
	if (m_bOnline == false)
	{
		return empty;
	}

	XGuard guard(m_cMutex);

	int cameraSize = m_cCamList.cvidcamera_size();

	for (s32 i = 0; i < m_cCamList.cvidcamera_size(); i ++)
	{
		const VidCamera &cam = m_cCamList.cvidcamera(i);
		if (cam.strid() == strCam)
		{
			return cam.strname();
		}
	}

	return empty;
	
}

bool StorClient::AddCam(VidCamera &pParam)
{
	XGuard guard(m_cMutex);

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

	return true;
}

bool StorClient::DeleteCam(astring strId)
{
	XGuard guard(m_cMutex);

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
	return true;
}

void StorClient::UpdateVidCameraList(const VidCameraList &list)
{
	/* Clean the list */
	VidCameraList empty;
	m_cCamList = list;

	int cameraSize = list.cvidcamera_size();

	for (s32 i = 0; i < list.cvidcamera_size(); i ++)
	{
		VidCamera cam = list.cvidcamera(i);
		m_CamOnlineList[cam.strid()] = cam.bonline();
		m_CamRecList[cam.strid()] = cam.brec();
	}

	return;
}

bool StorClient::SearchEvent(astring strId, s64 nStart, s64 nEnd)
{
	if (m_bOnline == false)
	{
		return false;
	}

#if 0
	OAPIClient pClient(m_pSocket);

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	return pClient.SearchEvent(strId, nStart, nEnd);
#endif
	return true;
}

bool StorClient::RegRealEvent()
{
	if (m_bOnline == false)
	{
		return false;
	}
#if 0
	OAPIClient pClient(m_pSocket);

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	return pClient.RegRealEvent();
#endif 
	return true;
}
bool StorClient::UnRegRealEvent()
{
	if (m_bOnline == false)
	{
		return false;
	}
#if 0
	OAPIClient pClient(m_pSocket);

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	return pClient.UnRegRealEvent();
#endif
	return true;
}

bool StorClient::HandleEvent(astring strId)
{
	if (m_bOnline == false)
	{
		return false;
	}

#if 0
	OAPIClient pClient(m_pSocket);

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	return pClient.HandleEvent(strId);
#endif
	return true;
}


bool StorClient::PtzCmd(astring strId, u32 action, double param)
{
	if (m_bOnline == false)
	{
		return false;
	}


#if 0
	XGuard guard(m_cMutex);

	/* Send del cam command */
	OAPIClient pClient(m_pSocket);

	/* Send add cam command  */
	return pClient.PtzCmd(strId, action, param);
#endif
	return true;
}

bool StorClient::GetOnline()
{
	return m_bOnline;
}

void StorClient::run()
{
#if 0 
	OAPIHeader header;
	int frameCnt =0;
	char *pRecv = NULL;
	int nRecvLen = 0;
	s32 nRet = 0;
	ck_string ckPort = m_stor.strport();

	u16 Port = ckPort.to_uint16(10);
	XGuard guard(m_cMutex);
	guard.Release();
	//VDC_DEBUG( "%s  %d \n",__FUNCTION__, __LINE__);	   
	while(m_Quit != true)
	{
		ve_sleep(1000);
		guard.Acquire();
		m_bOnline = false;
		StorFactoryChangeData data;
		data.cId.set_strstorid(m_stor.strid());
		data.type = STOR_FACTORY_STOR_OFFLINE;
		guard.Release();
		m_pNotify.CallChange(data);
		guard.Acquire();
		//VDC_DEBUG( "%s  %d \n",__FUNCTION__, __LINE__);	 
		try
		{
			XSDK::XString host = m_stor.strip();
			
			m_pSocket->Connect(host, Port);
			//VDC_DEBUG( "%s  %d \n",__FUNCTION__, __LINE__);	 
			oapi::OAPICameraListRsp list;
			OAPIClient pClient(m_pSocket);
			
			pClient.Setup(m_stor.struser(), m_stor.strpasswd(), "Nonce");
			
			//VDC_DEBUG( "%s  %d \n",__FUNCTION__, __LINE__);	 
			//m_pSocket->SetRecvTimeout(1 * 300);
			while(m_Quit != true)
			{
				guard.Release();
				nRet = m_pSocket->Recv((void *)&header, sizeof(header));
				guard.Acquire();
				if (nRet != sizeof(header))
				{
					if (m_pSocket->Valid() == true)
					{
						//VDC_DEBUG( "%s  %d \n",__FUNCTION__, __LINE__);	 
						/* Have not recevice any data */
						ve_sleep(200);
						continue;
					}else
					{
						m_pSocket->Close();
						break;
					}
				}
				//VDC_DEBUG( "%s  %d \n",__FUNCTION__, __LINE__);	 
				//printf("%s---%d\n", __FILE__, __LINE__);

				header.cmd = ntohl(header.cmd);
				header.length = ntohl(header.length);
				if (header.length > nRecvLen)
				{
					if (pRecv)
					{
						delete [] pRecv;
						pRecv = NULL;
					}

					/* Some thing has break */
					if (header.length > 1024 * 1024 * 8)
					{
						m_pSocket->Close();
						break;
					}
					pRecv = new char[header.length + 1];
					nRecvLen = header.length + 1;
				}
				
				s32 nRetBody = m_pSocket->Recv((void *)pRecv, header.length);
				if (nRetBody == header.length)
				{
					
					switch(header.cmd)
					{
						case OAPI_CAM_LIST_RSP:
						{
							oapi::OAPICameraListRsp list;
							pClient.ParseDeviceList(pRecv, header.length, list);
							UpdateVidCameraList(list);
							StorFactoryChangeData data;
							data.cId.set_strstorid(m_stor.strid());
							data.type = STOR_FACTORY_STOR_ONLINE;
							m_bOnline = true;
							guard.Release();
							m_pNotify.CallChange(data);
							guard.Acquire();
							
							break;
						}
						case OAPI_LOGIN_RSP:
						{
							oapi::LoginRsp rsp;
							pClient.ParseLogin(pRecv, header.length, rsp);
							if (rsp.bRetNonce == true)
							{
								pClient.Setup(m_stor.struser(), 
										m_stor.strpasswd(), rsp.Nonce);
							}
							if (rsp.bRet == true)
							{
								/* login ok, send device list */
								pClient.SendDeviceListRequest();
								/* Send the device notify */
								pClient.SendRegNotifyRequest();
								/* Send reg event notify */
								pClient.RegRealEvent();
							}
							break;
						}
						case OAPI_FRAME_PUSH:
						{
							//printf("Go a new frame %d\n", frameCnt++);
							break;
						}
						case OAPI_NOTIFY_DEVICE_ADD:
						{
							oapi::OAPICamAddNotify cam;
							pClient.ParseDevice(pRecv, header.length, cam);
							StorFactoryChangeData data;
							data.cId.set_strstorid(m_stor.strid());
							data.type = STOR_FACTORY_CAMERA_ADD;
							OAPIConverter::Converter(cam.cam, data.cCam);
							data.cId.set_strcameraid(data.cCam.strid());
							guard.Release();
							m_pNotify.CallChange(data);
							guard.Acquire();
							pClient.SendDeviceListRequest();
							m_bOnline = true;
							break;
						}
						case OAPI_NOTIFY_DEVICE_DEL:
						{
							astring strId;
							pClient.ParseDeviceStrId(pRecv, header.length, strId);
							StorFactoryChangeData data;
							data.cId.set_strstorid(m_stor.strid());
							data.type = STOR_FACTORY_CAMERA_DEL;
							data.cId.set_strcameraid(strId);
							
							guard.Release();
							m_pNotify.CallChange(data);
							guard.Acquire();
							pClient.SendDeviceListRequest();
							break;
						}
							break;
						case OAPI_NOTIFY_DEVICE_ONLINE:
						{
							astring strId;
							pClient.ParseDeviceStrId(pRecv, header.length, strId);
							StorFactoryChangeData data;
							data.cId.set_strstorid(m_stor.strid());
							data.type = STOR_FACTORY_CAMERA_ONLINE;
							data.cId.set_strcameraid(strId);
							
							guard.Release();
							m_pNotify.CallChange(data);
							guard.Acquire();
							//pClient.SendDeviceListRequest();
							break;
						}
						case OAPI_NOTIFY_DEVICE_OFFLINE:
						{
							astring strId;
							pClient.ParseDeviceStrId(pRecv, header.length, strId);
							StorFactoryChangeData data;
							data.cId.set_strstorid(m_stor.strid());
							data.type = STOR_FACTORY_CAMERA_OFFLINE;
							data.cId.set_strcameraid(strId);
							
							guard.Release();
							m_pNotify.CallChange(data);
							guard.Acquire();
							break;
						}
						case OAPI_NOTIFY_DEVICE_REC_ON:
						{
							astring strId;
							pClient.ParseDeviceStrId(pRecv, header.length, strId);
							StorFactoryChangeData data;
							data.cId.set_strstorid(m_stor.strid());
							data.type = STOR_FACTORY_CAMERA_REC_ON;
							data.cId.set_strcameraid(strId);
							
							guard.Release();
							m_pNotify.CallChange(data);
							guard.Acquire();
							//VDC_DEBUG( "STOR_FACTORY_CAMERA_REC_ON \n");
							break;
						}
						case OAPI_NOTIFY_DEVICE_REC_OFF:
						{
							astring strId;
							pClient.ParseDeviceStrId(pRecv, header.length, strId);
							StorFactoryChangeData data;
							data.cId.set_strstorid(m_stor.strid());
							data.type = STOR_FACTORY_CAMERA_REC_OFF;
							data.cId.set_strcameraid(strId);
							
							guard.Release();
							m_pNotify.CallChange(data);
							guard.Acquire();
							//pClient.SendDeviceListRequest();
							//VDC_DEBUG( "STOR_FACTORY_CAMERA_REC_OFF \n");
							break;
						}
						case OAPI_EVENT_PUSH:
						{
							oapi::OAPIEventNotify event;
							pClient.ParseEventNotify(pRecv, header.length, event);
							VidEvent cEvent;
							cEvent.set_strid(event.strId);
							cEvent.set_strdevice(event.strDevice);
							cEvent.set_strdevicename(event.strDeviceName);
							cEvent.set_ntime(event.nTime);
							cEvent.set_strtime(event.strTime);
							cEvent.set_strtype(event.strType);
							cEvent.set_strdesc(event.strDesc);
							cEvent.set_bhandled(event.bHandled);
							cEvent.set_strcomments(event.strComments);
							cEvent.set_bsearched(event.bSearched);
							
							guard.Release();
							m_pNotify.OnEvent(cEvent, m_stor);
							guard.Acquire();
							//VDC_DEBUG( "%s  %d \n",__FUNCTION__, __LINE__);
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
		{
			m_pSocket->Close();
			m_pSocket.Clear();
			/* Create a new socket for reconnect */
			m_pSocket = new XSocket;
		}

		guard.Release();

	}
	if (pRecv)
	{
		delete[] pRecv;
		pRecv = NULL;
	}
#endif

}

