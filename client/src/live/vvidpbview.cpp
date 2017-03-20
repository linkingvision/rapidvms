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
#include "vvidpbview.h"
#include "debug.hpp"
#include <QDesktopWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QDesktopWidget>
#include <QApplication>
#include "vvidpb1.h"
//#include "vscviewconf.h"

VVidPBView::VVidPBView(ClientFactory &pFactory, QWidget *parent, QTabWidget &pTabbed, QString strName)
    : m_pFactory(pFactory), QWidget(parent), m_pTabbed(pTabbed), m_currentFocus(-1), 
    m_bControlEnable(TRUE), m_strName(strName), m_bFloated(FALSE), 
	m_TimerTour(NULL)
{
    ui.setupUi(this);
    m_pParent = parent;
    m_lastHoverTime = time(NULL);

    this->setAttribute(Qt::WA_Hover,true);
    QVBoxLayout* layout = new QVBoxLayout();
    m_pVideo = new VVidPBWall(m_pFactory, this->ui.widget);
    m_pVideo->hide();
    //layout->setSpacing(10);

    layout->addWidget(m_pVideo);
    layout->setMargin(0);

    this->ui.widget->setLayout(layout);

    m_pVideo->show();

    SetupConnections();
    createContentMenu();

    //ui.pushButton8x8->hide();
    setMouseTracking(true);

    m_Timer = new QTimer(this);
    connect(m_Timer,SIGNAL(timeout()),this,SLOT(UpdateVideoControl())); 
    m_Timer->start(1000); 

    ShowFocusClicked(0);
}


void VVidPBView::SetupConnections()
{
	connect(ui.pushButton2x2, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode2x2()));
	connect(ui.pushButton3x3, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode3x3()));
	connect(ui.pushButton4x4, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode4x4()));
	connect(ui.pushButton6, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode6()));
	connect(ui.pushButton1, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode1()));
	connect(ui.pushButton12p1, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode12p1()));

	connect(ui.pushButton1x3, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode1x3()));
	connect(ui.pushButton1p6, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode1p6()));

	connect(ui.pushButton2x2, SIGNAL(clicked()), this, SLOT(SetLayoutMode2x2()));
	connect(ui.pushButton3x3, SIGNAL(clicked()), this, SLOT(SetLayoutMode3x3()));
	connect(ui.pushButton4x4, SIGNAL(clicked()), this, SLOT(SetLayoutMode4x4()));
	connect(ui.pushButton6, SIGNAL(clicked()), this, SLOT(SetLayoutMode6()));
	connect(ui.pushButton1, SIGNAL(clicked()), this, SLOT(SetLayoutMode1()));
	connect(ui.pushButton12p1, SIGNAL(clicked()), this, SLOT(SetLayoutMode12p1()));

	connect(ui.pushButton1x3, SIGNAL(clicked()), this, SLOT(SetLayoutMode1x3()));
	connect(ui.pushButton1p6, SIGNAL(clicked()), this, SLOT(SetLayoutMode1p6()));
	
	connect(m_pVideo, SIGNAL(ShowFocusClicked(int)), this,
	                                    SLOT(ShowFocusClicked(int)));
	connect(m_pVideo, SIGNAL(Layout1Clicked(int)), this,
	                                    SLOT(ShowLayout1Clicked(int)));
	
}

VVidPBView::~VVidPBView()
{
	
}

void VVidPBView::CameraDoubleClicked(astring strStor, astring strCam, 
						astring strCamName)
{

	m_pVideo->PlayVideoByWindow(m_currentFocus, strStor, strCam, strCamName);

	return;
}



void VVidPBView::mouseMoveEvent(QMouseEvent *event)
{
	QPoint posView = mapToGlobal(QPoint(0,0));
	QPoint posEvent = event->globalPos();
        VDC_DEBUG( "%s View (%d, %d)  Event (%d, %d)\n", 
			__FUNCTION__, posView.x(), posView.y(), 
			posEvent.x(), posEvent.y());
        //m_pVideo->OffAllFocus();
}

bool VVidPBView::event(QEvent *e)
{
	//VDC_DEBUG("%s Event Type %d\n", __FUNCTION__, e->type());


	if(e->type() == QEvent::HideToParent )
		//|| e->type() == QEvent::WindowDeactivate)
	{
		m_pVideo->OffAllFocus();
	}
	if(e->type() == QEvent::WindowActivate)
	{
		m_pVideo->ResizeAllVideo();
	}
	
	QHoverEvent *event = static_cast<QHoverEvent*> (e);
	if (event == NULL)
	{
		return true;
	}
   if(e->type() == QEvent::HoverMove)
    {
	QPoint posView = mapToGlobal(QPoint(0,0));
	QPoint posEvent = event->pos();
        //VDC_DEBUG( "%s View (%d, %d)  Event (%d, %d)\n", 
	//		__FUNCTION__, posView.x(), posView.y(), 
	//		posEvent.x(), posEvent.y());
        //m_pVideo->OffAllFocus();
        m_lastHoverTime = time(NULL);
    }

    if(e->type() == QEvent::HoverLeave)
    {
	QPoint posView = mapToGlobal(QPoint(0,0));
	QPoint posEvent = event->pos();
#if 0
        VDC_DEBUG( "%s Leave View (%d, %d)  Event (%d, %d)\n", 
			__FUNCTION__, posView.x(), posView.y(), 
			posEvent.x(), posEvent.y());
#endif
    }


    return QWidget::event(e);
}

void VVidPBView::ShowFocusClicked(int nId)
{
    //if (m_currentFocus != nId)
    {
        if (m_currentFocus != -1)
        {
            m_pVideo->SetVideoFocus(m_currentFocus, FALSE);
        }
        m_pVideo->SetVideoFocus(nId, TRUE);
        m_currentFocus = nId;
    }
}

void VVidPBView::ShowLayout1Clicked(int nId)
{
    m_pVideo->SetLayout1Mode(nId);
}

void VVidPBView::mouseDoubleClickEvent(QMouseEvent *e)
{
    VDC_DEBUG( "%s mouseDoubleClickEvent\n",__FUNCTION__);
    QWidget::mouseDoubleClickEvent(e);
#if 0
    if(isFullScreen()) {
        //setParent(m_pParent);
        //resize(m_pParent->width(), m_pParent->height());
        //showMaximized();
        this->setWindowState(Qt::WindowMaximized);
    } else {
        //m_pParent = parentWidget();
        //setParent(NULL);
        //showFullScreen();
        this->setWindowState(Qt::WindowFullScreen);
    }
#endif
}


void VVidPBView::createContentMenu()
{

}

