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
#include "vscimageviewer.h"

#include <qgraphicseffect.h>

enum WindowResizePolicy
{
    NORMAL,
    WIDTH,
    ALL,
    FREE
};

class VSCImageViewerPrivate
{
public:
    VSCImageViewerPrivate(VSCImageViewer* qq);
    ~VSCImageViewerPrivate();
    void centerVertical();
    void centerHorizontal();
    void setImage(QImage& image);
    void setScale(float scale);
    void scaleImage();
    float scale() const;
    bool scaled() const;

    QImageReader imageReader;
    QImage imageOrg, image, imageScaled;
    QPoint cursorMovedDistance;
    QRect drawingRect;
    QSize shrinkSize;
    VSCMapOverlay *mapOverlay;
    VSCInfoOverlay *infoOverlay;
    VSCControlsOverlay *controlsOverlay;
    VSCImageViewer* q;

    WindowResizePolicy resizePolicy;

    int freeSpaceLeft;
    int freeSpaceBottom;
    int freeSpaceRight;
    int freeSpaceTop;

    bool isDisplaying;

    static const float maxScale;
    static const float minScale;
    static const float zoomStep;

private:
    float currentScale;
};

const float VSCImageViewerPrivate::maxScale = 0.10;
const float VSCImageViewerPrivate::minScale = 3.0;
const float VSCImageViewerPrivate::zoomStep = 0.1;

VSCImageViewerPrivate::VSCImageViewerPrivate(VSCImageViewer* qq)
    : q(qq),
      shrinkSize(),
      currentScale(1.0),
      resizePolicy(NORMAL),
      isDisplaying(false)
{
    infoOverlay = new VSCInfoOverlay(q);
    mapOverlay = new VSCMapOverlay(q);
    controlsOverlay = new VSCControlsOverlay(q);
    infoOverlay->hide();
    controlsOverlay->hide();
}

VSCImageViewerPrivate::~VSCImageViewerPrivate()
{
}

bool VSCImageViewerPrivate::scaled() const
{
    return scale() != 1.0;
}

void VSCImageViewerPrivate::setImage(QImage& i) 
{
	imageOrg = i;
}

float VSCImageViewerPrivate::scale() const
{
    return currentScale;
}

void VSCImageViewerPrivate::setScale(float scale)
{
    currentScale = scale;
    QSize sz;
    sz = image.size();
    sz = sz.scaled(sz * scale, Qt::KeepAspectRatio);
    drawingRect.setSize(sz);
}

void VSCImageViewerPrivate::scaleImage()
{
    if(scaled()) {
        imageScaled = image.scaled(drawingRect.size(),Qt::KeepAspectRatio);
    }
}

void VSCImageViewerPrivate::centerHorizontal()
{
    QPoint point((q->width() - drawingRect.width()) / 2, drawingRect.y());
    drawingRect.moveTo(point);
}

void VSCImageViewerPrivate::centerVertical()
{
    QPoint point(drawingRect.x(), (q->height() - drawingRect.height()) / 2);
    drawingRect.moveTo(point);
}

VSCImageViewer::VSCImageViewer(): QWidget(), d(new VSCImageViewerPrivate(this))
{
    d->imageOrg.load(":/action/resources/defaultmap.jpg");
    d->image = d->imageOrg.scaled(width(), height(),Qt::KeepAspectRatio);
    d->drawingRect = d->image.rect();
    fitDefault();
}

VSCImageViewer::VSCImageViewer(QWidget* parent): QWidget(parent),
d(new VSCImageViewerPrivate(this))
{
    d->imageOrg.load(":/action/resources/defaultmap.jpg");
    d->image = d->imageOrg.scaled(width(), height(),Qt::KeepAspectRatio);
    d->drawingRect = d->image.rect();
}

VSCImageViewer::VSCImageViewer(QWidget* parent, QImage& image) : QWidget(parent),
d(new VSCImageViewerPrivate(this))
{
    setImage(image);
}

