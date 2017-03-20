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
#ifndef __VSC_VID_TREE_VIEW_H__
#define __VSC_VID_TREE_VIEW_H__

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include "common/vscvidtreeinf.h"
#include "vscvwidget.h"

class VSCVidTreeView : public VSCVidTreeInf
{
    Q_OBJECT
public:
    VSCVidTreeView(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVidTreeView();

signals:
	void ViewSelected(std::string strView);
public:
	void mousePressEvent(QMouseEvent *event);	
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	
public:
	virtual void VidFilter(astring strFilter);
	virtual void Init();
public:
	/* Take care all the vid Stor state change */
	bool CallChange(ClientFactoryChangeData data);
public slots:
	void SlotCallChange(int type, std::string strId);
public:
	void TreeUpdate();
private:
	QTreeWidgetItem * m_pRoot;
	bool m_bInit;
};

#endif
