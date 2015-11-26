#include "vsctoolbar.h"
#include "debug.hpp"
#include <QDesktopWidget>
#include <QApplication>
#include <QTime>
#include <QDate>
#include "vtaskmgr.hpp"


VSCToolBar::VSCToolBar(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |  Qt::WindowStaysOnBottomHint);
	//setWindowFlags( Qt::WindowStaysOnTopHint |  Qt::WindowStaysOnBottomHint);
	setWindowOpacity(0.8);
	int x, y, w, h;
	
	QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	
	x = rect.x();
	y = rect.y();
	w = rect.width();
	h = rect.height();
	//rect.getRect(&x, &y, &w, &h);
	VDC_DEBUG( "%s rect x:%d y:%d w:%d h:%d \n",__FUNCTION__, x, y, w, h);
	QPoint p(x + w/2 - 240, y + h - 150 );
	move(p);
	
	m_Timer = new QTimer(this);
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(showcurrentTime()));
	m_Timer->start(1000);

	m_TimerPrcessing = new QTimer(this);
	connect(m_TimerPrcessing, SIGNAL(timeout()), this, SLOT(showProcessingTimer()));
	m_TimerPrcessing->start(1000);
	
	m_TimerAlarm = new QTimer(this);
	connect(m_TimerAlarm, SIGNAL(timeout()), this, SLOT(showAlarm()));
	ui.pbAlarm->setIcon(QIcon(":/action/resources/alarmno.png"));
	//m_TimerAlarm->start(300);
	m_alarm = false;

	m_movie = new QMovie(":/action/resources/processing.gif");
	ui.lbProcess->setScaledContents(true);
	ui.lbProcess->setMovie(m_movie);
	m_movie->start();
	showProcessing(FALSE);


	m_cpu = new VSCCalculate(ui.cpu);
#if 0
	ui.Tour->addItem("Tour 1");
	ui.Tour->addItem("Tour 2");
	ui.Tour->addItem("Tour 3");
#endif

	//ui.pbTourStop->hide();
}

void VSCToolBar::showProcessing(bool bEnable)
{
	if (bEnable == TRUE)
	{
		ui.lbProcess->show();
	}else
	{
		ui.lbProcess->hide();
	}
		
}

void VSCToolBar::showcurrentTime()
{
	QTime currentTime = QTime::currentTime();
	QDate currentDate = QDate::currentDate();
	QString strCurrentTime =currentDate.toString("yyyy-MM-dd") + " " + currentTime.toString() + "  ";
	ui.time->setText(strCurrentTime);

}

void VSCToolBar::showProcessingTimer()
{
	if (VTaskMgr::GetTaskCnt() > 0)
	{
		showProcessing(TRUE);
	}else
	{
		showProcessing(FALSE);
	}

}

void VSCToolBar::NewAlarm()
{
	if (m_alarmCnt > 0)
	{
		m_alarmCnt = 20;
		return;
	}
	m_alarmCnt = 20;
	m_TimerAlarm->start(300);
	m_alarm = true;
}

void VSCToolBar::showAlarm()
{
	if (m_alarmCnt <= 0)
	{
		m_TimerAlarm->stop();
		ui.pbAlarm->setIcon(QIcon(":/action/resources/alarmno.png"));
	}else
	{
		if (m_alarm == false)
		{
			ui.pbAlarm->setIcon(QIcon(":/action/resources/alarmno.png"));
			m_alarm = true;
		}else
		{
			ui.pbAlarm->setIcon(QIcon(":/action/resources/alarm.png"));
			m_alarm = false;
		}
		m_alarmCnt --;
	}
}

