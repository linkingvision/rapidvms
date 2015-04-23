#include "vscvms.h"
#include "factory.hpp"
#include "QColor"
#include <QTranslator>
#include <QtWidgets/QApplication>
#include <vscdevicetree.h>
#include <QEventLoop>

#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "oapic.hpp"

using namespace cppkit;
using namespace std;


extern Factory *gFactory;

VSCVms::VSCVms(QTreeWidgetItem *parent, VSCVmsDataItem &pParam)
    : QTreeWidgetItem(parent), m_pParent(parent)
{
	memcpy(&m_Param, &pParam, sizeof(VSCVmsDataItem));
}

void VSCVms::mousePressEvent(QMouseEvent *event)
{

}



VSCVms::~VSCVms()
{

}

VSCVmsOAPI::VSCVmsOAPI(QTreeWidgetItem *parent, VSCVmsDataItem &pParam)
: VSCVms(parent, pParam), m_pThread(NULL)
{
	m_pThread = new VSCVMSOAPIThread(pParam);
	connect(m_pThread, SIGNAL(UpdateDeviceList(oapi::DeviceList)), this,
						SLOT(UpdateDeviceList(oapi::DeviceList)));	
	m_pThread->start();
}

VSCVmsOAPI::~VSCVmsOAPI()
{

}


void VSCVmsOAPI::printNVRList(const QJsonObject& json)
{
}

BOOL VSCVmsOAPI::Refresh()
{
	return TRUE;	
}


void VSCVmsOAPI::ShowRefresh(const QJsonObject& json)
{
	return ;	
}

void VSCVmsOAPI::mousePressEvent(QMouseEvent *event)
{

}

void VSCVmsOAPI::UpdateDeviceList(oapi::DeviceList plist)
{
	printf("UpdateDeviceList ==============\n");
}

void VSCVMSOAPIThread::run()
{
	XRef<XSocket> pSocket = new XSocket;

	OAPIHeader header;
	int frameCnt =0;
	char *pRecv = NULL;
	int nRecvLen = 0;
	s32 nRet = 0;
	ck_string ckPort = m_pParam.Port;

	u16 Port = ckPort.to_uint16(10);
	
	try
	{
		XSDK::XString host = m_pParam.IP;
		pSocket->Connect(host, Port);
		
		oapi::DeviceList list;
		OAPIClient pClient(pSocket);
		
		pClient.Setup(m_pParam.User, m_pParam.Password);
		pClient.SendDeviceListRequest();
		//pClient.StartLiveview(2);
		pSocket->SetRecvTimeout(1 * 1000);
		while(m_Quit != TRUE)
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
					case OAPI_CMD_DEVICE_LIST_RSP:
					{
						oapi::DeviceList list;
						pClient.ParseDeviceList(pRecv, header.length, list);
						break;
					}
					case OAPI_CMD_FRAME:
					{
						//printf("Go a new frame %d\n", frameCnt++);
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
	
	while(m_Quit != TRUE)
	{
		
	}
}

