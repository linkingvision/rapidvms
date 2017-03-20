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
#ifndef __VSC_VIEW_CONF_H__
#define __VSC_VIEW_CONF_H__

#include <QDialog>
#include "ui_vscviewconf.h"
#include "utility.hpp"
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>

typedef enum
{
    VSC_VIEW_CONF_NEW = 1,
    VSC_VIEW_CONF_MODIFY,
    VSC_VIEW_CONF_LAST
} VSCViewConfType;

class VSCViewConf : public QDialog
{
    Q_OBJECT

public:
    VSCViewConf(QWidget *parent = 0);
    ~VSCViewConf(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
public:
	VSCViewConfType GetType()
	{
		return m_Type;
	}
	bool SetName(string strName)
	{
		ui.lineEditName->setText(strName.c_str());
		return true;
	}
	bool GetName(string &strName)
	{
	       strName = ui.lineEditName->text().toStdString().c_str();
		return true;
		   	
	}

public slots: 
	void NewClicked();
	void ModifyClicked();

public:
	Ui::VSCViewConf ui;
	
private:
    VSCViewConfType m_Type;
private:
	QPoint m_DragPosition;
	bool m_Drag;

    
};

#endif // __VSC_VIEW_CONF_H__
