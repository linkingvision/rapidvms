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
#ifndef __VSC_VID_ITEM_INF_H__
#define __VSC_VID_ITEM_INF_H__

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>

typedef std::map<astring, VidCameraId> VidCameraIdMap;

class VSCVidItemInf : public QTreeWidgetItem
{
public:
	VSCVidItemInf(ClientFactory &pFactory, QTreeWidgetItem *parent);
	~VSCVidItemInf();
public:
	void UpdateOnline(bool bOnline);
	void UpdateRec(bool bRec);

public:
	virtual  void VidSetChecked(QTreeWidgetItem * item, bool bChecked);
	virtual  bool VidGetChecked();
	virtual void VidSetCheckedChild(QTreeWidgetItem * item, bool bChecked){}
	virtual void VidGetSelectedItems(VidCameraIdMap &pMap){}
	
public:
	virtual astring GetId(){return __FUNCTION__;}
	virtual astring GetName(){return __FUNCTION__;}
	virtual void VidFilter(astring strFilter){}

protected:
	ClientFactory &m_pFactory;
	bool m_bOnline;
	bool m_bRec;
	/* if the check has been called 
	    use for live and searchpb, search pb will call bsetchecked
	*/
	bool m_bSetChecked;

};

#endif /* __VSC_VID_ITEM_INF_H__ */