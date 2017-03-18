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

#ifndef __VSC_V_TREE_H__
#define __VSC_V_TREE_H__

#include <QWidget>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QMimeData>
#include "server/factory.hpp"

class VE_LIBRARY_API VSCQMimeView : public QMimeData
{
public:
    VSCQMimeView(int nViewId);
    ~VSCQMimeView(){}
public:
    int nId;
	
};


class VE_LIBRARY_API VSCViewTree : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
    //VSCViewTree(QTreeWidgetItem *parent, VSCViewDataItem &pParam);
	VSCViewTree(QTreeWidgetItem *parent);
    ~VSCViewTree();
	
public:
	u32 GetDeviceId()
	{
	    return 1;
	}
	
public:
    void UpdateOnline(BOOL bonline);
    void UpdateRecord(BOOL bRecording);	


private:
	//VSCViewDataItem m_Param;

};

#endif // VSC_V_IPC_H
