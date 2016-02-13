
#include <QMouseEvent>
#include <QPainter>
#include <QDrag>

#include "common/vidtree/vscvidtreecam.h"
#include "common/viditem/vscviditemcam.h"
#include "common/viditem/vscviditemvidstor.h"


VSCVidTreeCam::VSCVidTreeCam(ClientFactory &pFactory, QWidget *parent)
: VSCVidTreeInf(pFactory, parent), m_pRoot(NULL), m_bInit(false)
{	

	
}

void VSCVidTreeCam::Init()
{
	if (m_bInit == true)
	{
		return;
	}
	m_pRoot = new  QTreeWidgetItem(this);
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/device/resources/camera.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	m_pRoot->setIcon(0, icon1);

	m_pRoot->setText(0, QApplication::translate("Camera",
	        "Camera", 0));
	
	addTopLevelItem(m_pRoot);
	m_pRoot->setExpanded(true);
	
	TreeUpdate();
	
	/* Register the device callback */
	m_pFactory.GetStorFactory().RegChangeNotify((void *)this, VSCVidTreeCam::CallChange);
	m_bInit = true;
}

VSCVidTreeCam::~VSCVidTreeCam()
{
	//TODO UnReg change Notify
}

void VSCVidTreeCam::mousePressEvent(QMouseEvent *event)
{
	QTreeWidgetItem *selectedItem = currentItem();

	if (event->buttons() & Qt::RightButton)
	{
		return;
	}

	if (selectedItem)
	{
		VSCVidItemCam *pCam = dynamic_cast<VSCVidItemCam * >(selectedItem);
		if (pCam)
		{
			VSCVidItemVidStor *pStor = 
					dynamic_cast<VSCVidItemVidStor * >(pCam->parent());
			if (pStor)
			{
				VDC_DEBUG( "%s id Stor %s Camera %s \n",__FUNCTION__, 
						pStor->GetId().c_str(), pCam->GetId().c_str());
				QMimeData *mimeData = new VSCQMimeVidCam(pStor->GetId(), 
					pCam->GetId(), pCam->GetName());

				// Create drag
				QPixmap pixmap(":/action/resources/buttonhover.png");
				//QPainter painter(&pixmap);

				//mimeData->setText(pStor->GetName().c_str());
				//this->setCursor(QCursor(Qt::ClosedHandCursor));

				QDrag *drag = new QDrag(this);
				drag->setMimeData(mimeData);
				drag->setPixmap(pixmap);
				drag->setHotSpot(QPoint(drag->pixmap().width()/2,
				             drag->pixmap().height()/2));
				drag->exec();
				QTreeWidget::mousePressEvent(event);
				return ;
			}
		}

	}
	
	QTreeWidget::mousePressEvent(event);

}

void VSCVidTreeCam::mouseDoubleClickEvent(QMouseEvent *event)
{
	QTreeWidgetItem *selectedItem = currentItem();

	if (event->buttons() & Qt::RightButton)
	{
		return;
	}

	if (selectedItem)
	{
		VSCVidItemCam *pCam = dynamic_cast<VSCVidItemCam * >(selectedItem);
		if (pCam)
		{
			VSCVidItemVidStor *pStor = 
					dynamic_cast<VSCVidItemVidStor * >(pCam->parent());
			if (pStor)
			{
				VDC_DEBUG( "%s id Stor %s Camera %s \n",__FUNCTION__, 
						pStor->GetId().c_str(), pCam->GetId().c_str());
				emit CameraSelected(pStor->GetId(), pCam->GetId(), pCam->GetName());
				QTreeWidget::mouseDoubleClickEvent(event);
				return ;
			}
		}

	}
	
	QTreeWidget::mouseDoubleClickEvent(event);
	
}

bool VSCVidTreeCam::CallChange(void* pParam, StorFactoryChangeData data)
{
    int dummy = errno;
    VSCVidTreeCam * pObject = (VSCVidTreeCam * )pParam;

    if (pObject)
    {
        return pObject->CallChange1(data);
    }
}
bool VSCVidTreeCam::CallChange1(StorFactoryChangeData data)
{
	switch (data.type)
	{
		case STOR_FACTORY_STOR_ADD:
		{
			/* Process the add */
			StorAdd(data.cId);
			break;
		}
		case STOR_FACTORY_STOR_DEL:
		{
			/* Process the del */
			StorDel(data.cId);
			break;
		}
		case STOR_FACTORY_STOR_ONLINE:
		{
			/* Process the online */
			StorOnline(data.cId, true);
			break;
		}
		case STOR_FACTORY_STOR_OFFLINE:
		{
			/* Process the offline */
			StorOnline(data.cId, false);
			break;
		}
		default:
		   	break;
	};

	return true;
}

void VSCVidTreeCam::TreeUpdate()
{
	/* Delete all the child */
	qDeleteAll(m_pRoot->takeChildren());

	VidStorList storList;
	m_pFactory.GetConfDB().GetStorListConf(storList);
	int storSize = storList.cvidstor_size();
	
	for (s32 i = 0; i < storList.cvidstor_size(); i ++)
	{
		VidStor pStor = storList.cvidstor(i);
		VSCVidItemVidStor *pItemStor = new  VSCVidItemVidStor(pStor, 
								m_pFactory, m_pRoot);
	}
	
}

void VSCVidTreeCam::StorAdd(VidCameraId cId)
{
	int cnt = m_pRoot->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = m_pRoot->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == cId.strstorid())
		{
			/* already in the list */
			return;
		}
	}
	VidStor pStor;
	m_pFactory.GetConfDB().GetStorConf(cId.strstorid(), pStor);
	
	VSCVidItemVidStor *pItemStor = new  VSCVidItemVidStor(pStor, 
							m_pFactory, m_pRoot);

}
void VSCVidTreeCam::StorDel(VidCameraId cId)
{
	int cnt = m_pRoot->childCount();

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = m_pRoot->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem && pItem->GetId() == cId.strstorid())
		{
			m_pRoot->removeChild(pChild);
			return;
		}
	}
}

void VSCVidTreeCam::StorOnline(VidCameraId cId, bool bOnline)
{
	int cnt = m_pRoot->childCount();
	
	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = m_pRoot->child(i);
		VSCVidItemVidStor *pItem = dynamic_cast<VSCVidItemVidStor*>(pChild);
		if (pItem && pItem->GetId() == cId.strstorid())
		{
			pItem->UpdateOnline(bOnline);
			pItem->TreeUpdated(!bOnline);
			return;
		}
	}
}

void VSCVidTreeCam::VidFilter(astring strFilter)
{
	int cnt = m_pRoot->childCount();
	m_pRoot->setExpanded(true);

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = m_pRoot->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem)
		{
			pItem->VidFilter(strFilter);
		}
	}
}
