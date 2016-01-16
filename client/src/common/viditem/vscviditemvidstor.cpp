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
	m_pFactory.GetStorFactory().RegChangeNotify((void *)this, VSCVidItemVidStor::CallChange);

	UpdateOnline(m_pFactory.GetStorFactory().GetOnline(m_cStor.strid()));

	if (m_pFactory.GetStorFactory().GetOnline(m_cStor.strid()) == true)
	{
		TreeUpdated();
	}
	
}
VSCVidItemVidStor::~VSCVidItemVidStor()
{
	
}

bool VSCVidItemVidStor::CallChange(void* pParam, StorFactoryChangeData data)
{
    int dummy = errno;
    VSCVidItemVidStor * pObject = (VSCVidItemVidStor * )pParam;

    if (pObject)
    {
        return pObject->CallChange1(data);
    }
}
bool VSCVidItemVidStor::CallChange1(StorFactoryChangeData data)
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
	VSCVidItemCam *pItemCam = new  VSCVidItemCam(cCam,
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
}
void VSCVidItemVidStor::CameraRecOff(astring strId)
{
}

void VSCVidItemVidStor::TreeUpdated()
{
	qDeleteAll(takeChildren());
	VidCameraList cCamList = m_pFactory.GetStorFactory().GetVidCameraList(m_cStor.strid());
	StorClientOnlineMap bOnline = m_pFactory.GetStorFactory().GetVidCameraOnlineList(m_cStor.strid());

	int camSize = cCamList.cvidcamera_size();
	
	for (s32 i = 0; i < cCamList.cvidcamera_size(); i ++)
	{
		VidCamera pCam = cCamList.cvidcamera(i);
		VSCVidItemCam *pItemCam = new  VSCVidItemCam(pCam,
								m_pFactory, this);
		StorClientOnlineMap::iterator it = bOnline.find(pCam.strid()), ite = bOnline.end();
		if (it != ite)
		{
			pItemCam->UpdateOnline(bOnline[pCam.strid()]);
		}
	}

}

void VSCVidItemVidStor::VidFilter(astring strFilter)
{
	int cnt = childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem)
		{
			pItem->VidFilter(strFilter);
			return;
		}
	}
}

