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
#ifndef __VSC_DASH_BOARD_H__
#define __VSC_DASH_BOARD_H__

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QMimeData>
#include <QDrag>

#define NOMINMAX
#define NOMINMAX 

#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "ui_vscdashboard.h"
#include "vtaskmgr.hpp"

using  namespace tthread;

typedef std::map<std::string, QWidget *> VTaskItemWidgetList;

class VSCDashBoard : public QWidget
{
    Q_OBJECT

public:
	VSCDashBoard(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSCDashBoard();

public slots:
	void UpdateTaskList();
public:
    	Ui::VSCDashBoard ui;
private:
	VTaskItemList m_pTaskList;
	VTaskItemWidgetList m_pTaskWidget;

};


#endif // __VIDEO_PANEL_H__
