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
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>
#include "switchbutton.h"

SwitchButton::SwitchButton(QWidget* parent) : QPushButton(parent) {
    setCheckable(true);
    // Set default colors and labels
    setColors();
    setLabels();
}

void SwitchButton::setColors(const QColor on, const QColor off) {
    onColor=on;
    offColor=off;
    if (on.red()+on.green()+on.blue()>500) {
        onLabelColor=Qt::black;
    }
    else {
        onLabelColor=Qt::white;
    }
    if (off.red()+off.green()+off.blue()>500) {
        offLabelColor=Qt::black;
    }
    else {
        offLabelColor=Qt::white;
    }
}

void SwitchButton::setLabels(const QString on, const QString off) {
    onLabel=on;
    offLabel=off;
    setMinimumWidth(fontMetrics().width(offLabel)+fontMetrics().width(onLabel)+fontMetrics().height()*2);
}

void SwitchButton::paintEvent(QPaintEvent* paint) {
    QPushButton::paintEvent(paint);
    QPainter p(this);
    p.save();
    if (isChecked()) {
        //QRect rect=paint->rect().adjusted(paint->rect().width()/2,4,-4,-4);
		QRect rect=paint->rect().adjusted(paint->rect().width()/2,0,0,0);
        p.fillRect(rect,QBrush(onColor));
        QPoint textPosi=rect.center()-QPoint(fontMetrics().width(onLabel)/2,1-fontMetrics().ascent()/2);
        p.setPen(onLabelColor);
        p.drawText(textPosi,onLabel);
    }
    else {
        QRect rect=paint->rect().adjusted(0,0,-paint->rect().width()/2,0);
        p.fillRect(rect,QBrush(offColor));
        QPoint textPosi=rect.center()-QPoint(fontMetrics().width(offLabel)/2,1-fontMetrics().ascent()/2);
        p.setPen(offLabelColor);
        p.drawText(textPosi,offLabel);
    }
    p.restore();
}

