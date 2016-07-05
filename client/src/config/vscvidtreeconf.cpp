#include <QMouseEvent>
#include <QPainter>
#include <QDrag>

#include "config/vscvidtreeconf.h"
#include "config/viditem/vscviditemaddstor.h"
#include "config/viditem/vscviditemstorconf.h"
#include "config/viditem/vscviditemaddcam.h"
#include "config/viditem/vscviditemcamconf.h"
#include "config/viditem/vscviditemclientconf.h"
#include "config/viditem/vscviditemdiskconf.h"
#include "config/viditem/vscviditemviewconf.h"
#include "config/viditem/vscviditememapconf.h"


VSCVidTreeConf::VSCVidTreeConf(ClientFactory &pFactory, QWidget *parent)
: VSCVidTreeInf(pFactory, parent), m_bInit(false), m_pRoot(NULL)
{


}

void VSCVidTreeConf::Init()
{
	if (m_bInit == true)
	{
		return;
	}
	m_pRoot = new  QTreeWidgetItem(this);
	
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/action/resources/control_panel.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	m_pRoot->setIcon(0, icon1);

	m_pRoot->setText(0, QApplication::translate("Configurations",
	        "Configurations", 0));
	
	addTopLevelItem(m_pRoot);
	m_pRoot->setExpanded(true);
	
	TreeUpdate();
	//m_pFactory.GetStorFactory().RegChangeNotify((void *)this, VSCVidTreeConf::CallChange);
	connect(&(m_pFactory.GetStorFactory()), SIGNAL(SignalCallChange(int, std::string, std::string)), 
		this, SLOT(SlotCallChange(int, std::string, std::string)));
	
	m_bInit = true;
}

VSCVidTreeConf::~VSCVidTreeConf()
{

}

void VSCVidTreeConf::mousePressEvent(QMouseEvent *event)
{
	QTreeWidgetItem *selectedItem = currentItem();

	if (event->buttons() & Qt::RightButton)
	{
		return;
	}
	
	QTreeWidget::mousePressEvent(event);

}

void VSCVidTreeConf::mouseDoubleClickEvent(QMouseEvent *event)
{
	QTreeWidgetItem *selectedItem = currentItem();

	if (event->buttons() & Qt::RightButton)
	{
		return;
	}

	if (selectedItem)
	{
		VSCVidItemAddStor *pConf = dynamic_cast<VSCVidItemAddStor * >(selectedItem);
		if (pConf)
		{
			/* Vid Stor add clicked */
			emit SignalStorAddSelectd();
		}
		
		VSCVidItemAddCam *pAddCam = dynamic_cast<VSCVidItemAddCam * >(selectedItem);
		if (pAddCam)
		{
			/* Vid Stor add clicked */
			emit SignalCamAddSelectd(pAddCam->GetId());
		}
		
		VSCVidItemCamConf *pCamConf = dynamic_cast<VSCVidItemCamConf * >(selectedItem);
		if (pCamConf)
		{
			/* Vid Stor add clicked */
			emit SignalCamConfSelectd(pCamConf->GetStorId(), pCamConf->GetId());
		}
		
		VSCVidItemClientConf *pClientConf = dynamic_cast<VSCVidItemClientConf * >(selectedItem);
		if (pClientConf)
		{
			/* Client */
			emit SignalClientConfSelectd();
		}
		
		VSCVidItemViewConf *pViewConf = dynamic_cast<VSCVidItemViewConf * >(selectedItem);
		if (pViewConf)
		{
			/* Client */
			emit SignalViewConfSelectd();
		}
		
		VSCVidItemEmapConf *pEmapConf = dynamic_cast<VSCVidItemEmapConf * >(selectedItem);
		if (pEmapConf)
		{
			/* Client */
			emit SignalEmapConfSelectd();
		}
		
		VSCVidItemStorConf *pStorConf = dynamic_cast<VSCVidItemStorConf * >(selectedItem);
		if (pStorConf)
		{
			/* Vid Stor add clicked */
			emit SignalStorConfSelectd(pStorConf->GetId());
		}
		
		VSCVidItemDiskConf *pDiskConf = dynamic_cast<VSCVidItemDiskConf * >(selectedItem);
		if (pDiskConf)
		{
			/* Vid disk conf clicked */
			emit SignalDiskConfSelectd(pDiskConf->GetId());
		}
	}
	
	QTreeWidget::mouseDoubleClickEvent(event);
}


void VSCVidTreeConf::TreeUpdate()
{
	/* Delete all the child */
	qDeleteAll(m_pRoot->takeChildren());
	
	/* Add fixed items */
	VSCVidItemAddStor *pItemAddStor = new VSCVidItemAddStor(m_pFactory, m_pRoot);
	VSCVidItemClientConf *pItemClient = new VSCVidItemClientConf(m_pFactory, m_pRoot);
	VSCVidItemViewConf *pItemViewConf = new VSCVidItemViewConf(m_pFactory, m_pRoot);
	VSCVidItemEmapConf *pItemEmapConf = new VSCVidItemEmapConf(m_pFactory, m_pRoot);
	
	VidStorList storList;
	m_pFactory.GetConfDB().GetStorListConf(storList);
	int storSize = storList.cvidstor_size();
	
	for (s32 i = 0; i < storList.cvidstor_size(); i ++)
	{
		VidStor pStor = storList.cvidstor(i);
		VSCVidItemStorConf *pItemStor = new  VSCVidItemStorConf(pStor, 
								m_pFactory, m_pRoot);
	}
	
}

void VSCVidTreeConf::SlotCallChange(int type, std::string strId, std::string strCam)
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
bool VSCVidTreeConf::CallChange(StorFactoryChangeData data)
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

void VSCVidTreeConf::StorAdd(VidCameraId cId)
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
	
	VSCVidItemStorConf *pItemStor = new  VSCVidItemStorConf(pStor, 
							m_pFactory, m_pRoot);

}
void VSCVidTreeConf::StorDel(VidCameraId cId)
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

void VSCVidTreeConf::StorOnline(VidCameraId cId, bool bOnline)
{
	int cnt = m_pRoot->childCount();
	
	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = m_pRoot->child(i);
		VSCVidItemStorConf *pItem = dynamic_cast<VSCVidItemStorConf*>(pChild);
		if (pItem && pItem->GetId() == cId.strstorid())
		{
			pItem->UpdateOnline(bOnline);
			pItem->TreeUpdated(!bOnline);
			return;
		}
	}
}

void VSCVidTreeConf::VidFilter(astring strFilter)
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
