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
#include "vvidpbcontrol.h"
#include "server/factory.hpp"
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
#include <QDialog>
#include "ui_vvidpbcontrol.h"
#include "vscdateset.h"
#include "vscexport.h"

//background-color:rgb(42, 43, 44)


VVidPBControl::VVidPBControl(QWidget *parent, Qt::WindowFlags flags)
    : p_ui(new Ui::VVidPBControl), ui(*p_ui), QWidget(parent, flags), 
    	m_pDateSetting(NULL), m_pExport(NULL), m_bPause(false)
{

	ui.setupUi(this);

	connect(ui.pbDate, SIGNAL(clicked()), this, SLOT(SlotDateSetting()));
	connect(ui.pbExport, SIGNAL(clicked()), this, SLOT(SlotExport()));
	connect(ui.pbPlay, SIGNAL(clicked()), this, SLOT(SlotPlay()));
	connect(ui.pbPause, SIGNAL(clicked()), this, SLOT(SlotPause()));

	connect(ui.timeline, SIGNAL(SignalSeekTime(unsigned int)), this, 
				SLOT(SlotSeekTime(unsigned int)));

	connect(ui.pbZoomin1, SIGNAL(clicked()), ui.timeline, SLOT(SlotZoomIn()));
	connect(ui.pbZoomin2, SIGNAL(clicked()), ui.timeline, SLOT(SlotZoomIn()));

	connect(ui.pbZoomout1, SIGNAL(clicked()), ui.timeline, SLOT(SlotZoomOut()));
	connect(ui.pbZoomout2, SIGNAL(clicked()), ui.timeline, SLOT(SlotZoomOut()));

	connect(this, SIGNAL(SignalTimeUpdate(unsigned int)), 
					ui.timeline, SLOT(SlotUpdateTime(unsigned int)));
	//m_pDateSetting1 = new VSCDateSetWidget();
	//m_pDateSetting = new VSCDateSet();

	/* Default is play */
	ui.pbPause->show();
	ui.pbPlay->hide();
	

}

VVidPBControl::~VVidPBControl()
{

}

void VVidPBControl::SlotUpdateTime(unsigned int nTime)
{
	if (m_bPause == false)
	{
		emit SignalTimeUpdate(nTime);
	}
}

void VVidPBControl::SlotClearPlay()
{
	ui.timeline->ClearPlay();
}

bool VVidPBControl::SetPlaying(bool bPlaying)
{
	if (bPlaying == true)
	{
		ui.pbPause->show();
		ui.pbPlay->hide();
		m_bPause = false;
	}else
	{
		ui.pbPause->hide();
		ui.pbPlay->show();	
		m_bPause = true;
	}

	return true;
}

void VVidPBControl::SlotSeekTime(unsigned int nTime)
{
	/* First pause */
	SlotPause();
	emit SignalSeekTime(nTime);
}

void VVidPBControl::SlotSeekTimeFromDate(unsigned int nTime)
{
	SlotSeekTime(nTime);
	emit SignalTimeUpdate(nTime);
}

void VVidPBControl::SlotPlay()
{
	ui.pbPlay->hide();
	ui.pbPause->show();
	m_bPause = false;
	emit SignalPlay();
}
void VVidPBControl::SlotPause()
{
	ui.pbPause->hide();
	ui.pbPlay->show();
	m_bPause = true;

	emit SignalPause();
}

bool VVidPBControl::SetDelegate(VVidPBDelegate *pDelegate)
{
	/* Set the Delegate to the timeline  */
	ui.timeline->SetDelegate(pDelegate);

	/* Set the Delegate to the date */
	if (m_pDateSetting == NULL)
	{
		m_pDateSetting = new VSCDateSet(this);
		m_pDateSetting->hide();
		connect(m_pDateSetting, SIGNAL(SignalSeekTime(unsigned int)), this, SLOT(SlotSeekTimeFromDate(unsigned int))); 		
	}
	m_pDateSetting->SetDelegate(pDelegate);

	if (m_pExport == NULL)
	{
		m_pExport = new VSCExport(m_strStor, m_strId, 
						m_strCamName, ui.timeline->GetCurrentTime(), this);
	}
	m_pExport->SetDelegate(pDelegate);
	
	return true;
}

bool VVidPBControl::SetPlayParam(astring strStor, astring strId, astring strCamName)
{
	m_strStor = strStor;
	m_strId = strId;
	m_strCamName = strCamName;
	return true;
}

void VVidPBControl::SlotExport()
{
	if (m_pExport == NULL)
	{
		m_pExport = new VSCExport(m_strStor, m_strId, 
						m_strCamName, ui.timeline->GetCurrentTime(), this);
	}
	m_pExport->show();
	QPoint pos = mapTo(this, QCursor::pos());
	m_pExport->move(pos.x() - m_pExport->width(),  
					pos.y()- m_pExport->height());
	m_pExport->SetStartTime(ui.timeline->GetCurrentTime());
}

void VVidPBControl::SlotDateSetting()
{
	if (m_pDateSetting == NULL)
	{
		m_pDateSetting = new VSCDateSet(this);

		connect(m_pDateSetting, SIGNAL(signalSeekTime(int)), this, SIGNAL(signalSeekTime(int))); 
	}
	QPoint pos = mapTo(this, QCursor::pos());
	m_pDateSetting->move(pos.x() - m_pDateSetting->width(),  
					pos.y()- m_pDateSetting->height());
	m_pDateSetting->show();
}
