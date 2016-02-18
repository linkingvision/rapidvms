#include "oapic.hpp"
int main(int argc, char *argv[])
{
	XRef<XSocket> pSocket = new XSocket;

	OAPIHeader header;
	int frameCnt =0;
	char *pRecv = NULL;
	int nRecvLen = 0;
	s32 nRet = 0;
	
	try
	{
		XSDK::XString host = "127.0.0.1";
		pSocket->Connect(host, 9080);
		
		OAPIClient pClient(pSocket);
		
		pClient.Setup("admin", "admin");
		pClient.SendDeviceListRequest();
		pClient.StartLiveview(2);
		pSocket->SetRecvTimeout(1 * 1000);
		while(1)
		{
			nRet = pSocket->Recv((void *)&header, sizeof(header));
			if (nRet != sizeof(header))
			{
				if (pSocket->Valid() == true)
				{
					continue;
				}else
				{
					break;
				}
			}
			//TODO check if return 0

			header.version = ntohl(header.version);
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
			
			s32 nRetBody = pSocket->Recv((void *)pRecv, header.length);
			if (nRetBody == header.length)
			{
				switch(header.cmd)
				{
					case OAPI_CAM_LIST_RSP:
					{
						oapi::DeviceListReq list;
						pClient.ParseDeviceList(pRecv, header.length, list);
						break;
					}
					case OAPI_FRAME_PUSH:
					{
						printf("Go a new frame %d\n", frameCnt++);
						break;
					}
					case OAPI_NOTIFY_DEVICE_ADD:
						break;
					case OAPI_NOTIFY_DEVICE_DEL:
						break;
					case OAPI_NOTIFY_DEVICE_ONLINE:
						break;
					case OAPI_NOTIFY_DEVICE_OFFLINE:
						break;
					case OAPI_NOTIFY_DEVICE_RECORDING_ON:
						break;
					case OAPI_NOTIFY_DEVICE_RECORDING_OFF:
						break;
					case OAPI_NOTIFY_DEVICE_GROUP_CHANGE:
						break;
					default:
						break;		
				}
			}

		}

	}
	catch( XSDK::XException& ex )
	{
		
	}
	
	x_sleep(10000);
}