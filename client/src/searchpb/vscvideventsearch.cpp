#include "vscvideventsearch.h"
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


VSCVidEventSearch::VSCVidEventSearch(ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
    : m_pFactory(pFactory), m_pVideo(NULL), QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);

	//connect(ui.pbTasklist, SIGNAL(clicked()), this, SLOT(UpdateTaskList()));
	
	QVBoxLayout* layout = new QVBoxLayout();
	m_pVideo = new VVidPB1(m_pFactory, ui.video);
	m_pVideo->hide();
	//layout->setSpacing(10);

	layout->addWidget(m_pVideo);
	layout->setMargin(0);

	this->ui.video->setLayout(layout);
	m_pVideo->show();

	connect(&(m_pFactory.GetStorFactory()), SIGNAL(SignalEvent(std::string)), 
		this, SLOT(SlotEvent(std::string)));
}

VSCVidEventSearch::~VSCVidEventSearch()
{

}

void VSCVidEventSearch::SlotEvent(std::string strEvent)
{	
	VidEvent cEvent;
	bool bRet = cEvent.ParseFromString(strEvent);

	if (bRet == true)
	{
		EventTableUpdate(cEvent);
	}
	return;
}

void VSCVidEventSearch::EventTableUpdate(VidEvent &cEvent)
{
	int nRowCnt = ui.tableWidget->rowCount();

	for (s32 j = nRowCnt - 1; j >= 0; j --)
	{
		ui.tableWidget->removeRow(j);
	}
	ui.tableWidget->clear();

	int insertRow = 0;
	ui.tableWidget->insertRow(insertRow);
	ui.tableWidget->setItem(insertRow, 0, new QTableWidgetItem(cEvent.strstorname().c_str()));
	ui.tableWidget->setItem(insertRow, 1, new QTableWidgetItem(cEvent.strdevicename().c_str()));
	ui.tableWidget->setItem(insertRow, 2, new QTableWidgetItem(cEvent.strtype().c_str()));
	ui.tableWidget->setItem(insertRow, 3, new QTableWidgetItem(cEvent.strtime().c_str()));

}




