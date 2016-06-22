
#ifndef __VSC_STOR_STREAM_IMPL_H_
#define __VSC_STOR_STREAM_IMPL_H_

inline StorStream::StorStream(VidStor &stor, astring strId, unsigned int nStream, 
		bool bPlayback, u32 nPlaytime)
:m_strId(strId), m_nStream(nStream), m_pCallback(NULL), m_pParam(NULL), 
m_Quit(false), m_pSocket(new XSocket), m_stor(stor), m_nLastTime(0), 
m_bPlayback(bPlayback), m_nPlaytime(nPlaytime), m_bOnline(false), m_bPbPause(false)
{
	m_play.Init(FALSE, "fake", "fake", "fake", false, VSC_CONNECT_TCP);
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}
inline StorStream::~StorStream()
{
	if (m_bPlayback == true)
	{
		StopPlayback();
	}
	
}

inline bool StorStream::PausePlayback()
{
	if (m_bOnline == false || m_bPlayback == false)
	{
		return false;
	}

	OAPIClient pClient(m_pSocket);

	XGuard guard(m_cMutex);
	m_bPbPause = true;
	/* Send add cam command  */
	return pClient.PausePlayback(m_strId);
}
inline bool StorStream::ResumePlayback()
{
	if (m_bOnline == false || m_bPlayback == false)
	{
		return false;
	}
	OAPIClient pClient(m_pSocket);

	XGuard guard(m_cMutex);
	m_bPbPause = false;
	/* Send add cam command  */
	return pClient.ResumePlayback(m_strId);
}

inline bool StorStream::SeekPlayback(u32 nPlaytime)
{	
	if (m_bOnline == false || m_bPlayback == false)
	{
		return false;
	}
	OAPIClient pClient(m_pSocket);

	XGuard guard(m_cMutex);
	m_nPlaytime = nPlaytime;
	/* Send add cam command  */
	return pClient.SeekPlayback(m_strId, nPlaytime);
}

inline bool StorStream::StopPlayback()
{
	if (m_bOnline == false || m_bPlayback == false)
	{
		return false;
	}

	OAPIClient pClient(m_pSocket);

	m_bPlayback = false;

	XGuard guard(m_cMutex);
	/* Send add cam command  */
	return pClient.StopPlayback(m_strId);
}

inline bool StorStream::StartStorStream()
{
	start();
	return true;
}
inline bool StorStream::StopStorStream()
{
	m_Quit = true;
	return true;
}
inline bool StorStream::RegDataCallback(CameraDataCallbackFunctionPtr pCallback, 
				void * pParam)
{
	XGuard guard(m_cMutex);
	m_pCallback = pCallback;
	m_pParam = pParam;

	return true;
}
inline bool StorStream::UnRegDataCallback()
{
	XGuard guard(m_cMutex);
	m_pCallback = NULL;
	m_pParam = NULL;	

	return true;
	
}

inline bool StorStream::EnableMot(HWND hWnd, bool bEnable)
{
	m_play.EnableMot(hWnd, bEnable);
	return true;
}

inline bool StorStream::AttachWidget(HWND hWnd, int w, int h, RenderType render)
{
	m_play.AttachWidget(hWnd, w, h, render);

	return true;
}
inline bool StorStream::UpdateWidget(HWND hWnd, int w, int h)
{
	m_play.UpdateWidget(hWnd, w, h);
	return true;
}
inline bool StorStream::DetachWidget(HWND hWnd)
{
	m_play.DetachWidget(hWnd);
	return true;
}

inline bool StorStream::GetStreamInfo(VideoStreamInfo &pInfo)
{
	return m_play.GetStreamInfo(pInfo);
}


inline void StorStream::run()
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
		ve_sleep(1000);
		guard.Acquire();
		m_bOnline = false;
		guard.Release();

		guard.Acquire();
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
						//printf("%s---%d Receive error\n", __FILE__, __LINE__);
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
								/* login ok, send view or the playback */
								if (m_bPlayback == true)
								{
									pClient.StartPlayback(m_strId, m_nPlaytime);
									if (m_bPbPause == false)
									{
										pClient.ResumePlayback(m_strId);
									}
								}else
								{
									pClient.StartLiveview(m_strId, m_nStream);
								}
								m_bOnline = true;
							}
							break;
						}
						case OAPI_FRAME_PUSH:
						{
							//printf("Go a new frame %d\n", frameCnt++);
							VideoFrameHeader * frameHeader = (VideoFrameHeader *)pRecv;
							VideoFrame pFrame;
							pFrame.streamType = (VideoStreamType)ntohl(frameHeader->streamType);
							pFrame.frameType = (VideoFrameType)ntohl(frameHeader->frameType);
							pFrame.secs = ntohl(frameHeader->secs);
							pFrame.msecs = ntohl(frameHeader->msecs);
							pFrame.dataBuf = (u8   *)(pRecv + sizeof(VideoFrameHeader));
							pFrame.dataLen = header.length - sizeof(VideoFrameHeader);
							pFrame.bufLen = pFrame.dataLen;
							if (m_pCallback != NULL)
							{
								m_pCallback(pFrame, m_pParam);
							}else
							{
								guard.Release();
								if (m_nLastTime == 0 || pFrame.secs != m_nLastTime)
								{
									emit SignalPlayTime(m_nLastTime);
									m_nLastTime = pFrame.secs;
								}
								m_play.PutRawData(pFrame);
								guard.Acquire();
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
		guard.Release();

	}

}

#endif /* __VSC_STOR_STREAM_IMPL_H_ */
