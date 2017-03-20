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
#ifndef __VSC_VID_TREE_INF_H__
#define __VSC_VID_TREE_INF_H__

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>
#include "common/vscviditeminf.h"



/*  QTreeWidget::itemClicked for get the item is checked or not */

class VSCVidTreeInf : public QTreeWidget
{
    Q_OBJECT
public:
    VSCVidTreeInf(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVidTreeInf();
	
public:
	virtual void VidFilter(astring strFilter){}
	virtual void Init(){}
	virtual void VidSetCheckedChild(QTreeWidgetItem * item, bool bChecked){}
	virtual void VidGetSelectedItems(VidCameraIdMap &pMap){}
public slots:
	void SlotItemClicked(QTreeWidgetItem * item, int column);
	
protected:
	ClientFactory &m_pFactory;
	/* if the check has been called 
	    use for live and searchpb, search pb will call bsetchecked
	*/
	bool m_bSetChecked;
};

#endif