VSCImageViewer::~VSCImageViewer()
{
    delete d;
}

void VSCImageViewer::setImage(QImage& image)
{
    d->setImage(image);
    d->image = d->imageOrg.scaled(width(), height(),Qt::KeepAspectRatio);
    d->drawingRect = d->image.rect();
    fitDefault();
}

bool VSCImageViewer::getImageSize(s32 &w, s32 &h, s32 &wScale, s32 &hScale)
{
	QSize sz;
	sz = d->imageOrg.size();
	w = sz.width();
	h = sz.height();

	QSize sz2;
	sz2 = d->image.size();
	wScale = sz2.width();
	hScale = sz2.height();

	return true;
}

void VSCImageViewer::onAnimation()
{
    d->scaleImage();
    update();
}

void VSCImageViewer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setBrush(QColor(41,42,43,80));
    painter.drawRect(0,0,this->width(),this->height());

    char strText[1024];
    sprintf(strText, "%d x %d", this->width(),this->height());

    //QFont font("sans");
    //font.setPointSize(11);
    //painter.setFont(font);
    painter.setPen(QPen(Qt::black, 2));


    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    if(d->scaled()) {
        painter.drawImage(d->drawingRect, d->imageScaled);
    }
    else {
        painter.drawImage(d->drawingRect, d->image);
    }
    painter.drawText(this->width() - 100, this->height() - 10, strText);
    //painter.drawText(5, 20, QObject::tr("Drop camera to here ..."));
}

void VSCImageViewer::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        this->setCursor(QCursor(Qt::ClosedHandCursor));
        d->cursorMovedDistance = event->pos();
    }
}

void VSCImageViewer::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        d->cursorMovedDistance -= event->pos();

        int left = d->drawingRect.x() - d->cursorMovedDistance.x();
        int top = d->drawingRect.y() - d->cursorMovedDistance.y();
        int right = left + d->drawingRect.width();
        int bottom = top + d->drawingRect.height();

        if (left < 0 && right > size().width())
            d->drawingRect.moveLeft(left);

        if (top < 0 && bottom > size().height())
            d->drawingRect.moveTop(top);

        d->cursorMovedDistance = event->pos();
        update();
        d->mapOverlay->updateMap(size(),d->drawingRect);
    }
}

void VSCImageViewer::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        this->setCursor(QCursor(Qt::ArrowCursor));
        d->cursorMovedDistance = event->pos();
    }
}

void VSCImageViewer::fitWidth()
{
    float scale = (float) width() / d->image.width();
    d->drawingRect.setX(0);
    d->setScale(scale);
    if(d->drawingRect.height()<=height()) {
        QPoint point(0, (height() - d->drawingRect.height()) / 2);
        d->drawingRect.moveTo(point);
    }
    else
        d->drawingRect.moveTop(0);
    if(d->scaled()) {
        d->imageScaled = d->image.scaled(d->drawingRect.size(),Qt::KeepAspectRatio);
    }
    update();
}

void VSCImageViewer::fitHorizontal()
{
    float scale = (float) width() / d->image.width();
    d->drawingRect.setX(0);
    d->setScale(scale);
    d->centerVertical();
}

void VSCImageViewer::fitVertical()
{
    float scale = (float) height() / d->image.height();
    d->drawingRect.setY(0);
    d->setScale(scale);
    d->centerHorizontal();
}

void VSCImageViewer::fitAll()
{
    bool h = d->image.height() < this->height();
    bool w = d->image.width() < this->width();
    if(h && w) {
        fitOriginal();
    }
    else {
        float widgetAspect = (float) height() / width();
        float drawingAspect = (float) d->drawingRect.height() /
                d->drawingRect.width();
        if(widgetAspect < drawingAspect)
            fitVertical();
        else
            fitHorizontal();
    }
    if(d->scaled()) {
        d->imageScaled = d->image.scaled(d->drawingRect.size(),Qt::KeepAspectRatio);
    }
    update();
}

