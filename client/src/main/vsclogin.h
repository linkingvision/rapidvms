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
#ifndef __VSC_LOGIN_H__
#define __VSC_LOGIN_H__

#include <QDialog>
#include "ui_vsclogin.h"
#include "utility.hpp"
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>

class VSCLogin : public QDialog
{
    Q_OBJECT

public:
    VSCLogin(QWidget *parent = 0);
    ~VSCLogin(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
public:
	BOOL GetIsLogin()
	{
		return m_isLogin;
	}
	BOOL SetDefault()
	{
		m_isLogin = FALSE;
		return TRUE;
	}
	BOOL GetUserPasswd(astring &strUser, astring &strPasswd);

public slots: 
	void LoginClicked();
	void ExitClicked();

public:
	Ui::VSCLogin ui;
private:
	QPoint m_DragPosition;
	bool m_Drag;

private:
	BOOL m_isLogin;
	
};

#endif // __VSC_LOGIN_H__
