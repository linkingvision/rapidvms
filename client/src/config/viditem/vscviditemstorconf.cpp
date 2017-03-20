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
#include "config/viditem/vscviditemstorconf.h"
#include "config/viditem/vscviditemclientconf.h"
#include "config/viditem/vscviditemaddcam.h"
#include "config/viditem/vscviditemcamconf.h"
#include "config/viditem/vscviditemdiskconf.h"

VSCVidItemStorConf::VSCVidItemStorConf(VidStor cStor, ClientFactory &pFactory, QTreeWidgetItem *parent)
: VSCVidItemInf(pFactory, parent), m_cStor(cStor), m_pItemAddCam(NULL)
{
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/device/resources/stor.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	setIcon(0, icon1);
	setText(0, QApplication::translate(" ", m_cStor.strname().c_str(), 0));
	this->setTextColor(0, QColor(41, 42, 43));
	
	//TreeUpdated();

	//m_pFactory.GetStorFactory().RegChangeNotify((void *)this, VSCVidItemStorConf::CallChange);
	connect(&(m_pFactory.GetStorFactory()), SIGNAL(SignalCallChange(int, std::string, std::string)), 
		this, SLOT(SlotCallChange(int, std::string, std::string)));

	UpdateOnline(m_pFactory.GetStorFactory().GetOnline(m_cStor.strid()));
	if (m_pFactory.GetStorFactory().GetOnline(m_cStor.strid()) == true)
	{
		TreeUpdated();
	}
	
	
}
VSCVidItemStorConf::~VSCVidItemStorConf()
{
	
}

void VSCVidItemStorConf::SlotCallChange(int type, std::string strId, std::string strCam)
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

bool VSCVidItemStorConf::CallChange(StorFactoryChangeData data)
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

void VSCVidItemStorConf::CameraAdd(VidCamera cCam)
{
	if (m_pItemAddCam == NULL)
	{
		return;
	}
	int cnt = m_pItemAddCam->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = m_pItemAddCam->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == cCam.strid())
		{
			/* already in the list */
			m_pItemAddCam->removeChild(pChild);
		}
	}

	VSCVidItemCamConf *pItemCam = new  VSCVidItemCamConf(m_cStor, cCam,
								m_pFactory, m_pItemAddCam);

}
void VSCVidItemStorConf::CameraDelete(astring strId)
{
	if (m_pItemAddCam == NULL)
	{
		return;
	}
	int cnt = m_pItemAddCam->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = m_pItemAddCam->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == strId)
		{
			/* already in the list */
			m_pItemAddCam->removeChild(pChild);
		}
	}
}
void VSCVidItemStorConf::CameraOnline(astring strId)
{
	if (m_pItemAddCam == NULL)
	{
		return;
	}
	int cnt = m_pItemAddCam->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = m_pItemAddCam->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == strId)
		{
			/* already in the list */
			pItem->UpdateOnline(true);
		}
	}
}
void VSCVidItemStorConf::CameraOffline(astring strId)
{
	if (m_pItemAddCam == NULL)
	{
		return;
	}
	int cnt = m_pItemAddCam->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = m_pItemAddCam->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == strId)
		{
			/* already in the list */
			pItem->UpdateOnline(false);
		}
	}
}

void VSCVidItemStorConf::CameraRecOn(astring strId)
{
	if (m_pItemAddCam == NULL)
	{
		return;
	}
	int cnt = m_pItemAddCam->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = m_pItemAddCam->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == strId)
		{
			/* already in the list */
			pItem->UpdateRec(true);
		}
	}
}
void VSCVidItemStorConf::CameraRecOff(astring strId)
{
	if (m_pItemAddCam == NULL)
	{
		return;
	}
	int cnt = m_pItemAddCam->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = m_pItemAddCam->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == strId)
		{
			/* already in the list */
			pItem->UpdateRec(false);
		}
	}
}

void VSCVidItemStorConf::TreeUpdated(bool bClear)
{
	qDeleteAll(takeChildren());
	if (bClear == true)
	{
		return;
	}
		
	/* Add fixed items */
	m_pItemAddCam = new VSCVidItemAddCam(m_cStor, m_pFactory, this);
	VSCVidItemDiskConf *pDiskConf = new VSCVidItemDiskConf(m_cStor, m_pFactory, this);
	
	VidCameraList cCamList = m_pFactory.GetStorFactory().GetVidCameraList(m_cStor.strid());
	StorClientOnlineMap bOnline = m_pFactory.GetStorFactory().GetVidCameraOnlineList(m_cStor.strid());
	StorClientRecMap bRec = m_pFactory.GetStorFactory().GetVidCameraRecList(m_cStor.strid());

	int camSize = cCamList.cvidcamera_size();
	
	for (s32 i = 0; i < cCamList.cvidcamera_size(); i ++)
	{
		VidCamera pCam = cCamList.cvidcamera(i);
		/* All the camera are the child of the Add Cam */
		VSCVidItemCamConf *pItemCam = new  VSCVidItemCamConf(m_cStor, pCam, m_pFactory, m_pItemAddCam);
		StorClientOnlineMap::iterator it = bOnline.find(pCam.strid()), ite = bOnline.end();
		if (it != ite)
		{
			pItemCam->UpdateOnline(bOnline[pCam.strid()]);
			pItemCam->UpdateRec(bRec[pCam.strid()]);
		}
	}
	
	return;

}


void VSCVidItemStorConf::VidFilter(astring strFilter)
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
