
#ifndef __VSC_STOR_CLIENT_IMPL_H_
#define __VSC_STOR_CLIENT_IMPL_H_

inline StorClient::StorClient(VidStor &stor, StorFactoryNotifyInterface &pNotify)
:m_stor(stor), m_pNotify(pNotify), m_bOnline(false), m_pSocket(new XSocket), 
m_Quit(false)
{
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
	StartStorClient();
}

inline StorClient::~StorClient()
{
	StopStorClient();
}

/* Start Stop Refresh the Stor client Thread */
inline bool StorClient::StartStorClient()
{
	start();
	return true;
}
inline bool StorClient::StopStorClient()
{
	m_Quit = true;
	return true;
}

inline StorClientOnlineMap StorClient::GetVidCameraOnlineList()
{
	if (m_bOnline == false)
	{
		StorClientOnlineMap empty;
		return empty;
	}

	XGuard guard(m_cMutex);
	return m_CamOnlineList;
}

inline StorClientRecMap StorClient::GetVidCameraRecList()
{
	if (m_bOnline == false)
	{
		StorClientRecMap empty;
		return empty;
	}

	XGuard guard(m_cMutex);
	return m_CamRecList;
}

inline VidCameraList StorClient::GetVidCameraList()
{
	if (m_bOnline == false)
	{
		VidCameraList empty;
		return empty;
	}

	XGuard guard(m_cMutex);
	return m_cCamList;
}

inline astring StorClient::GetVidCameraName(astring strCam)
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

inline void StorClient::UpdateVidCameraList(oapi::OAPICameraListRsp list)
{
	
	XGuard guard(m_cMutex);
	/* Clean the list */
	VidCameraList empty;
	m_cCamList = empty;
	
	//list push_back
	std::vector<oapi::OAPICamera>::iterator it;
	for (it = list.list.begin(); it != list.list.end(); it ++)
	{
		VidCamera *pCam = m_cCamList.add_cvidcamera();
		OAPIConverter::Converter(*it, *pCam);
		m_CamOnlineList[(*it).strId] = (*it).bOnline;
		m_CamRecList[(*it).strId] = (*it).bRec;
	}

	return;
}

inline bool StorClient::SearchEvent(astring strId, s64 nStart, s64 nEnd)
{
	if (m_bOnline == false)
	{
		return false;
	}

	OAPIClient pClient(m_pSocket);

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	return pClient.SearchEvent(strId, nStart, nEnd);
}

inline bool StorClient::RegRealEvent()
{
	if (m_bOnline == false)
	{
		return false;
	}

	OAPIClient pClient(m_pSocket);

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	return pClient.RegRealEvent();
}
inline bool StorClient::UnRegRealEvent()
{
	if (m_bOnline == false)
	{
		return false;
	}

	OAPIClient pClient(m_pSocket);

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	return pClient.UnRegRealEvent();
}

inline bool StorClient::HandleEvent(astring strId)
{
	if (m_bOnline == false)
	{
		return false;
	}

	OAPIClient pClient(m_pSocket);

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	return pClient.HandleEvent(strId);

}

inline bool StorClient::AddCam(VidCamera &pParam)
{
	if (m_bOnline == false)
	{
		return false;
	}

	OAPIClient pClient(m_pSocket);

	oapi::OAPIAddCameraReq sCam;
	OAPIConverter::Converter(pParam, sCam.cam);

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	return pClient.AddCam(sCam);
}
inline bool StorClient::DeleteCam(astring strId)
{
	if (m_bOnline == false)
	{
		return false;
	}

	XGuard guard(m_cMutex);

	/* Send del cam command */
	OAPIClient pClient(m_pSocket);

	/* Send add cam command  */
	return pClient.DeleteCam(strId);
}

inline bool StorClient::PtzCmd(astring strId, u32 action, double param)
{
	if (m_bOnline == false)
	{
		return false;
	}

	XGuard guard(m_cMutex);

	/* Send del cam command */
	OAPIClient pClient(m_pSocket);

	/* Send add cam command  */
	return pClient.PtzCmd(strId, action, param);
}

inline bool StorClient::GetOnline()
{
	return m_bOnline;
}

inline void StorClient::run()
{
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

}


#endif /* __VSC_STOR_CLIENT_IMPL_H_ */
