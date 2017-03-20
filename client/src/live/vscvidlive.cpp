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
#include "live/vscvidlive.h"
#include "live/vscview.h"
#include "live/vvidpbview.h"
#include "vscvwidget.h"
#include "vscvideowall.h"
#include "vscloading.hpp"
#include "common/vidtree/vscvidtreecam.h"
#include "common/vidtree/vscvidtreeview.h"
#include "common/vidtree/vscvidtreeemap.h"
#include <QDesktopWidget>
#include "vemap.hpp"


Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

VSCVidLive::VSCVidLive(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent)
: VSCVidInf(pFactory, pTab, parent)
{
	
#if 1	
	m_pView = new VSCView(m_pFactory, &m_pMainArea, m_pMainArea, tr("Main View"));
	m_pView->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint  );
	//connect(m_pDeviceList, SIGNAL(CameraDoubleClicked(int)), m_pView, SLOT(CameraDoubleClicked(int)));

	m_pMainArea.addTab(m_pView,QIcon(tr(":/view/resources/3x3.png")), tr("Main View"));
	m_pMainArea.setCurrentWidget(m_pView);
	//connect(m_pEventThread, SIGNAL(EventNotify(int, int)), 
		//	m_pView, SLOT(DeviceEvent(int, int)));
#endif

	m_pCameraTree = new VSCVidTreeCam(m_pFactory, parent);
	m_pCameraTree->Init();
	m_pGroupTree = new VSCVidTreeInf(m_pFactory, parent);
	m_pGroupTree->Init();
	m_pViewTree = new VSCVidTreeView(m_pFactory, parent);
	m_pViewTree->Init();
	m_pEmapTree = new VSCVidTreeEmap(m_pFactory, parent);
	m_pEmapTree->Init();

	connect(m_pCameraTree, SIGNAL(CameraSelected(std::string, std::string, std::string)), 
		m_pView, SLOT(CameraDoubleClicked(std::string, std::string, std::string)));
	connect(m_pViewTree, SIGNAL(ViewSelected(std::string)), 
		m_pView, SLOT(ShowViewClicked(std::string)));
	connect(m_pEmapTree, SIGNAL(EmapSelected(std::string)), 
		this, SLOT(SlotNewEmap(std::string)));


#if 0
	QTreeWidgetItem * pGroup = new  QTreeWidgetItem(m_pGroupTree);
	pGroup->setText(0, QApplication::translate("", "Group Tree", 0));

	m_pGroupTree->addTopLevelItem(pGroup);
#endif
	
}
VSCVidLive::~VSCVidLive()
{
	
}

void VSCVidLive::VidShow()
{
	//m_parent->addDockWidget(Qt::LeftDockWidgetArea, m_pDockDevicelist);

}
void VSCVidLive::VidHide()
{
	//m_parent->removeDockWidget(m_pDockDevicelist);

}

void VSCVidLive::VidNewLiveView()
{
	int tabNum = m_pMainArea.count();
	int currentNum = -1;
	for (int i=0; i<tabNum; i++)
	{
		QWidget *qwidget = m_pMainArea.widget(i);
		VSCView *pView = dynamic_cast<VSCView* >(qwidget);
		if (pView)
		{
			currentNum++;
		}
	}

	VSCLoading *loading = new VSCLoading(NULL);
	loading->show();
	QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	loading->setGeometry(rect.width()/2, rect.height()/2, 64, 64);
	QCoreApplication::processEvents();
	
	VSCView *pView = new VSCView(m_pFactory, &m_pMainArea, m_pMainArea, tr("View %1").arg(currentNum+1));
	pView->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint  );
	//connect(m_pDeviceList, SIGNAL(CameraDoubleClicked(int)), m_pView, SLOT(CameraDoubleClicked(int)));

	m_pMainArea.addTab(pView,QIcon(tr(":/view/resources/3x3.png")), tr("View %1").arg(currentNum+1));
	m_pMainArea.setCurrentWidget(pView);
	delete loading;
}

void VSCVidLive::VidNewLivePB()
{
	int tabNum = m_pMainArea.count();
	int currentNum = -1;
	for (int i=0; i<tabNum; i++)
	{
		QWidget *qwidget = m_pMainArea.widget(i);
		VVidPBView *pView = dynamic_cast<VVidPBView* >(qwidget);
		if (pView)
		{
			currentNum++;
		}
	}

	VSCLoading *loading = new VSCLoading(NULL);
	loading->show();
	QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	loading->setGeometry(rect.width()/2, rect.height()/2, 64, 64);
	QCoreApplication::processEvents();
	
	VVidPBView *pView = new VVidPBView(m_pFactory, &m_pMainArea, m_pMainArea, tr("Playback%1").arg(currentNum + 1));
	pView->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint  );
	//connect(m_pDeviceList, SIGNAL(CameraDoubleClicked(int)), m_pView, SLOT(CameraDoubleClicked(int)));

	m_pMainArea.addTab(pView,QIcon(tr(":/view/resources/3x3.png")), tr("Playback %1").arg(currentNum+1));
	m_pMainArea.setCurrentWidget(pView);
	delete loading;
}

void VSCVidLive::VidNewEmap(std::string strId)
{
	VEMap *pEmap =  VEMap::CreateObject(strId, m_pFactory, m_pMainArea, &m_pMainArea);
	pEmap->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	astring strEmapName;
	VidEmap cEmap;
	m_pFactory.GetConfDB().GetEmapConf(strId, cEmap);

	strEmapName = cEmap.strname();
	
	m_pMainArea.addTab(pEmap,QIcon(tr(":/action/resources/map.png")), strEmapName.c_str());
	m_pMainArea.setCurrentWidget(pEmap);

}

QWidget * VSCVidLive::GetMainView()
{
	return m_pView;
}

VSCVidTreeInf *VSCVidLive::GetCameraTree()
{
	return m_pCameraTree;
}
VSCVidTreeInf *VSCVidLive::GetGroupTree()
{
	return m_pGroupTree;
}
VSCVidTreeInf *VSCVidLive::GetEmapTree()
{
	return m_pEmapTree;
}
VSCVidTreeInf *VSCVidLive::GetViewTree()
{
	return m_pViewTree;
}
