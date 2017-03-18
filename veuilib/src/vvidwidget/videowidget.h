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
#ifndef __VIDEO_WIDGET_H
#define __VIDEO_WIDGET_H

#include <QWidget>

#define NOMINMAX
#define NOMINMAX 
//#include <inttypes.h>


#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
//#include "mpipeline.hpp"
using  namespace tthread;


class VideoWidget : public QWidget
{
    Q_OBJECT

public:
    VideoWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~VideoWidget();


public:
	void mouseMoveEvent(QMouseEvent *e); 
	bool event(QEvent * e);

	void resizeEvent( QResizeEvent * event );
	void paintEvent(QPaintEvent *);
	void setRunning(bool isRunning)
	{
		m_isRunning = isRunning;
		return;
	}
	
	
signals:
	void videoMouseMove(QMouseEvent *e);
	void videoHoverMove(QHoverEvent *e);
	void videoHoverEnter(QHoverEvent *e);
	void videoHoverLeave(QHoverEvent *e);
    void videoResize();

private:
	bool m_isRunning;
};

#endif // __VIDEO_WIDGET_H
