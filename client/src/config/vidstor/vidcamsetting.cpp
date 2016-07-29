#include "config/vidstor/vidcamsetting.h"
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
#include "config/vidstor/camsetting/vsccaminfo.h"
#include "config/vidstor/camsetting/vsccamrec.h"
#include "config/vidstor/camsetting/vscstreamselect.h"
#include "config/vidstor/camsetting/vscmotion.h"
#include "config/vidstor/camsetting/vscschedule.h"

using namespace Poco;

VidCamSetting::VidCamSetting(VidStor pStor, astring strCam, ClientFactory &pFactory, 
QWidget *parent, Qt::WindowFlags flags)
: m_pStor(pStor), m_strCam(strCam), m_pFactory(pFactory), QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);
	VSCCamInfo *pCamInfo = new VSCCamInfo(pFactory, m_pStor, m_strCam, this);
	ui.tabWidget->addTab(pCamInfo, tr("Information"));
	ui.tabWidget->addTab(new VSCStreamSelect(pFactory, m_pStor, m_strCam, this), tr("Stream"));
	ui.tabWidget->addTab(new VSCCamRec(pFactory, m_pStor, m_strCam, this), tr("Recording"));
	ui.tabWidget->addTab(new VSCMotion(pFactory, m_pStor, m_strCam, this), tr("Motion"));
	ui.tabWidget->addTab(new VSCSchedule(pFactory, m_pStor, m_strCam, this), tr("Schedule"));

	//connect(this, SIGNAL(SignalSectionClicked(int, int)), ui.tableWidget, SIGNAL(cellClicked(int, int)));
	ui.storName->setText(pCamInfo->GetCameraName().c_str());
	
	QVBoxLayout* layout = new QVBoxLayout();
    m_pVideo = new VSCVWidget(m_pFactory, 0, this->ui.video, true);
	m_pVideo->hide();
    //layout->setSpacing(10);

    layout->addWidget(m_pVideo);
    layout->setMargin(0);
	
	this->ui.video->setLayout(layout);
	m_pVideo->show();
	m_pVideo->ShowVideoInfo(false);
	m_pVideo->StartPlay(m_pStor.strid(), m_strCam, "");
	
}

VidCamSetting::~VidCamSetting()
{

}



