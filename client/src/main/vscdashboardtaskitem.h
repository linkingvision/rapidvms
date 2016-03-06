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
