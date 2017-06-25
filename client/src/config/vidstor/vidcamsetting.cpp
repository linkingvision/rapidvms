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
#include "vscloading.hpp"

using namespace Poco;

VidCamSetting::VidCamSetting(VidStor pStor, astring strCam, ClientFactory &pFactory, 
QWidget *parent, Qt::WindowFlags flags)
: m_pStor(pStor), m_strCam(strCam), m_pFactory(pFactory), QWidget(parent, flags)
{
	ui.setupUi(this);
	VSCLoading * pLoading = VSCLoading::Create();
	
	setAcceptDrops(true);
	setMouseTracking(true);
	VSCCamInfo *pCamInfo = new VSCCamInfo(pFactory, m_pStor, m_strCam, this);
	ui.tabWidget->addTab(pCamInfo, tr("Information"));
	QCoreApplication::processEvents();
	ui.tabWidget->addTab(new VSCStreamSelect(pFactory, m_pStor, m_strCam, this), tr("Stream"));
	QCoreApplication::processEvents();
	ui.tabWidget->addTab(new VSCCamRec(pFactory, m_pStor, m_strCam, this), tr("Recording"));
	QCoreApplication::processEvents();
	ui.tabWidget->addTab(new VSCMotion(pFactory, m_pStor, m_strCam, this), tr("Motion"));
	QCoreApplication::processEvents();
	ui.tabWidget->addTab(new VSCSchedule(pFactory, m_pStor, m_strCam, this), tr("Schedule"));
	QCoreApplication::processEvents();

	//connect(this, SIGNAL(SignalSectionClicked(int, int)), ui.tableWidget, SIGNAL(cellClicked(int, int)));
	ui.storName->setText(pCamInfo->GetCameraName().c_str());
	
	QVBoxLayout* layout = new QVBoxLayout();
    m_pVideo = new VSCVWidget(m_pFactory, 0, this->ui.video, true);
	m_pVideo->hide();
    //layout->setSpacing(10);
	QCoreApplication::processEvents();

    layout->addWidget(m_pVideo);
    layout->setMargin(0);
	
	this->ui.video->setLayout(layout);
	m_pVideo->show();
	QCoreApplication::processEvents();
	m_pVideo->ShowVideoInfo(false);
	m_pVideo->StartPlay(m_pStor.strid(), m_strCam, "");
	QCoreApplication::processEvents();

	delete pLoading;
	
}

VidCamSetting::~VidCamSetting()
{

}



