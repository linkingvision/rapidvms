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
#include "common/viditem/vscviditemvidstor.h"
#include "common/viditem/vscviditemcam.h"

VSCVidItemVidStor::VSCVidItemVidStor(VidStor cStor, ClientFactory &pFactory, QTreeWidgetItem *parent)
: m_cStor(cStor), VSCVidItemInf(pFactory, parent)
{
	//TreeUpdated();

	QIcon icon1;
	icon1.addFile(QStringLiteral(":/device/resources/stor.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	setIcon(0, icon1);

	setText(0, QApplication::translate(" ", m_cStor.strname().c_str(), 0));
	//m_pFactory.GetStorFactory().RegChangeNotify((void *)this, VSCVidItemVidStor::CallChange);

	connect(&(m_pFactory.GetStorFactory()), SIGNAL(SignalCallChange(int, std::string, std::string)), 
		this, SLOT(SlotCallChange(int, std::string, std::string)));
	
	UpdateOnline(m_pFactory.GetStorFactory().GetOnline(m_cStor.strid()));

	if (m_pFactory.GetStorFactory().GetOnline(m_cStor.strid()) == true)
	{
		TreeUpdated();
	}
	
}
VSCVidItemVidStor::~VSCVidItemVidStor()
{
	
}

void VSCVidItemVidStor::SlotCallChange(int type, std::string strId, std::string strCam)
{
	StorFactoryChangeData data;
	data.type = (StorFactoryChangeType)type;
	bool bRet1 = data.cId.ParseFromString(strId);
	bool bRet2 = data.cCam.ParseFromString(strCam);

	if (bRet1 == true && bRet2 == true)
	{
		CallChange(data);
	}
}

bool VSCVidItemVidStor::CallChange(StorFactoryChangeData data)
{
	if (data.cId.strstorid() != m_cStor.strid())
	{
		return true;
	}
	switch (data.type)
	{
		case STOR_FACTORY_CAMERA_ADD:
		{
			CameraAdd(data.cCam);
			break;
		}
		case STOR_FACTORY_CAMERA_DEL:
		{
			CameraDelete(data.cId.strcameraid());
			break;
		}
		case STOR_FACTORY_CAMERA_ONLINE:
		{
			CameraOnline(data.cId.strcameraid());
			break;
		}
		case STOR_FACTORY_CAMERA_OFFLINE:
		{
			CameraOffline(data.cId.strcameraid());
			break;
		}
		case STOR_FACTORY_CAMERA_REC_ON:
		{
			CameraRecOn(data.cId.strcameraid());
			break;
		}
		case STOR_FACTORY_CAMERA_REC_OFF:
		{
			CameraRecOff(data.cId.strcameraid());
			break;
		}
		default:
		   	break;
	};

	return true;
}

void VSCVidItemVidStor::CameraAdd(VidCamera cCam)
{
	int cnt = this->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = this->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == cCam.strid())
		{
			/* already in the list */
			this->removeChild(pChild);
		}
	}
	VidCameraId cCamId;
	cCamId.set_strcameraid(cCam.strid());
	cCamId.set_strstorid(GetId());
	VSCVidItemCam *pItemCam = new  VSCVidItemCam(cCam,cCamId,
								m_pFactory, this);
}
void VSCVidItemVidStor::CameraDelete(astring strId)
{
	int cnt = this->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = this->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == strId)
		{
			/* already in the list */
			this->removeChild(pChild);
		}
	}
}
void VSCVidItemVidStor::CameraOnline(astring strId)
{
	int cnt = this->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = this->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == strId)
		{
			/* already in the list */
			pItem->UpdateOnline(true);
		}
	}
}
void VSCVidItemVidStor::CameraOffline(astring strId)
{
	int cnt = this->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = this->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == strId)
		{
			/* already in the list */
			pItem->UpdateOnline(false);
		}
	}
}
void VSCVidItemVidStor::CameraRecOn(astring strId)
{
	int cnt = this->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = this->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == strId)
		{
			/* already in the list */
			pItem->UpdateRec(true);
		}
	}
}
void VSCVidItemVidStor::CameraRecOff(astring strId)
{
	int cnt = this->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = this->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == strId)
		{
			/* already in the list */
			pItem->UpdateRec(false);
		}
	}
}
void VSCVidItemVidStor::VidSetCheckedChild(QTreeWidgetItem * item, bool bChecked)
{
	int cnt = this->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = this->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		pItem->VidSetChecked(item, bChecked);
	}
	m_bSetChecked = true;
}

void VSCVidItemVidStor::VidGetSelectedItems(VidCameraIdMap &pMap)
{
	int cnt = this->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = this->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		pItem->VidGetSelectedItems(pMap);
	}
}

void VSCVidItemVidStor::TreeUpdated(bool bClear)
{
	qDeleteAll(takeChildren());
	if (bClear == true)
	{
		return;
	}
	VidCameraList cCamList = m_pFactory.GetStorFactory().GetVidCameraList(m_cStor.strid());
	StorClientOnlineMap bOnline = m_pFactory.GetStorFactory().GetVidCameraOnlineList(m_cStor.strid());
	StorClientRecMap bRec = m_pFactory.GetStorFactory().GetVidCameraRecList(m_cStor.strid());

	int camSize = cCamList.cvidcamera_size();
	
	for (s32 i = 0; i < cCamList.cvidcamera_size(); i ++)
	{
		VidCamera pCam = cCamList.cvidcamera(i);
		VidCameraId cCamId;
		cCamId.set_strcameraid(pCam.strid());
		cCamId.set_strstorid(GetId());
		VSCVidItemCam *pItemCam = new  VSCVidItemCam(pCam, cCamId,
								m_pFactory, this);
		StorClientOnlineMap::iterator it = bOnline.find(pCam.strid()), ite = bOnline.end();
		if (it != ite)
		{
			pItemCam->UpdateOnline(bOnline[pCam.strid()]);
			pItemCam->UpdateRec(bRec[pCam.strid()]);
		}
	}
	if (m_bSetChecked == true)
	{
		VidSetCheckedChild(NULL, false);
	}

}

void VSCVidItemVidStor::VidFilter(astring strFilter)
{
	int cnt = childCount();
	setExpanded(true);

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem)
		{
			pItem->VidFilter(strFilter);
		}
	}
}

