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
#ifndef __VIDEO_DASH_BOARD_TASK_ITEM_H__
#define __VIDEO_DASH_BOARD_TASK_ITEM_H__

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QMimeData>
#include <QDrag>
#include <QMovie>

#define NOMINMAX
#define NOMINMAX 

#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "ui_vscdashboardtaskitem.h"
#include "vtaskmgr.hpp"

using  namespace tthread;

class VSCDashBoardTaskItem : public QWidget
{
	Q_OBJECT
		
public:
	VSCDashBoardTaskItem(VTaskItem * pTask,  QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSCDashBoardTaskItem();
	
public slots:
	void UpdateProcess();
	void DeleteTask();

signals:
	void TaskDeleted();

public:
    	Ui::VSCDashBoardTaskItem ui;
private:
	VTaskItem * m_pTask;
	QMovie * m_movie;
	QTimer *m_Timer;

};


#endif // __VIDEO_PANEL_TASK_ITEM_H__
