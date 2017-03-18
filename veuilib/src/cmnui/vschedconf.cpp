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
#include "vschedconf.hpp"
#include "ui_vschedconf.h"
#include "recordwrapper.hpp"
#include <QRubberBand>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>

class VSchedConfPri
{
public:
	VSchedConfPri();
	~VSchedConfPri();
public:
};

VSchedConfPri::VSchedConfPri()
{
}
VSchedConfPri::~VSchedConfPri()
{
}

VSchedConf::VSchedConf(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags), p_ui(new Ui::VSchedConf), ui(*p_ui), 
	m_pPri(NULL)
{
	/* Width min 24 hour * 10,  heigh min 7 day * 30 */
	m_pPri = new VSchedConfPri();
	ui.setupUi(this);


	connect(ui.pbOff, SIGNAL(clicked()), this, SLOT(SlotRecordOff()));
	connect(ui.pbSched, SIGNAL(clicked()), this, SLOT(SlotRecordSchedule()));
	connect(ui.pbEvent, SIGNAL(clicked()), this, SLOT(SlotRecordAlarm()));
	connect(ui.pbWorkDay, SIGNAL(clicked()), this, SLOT(SlotRecordWorkDay()));
	connect(ui.pbAllDay, SIGNAL(clicked()), this, SLOT(SlotRecordAllDay()));
	connect(ui.pbClear, SIGNAL(clicked()), this, SLOT(SlotRecordClear()));
}

VSchedConf::~VSchedConf()
{
	if (m_pPri)
	{
		delete m_pPri;
		m_pPri = NULL;
	}
	
}

bool VSchedConf::GetCurrentSched(RecordSchedWeek &pSched)
{	
	ui.widget->GetCurrentSched(pSched);
	return true;
}

bool VSchedConf::SetCurrentSched(RecordSchedWeek &pSched)
{
	ui.widget->SetCurrentSched(pSched);
	return true;
}

void VSchedConf::SlotRecordOff()
{
	ui.widget->UpdateSchedType(R_OFF);
}
void VSchedConf::SlotRecordAlarm()
{
	ui.widget->UpdateSchedType(R_ALARM);
}
void VSchedConf::SlotRecordSchedule()
{
	ui.widget->UpdateSchedType(R_SCHED);
}

void VSchedConf::SlotRecordAllDay()
{
	ui.widget->SetSchedAllDay();
}
void VSchedConf::SlotRecordWorkDay()
{
	ui.widget->SetSchedWorkDay();
}

void VSchedConf::SlotRecordClear()
{
	ui.widget->SetSchedClear();
}





