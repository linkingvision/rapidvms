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
#ifndef __VIDEO_VIDVIEWCONF_H__
#define __VIDEO_VIDVIEWCONF_H__

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QMimeData>
#include <QDrag>

#define NOMINMAX
#define NOMINMAX 

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include "ui_vidviewconf.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>

class VidViewConfTableItem : public QTableWidgetItem
{
public:
	VidViewConfTableItem(VidView &pView, bool bNew)
		: m_sView(pView), m_bNew(bNew)
	{
	}
	~VidViewConfTableItem(){}
public:
	bool IsNew(){return m_bNew;}
	VidView GetView(){return m_sView;}
private:
	VidView m_sView;
	bool m_bNew;
};

class VidViewConf : public QWidget
{
    Q_OBJECT

public:
	VidViewConf(ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VidViewConf();

public:
	void TreeWidgetUpdate();

public slots:
	void SlotDeleteView();

public:
    	Ui::VidViewConf ui;
private:
	ClientFactory &m_pFactory;

};


#endif // __VIDEO_VIDVIEWCONF_H__
