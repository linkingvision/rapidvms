/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
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
    : m_pFactory(pFactory), m_EventId(0), m_pVideo(NULL),m_bHolded(FALSE), 
    m_nLastUpdateTime(time(NULL)), QWidget(parent, flags)
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

	ui.pbHold->setText(tr("Hold"));

	connect(&(m_pFactory.GetStorFactory()), SIGNAL(SignalEvent(std::string)), 
		this, SLOT(SlotEvent(std::string)));
	connect(this, SIGNAL(SignalSectionClicked(int, int)), ui.tableWidget, SIGNAL(cellClicked(int, int)));
	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(SlotSectionClicked(int, int)));

	connect(ui.pbHold, SIGNAL(clicked()), this, SLOT(SlotHold()));

	ui.tableWidget->setColumnWidth(2, 120);
}

VSCEventConsole::~VSCEventConsole()
{

}

void VSCEventConsole::SlotHold()
{
	if (m_bHolded == FALSE)
	{
		ui.pbHold->setText(tr("UnHold"));
		m_bHolded = TRUE;
	}else
	{
		ui.pbHold->setText(tr("Hold"));
		m_bHolded = FALSE;
	}
}

void VSCEventConsole::SlotEvent(std::string strEvent)
{	
	VidEvent cEvent;
	bool bRet = cEvent.ParseFromString(strEvent);

	if (bRet == true && cEvent.bsearched() == false)
	{
		EventTableUpdate(cEvent);
	}
	return;
}

void VSCEventConsole::EventTableUpdate(VidEvent &cEvent)
{
	s64 nCurrentTime = time(NULL);

	m_EventMap[m_EventId] = cEvent;
	m_EventMap[m_EventId].set_nidx(m_EventId);

	/* The update  */
	if (m_bHolded == TRUE || abs(nCurrentTime - m_nLastUpdateTime) == 0)
	{
		m_EventId ++;
		return;
	}
	m_nLastUpdateTime = nCurrentTime;
	
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
		nStart = m_EventId - VSC_MAX_EVENT_NUM;
	}

	for (s32 i = nMax; i >= nStart; i --)
	{
		VidEvent cEventCurrent = m_EventMap[i];
		int insertRow = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(insertRow);
		QTableWidgetItem *pItem0 = new VidEventTableItem(cEventCurrent, cEventCurrent.strstorname().c_str());
		QTableWidgetItem *pItem1 = new QTableWidgetItem(cEventCurrent.strdevicename().c_str());
		QTableWidgetItem *pItem2 = new QTableWidgetItem(cEventCurrent.strtype().c_str());
		QTableWidgetItem *pItem3 = new QTableWidgetItem(cEventCurrent.strtime().c_str());
		if (cEventCurrent.bhandled() == false)
		{
			pItem0->setBackgroundColor(QColor(200,200,200));
			pItem1->setBackgroundColor(QColor(200,200,200));
			pItem2->setBackgroundColor(QColor(200,200,200));
			pItem3->setBackgroundColor(QColor(200,200,200));
		}
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

void VSCEventConsole::SlotSectionClicked(int row, int column)
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
			m_EventMap[cEvent.nidx()] = cEvent;
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


void VSCEventConsole::SetEventUI(VidEvent pEvent)
{
	VDC_DEBUG("%s VSCEventConsole::SetEventUI \n", __FUNCTION__);
	ui.lineEditStorName->setText(pEvent.strstorname().c_str());
	ui.lineEditName->setText(pEvent.strdevicename().c_str());

	ui.lineEditType->setText(pEvent.strtype().c_str());
	ui.lineEditTime->setText(pEvent.strtime().c_str());

	ui.lineEditDesc->setText(pEvent.strdesc().c_str());
	//ui.lineEditMark->setText(pEvent.str().c_str());
	
}




