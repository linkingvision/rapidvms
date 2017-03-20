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
#include <QMouseEvent>
#include <QPainter>
#include <QDrag>

#include "common/vidtree/vscvidtreeview.h"
#include "common/viditem/vscviditemview.h"

VSCVidTreeView::VSCVidTreeView(ClientFactory &pFactory, QWidget *parent)
: VSCVidTreeInf(pFactory, parent), m_pRoot(NULL), m_bInit(false)
{
	
}
VSCVidTreeView::~VSCVidTreeView()
{
	
}

void VSCVidTreeView::Init()
{
	if (m_bInit == true)
	{
		return;
	}
	m_pRoot = new  QTreeWidgetItem(this);
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/device/resources/view.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	m_pRoot->setIcon(0, icon1);

	m_pRoot->setText(0, QApplication::translate("View",
	        "View", 0));
	
	addTopLevelItem(m_pRoot);
	m_pRoot->setExpanded(true);
	
	TreeUpdate();
	
	/* Register the device callback */
	//m_pFactory.RegChangeNotify((void *)this, VSCVidTreeView::CallChange);
	connect(&(m_pFactory), SIGNAL(SignalCallChange(int, std::string)), this, SLOT(SlotCallChange(int, std::string)));
	m_bInit = true;
}

void VSCVidTreeView::mouseMoveEvent(QMouseEvent *event)
{
	QTreeWidgetItem *selectedItem = currentItem();

	VDC_DEBUG( "%s  %d \n",__FUNCTION__, __LINE__);
	if (selectedItem)
	{
		VSCVidItemView *pView = dynamic_cast<VSCVidItemView * >(selectedItem);
		if (pView)
		{
			QMimeData *mimeData = new VSCQMimeVidView(pView->GetId());

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
			//QTreeWidget::mouseMoveEvent(event);
			return ;
		}

	}
}

void VSCVidTreeView::mousePressEvent(QMouseEvent *event)
{
	QTreeWidgetItem *selectedItem = currentItem();

	if (event->buttons() & Qt::RightButton)
	{
		return;
	}
	VDC_DEBUG( "%s  %d \n",__FUNCTION__, __LINE__);
	
	QTreeWidget::mousePressEvent(event);

}

void VSCVidTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
	QTreeWidgetItem *selectedItem = currentItem();

	if (event->buttons() & Qt::RightButton)
	{
		return;
	}

	VDC_DEBUG( "%s  %d \n",__FUNCTION__, __LINE__);

	if (selectedItem)
	{
		VSCVidItemView *pView = dynamic_cast<VSCVidItemView * >(selectedItem);
		if (pView)
		{
			emit ViewSelected(pView->GetId());
			QTreeWidget::mouseDoubleClickEvent(event);
			event->accept();
			return ;
		}

	}
	
	QTreeWidget::mouseDoubleClickEvent(event);
	
}

void VSCVidTreeView::SlotCallChange(int type, std::string strId)
{
	ClientFactoryChangeData data;
	data.type = (ClientFactoryChangeType)type;
	data.id = strId;
	CallChange(data);
}

bool VSCVidTreeView::CallChange(ClientFactoryChangeData data)
{
	switch (data.type)
	{
		case CLIENT_FACTORY_VIEW_ADD:
		{
			TreeUpdate();
			break;
		}
		case CLIENT_FACTORY_VIEW_DEL:
		{
			TreeUpdate();
			break;
		}
		default:
		   	break;
	};

	return true;
}

void VSCVidTreeView::TreeUpdate()
{
	/* Delete all the child */
	qDeleteAll(m_pRoot->takeChildren());

	VidViewList viewList;
	m_pFactory.GetConfDB().GetViewListConf(viewList);
	int viewSize = viewList.cvidview_size();
	
	for (s32 i = 0; i < viewList.cvidview_size(); i++)
	{
		VidView pView = viewList.cvidview(i);
		VSCVidItemView *pItemView = new  VSCVidItemView(pView, 
								m_pFactory, m_pRoot);
	}
	
}

void VSCVidTreeView::VidFilter(astring strFilter)
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





