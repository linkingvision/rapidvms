#include "vsceventconsole.h"
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
#include "vtaskmgr.hpp"


VSCEventConsole::VSCEventConsole(ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
    : m_pFactory(pFactory), m_EventId(0), m_pVideo(NULL), QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);

	//connect(ui.pbTasklist, SIGNAL(clicked()), this, SLOT(UpdateTaskList()));
	
	QVBoxLayout* layout = new QVBoxLayout();
	m_pVideo = new VVidPB1(m_pFactory, ui.video);
	m_pVideo->hide();
	//layout->setSpacing(10);

	layout->addWidget(m_pVideo);
	layout->setMargin(0);

	this->ui.video->setLayout(layout);
	m_pVideo->show();

	connect(&(m_pFactory.GetStorFactory()), SIGNAL(SignalEvent(std::string)), 
		this, SLOT(SlotEvent(std::string)));
	ui.tableWidget->setColumnWidth(2, 120);
}

VSCEventConsole::~VSCEventConsole()
{

}

void VSCEventConsole::SlotEvent(std::string strEvent)
{	
	VidEvent cEvent;
	bool bRet = cEvent.ParseFromString(strEvent);

	if (bRet == true)
	{
		EventTableUpdate(cEvent);
	}
	return;
}

void VSCEventConsole::EventTableUpdate(VidEvent &cEvent)
{
	m_EventMap[m_EventId] = cEvent;

	int nRowCnt = ui.tableWidget->rowCount();

	for (s32 j = nRowCnt - 1; j >= 0; j --)
	{
		ui.tableWidget->removeRow(j);
	}
	ui.tableWidget->clear();

	s32 nMax = m_EventId;
	s32 nStart = 0;
	if (m_EventId > VSC_MAX_EVENT_NUM)
	{
		nMax = VSC_MAX_EVENT_NUM;
		nStart = m_EventId - VSC_MAX_EVENT_NUM;
	}

	for (s32 i = nMax; i >= nStart; i --)
	{
		VidEvent cEventCurrent = m_EventMap[i];
		int insertRow = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(insertRow);
		QTableWidgetItem *pItem0 = new QTableWidgetItem(cEventCurrent.strstorname().c_str());
		QTableWidgetItem *pItem1 = new QTableWidgetItem(cEventCurrent.strdevicename().c_str());
		QTableWidgetItem *pItem2 = new QTableWidgetItem(cEventCurrent.strtype().c_str());
		QTableWidgetItem *pItem3 = new QTableWidgetItem(cEventCurrent.strtime().c_str());
		pItem0->setBackgroundColor(QColor(255,0,0));
		pItem1->setBackgroundColor(QColor(255,0,0));
		pItem2->setBackgroundColor(QColor(255,0,0));
		pItem3->setBackgroundColor(QColor(255,0,0));
		ui.tableWidget->setItem(insertRow, 0, pItem0);
		ui.tableWidget->setItem(insertRow, 1, pItem1);
		ui.tableWidget->setItem(insertRow, 2, pItem2);
		ui.tableWidget->setItem(insertRow, 3, pItem3);
	}
	m_EventId ++;
	

	/* Erase the first one */
	if (m_EventId >= VSC_MAX_EVENT_NUM)
	{
		m_EventMap.erase(m_EventId - VSC_MAX_EVENT_NUM);
	}
}




