#include "vsceventconsole.h"
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

VSCEventConsole::VSCEventConsole(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);

	//connect(ui.pbTasklist, SIGNAL(clicked()), this, SLOT(UpdateTaskList()));
}

VSCEventConsole::~VSCEventConsole()
{

}




