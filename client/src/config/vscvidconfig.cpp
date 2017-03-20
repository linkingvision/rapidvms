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
#include "config/vscvidconfig.h"
#include "config/vidstor/vidstoradd.h"
#include "config/vidstor/vidcamadd.h"
#include "config/vidstor/vidstorsetting.h"
#include "config/vidstor/vidcamsetting.h"
#include "config/vidclientsetting.h"
#include "config/vidstor/disksetting/vschddedit.h"
#include "config/vidview/vidviewconf.h"
#include "config/videmap/videmapconf.h"

VSCVidConf::VSCVidConf(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent)
: VSCVidInf(pFactory, pTab, parent)
{
	m_pConfTree = new VSCVidTreeConf(m_pFactory, parent);
	m_pConfTree->Init();
	m_pConfTree->hide();

	connect(m_pConfTree, SIGNAL(SignalStorAddSelectd()), this, SLOT(SlotVidStorAdd()));
	connect(m_pConfTree, SIGNAL(SignalCamAddSelectd(std::string)), this, 
								SLOT(SlotVidCamAdd(std::string)));
						
	connect(m_pConfTree, SIGNAL(SignalClientConfSelectd()), this, SLOT(SlotVidClientConf()));
	connect(m_pConfTree, SIGNAL(SignalStorConfSelectd(std::string)), this, 
								SLOT(SlotVidStorConf(std::string)));
	connect(m_pConfTree, SIGNAL(SignalDiskConfSelectd(std::string)), this, 
								SLOT(SlotVidDiskConf(std::string)));
	connect(m_pConfTree, SIGNAL(SignalCamConfSelectd(std::string, std::string)), this, 
								SLOT(SlotVidCamConf(std::string, std::string)));
								
	connect(m_pConfTree, SIGNAL(SignalViewConfSelectd()), this, SLOT(SlotVidViewConf()));
	connect(m_pConfTree, SIGNAL(SignalEmapConfSelectd()), this, SLOT(SlotVidEmapConf()));
}
VSCVidConf::~VSCVidConf()
{
	
}

VSCVidTreeInf *VSCVidConf::GetConfTree()
{
	return m_pConfTree;
}

void VSCVidConf::VidShow()
{

}
void VSCVidConf::VidHide()
{

}

void VSCVidConf::SlotVidStorAdd()
{
	VidStorAdd *pAdd = new VidStorAdd(m_pFactory, &m_pMainArea);
	//pAdd->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
	//connect(m_pDeviceList, SIGNAL(CameraDoubleClicked(int)), m_pView, SLOT(CameraDoubleClicked(int)));

	m_pMainArea.addTab(pAdd,QIcon(tr(":/device/resources/addstor.png")), tr("Stor Add"));
	m_pMainArea.setCurrentWidget(pAdd);
}

void VSCVidConf::SlotVidCamAdd(std::string strStorId)
{
	VidStor pStor;
	m_pFactory.GetConfDB().GetStorConf(strStorId, pStor);
	VidCamAdd *pAdd = new VidCamAdd(pStor, m_pFactory, &m_pMainArea);
	//pAdd->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
	//connect(m_pDeviceList, SIGNAL(CameraDoubleClicked(int)), m_pView, SLOT(CameraDoubleClicked(int)));

	m_pMainArea.addTab(pAdd,QIcon(tr(":/device/resources/addcam.png")), tr("Camera Add"));
	m_pMainArea.setCurrentWidget(pAdd);
}

void VSCVidConf::SlotVidClientConf()
{
	VidClientSetting *pConf = new VidClientSetting(m_pFactory, &m_pMainArea);

	m_pMainArea.addTab(pConf,QIcon(tr(":/action/resources/control_panel.png")), tr("Client"));
	m_pMainArea.setCurrentWidget(pConf);

}

void VSCVidConf::SlotVidViewConf()
{
	VidViewConf *pConf = new VidViewConf(m_pFactory, &m_pMainArea);
	
	m_pMainArea.addTab(pConf,QIcon(tr(":/device/resources/view.png")), tr("View"));
	m_pMainArea.setCurrentWidget(pConf);
}


void VSCVidConf::SlotVidEmapConf()
{
	VidEmapConf *pConf = new VidEmapConf(m_pFactory, &m_pMainArea);

	m_pMainArea.addTab(pConf,QIcon(tr(":/action/resources/map.png")), tr("Emap"));
	m_pMainArea.setCurrentWidget(pConf);
}

void VSCVidConf::SlotVidStorConf(std::string strStorId)
{
	VidStor pStor;
	m_pFactory.GetConfDB().GetStorConf(strStorId, pStor);
	VidStorSetting *pConf = new VidStorSetting(pStor, m_pFactory, &m_pMainArea);
	m_pMainArea.addTab(pConf,QIcon(tr(":/action/resources/control_panel.png")), tr("Stor"));
	m_pMainArea.setCurrentWidget(pConf);	
}

void VSCVidConf::SlotVidDiskConf(std::string strStorId)
{
	VidStor pStor;
	m_pFactory.GetConfDB().GetStorConf(strStorId, pStor);
	VSCHddEdit *pConf = new VSCHddEdit(pStor, &m_pMainArea);
	m_pMainArea.addTab(pConf,QIcon(tr(":/action/resources/disksetting.png")), tr("Disk"));
	m_pMainArea.setCurrentWidget(pConf);	
}

void VSCVidConf::SlotVidCamConf(std::string strStor, std::string strCam)
{
	VidStor pStor;
	m_pFactory.GetConfDB().GetStorConf(strStor, pStor);
	VidCamSetting *pConf = new VidCamSetting(pStor, strCam, m_pFactory, &m_pMainArea);
	m_pMainArea.addTab(pConf,QIcon(tr(":/device/resources/camera.png")), tr("Camera"));
	m_pMainArea.setCurrentWidget(pConf);	
}
