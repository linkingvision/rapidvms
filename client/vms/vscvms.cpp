#include "vscvms.h"
#include "factory.hpp"
#include "QColor"
#include <QTranslator>
#include <QtWidgets/QApplication>
#include <vscdevicetree.h>
#include <QEventLoop>
#include "vscdeviceipc.h"

#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "oapic.hpp"

using namespace cppkit;
using namespace std;


extern Factory *gFactory;

VSCVms::VSCVms(QTreeWidgetItem *parent, VSCVmsDataItem &pParam)
    : QTreeWidgetItem(parent), m_pParent(parent)
{
	qRegisterMetaType<oapi::DeviceListRsp>("oapi::DeviceListRsp");     
	memcpy(&m_Param, &pParam, sizeof(VSCVmsDataItem));
}

void VSCVms::mousePressEvent(QMouseEvent *event)
{

}



VSCVms::~VSCVms()
{

}

VSCVmsOAPI::VSCVmsOAPI(QTreeWidgetItem *parent, VSCVmsDataItem &pParam)
: VSCVms(parent, pParam), m_pThread(NULL), m_loading(NULL)
{
	m_pThread = new VSCVMSOAPIThread(pParam);
	connect(m_pThread, SIGNAL(UpdateDeviceListRsp(oapi::DeviceListRsp)), this,
						SLOT(UpdateDeviceListRsp(oapi::DeviceListRsp)));	
	m_pThread->start();
}

VSCVmsOAPI::~VSCVmsOAPI()
{
	m_pThread->setQuit();
}

BOOL VSCVmsOAPI::Refresh()
{
	if (m_loading)
	{
		delete m_loading;
		m_loading = NULL;
	}
	m_loading = new VSCLoading(NULL);
	m_pThread->setQuit();

	qDeleteAll(this->takeChildren());

	m_pThread = new VSCVMSOAPIThread(m_Param);
	connect(m_pThread, SIGNAL(UpdateDeviceListRsp(oapi::DeviceListRsp)), this,
						SLOT(UpdateDeviceListRsp(oapi::DeviceListRsp)));	
	m_pThread->start();
	return TRUE;	
}

void VSCVmsOAPI::mousePressEvent(QMouseEvent *event)
{

}

void VSCVmsOAPI::UpdateDeviceListRsp(oapi::DeviceListRsp plist)
{
	printf("UpdateDeviceListRsp ==============\n");

	std::vector<oapi::Device>::iterator iter; 
	for (iter = plist.list.begin(); iter != plist.list.end(); iter++)
	{  
		AddIPCamera(*iter, this);
	} 
	if (m_loading)
	{
		delete m_loading;
		m_loading = NULL;
	}
}

void VSCVmsOAPI::AddIPCamera(oapi::Device &pParam, QTreeWidgetItem *qtreewidgetitem)
{
    //QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);
    QIcon icon1;
    icon1.addFile(QStringLiteral(":/device/resources/dome.png"), QSize(), QIcon::Normal, QIcon::Off);

    QTreeWidgetItem *qtreewidgetitemChild = new VSCDeviceIPCOAPI(qtreewidgetitem, pParam, 
				m_Param);

    qtreewidgetitemChild->setIcon(0, icon1);

    qtreewidgetitemChild->setText(0, QApplication::translate("",
            pParam.Name.c_str(), 0));

    qtreewidgetitem->setExpanded(true);
#if 0
    VSCDeviceIPC *pIPC = dynamic_cast<VSCDeviceIPC*>(qtreewidgetitemChild);
    if (pParam.m_Conf.data.conf.Recording == 1)
    {
		pIPC->UpdateRecord(TRUE);
    }else
    {
    	pIPC->UpdateRecord(FALSE);
    }
#endif
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

	while(m_Quit != TRUE)
	{
		try
		{
			XSDK::XString host = m_pParam.IP;
			pSocket->Connect(host, Port);
			
			oapi::DeviceListRsp list;
			OAPIClient pClient(pSocket);
			
			pClient.Setup(m_pParam.User, m_pParam.Password, "Nonce");
			
	
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
				//printf("%s---%d\n", __FILE__, __LINE__);

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
							oapi::DeviceListRsp list;
							pClient.ParseDeviceList(pRecv, header.length, list);
							emit UpdateDeviceListRsp(list);
							break;
						}
						case OAPI_CMD_LOGIN_RSP:
						{
							oapi::LoginRsp rsp;
							pClient.ParseLogin(pRecv, header.length, rsp);
							if (rsp.bRetNonce == true)
							{
								pClient.Setup(m_pParam.User, 
										m_pParam.Password, rsp.Nonce);
							}
							if (rsp.bRet == true)
							{
								/* login ok, send device list */
								pClient.SendDeviceListRequest();
							}
							break;
						}
						case OAPI_CMD_FRAME_PUSH:
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

	}

}

