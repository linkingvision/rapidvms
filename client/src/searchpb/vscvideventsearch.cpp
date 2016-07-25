#include "vscvideventsearch.h"
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
#include "main/vsceventconsole.h"


VSCVidEventSearch::VSCVidEventSearch(ClientFactory &pFactory, VSCVidTreeInf &pCamTree, QWidget *parent, Qt::WindowFlags flags)
    : m_pFactory(pFactory), m_pVideo(NULL), m_pCamTree(pCamTree), QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);
	/* A day before */
	m_startTime = time(NULL) - 60 * 60 * 24;
	QDateTime time1, time2;
	time1.setTime_t(m_startTime);
	time2.setTime_t(m_startTime + 60 * 60 * 24);

	m_endMax.setTime_t(m_startTime + 60 * 60 * 24);
	m_endMin.setTime_t(m_startTime + 60);

	ui.startTime->setDateTime(time1);
	ui.endTime->setDateTime(time2);

	ui.endTime->setMinimumDateTime(m_endMin);
	ui.endTime->setMaximumDateTime(m_endMax);

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
	connect(this, SIGNAL(SignalSectionClicked(int, int)), ui.tableWidget, SIGNAL(cellClicked(int, int)));
	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(SlotSectionClicked(int, int)));
	
	connect(ui.pbSearch, SIGNAL(clicked()), this, SLOT(SlotSearch()));
	connect(ui.startTime, SIGNAL(dateTimeChanged(QDateTime)), this, 
					SLOT(SlotStartDateTimeChanged(QDateTime)));
	ui.tableWidget->setColumnWidth(2, 120);
}

VSCVidEventSearch::~VSCVidEventSearch()
{

}

void VSCVidEventSearch::SlotEvent(std::string strEvent)
{	
	VidEvent cEvent;
	bool bRet = cEvent.ParseFromString(strEvent);
	/* Only display the search event */
	if (bRet == true && cEvent.bsearched() == true)
	{
		EventTableUpdate(cEvent);
	}
	return;
}

void VSCVidEventSearch::EventTableUpdate(VidEvent &cEvent)
{
	int nRowCnt = ui.tableWidget->rowCount();
	if (nRowCnt >= 1000)
	{
		/* Already got too many event, need change search time */
		return;
	}

	ui.tableWidget->insertRow(nRowCnt);
	QTableWidgetItem *pItem0 = new VidEventTableItem(cEvent, cEvent.strstorname().c_str());
	QTableWidgetItem *pItem1 = new QTableWidgetItem(cEvent.strdevicename().c_str());
	QTableWidgetItem *pItem2 = new QTableWidgetItem(cEvent.strtype().c_str());
	QTableWidgetItem *pItem3 = new QTableWidgetItem(cEvent.strtime().c_str());
	if (cEvent.bhandled() == false)
	{
		pItem0->setBackgroundColor(QColor(255,0,0));
		pItem1->setBackgroundColor(QColor(255,0,0));
		pItem2->setBackgroundColor(QColor(255,0,0));
		pItem3->setBackgroundColor(QColor(255,0,0));
	}
	ui.tableWidget->setItem(nRowCnt, 0, pItem0);
	ui.tableWidget->setItem(nRowCnt, 1, pItem1);
	ui.tableWidget->setItem(nRowCnt, 2, pItem2);
	ui.tableWidget->setItem(nRowCnt, 3, pItem3);
}

void VSCVidEventSearch::SlotSectionClicked(int row, int column)
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidEventTableItem *pEvent = dynamic_cast<VidEventTableItem * >(firstCheck);
	if (pEvent )
	{
		SetEventUI(pEvent->GetEvent());
		VidEvent cEvent = pEvent->GetEvent();
		/* Set this event handled */
		if (cEvent.bhandled() != true)
		{
			/* Send to server handled */
			m_pFactory.GetStorFactory().HandleEvent(cEvent.strstorid(), 
									cEvent.strid());
			/* Set handled in the map */
			cEvent.set_bhandled(true);
			pEvent->SetEvent(cEvent);
			ui.tableWidget->item(ui.tableWidget->currentRow(), 0)->setBackgroundColor(QColor(255,255,255));
			ui.tableWidget->item(ui.tableWidget->currentRow(), 1)->setBackgroundColor(QColor(255,255,255));
			ui.tableWidget->item(ui.tableWidget->currentRow(), 2)->setBackgroundColor(QColor(255,255,255));
			ui.tableWidget->item(ui.tableWidget->currentRow(), 3)->setBackgroundColor(QColor(255,255,255));
		}

		/* Start playplay video  */
		m_pVideo->StopPlay();
		m_pVideo->StartPlay(cEvent.strstorid(), cEvent.strdevice(), cEvent.strdevicename(), cEvent.ntime());
	}
	
}


void VSCVidEventSearch::SetEventUI(VidEvent pEvent)
{
	VDC_DEBUG("%s VSCVidEventSearch::SetEventUI \n", __FUNCTION__);
	ui.lineEditStorName->setText(pEvent.strstorname().c_str());
	ui.lineEditName->setText(pEvent.strdevicename().c_str());

	ui.lineEditType->setText(pEvent.strtype().c_str());
	ui.lineEditTime->setText(pEvent.strtime().c_str());

	ui.lineEditDesc->setText(pEvent.strdesc().c_str());
	//ui.lineEditMark->setText(pEvent.str().c_str());
	
}

void VSCVidEventSearch::SlotSearch()
{
	int nRowCnt = ui.tableWidget->rowCount();

	for (s32 j = nRowCnt - 1; j >= 0; j --)
	{
		ui.tableWidget->removeRow(j);
	}
	ui.tableWidget->clear();
	
	s64 nStartTime = ui.startTime->dateTime().toTime_t();
	s64 nEndTime = ui.endTime->dateTime().toTime_t();

	VidCameraIdMap selectCamMap;
	m_pCamTree.VidGetSelectedItems(selectCamMap);

	bool bGotSelected = false;
	astring strStorId;
	astring strId;
       VidCameraIdMap::iterator it = selectCamMap.begin(); 
       for(; it!=selectCamMap.end(); ++it)
	{
		VidCameraId camId;
		camId = (*it).second;
		strStorId = camId.strstorid();
		strId = camId.strcameraid();
		bGotSelected = true;
	}

	if (bGotSelected == true)
	{
		m_pFactory.GetStorFactory().SearchEvent(strStorId, strId, 
					nStartTime, nEndTime);
	}
}
void VSCVidEventSearch::SlotStartDateTimeChanged(const QDateTime &dateTime)
{
	m_endMax.setTime_t(dateTime.toTime_t() + 60 * 60 * 24);
	m_endMin.setTime_t(dateTime.toTime_t() + 60);

	ui.endTime->setMinimumDateTime(m_endMin);
	ui.endTime->setMaximumDateTime(m_endMax);	
}




