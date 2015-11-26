#include "vscplaycontrol.h"
#include "debug.hpp"
#include <QDesktopWidget>
#include <QApplication>


VSCPlayControl::VSCPlayControl(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |  Qt::WindowStaysOnBottomHint);
	//setWindowFlags( Qt::WindowStaysOnTopHint |  Qt::WindowStaysOnBottomHint);
	setWindowOpacity(0.8);
	int x, y, w, h;
	
	QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	
	x = rect.x();
	y = rect.y();
	w = rect.width();
	h = rect.height();
	//rect.getRect(&x, &y, &w, &h);
	VDC_DEBUG( "%s rect x:%d y:%d w:%d h:%d \n",__FUNCTION__, x, y, w, h);
	QPoint p(x + w/2 - 240, y + h - 150 );
	move(p);
}

