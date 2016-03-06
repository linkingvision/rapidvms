#include "vscdashboard.h"
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
#include "vscdashboardtaskitem.h"

VSCDashBoard::VSCDashBoard(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);
	ui.scrollArea->setBackgroundRole(QPalette::Light);

	/* Setup connection */
	connect(ui.pbTasklist, SIGNAL(clicked()), this, SLOT(UpdateTaskList()));


	UpdateTaskList();
}

VSCDashBoard::~VSCDashBoard()
{

}

void VSCDashBoard::UpdateTaskList()
{
	/* Remove old task */

	VTaskItemWidgetList::iterator it1 = m_pTaskWidget.begin(); 
	for(; it1!=m_pTaskWidget.end(); ++it1)
	{
		ui.TaskLayout->removeWidget((*it1).second);
		delete (*it1).second;
	}

	m_pTaskWidget.clear();

	/* Update the task list */
	VTaskMgr::GetTaskList(m_pTaskList);

        VTaskItemList::iterator it = m_pTaskList.begin(); 
        for(; it!=m_pTaskList.end(); ++it)
        {
		VSCDashBoardTaskItem *pItem = new VSCDashBoardTaskItem((*it).second, this); 
		ui.TaskLayout->addWidget(pItem);
		m_pTaskWidget[(*it).second->GetId()] = pItem;
		
		connect(pItem, SIGNAL(TaskDeleted()), this, SLOT(UpdateTaskList()));
			
        }
}



