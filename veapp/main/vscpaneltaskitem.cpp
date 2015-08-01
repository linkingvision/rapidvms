#include "vscpaneltaskitem.h"
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

VSCPanelTaskItem::VSCPanelTaskItem(VTaskItem * pTask, QWidget *parent, Qt::WindowFlags flags)
    : m_pTask(pTask), QWidget(parent, flags)
{
	ui.setupUi(this);

	//connect(ui.pbSetting, SIGNAL(clicked()), this, SIGNAL(Setting()));
	//connect(ui.pbAlarm, SIGNAL(clicked()), this, SIGNAL(AddEvent()));
	if (m_pTask)
		ui.TaskName->setText(m_pTask->GetTaskName().c_str());

	m_movie = new QMovie(":/action/resources/processing.gif");
	ui.lbProcess->setScaledContents(true);
	ui.lbProcess->setMovie(m_movie);
	m_movie->start();

	UpdateProcess();
	m_Timer = new QTimer(this);
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(UpdateProcess()));
	connect(ui.pbDelete, SIGNAL(clicked()), this, SLOT(DeleteTask()));
	
	m_Timer->start(5000);

	
}

VSCPanelTaskItem::~VSCPanelTaskItem()
{
	//delete m_movie;
}


void VSCPanelTaskItem::DeleteTask()
{
	VTaskMgr::DeleteTask(m_pTask);
	emit TaskDeleted();
}

void VSCPanelTaskItem::UpdateProcess()
{
	int process = 0;
	BOOL IsEol = FALSE;

	if (m_pTask)
	{
		process = m_pTask->GetPercentage();
		IsEol = m_pTask->IsEol();
	}

	ui.progressBar->setValue(process);

	if (process == 100 || IsEol == TRUE)
	{
		m_movie->stop();
	}
	

}


