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
#ifndef __VSC_VID_ITEM_VIDSTOR_H__
#define __VSC_VID_ITEM_VIDSTOR_H__

#include "common/vscviditeminf.h"

class VSCVidItemVidStor : public QObject, public VSCVidItemInf
{
Q_OBJECT
public:
	VSCVidItemVidStor(VidStor cStor, ClientFactory &pFactory, QTreeWidgetItem *parent);
	~VSCVidItemVidStor();

public:
	/* Take care all the child state change */
	//static bool CallChange(void* pParam, StorFactoryChangeData data);
	bool CallChange(StorFactoryChangeData data);
public slots:
	void SlotCallChange(int type, std::string strId, std::string strCam);
	
public:
	/* Get all the child of this VidStor */
	void TreeUpdated(bool bClear = false);
	void CameraAdd(VidCamera cCam);
	void CameraDelete(astring strId);
	void CameraOnline(astring strId);
	void CameraOffline(astring strId);
	void CameraRecOn(astring strId);
	void CameraRecOff(astring strId);
	

public:
	virtual astring GetId(){return m_cStor.strid();}
	virtual void VidFilter(astring strFilter);
	virtual void VidSetCheckedChild(QTreeWidgetItem * item, bool bChecked);
	virtual void VidGetSelectedItems(VidCameraIdMap &pMap);
private:
	VidStor m_cStor;

};

#endif /* __VSC_VID_ITEM_VIDSTOR_H__ */