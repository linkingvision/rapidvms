#include "vscvms.h"
#include "factory.hpp"
#include "QColor"
#include <QTranslator>
#include <QtWidgets/QApplication>
#include <vscdevicetree.h>
#include <QEventLoop>


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
: VSCVms(parent, pParam)
{
	
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

