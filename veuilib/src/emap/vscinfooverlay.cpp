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
#include "vscinfooverlay.h"

VSCInfoOverlay::VSCInfoOverlay(QWidget *parent) :QWidget(parent) {
    setPalette(Qt::transparent);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    currentText = "No file opened.";
    this->setFixedHeight(30);
    updateSize();
}

void VSCInfoOverlay::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QFont font;
    font.setPixelSize(13);
    painter.setFont(font);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::green));
    painter.drawText(QRect(10,5,950,16),currentText);
}

void VSCInfoOverlay::setText(QString text) {
    currentText = text;
    this->update();
}

void VSCInfoOverlay::updateSize() {
    this->setMinimumWidth(this->parentWidget()->size().rwidth()-60);
}



