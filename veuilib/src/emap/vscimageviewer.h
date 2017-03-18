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
#ifndef __VSC_IMAGEVIEWER_H__
#define __VSC_IMAGEVIEWER_H__

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QImageReader>
#include <QColor>
#include <QPalette>
#include <QTimer>
#include <QDebug>
#include <QErrorMessage>
#include <vector>
#include "utility.hpp"
#include "vscmapoverlay.h"
#include "vscinfooverlay.h"
#include "vsccontrolsoverlay.h"
#include <time.h>

class VSCImageViewerPrivate;

class VSCImageViewer : public QWidget
{
    Q_OBJECT

public:
    VSCImageViewer();
    VSCImageViewer(QWidget* parent);
    VSCImageViewer(QWidget* parent, QImage& image);
    ~VSCImageViewer();
    void setImage(QImage& image);
    bool getImageSize(s32 &w, s32 &h, s32 &wScale, s32 &hScale);
    VSCControlsOverlay* getControls();
    bool isDisplaying();


signals:
    void sendDoubleClick();
    void imageChanged();

public slots:
    void slotFitNormal();
    void slotFitWidth();
    void slotFitAll();
    void slotZoomIn();
    void slotZoomOut();
    void slotSetInfoString(QString);
    void slotShowInfo(bool);
    void slotShowControls(bool);

private slots:
    void onAnimation();

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);


private:
    void fitDefault();
    void fitHorizontal();
    void fitVertical();
    void fitOriginal();
    void fitWidth();
    void fitAll();
    void setScale(float scale);
    void centerImage();
    VSCImageViewerPrivate* d;
};

#endif // __VSC_IMAGEVIEWER_H__
