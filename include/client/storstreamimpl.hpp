
#ifndef __VSC_STOR_STREAM_IMPL_H_
#define __VSC_STOR_STREAM_IMPL_H_

inline StorStream::StorStream(VidStor &stor, astring strId, unsigned int nStream)
:m_strId(strId), m_nStream(nStream), m_pCallback(NULL), m_pParam(NULL), 
m_Quit(false), m_pSocket(new XSocket), m_stor(stor)
{
	m_play.Init(FALSE, " ", " ", " ");
}
inline StorStream::~StorStream()
{
	
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
inline bool StorStream::UnRegDataCallback(void * pParam)
{
	XGuard guard(m_cMutex);
	m_pCallback = NULL;
	m_pParam = NULL;	

	return true;
	
}

inline bool StorStream::AttachWidget(HWND hWnd, int w, int h)
{
	m_play.AttachWidget(hWnd, w, h);

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

	while(m_Quit != true)
	{
		ve_sleep(500);
		
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
						ve_sleep(200);
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
								pClient.StartLiveview(m_strId, m_nStream);
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
							m_play.PutRawData(pFrame);
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

	}

}

#endif /* __VSC_STOR_STREAM_IMPL_H_ */