void VSCImageViewer::fitOriginal()
{
   setScale(1.0);
   centerImage();
   update();
}

void VSCImageViewer::fitDefault() {
    switch(d->resizePolicy) {
        case NORMAL: fitOriginal(); break;
        case WIDTH: fitWidth(); break;
        case ALL: fitAll(); break;
        default: centerImage(); break;
    }
    d->scaleImage();
    d->mapOverlay->updateMap(size(),d->drawingRect);
}

void VSCImageViewer::centerImage() {
    int left = d->drawingRect.left()<0?0:d->drawingRect.left();
    int right = d->drawingRect.right()<0?0:d->drawingRect.right();
    int top = d->drawingRect.top()<0?0:d->drawingRect.top();
    int bottom = d->drawingRect.bottom()<0?0:d->drawingRect.bottom();
    int spaceLeft = left - rect().left();
    int spaceTop = top - rect().top();
    int spaceRight = rect().right() - right;
    int spaceBottom = rect().bottom() - bottom;

    if (spaceLeft < 0 && spaceRight > 0)
        d->drawingRect.translate(spaceRight, 0);
    else if (spaceLeft > 0 || spaceRight > 0)
        d->centerHorizontal();

    if (spaceTop < 0 && spaceBottom > 0)
        d->drawingRect.translate(0, spaceBottom);
    else if (spaceTop > 0 || spaceBottom > 0)
        d->centerVertical();
}

void VSCImageViewer::slotFitNormal() {
    d->resizePolicy = NORMAL;
    fitDefault();
}

void VSCImageViewer::slotFitWidth() {
    d->resizePolicy = WIDTH;
    fitDefault();
}

void VSCImageViewer::slotFitAll() {
    d->resizePolicy = ALL;
    fitDefault();
}

void VSCImageViewer::resizeEvent(QResizeEvent* event)
{
    resize(event->size());

    d->image = d->imageOrg.scaled(width(), height(),Qt::KeepAspectRatio);
    d->drawingRect = d->image.rect();
	
    fitDefault();
    d->scaleImage();
    d->mapOverlay->updateMap(size(),d->drawingRect);
    d->controlsOverlay->updateSize();
    d->mapOverlay->updatePosition();
}

void VSCImageViewer::mouseDoubleClickEvent(QMouseEvent *event) {
    emit sendDoubleClick();
}

void VSCImageViewer::slotZoomIn() {
    float possibleScale = d->scale() + d->zoomStep;
    if (possibleScale <= d->minScale) {
        d->setScale(possibleScale);
    }
    else {
        d->setScale(d->minScale);
    }
    d->centerHorizontal();
    d->centerVertical();
    d->resizePolicy = FREE;
    d->scaleImage();
    update();
    d->mapOverlay->updateMap(size(),d->drawingRect);
}

void VSCImageViewer::slotZoomOut() {
    float possibleScale = d->scale() - d->zoomStep;
    if (possibleScale >= d->maxScale) {
        d->setScale(possibleScale);
    }
    else {
        d->setScale(d->maxScale);
    }
    d->centerHorizontal();
    d->centerVertical();
    d->resizePolicy = FREE;
    d->scaleImage();
    update();
    d->mapOverlay->updateMap(size(),d->drawingRect);
}


void VSCImageViewer::setScale(float scale)
{
    d->setScale(scale);
}

void VSCImageViewer::slotSetInfoString(QString info) {
    d->infoOverlay->setText(info);
}

void VSCImageViewer::slotShowInfo(bool x) {
    x?d->infoOverlay->show():d->infoOverlay->hide();
}

void VSCImageViewer::slotShowControls(bool x) {
    x?d->controlsOverlay->show():d->controlsOverlay->hide();
}

VSCControlsOverlay* VSCImageViewer::getControls() {
    return d->controlsOverlay;
}

bool VSCImageViewer::isDisplaying() {
    return d->isDisplaying;
}
