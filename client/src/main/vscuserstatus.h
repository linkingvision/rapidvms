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
#ifndef __VSC_USER_STATUS_H__
#define __VSC_USER_STATUS_H__

#include <QDialog>
#include "ui_vscuserstatus.h"
#include "utility.hpp"
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>

class VSCUserStatus : public QDialog
{
    Q_OBJECT

public:
    VSCUserStatus(QWidget *parent = 0);
    ~VSCUserStatus(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
public:
	BOOL IsQuit()
	{
		return m_quit;
	}

public slots: 
	void QuitClicked();
	void CancelClicked();

public:
	Ui::VSCUserStatus ui;
private:
	QPoint m_DragPosition;
	bool m_Drag;

private:
	BOOL m_quit;
	
};

#endif // __VSC_USER_STATUS_H__
