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
#include "vscemapbutton.h"
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

#include "vscvwidget.h"

//background-color:rgb(42, 43, 44)

VSCEmapButton::VSCEmapButton(QWidget *parent)
:QPushButton(parent)
{

	this->setAcceptDrops(true);
}

VSCEmapButton::~VSCEmapButton()
{

}

void VSCEmapButton::DragCommon()
{
	u32 nId = m_nId;
	VDC_DEBUG( "%s id %d\n",__FUNCTION__, nId);
	QMimeData *mimeData = new QMimeData();
	mimeData->setText(QString::number(nId));

	// Create drag
	QPixmap pixmap(":/action/resources/libuidome.png");
	QPainter painter(&pixmap);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap);
	      drag->setHotSpot(QPoint(drag->pixmap().width()/2,
	             drag->pixmap().height()/2));
	drag->exec();
}

void VSCEmapButton::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);
	Qt::MouseButtons mouseButtons = event->buttons();
	if( mouseButtons != Qt::LeftButton )
	{
		return;
	}

	return DragCommon();
}


