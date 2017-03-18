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
#include "vscmapoverlay.h"

class VSCMapOverlayPrivate
{
public:
    VSCMapOverlayPrivate(VSCMapOverlay* qq);
    QPen penInner;
    QPen penOuter;

    QRect outerRect;
    QRect innerRect;

    VSCMapOverlay* q;

    /**
     * @brief Drawing size of map
     * This means cubic size of map because width == height
     */

    int mapSz;
    int mapMargin;
};

VSCMapOverlayPrivate::VSCMapOverlayPrivate(VSCMapOverlay* qq) :
    mapSz(100),
    mapMargin(10),
    q(qq)
{
}

VSCMapOverlay::VSCMapOverlay(QWidget *parent) : QWidget(parent),
    d(new VSCMapOverlayPrivate(this))
{
    updatePosition();
}

void VSCMapOverlay::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(QPen(QColor(40,40,40,160)));
    painter.fillRect(d->outerRect, QBrush(QColor(80,80,80,160), Qt::SolidPattern));
    painter.drawRect(d->outerRect);
    painter.fillRect(d->innerRect, QBrush(QColor(230,230,230,160), Qt::SolidPattern));
    painter.drawRect(d->innerRect);
}

void VSCMapOverlay::updatePosition() {
    this->setGeometry(parentWidget()->width()-d->mapSz-d->mapMargin,
                      parentWidget()->height()-d->mapSz-d->mapMargin,
                      d->mapSz+1,
                      d->mapSz+1);
}

// calculates outer(image) and inner(view area) squares
void VSCMapOverlay::updateMap(const QSize& windowSz, const QRect& drawingRect)
{
    if(windowSz.height() < drawingRect.height()
            || windowSz.width() < drawingRect.width()) {
        this->show();
        d->outerRect.setX(0);
        d->outerRect.setY(0);
        QSize outerSz = drawingRect.size();
        outerSz.scale(d->mapSz, d->mapSz, Qt::KeepAspectRatio);
        d->outerRect.setSize(outerSz);

        float widthDiff = (float) windowSz.width() / drawingRect.width();
        float heightDiff = (float) windowSz.height() / drawingRect.height();

        if (widthDiff > 1)
            widthDiff = 1;

        if (heightDiff > 1)
            heightDiff = 1;

        int width = outerSz.width() * widthDiff;
        int height = outerSz.height() * heightDiff;

        QSize innerSz(width, height);

        float xSpeedDiff = (float) innerSz.width() / windowSz.width();
        float ySpeedDiff = (float) innerSz.height() / windowSz.height();

        int x = -drawingRect.left() * xSpeedDiff;
        int y = -drawingRect.top() * ySpeedDiff;

        if (x < 0)
            x = 0;

        if (y < 0)
            y = 0;

        d->innerRect.setTopLeft(QPoint(x, y));
        d->innerRect.setSize(innerSz);

        //topdesign
        d->innerRect.setTopLeft(d->innerRect.topLeft()+QPoint(1,1));
        d->outerRect.setBottomRight(d->outerRect.bottomRight()-QPoint(1,1));

        //move to bottom left border
        d->outerRect.translate(d->mapSz - d->outerRect.width(),
                            d->mapSz - d->outerRect.height());
        d->innerRect.translate(d->mapSz - d->outerRect.width(),
                            d->mapSz - d->outerRect.height());
        update();
    }
    else {
        this->hide();
    }
}

void VSCMapOverlay::mouseMoveEvent(QMouseEvent* event)
{
    QWidget::mouseMoveEvent(event);
    event->accept();
}

void VSCMapOverlay::mousePressEvent(QMouseEvent* event)
{
    QWidget::mousePressEvent(event);
    event->accept();
}

void VSCMapOverlay::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget::mouseReleaseEvent(event);
    event->accept();
}
