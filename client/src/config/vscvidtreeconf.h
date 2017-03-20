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
#ifndef __VSC_VID_TREE_CONF_H__
#define __VSC_VID_TREE_CONF_H__

#include "common/vscvidtreeinf.h"
#include "vscvwidget.h"

class VSCVidTreeConf : public VSCVidTreeInf
{
    Q_OBJECT
public:
    VSCVidTreeConf(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVidTreeConf();

signals:
	void SignalStorAddSelectd();
	void SignalStorConfSelectd(std::string strStor);
	void SignalDiskConfSelectd(std::string strStor);
	void SignalClientConfSelectd();
	void SignalViewConfSelectd();
	void SignalEmapConfSelectd();
	void SignalCamAddSelectd(std::string strStor);
	void SignalCamConfSelectd(std::string strStor, std::string strCam);
	
public:
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	
public:
	virtual void VidFilter(astring strFilter);
	virtual void Init();
public:
	/* Take care all the child state change */
	//static bool CallChange(void* pParam, StorFactoryChangeData data);
	bool CallChange(StorFactoryChangeData data);
public slots:
	void SlotCallChange(int type, std::string strId, std::string strCam);
	
public:
	void TreeUpdate();
	void StorAdd(VidCameraId cId);
	void StorDel(VidCameraId cId);
	void StorOnline(VidCameraId cId, bool bOnline);
private:
	QTreeWidgetItem * m_pRoot;
	bool m_bInit;
};

#endif