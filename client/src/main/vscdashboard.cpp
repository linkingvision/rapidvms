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



