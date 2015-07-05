#include "vscpanel.h"
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

VSCPanel::VSCPanel(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
	 ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);
	ui.scrollArea->setBackgroundRole(QPalette::Light);
}

VSCPanel::~VSCPanel()
{

}

void VSCPanel::dragEnterEvent(QDragEnterEvent *event)
{
    VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__, m_pParam.nId);
    event->accept();
}

void VSCPanel::dragMoveEvent(QDragMoveEvent *event)
{
    VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__, m_pParam.nId);
    event->accept();
}

void VSCPanel::dropEvent(QDropEvent *event)
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


