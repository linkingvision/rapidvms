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
#ifndef __VSC_USER_EXPORT_H__
#define __VSC_USER_EXPORT_H__

#include <QDialog>
#include "ui_vscexport.h"
#include "utility.hpp"
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>
#include "vtaskmgr.hpp"

class VVidPBDelegate;

class VSCExport : public QDialog
{
	Q_OBJECT

public:
	VSCExport(astring strStor, astring strId, astring strCamName, u32 startTime, QWidget *parent = 0);
	~VSCExport(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

public:	
	bool SetStartTime(u32 startTime);
	bool SetDelegate(VVidPBDelegate *pDelegate);


public slots: 
	void StartClicked();
	void CancelClicked();
	void StartDateTimeChanged(const QDateTime &dateTime);

public:
	Ui::VSCExport ui;
	
private:
	QPoint m_DragPosition;
	bool m_Drag;
	u32 m_startTime;

	QDateTime m_endMax;
	QDateTime m_endMin;
	astring m_strStor;
	astring m_strId;
	astring m_strCamName;
	VVidPBDelegate *m_pDelegate;
	XMutex m_cMutex;

};

#endif // __VSC_USER_STATUS_H__
