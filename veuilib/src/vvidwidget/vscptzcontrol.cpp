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
#include "vscptzcontrol.h"
#include "server/factory.hpp"
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
#include <QDialog>

//background-color:rgb(42, 43, 44)


VSCPTZControl::VSCPTZControl(ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags), m_pFactory(pFactory)
{
	ui.setupUi(this);

	setAcceptDrops(true);
	setMouseTracking(true);
	setStyleSheet("background-color: rgba(151, 152, 153, 100);");
#if 1
	setWindowOpacity( 0.8 );
#ifdef WIN32
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::Tool);
#else
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::ToolTip);
#endif
#endif

	ui.speedControl->setValue(50);

	/* Click event */
	connect( this->ui.pbLeft, SIGNAL( clicked() ), this, SLOT(PTZLeft()));
	connect( this->ui.pbRight, SIGNAL( clicked() ), this, SLOT(PTZRight()));
	connect( this->ui.pbUp, SIGNAL( clicked() ), this, SLOT(PTZUp()));
	connect( this->ui.pbDown, SIGNAL( clicked() ), this, SLOT(PTZDown()));
	connect( this->ui.pbZoomin, SIGNAL( clicked() ), this, SLOT(PTZZoomin()));
	connect( this->ui.pbZoomout, SIGNAL( clicked() ), this, SLOT(PTZZoomout()));

	/* Release stand for stop */
	connect( this->ui.pbLeft, SIGNAL( released() ), this, SLOT(PTZStop()));
	connect( this->ui.pbRight, SIGNAL( released() ), this, SLOT(PTZStop()));
	connect( this->ui.pbUp, SIGNAL( released() ), this, SLOT(PTZStop()));
	connect( this->ui.pbDown, SIGNAL( released() ), this, SLOT(PTZStop()));
	connect( this->ui.pbZoomin, SIGNAL( released() ), this, SLOT(PTZStop()));
	connect( this->ui.pbZoomout, SIGNAL( released() ), this, SLOT(PTZStop()));
	
}

VSCPTZControl::~VSCPTZControl()
{

}


bool VSCPTZControl::event(QEvent *e)
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

void VSCPTZControl::SetControlParam(astring strStorId, astring strCamId)
{
	m_strStorId = strStorId;
	m_strCamId = strCamId;
}

void VSCPTZControl::PTZAction(FPtzAction action)
{
	float speed = 0.5;

	int nSpeed = ui.speedControl->value();
	speed = (double)nSpeed /(double)100;

	//m_pProxy->PtzAction(action, speed);
	m_pFactory.GetStorFactory().PtzCmd(m_strStorId, m_strCamId,
		action, speed);
	


	return;
	
}

void VSCPTZControl::mousePressEvent(QMouseEvent *event)  
{  
    if (event->button() == Qt::LeftButton) {  
        m_Drag = true;  
        m_DragPosition = event->globalPos() - this->pos();  
        event->accept();  
    }  
}  
  
void VSCPTZControl::mouseMoveEvent(QMouseEvent *event)  
{  
    if (m_Drag && (event->buttons() && Qt::LeftButton)) {  
        move(event->globalPos() - m_DragPosition);  
        event->accept();  
    }  
}  

void VSCPTZControl::mouseReleaseEvent(QMouseEvent *)  
{  
    m_Drag = false;  
} 
  

void VSCPTZControl::PTZStop()
{
	PTZAction(F_PTZ_STOP);
}
void VSCPTZControl::PTZZoomin()
{
	PTZAction(F_PTZ_ZOOM_IN);
}
void VSCPTZControl::PTZZoomout()
{
	PTZAction(F_PTZ_ZOOM_OUT);
}
void VSCPTZControl::PTZLeft()
{
	PTZAction(F_PTZ_LEFT);
}
void VSCPTZControl::PTZUp()
{
	PTZAction(F_PTZ_UP);
}
void VSCPTZControl::PTZDown()
{
	PTZAction(F_PTZ_DOWN);
}
void VSCPTZControl::PTZRight()
{
	PTZAction(F_PTZ_RIGHT);
}

