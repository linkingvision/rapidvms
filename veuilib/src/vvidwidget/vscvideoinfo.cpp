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
#include "vscvideoinfo.h"
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

//background-color:rgb(42, 43, 44)


VSCVideoInfo::VSCVideoInfo(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
	 ui.setupUi(this);
#if 0
	setAttribute(Qt::WA_TranslucentBackground);
	setStyleSheet("background:transparent;");
#ifdef WIN32
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::Tool);
#else
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::ToolTip);
	setFocusPolicy(Qt::NoFocus);
#endif	
#endif
}

VSCVideoInfo::~VSCVideoInfo()
{

}
bool VSCVideoInfo::event(QEvent *e)
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

