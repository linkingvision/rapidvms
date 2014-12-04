#include "vscvms.h"
#include "factory.hpp"
#include "QColor"
#include "vsczbtreeitem.h"
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
	//VDC_DEBUG( "%s \n",__FUNCTION__);
}



VSCVms::~VSCVms()
{

}

VSCVmsPg::VSCVmsPg(QTreeWidgetItem *parent, VSCVmsDataItem &pParam)
    : VSCVms(parent, pParam)
{

}

void VSCVmsPg::mousePressEvent(QMouseEvent *event)
{
	//VDC_DEBUG( "%s \n",__FUNCTION__);
}


VSCVmsPg::~VSCVmsPg()
{

}


VSCVmsZb::VSCVmsZb(QTreeWidgetItem *parent, VSCVmsDataItem &pParam)
    : VSCVms(parent, pParam)
{




	
}

void VSCVmsZb::printNVRList(const QJsonObject& json)
{
}

BOOL VSCVmsZb::Refresh()
{
	return TRUE;	
}


void VSCVmsZb::ShowRefresh(const QJsonObject& json)
{
	return ;	
}

void VSCVmsZb::mousePressEvent(QMouseEvent *event)
{

}

BOOL VSCVmsZb::GetRecorderMap(RecorderMap & pMap)
{
	//pMap = mMap;
	return true;
}

VSCVmsZb::~VSCVmsZb()
{

}
