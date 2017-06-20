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
#include "vscemapcam.h"
#include "client/clientfactory.hpp"
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

#include "vscvwidget.h"
#include "vvidpb1.h"

//background-color:rgb(42, 43, 44)

VSCEmapCam::VSCEmapCam(VidEmapCamera &cCam, ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
    : m_cCam(cCam), m_pFactory(pFactory), QWidget(parent, flags)
{
	 ui.setupUi(this);
	//setAttribute(Qt::WA_TranslucentBackground);
	setStyleSheet("background:transparent;");
	this->setAcceptDrops(true);
#ifdef WIN32
	//setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::Tool);
#else
	//setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::ToolTip);
	//setFocusPolicy(Qt::NoFocus);
#endif
	ui.play->setId(0);
	connect(ui.play, SIGNAL( clicked() ), this, SLOT(DragEmap()));
	m_pPlay = new QAction(QIcon(tr(":/action/resources/libuiplay.png")), tr("PLAY"), this);
	connect(m_pPlay, SIGNAL(triggered()), this, SLOT(PlayVideo()));
	
	m_pPlayback = new QAction(QIcon(tr(":/action/resources/libuiinstantpb.png")), tr("PLAYBACK"), this);
	connect(m_pPlayback, SIGNAL(triggered()), this, SLOT(PlaybackVideo()));
	
	m_pDelete = new QAction(QIcon(tr(":/action/resources/libuidel.png")), tr("DELETE"), this);
	connect(m_pDelete, SIGNAL(triggered()), this, SLOT(EmapDelete()));

	m_TimerAlarm = new QTimer(this);
	connect(m_TimerAlarm, SIGNAL(timeout()), this, SLOT(showAlarm()));
	ui.play->setIcon(QIcon(":/action/resources/libuidome.png"));
	astring strName = m_pFactory.GetStorFactory().GetVidCameraName(m_cCam.cid().strstorid(), 
						m_cCam.cid().strcameraid());
	ui.name->setText(strName.c_str());

	m_alarm = false;
}

VSCEmapCam::~VSCEmapCam()
{
	if (m_pPlay)
		delete m_pPlay;

	if (m_pDelete)
		delete m_pDelete;

}

void VSCEmapCam::OnDeviceEvent(int deviceId, int type)
{
	if (m_alarmCnt > 0)
	{
		m_alarmCnt = 20;
		return;
	}
	m_alarmCnt = 20;
	m_TimerAlarm->start(1000);
	m_alarm = true;
}

void VSCEmapCam::PlayVideo()
{

	VSCVWidget * pVideo = new VSCVWidget(m_pFactory, 0, NULL, true);
	astring strName = m_pFactory.GetStorFactory().GetVidCameraName(m_cCam.cid().strstorid(), 
						m_cCam.cid().strcameraid());
	pVideo->ShowVideoInfo(false);
	
	pVideo->setWindowTitle(QApplication::translate("VSCMainWindowsClass", 
								strName.c_str(), 0));	
	pVideo->StartPlay(m_cCam.cid().strstorid(), m_cCam.cid().strcameraid(), strName);
	pVideo->show();
}

void VSCEmapCam::PlaybackVideo()
{
	astring strName = m_pFactory.GetStorFactory().GetVidCameraName(m_cCam.cid().strstorid(), 
						m_cCam.cid().strcameraid());
	VVidPB1 *pPB1 = new VVidPB1(m_pFactory);
	pPB1->show();

	s32 currentTime = time(NULL) - 5 * 60;
	if (currentTime < 0)
	{
		currentTime = 0;
	}
	
	pPB1->StartPlay(m_cCam.cid().strstorid(), m_cCam.cid().strcameraid(), strName, currentTime);
}

void VSCEmapCam::EmapDelete()
{
	emit EmapCamDel(m_cCam.cid().strcameraid());
}

#if 0
bool VSCEmapCam::event(QEvent *e)
{
    if(e->type() == QEvent::HoverEnter)
    {
	   VDC_DEBUG( "%s HoverEnter\n", __FUNCTION__);
#ifdef WIN32
        //emit signalFocus();
#endif
        //e->accept();
    }
    else if(e->type() == QEvent::HoverLeave)
    {
    	VDC_DEBUG( "%s HoverLeave\n", __FUNCTION__);
        //hide();
    }

    return QWidget::event(e);
}
#endif

void VSCEmapCam::contextMenuEvent(QContextMenuEvent* e) 
{
	QMenu * popupMenu;
	popupMenu = new QMenu();
#ifdef WIN32
	popupMenu->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif
	popupMenu->addAction(m_pPlay);
	popupMenu->addAction(m_pPlayback);

	popupMenu->addSeparator();
	popupMenu->addAction(m_pDelete);

	popupMenu->addSeparator();

	popupMenu->exec(e->globalPos()); 
	delete popupMenu;
	popupMenu = NULL;
}

void VSCEmapCam::DragCommon()
{
	QMimeData *mimeData = new VSCQMimeVidCam(m_cCam.cid().strstorid(), 
		m_cCam.cid().strcameraid(), "Camera");

	// Create drag
	QPixmap pixmap(":/action/resources/libuidome.png");
	//QPainter painter(&pixmap);

	//mimeData->setText(pStor->GetName().c_str());
	//this->setCursor(QCursor(Qt::ClosedHandCursor));

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap);
	drag->setHotSpot(QPoint(drag->pixmap().width()/2,
	             drag->pixmap().height()/2));
	drag->exec();
}

void VSCEmapCam::DragEmap()
{
	return DragCommon();
}

void VSCEmapCam::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);
	Qt::MouseButtons mouseButtons = event->buttons();
	if( mouseButtons != Qt::LeftButton )
	{
		return;
	}

	return DragCommon();
}

void VSCEmapCam::showAlarm()
{
	if (m_alarmCnt <= 0)
	{
		m_TimerAlarm->stop();
		ui.play->setIcon(QIcon(":/action/resources/libuidome.png"));
		ui.name->setStyleSheet(QStringLiteral("color: rgb(0, 170, 0);"));
	}else
	{
		if (m_alarm == false)
		{
			ui.play->setIcon(QIcon(":/action/resources/libuidome.png"));
			ui.name->setStyleSheet(QStringLiteral("color: rgb(0, 170, 0);"));
			m_alarm = true;
		}else
		{
			ui.play->setIcon(QIcon(":/action/resources/libuidomealarm.png"));
			ui.name->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
			m_alarm = false;
		}
		m_alarmCnt --;
	}
}


