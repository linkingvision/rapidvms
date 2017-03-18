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
#include "vschedtable.hpp"
#include "recordwrapper.hpp"
#include <QRubberBand>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>

class VSchedTablePri
{
public:
	VSchedTablePri();
	~VSchedTablePri();
public:
	RecordSchedWeek conf;
	QLabel * pLabelHour[VDB_HOURS_IN_DAY];
	QLabel * pLabelDay[VDB_DAYS_IN_WEEK];
	QRubberBand* pRubberBand;
	int nStartX;
	int nStartY;
};

VSchedTablePri::VSchedTablePri()
	:nStartX(0), nStartY(0)
{
}
VSchedTablePri::~VSchedTablePri()
{
}

VSchedTable::VSchedTable(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags), 
	m_pPri(NULL), m_bPressed(false)
{
	/* Width min 24 hour * 30,  heigh min 7 day * 30 */
	m_pPri = new VSchedTablePri();
	m_pPri->pRubberBand = new QRubberBand(QRubberBand::Rectangle,this);
 	m_pPri->pRubberBand->setStyleSheet("border:1px solid red; background-color:rgb(200, 200, 200);");

	for (int i = 0; i < VDB_HOURS_IN_DAY; i ++)
	{
		m_pPri->pLabelHour[i] = new QLabel(QString("%1").arg(i), this);
	}

	m_pPri->pLabelDay[0] = new QLabel(tr("Mon"), this);
	m_pPri->pLabelDay[1] = new QLabel(tr("Tue"), this);
	m_pPri->pLabelDay[2] = new QLabel(tr("Wed"), this);
	m_pPri->pLabelDay[3] = new QLabel(tr("Thu"), this);
	m_pPri->pLabelDay[4] = new QLabel(tr("Fri"), this);
	m_pPri->pLabelDay[5] = new QLabel(tr("Sat"), this);
	m_pPri->pLabelDay[6] = new QLabel(tr("Sun"), this);
	


	//connect(ui.pbHold, SIGNAL(clicked()), this, SLOT(SlotHold()));
}

VSchedTable::~VSchedTable()
{
	if (m_pPri)
	{
		delete m_pPri;
		m_pPri = NULL;
	}
	
}

bool VSchedTable::SetCurrentSched(RecordSchedWeek &pSched)
{
	m_pPri->conf = pSched;

	return true;
}

bool VSchedTable::GetCurrentSched(RecordSchedWeek &pSched)
{	
	pSched = m_pPri->conf;
	return true;
}

void VSchedTable::paintEvent(QPaintEvent *e)
{
	/* Move the label */
	int widthPerHour = width()/(VDB_HOURS_IN_DAY + 1);
	int heightPerDay = height()/(VDB_DAYS_IN_WEEK + 1);
	for (int k = 1; k <= VDB_HOURS_IN_DAY; k ++)
	{
		m_pPri->pLabelHour[k - 1]->setGeometry(widthPerHour * k, 0, widthPerHour, heightPerDay);
		//m_pPri->pLabelHour[i].move(widthPerHour * k, 0);
	}

	for (int m = 1; m <= VDB_DAYS_IN_WEEK; m ++)
	{
		m_pPri->pLabelDay[m - 1]->setGeometry(0, heightPerDay * m, widthPerHour, heightPerDay);
		//m_pPri->pLabelHour[i].move(widthPerHour * k, 0);
	}
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	for (int i = 1; i <= VDB_DAYS_IN_WEEK; i ++)
	{
		for (int j = 1; j <= VDB_HOURS_IN_DAY; j ++)
		{
			if (m_pPri->conf.week[i -1].type[j - 1] == R_OFF)
			{
				painter.setBrush(QBrush(QColor(80,80,80),Qt::SolidPattern));
			}else if (m_pPri->conf.week[i - 1].type[j - 1] == R_ALARM) /* Red */
			{
				painter.setBrush(QBrush(QColor(255,0,0),Qt::SolidPattern));
			}else /* Default is schedule, green */
			{
				painter.setBrush(QBrush(QColor(0,128,0),Qt::SolidPattern));
			}
            		painter.drawRect(widthPerHour * j + 1, heightPerDay * i  + 1, 
						widthPerHour - 2, heightPerDay - 2);

		}
	}

	return;

}
void VSchedTable::mousePressEvent(QMouseEvent *e)
{
	QPoint pos = e->pos();
	
	m_pPri->nStartX = pos.x();
	m_pPri->nStartY = pos.y();
	m_pPri->pRubberBand->setGeometry(pos.x(),pos.y(),0,0);
	m_pPri->pRubberBand->show();	
	m_bPressed = true;
}
void VSchedTable::mouseMoveEvent(QMouseEvent *e)
{
	QPoint pos = e->pos();
	if(pos.x() >= this->rect().right())
	{
	    pos.setX(this->rect().right());
	}

	if(pos.x() <= this->rect().left())
	{
	    pos.setX(this->rect().left());
	}

	if(pos.y() <= this->rect().top())
	{
	    pos.setY(this->rect().top());
	}

	if(pos.y() >= this->rect().bottom())
	{
	    pos.setY(this->rect().bottom());
	}

	m_pPri->pRubberBand->setGeometry(m_pPri->nStartX,
				m_pPri->nStartY, pos.x()-m_pPri->nStartX, pos.y()-m_pPri->nStartY);
	m_pPri->pRubberBand->show();
	
}
void VSchedTable::mouseReleaseEvent(QMouseEvent *e)
{
}

bool VSchedTable::UpdateSchedType(RecordingType type)
{
	if (m_bPressed == false)
	{
		return true;
	}else
	{
		m_bPressed = false;
	}
	int widthPerHour = width()/(VDB_HOURS_IN_DAY + 1);
	int heightPerDay = height()/(VDB_DAYS_IN_WEEK + 1);

	int nStartX = m_pPri->nStartX/widthPerHour;
	int nStartY = m_pPri->nStartY/heightPerDay;

	int debugX = nStartX + m_pPri->pRubberBand->width() / widthPerHour;
	int debugY = nStartY + m_pPri->pRubberBand->height() / heightPerDay;

	for (int x = nStartX; x <= nStartX + m_pPri->pRubberBand->width()/widthPerHour; 
				x ++)
	{
		for (int y = nStartY; y <= nStartY + m_pPri->pRubberBand->height()/heightPerDay; 
		y ++)
		{
			m_pPri->conf.week[y -1].type[x -1] = type;
			VDC_DEBUG("%s  (%d, %d)\n", __FUNCTION__, x, y);
		}
	}
	
	update();
	m_pPri->pRubberBand->hide();

	return true;
}
void VSchedTable::SlotRecordOff()
{
	UpdateSchedType(R_OFF);
}
void VSchedTable::SlotRecordAlarm()
{
	UpdateSchedType(R_ALARM);
}
void VSchedTable::SlotRecordSchedule()
{
	UpdateSchedType(R_SCHED);
}

bool VSchedTable::SetSchedAllDay()
{
	m_pPri->conf = RecordSchedWeek::CreateAllDay();
	update();
	return true;
}
bool VSchedTable::SetSchedWorkDay()
{
	m_pPri->conf = RecordSchedWeek::CreateWorkDay();
	update();
	return true;
}

bool VSchedTable::SetSchedClear()
{
	m_pPri->conf = RecordSchedWeek::CreateOff();
	update();
	return true;
}





