#include "vscpanel.h"
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
#include "vtaskmgr.hpp"
#include "vscpaneltaskitem.h"

extern Factory *gFactory;

VSCPanel::VSCPanel(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);
	ui.scrollArea->setBackgroundRole(QPalette::Light);

	/* Setup connection */
	connect(ui.pbView, SIGNAL(clicked()), this, SIGNAL(AddSurveillance()));
	connect(ui.pbSearch, SIGNAL(clicked()), this, SIGNAL(Search()));
	connect(ui.pbMining, SIGNAL(clicked()), this, SIGNAL(AddDmining()));
	connect(ui.pbEmap, SIGNAL(clicked()), this, SIGNAL(AddEmap()));
	//connect(ui.pbPlan, SIGNAL(clicked()), this, SIGNAL(AddSurveillance())); //TODO

	connect(ui.pbRecorder, SIGNAL(clicked()), this, SIGNAL(AddRecorder()));
	connect(ui.pbCamera, SIGNAL(clicked()), this, SIGNAL(AddCamera()));

	connect(ui.pbSetting, SIGNAL(clicked()), this, SIGNAL(Setting()));
	connect(ui.pbAlarm, SIGNAL(clicked()), this, SIGNAL(AddEvent()));
	connect(ui.pbVIPC, SIGNAL(clicked()), this, SIGNAL(AddVIPC()));

	connect(ui.pbTasklist, SIGNAL(clicked()), this, SLOT(UpdateTaskList()));


	UpdateTaskList();
}

VSCPanel::~VSCPanel()
{

}

void VSCPanel::UpdateTaskList()
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
		VSCPanelTaskItem *pItem = new VSCPanelTaskItem((*it).second, this); 
		ui.TaskLayout->addWidget(pItem);
		m_pTaskWidget[(*it).second->GetId()] = pItem;
		
		connect(pItem, SIGNAL(TaskDeleted()), this, SLOT(UpdateTaskList()));
			
        }
}



