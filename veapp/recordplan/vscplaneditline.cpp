#include "vscplaneditline.h"

#include <QPalette>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

VSCPlanEditLine::VSCPlanEditLine(QWidget *parent /* = 0 */)
	: QWidget(parent), moving(false), m_Editable(false), 
		mouseX_0(0), mouseX_1(0), begin_mouseX(0), movingX(0)
{
	ui.setupUi(this);
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(100, 100, 100));
	this->setPalette(palette);
	this->setAutoFillBackground(true);

	/* Set all the plan every 30min to false */
	for (int i = 0; i < VSC_PLAN_UNIT_LEN; i ++)
	{
		m_planList[i] = 0;
	}

	m_startMoving = -1;

	m_pClear = new QAction(QIcon(tr(":/action/resources/user-trash-2.png")), tr("Clear"), this);
	connect(m_pClear, SIGNAL(triggered()), this, SLOT(on_Clear()));

	//m_PlanModList = new QList<zexabox::PlanMod>();
	TimeofPlan = new QList<int>();

	m_Width = width();
	m_Height = height();
}

VSCPlanEditLine::~VSCPlanEditLine()
{

}

void VSCPlanEditLine::InitTimeofPlan(int pos)
{
	/*
	if (m_PlanModList != NULL)
	{
		int i, TimePoint;
		TimeofPlan->clear();
		for (i=0; i<24; i++)
		{
			TimePoint = m_PlanModList->at(pos).Week[m_LineId][i];
			if ( TimePoint != -1 )
			{
				TimeofPlan->append(TimePoint);
			}
			else
			{
				m_Tnum = i - 1;
				break;
			}	
		}
	}
	else
	{
		TimeofPlan->clear();
	}
	*/
	return;
}

void VSCPlanEditLine::on_Clear()
{
	for (int i = 0; i < VSC_PLAN_UNIT_LEN; i ++)
	{
		m_planList[i] = 0;
	}
	update();
}

void VSCPlanEditLine::SetAlldayMod(bool fal)
{
	m_Editable = false;
	if (fal)
	{
		TimeofPlan->clear();
		for (int i=0; i<24; i++)
		{
			TimeofPlan->append(i);
		}
		update();
	}
}

void VSCPlanEditLine::SetWeekDayMod(bool fal)
{
	m_Editable = false;
	if (fal)
	{
		TimeofPlan->clear();
		if (m_LineId<5)
		{
			for (int i=0; i<24; i++)
			{
				TimeofPlan->append(i);
			}
		}
		update();
	}
}

void VSCPlanEditLine::InitPlanLine(int ModId)
{
	m_Editable = true;

	InitTimeofPlan(ModId - 1);
	
	update();
}

void VSCPlanEditLine::UndoLastStep(int ModId)
{
	if (TimeofPlan->isEmpty())
	{
		return;
	}
	else
	{
		TimeofPlan->removeLast();
		update();
	}

	return;
}

void* VSCPlanEditLine::GetPlan()
{
	return TimeofPlan;
}
/*
void VSCPlanEditLine::RecieveModInfo(QList<zexabox::PlanMod>* PlanModlist)
{
	m_PlanModList = PlanModlist;
}
*/

void VSCPlanEditLine::ShowPlan()
{
	QPainter painter(this);
	painter.setPen(QPen(QColor(103, 207, 0),28));

	VDC_DEBUG("%s\n", __FUNCTION__);

	for (int i = 0; i < VSC_PLAN_UNIT_LEN; i ++)
	{
		if (m_planList[i] == 1)
		{
			painter.setPen(QPen(QColor(103, 207, 0),28));
			VDC_DEBUG( "%s %d (%d, %d)\n",__FUNCTION__,i,  i*(width()/VSC_PLAN_UNIT_LEN), 
										(i+1)*(width()/VSC_PLAN_UNIT_LEN));
			painter.drawLine(i*(width()/VSC_PLAN_UNIT_LEN + 14) , 
				30, (i+1)*(width()/VSC_PLAN_UNIT_LEN) - 14, 30);
		}else
		{
			painter.setPen(QPen(QColor(100, 100, 100),28));
			painter.drawLine(i*(width()/VSC_PLAN_UNIT_LEN + 14) , 
				30, (i+1)*(width()/VSC_PLAN_UNIT_LEN) - 14, 30);			
		}
	}

	return;

}

void VSCPlanEditLine::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QFont font("sans");
	font.setPointSize(10);
	//font.setBold(true);
	painter.setFont(font);

	/* --------------------- */
	painter.setPen(QPen(Qt::white, 30));
	painter.drawLine(0, 0, m_Width, 0);

	int j=0;
	for (int i = 0; i<24; i++)
	{
		painter.setPen(QPen(QColor(100, 100, 100), 1));

		painter.drawLine(i*m_Width/24, 12, i*m_Width/24, 30);
		if (j%2 == 0)
		{
			painter.setPen(QPen(Qt::black, 1));
			if (i>9)
				painter.drawText(i*m_Width/24-4, 10, QString("%1").arg(i));
			else
				painter.drawText(i*m_Width/24, 10, QString("%1").arg(i));
		}
		j++;
	}

	/* show the m_planList*/
	ShowPlan();

	event->accept();
}

void VSCPlanEditLine::mousePressEvent(QMouseEvent *event)
{
	if(event->button() != Qt::LeftButton)
	{
		return ;
	}
	if (m_Editable)
	{
		int x = event->x();
		int i = x/(width()/VSC_PLAN_UNIT_LEN);
		if (i >= 0 && i < VSC_PLAN_UNIT_LEN)
		{
			VDC_DEBUG( "%s i %d\n",__FUNCTION__, i);
			if (m_planList[i] == 0)
			{
				m_planList[i] =1;/* Set checked */
			}else
			{
				m_planList[i] =0;/* Set unchecked */
			}
		}
		m_startMoving = i;

		update();
		event->accept();
	}	
}

void VSCPlanEditLine::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_Editable)
	{
		int x = event->x();
		int i = x/(width()/VSC_PLAN_UNIT_LEN);
		if (m_startMoving  >= 0 && m_startMoving < VSC_PLAN_UNIT_LEN && m_startMoving != i)
		{
			for (int j = m_startMoving; j <= i;  j ++)
			{
				m_planList[j] =1;
			}
		}
		
		event->accept();
		update();
		m_startMoving = -1;
	}
}

void VSCPlanEditLine::mouseMoveEvent(QMouseEvent *event)
{

	if (m_Editable)
	{
		int x = event->x();
		int i = x/(width()/VSC_PLAN_UNIT_LEN);
		if (m_startMoving  >= 0 && m_startMoving < VSC_PLAN_UNIT_LEN && m_startMoving != i)
		{
			for (int j = m_startMoving; j <= i;  j ++)
			{
				m_planList[j] =1;
			}
		}
		
		event->accept();
		update();
	}	

}

void VSCPlanEditLine::contextMenuEvent(QContextMenuEvent* e) 
{
    QDesktopWidget *desktop = QApplication::desktop();
    popupMenu = new QMenu(this);
    popupMenu->addAction(m_pClear);

    popupMenu->exec(e->globalPos()); 
    delete popupMenu;
    popupMenu = NULL;
}

