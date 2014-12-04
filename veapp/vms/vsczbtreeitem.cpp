
#include "vsczbtreeitem.h"
#include "factory.hpp"
#include "QColor"
#include <QEventLoop>

extern Factory *gFactory;

VSCQMimeDataIPC::VSCQMimeDataIPC()
{

}

VSCZbTreeItemNVR::VSCZbTreeItemNVR(QTreeWidgetItem *parent)
    : QTreeWidgetItem(parent)
{

}

void VSCZbTreeItemNVR::mousePressEvent(QMouseEvent *event)
{
	//VDC_DEBUG( "%s \n",__FUNCTION__);
}

/*VSCZbTreeItemIPC::VSCZbTreeItemIPC(QTreeWidgetItem *parent, u32 TreeItemIPCid , QString TreeItemIPCip, QString ServerIp)
    : QTreeWidgetItem(parent),m_Param.m_Conf.data.conf.nId(TreeItemIPCid),
	m_Param.m_Conf.data.conf.IP(TreeItemIPCip),mServerIp(ServerIp)//hjy
{

}*/

VSCZbTreeItemIPC::VSCZbTreeItemIPC(QTreeWidgetItem *parent, DeviceParam &pParam, QString ServerIp)
	: QTreeWidgetItem(parent),mServerIp(ServerIp)
{
	m_Param = pParam;
}

void VSCZbTreeItemIPC::GetResult(const QJsonObject& json)
{
	qDebug() << QString("DeleteTreeItemIPC GetResult!");
}

void VSCZbTreeItemIPC::VSCZbIPCUpdateOnline(BOOL bonline)//由IPC是否在线对显示的IPC名字颜色进行修改
{
	if (bonline == TRUE)
	{
	    this->setTextColor(0, QColor(0, 170, 0));
	      //this->setTextColor(0, QColor(205, 0, 0));
	      //this->setTextColor(0, QColor(255, 0, 0));
	      //VDC_DEBUG( "%s Set to Green %d\n",__FUNCTION__, m_Param.m_Conf.data.conf.nId);
	}else
	{
	    this->setTextColor(0, QColor(194, 194, 194));
	      //VDC_DEBUG( "%s Set to black %d\n",__FUNCTION__, m_Param.m_Conf.data.conf.nId);
	}
}

void VSCZbTreeItemIPC::VSCZbIPCRecord()
{
	//TODO rest
}

void VSCZbTreeItemIPC::VSCZbIPCStopRecord()
{
	//TODO rest
}

void VSCZbTreeItemIPC::VSCZbIPCUpdateRecord(BOOL bRecording)//由IPC是否在录制对IPC的图标的颜色进行修改
{
	if (bRecording == TRUE)
    {
       QIcon icon1;
       //icon1.addFile(QStringLiteral(":/device/resources/camera-record.png"), QSize(), QIcon::Normal, QIcon::Off);
       icon1.addFile(QStringLiteral(":/device/resources/dome-record.png"), QSize(), QIcon::Normal, QIcon::Off);

       setIcon(0, icon1);
     }
	else
    {
       QIcon icon1;
       icon1.addFile(QStringLiteral(":/device/resources/dome.png"), QSize(), QIcon::Normal, QIcon::Off);
       setIcon(0, icon1);		
    }
}

BOOL VSCZbTreeItemIPC:: DeleteTreeItemIPC()
{

	return TRUE;
}

void VSCZbTreeItemIPC::mousePressEvent(QMouseEvent *event)
{
	//VDC_DEBUG( "%s \n",__FUNCTION__);
}



