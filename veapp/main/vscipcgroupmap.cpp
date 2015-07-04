#include "vscipcgroupmap.h"
#include "factory.hpp"
#include <QResizeEvent>
#include <QMimeData>
#include <QDrag>
#include <QAction>
#include <QDesktopWidget>
#include <QApplication>
#include <QDateTime>
#include <QTime>
#include <time.h>
#include <QStyleOption>
#include <QPainter>
#include <QScrollArea>
#include "vscviewtree.h"
#include "vscvwidget.h"

extern Factory *gFactory;

VSCVGroupMap::VSCVGroupMap(VSCVGroupDataItem &pParam, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
	 ui.setupUi(this);
	 memcpy(&m_pParam, &pParam, sizeof(VSCVGroupDataItem));
	setAcceptDrops(true);
	setMouseTracking(true);
}

VSCVGroupMap::~VSCVGroupMap()
{

}

void VSCVGroupMap::dragEnterEvent(QDragEnterEvent *event)
{
    VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__, m_pParam.nId);
    event->accept();
}

void VSCVGroupMap::dragMoveEvent(QDragMoveEvent *event)
{
    VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__, m_pParam.nId);
    event->accept();
}

void VSCVGroupMap::dropEvent(QDropEvent *event)
{
	QMimeData * pMime = (QMimeData *)event->mimeData();
	VSCQMimeView *myData =
	 dynamic_cast<VSCQMimeView * > (pMime);
	if (myData) 
	{
		VDC_DEBUG( "%s View in dropEvent id %d\n",__FUNCTION__, myData->nId);
		return;
	}
	VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__, m_pParam.nId);
	if (event->mimeData()->hasText() == false)
	{
		return;
	}

	s32 nId = atoi(event->mimeData()->text().toLatin1().data());
	if (nId < VWIDGET_ID_OFFSET)
	{
		//add this id to this group
		gFactory->UpdateDeviceGroup(nId, m_pParam.nId);
		return;
	}
    	return;
}



VSCVGroupWall::VSCVGroupWall(QWidget *parent)
: QWidget(parent)
{
	m_pArea = new QScrollArea(this);
	m_pArea->setWidgetResizable(true);
	m_pArea->setBackgroundRole(QPalette::Light);
	// Fake up a grid to scroll
	QWidget *client = new QWidget(this);
	m_pLayout = new QGridLayout(client);
	client->setLayout(m_pLayout);
	
	
	m_pLayout->setSpacing(0);
	m_pLayout->setMargin(0);
	m_pLayout->setVerticalSpacing(0);
	m_pLayout->setHorizontalSpacing(0);

	m_pArea->setWidget(client);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(m_pArea);
	setLayout(layout);
	layout->setMargin(0);
	s32 cnt  = 0;
	
	/* loop to add the group to wall */
	
	VSCVGroupData pData;
	VSCVGroupDataItem pDefault;
	/* Add default group */
	memset(&pDefault, 0, sizeof(VSCVGroupDataItem));
	VSCVGroupMap *pMapDefault = new VSCVGroupMap(pDefault, this);
	pMapDefault->ui.name->setText("Default");
	m_pLayout->addWidget(pMapDefault, cnt/4, cnt%4);
	cnt ++;
	
	gFactory->GetVGroup(pData);
	for (s32 i = 1; i < CONF_VGROUP_NUM_MAX; i ++)
	{
	    	if (pData.data.conf.group[i].Used == 1)
	    	{
	    		VSCVGroupMap *pMap = new VSCVGroupMap(pData.data.conf.group[i], this);
			pMap->ui.name->setText(pData.data.conf.group[i].Name);
			m_pLayout->addWidget(pMap, cnt/4, cnt%4);
			cnt ++;
	    	}else
	    	{
	    		continue;
	    	}
	}
	
}

VSCVGroupWall::~VSCVGroupWall()
{

}


