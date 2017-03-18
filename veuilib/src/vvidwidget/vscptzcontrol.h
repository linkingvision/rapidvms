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
#ifndef __PTZ_CONTROL_H
#define __PTZ_CONTROL_H

#include <QWidget>

#define NOMINMAX
#define NOMINMAX 
//#include <inttypes.h>

#include <QDialog>
#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "ui_vscptzcontrol.h"
#include "client/clientfactory.hpp"
using  namespace tthread;


class VSCPTZControl : public QDialog
{
    Q_OBJECT

public:
	VSCPTZControl(ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSCPTZControl();
	bool event(QEvent *e);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	void SetControlParam(astring strStorId, astring strCamId);

private slots:
	void PTZAction(FPtzAction action);
	void PTZStop();
	void PTZZoomin();
	void PTZZoomout();
	void PTZLeft();
	void PTZUp();
	void PTZDown();
	void PTZRight();

public:
	Ui::VSCPTZControl ui;
private:
	QPoint m_DragPosition;
	bool m_Drag;

	ClientFactory &m_pFactory;
	astring m_strStorId;
	astring m_strCamId;
};

#endif // __VIDEO_CONTROL_H
