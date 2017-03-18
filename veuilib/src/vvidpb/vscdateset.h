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
#ifndef __VSC_DATE_SET_CONF_H__
#define __VSC_DATE_SET_CONF_H__

#include <QDialog>
#include "ui_vscdateset.h"
#include "utility.hpp"
#include <QTimer>
#include <QLabel>
#include <QDate>
#include <QDialog>
#include <QPoint>
#include <QMouseEvent>

#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "utility.hpp"
#include "utility/videotype.hpp"
#include "XSDK/TimeUtils.h"
#include "XSDK/XBlockingQueue.h"

using namespace UtilityLib;
using namespace XSDK;

class VVidPBDelegate;

class VSCDateSet : public QDialog
{
    Q_OBJECT

public:
	VSCDateSet(QWidget *parent = 0);
	~VSCDateSet(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
public:
	bool SetDelegate(VVidPBDelegate *pDelegate);
	bool ClearPlay();
	
public slots: 
	void SlotDataClicked(const QDate & date);
	void SlotPageChanged(int year, int month);

signals:
    	void SignalSeekTime(unsigned int);

public:
	Ui::VSCDateSet ui;

private:
	QPoint m_DragPosition;
	bool m_Drag;
	VVidPBDelegate *m_pDelegate;
	XMutex m_cMutex;
};

#endif // __VSC_DATE_SET_CONF_H__
