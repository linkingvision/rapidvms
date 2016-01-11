
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
	}

	return;
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

	while(m_Quit != true)
	{
		ve_sleep(500);
		guard.Acquire();
		m_bOnline = false;
		
		try
		{
			XSDK::XString host = m_stor.strip();
			
			m_pSocket->Connect(host, Port);
			
			oapi::OAPICameraListRsp list;
			OAPIClient pClient(m_pSocket);
			
			pClient.Setup(m_stor.struser(), m_stor.strpasswd(), "Nonce");
			
	
			m_pSocket->SetRecvTimeout(1 * 300);
			while(m_Quit != true)
			{
				nRet = m_pSocket->Recv((void *)&header, sizeof(header));
				if (nRet != sizeof(header))
				{
					if (m_pSocket->Valid() == true)
					{
						/* Have not recevice any data */
						guard.Release();
						ve_sleep(200);
						guard.Acquire();
						continue;
					}else
					{
						m_pSocket->Close();
						break;
					}
				}
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
					pRecv = new char[header.length + 1];
					nRecvLen = header.length + 1;
				}
				
				s32 nRetBody = m_pSocket->Recv((void *)pRecv, header.length);
				if (nRetBody == header.length)
				{
					
					switch(header.cmd)
					{
						case OAPI_CMD_DEVICE_LIST_RSP:
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
						case OAPI_CMD_LOGIN_RSP:
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
								pClient.SendRegNotifyRequest();
							}
							break;
						}
						case OAPI_CMD_FRAME_PUSH:
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
							//pClient.SendDeviceListRequest();
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
							//pClient.SendDeviceListRequest();
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
							//pClient.SendDeviceListRequest();
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
							//pClient.SendDeviceListRequest();
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
							break;
						}
						default:
							break;		
					}

					guard.Acquire();
				}

			}

		}
		catch( XSDK::XException& ex )
		{
			
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
