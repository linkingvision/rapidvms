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
#ifndef VSC_TOOL_BAR_H
#define VSC_TOOL_BAR_H

#include <QWidget>
#include "ui_vsctoolbar.h"
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include "vsccalculate.hpp"

class VSCToolBar : public QWidget
{
    Q_OBJECT

public:
    VSCToolBar(QWidget *parent = 0);
    ~VSCToolBar(){}
	
public slots:
	void showcurrentTime();
	void showAlarm();
	void NewAlarm();
	void showProcessingTimer();
	void showProcessing(bool bEnable);
public:
    Ui::VSCToolBar ui;
	
private:
	QTimer *m_Timer;
	QTimer *m_TimerAlarm;
	QTimer *m_TimerPrcessing;
	bool m_alarm;
	int m_alarmCnt;
	VSCCalculate * m_cpu;
	QMovie * m_movie;
    
};

#endif // VSC_TOOL_BAR_H
