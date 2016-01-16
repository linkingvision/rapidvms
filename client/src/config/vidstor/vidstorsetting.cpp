#include "config/vidstor/vidstorsetting.h"
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
#include "Poco/UUIDGenerator.h"
#include "config/vidstor/storsetting/vscstorversion.h"

using namespace Poco;

VidStorSetting::VidStorSetting(VidStor pStor, ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
	: m_pStor(pStor), m_pFactory(pFactory), QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);
	
	//ui.tabWidget->addTab(new VSCVersion(pFactory, this), tr("Version"));

	//connect(this, SIGNAL(SignalSectionClicked(int, int)), ui.tableWidget, SIGNAL(cellClicked(int, int)));
}

VidStorSetting::~VidStorSetting()
{

}



