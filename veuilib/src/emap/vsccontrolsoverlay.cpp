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
#include "vsccontrolsoverlay.h"

VSCControlsOverlay::VSCControlsOverlay(QWidget *parent) :
    QWidget(parent)
{
    setPalette(Qt::transparent);
    this->setGeometry(parentWidget()->size().rwidth()-60,0,60,20);
    pen.setColor(Qt::green);
    pen.setWidth(1);
}

void VSCControlsOverlay::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(pen);
    painter.drawLine(QLine(4,15,15,15)); //minimize
    painter.drawRect(QRect(24,10,10,5)); //square
    painter.drawLine(QLine(44,4,54,14)); //cross 1
    painter.drawLine(QLine(44,15,54,5)); //cross 2
}

void VSCControlsOverlay::updateSize() {
    this->setGeometry(parentWidget()->size().rwidth()-60,0,60,20);
}

void VSCControlsOverlay::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if(event->pos().rx() >=39)
            emit exitClicked();
        if(event->pos().rx() <39 && event->pos().rx() >=19)
            emit exitFullscreenClicked();
        if(event->pos().rx() <19 && event->pos().rx() >=0)
            emit minimizeClicked();
    }
}











