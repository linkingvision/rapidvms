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
#include "vscdevicelist.h"
#include "vscdeviceipc.h"
#include "server/factory.hpp"
#include "vscviewtree.h"
//#include "vscdiskdevice.h"

//extern Factory *gFactory;


VSCDeviceList::VSCDeviceList(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	/* Add Group first */
	VGroupTreeUpdated();
	CameraTreeUpdated();
	DiskTreeUpdated();
	VmsTreeUpdated();
	ViewTreeUpdated();
	VIPCTreeUpdated();

	SetupConnections();
	m_Timer = new QTimer(this);
	//connect(m_Timer,SIGNAL(timeout()),this,SLOT(UpdateOnline()));  
	//m_Timer->start(1000 * 3); 

	m_TimerDisk = new QTimer(this);
	connect(m_TimerDisk,SIGNAL(timeout()),this,SLOT(DiskTreeUpdated()));
	/* Disk 60s update */
	m_TimerDisk->start(1000 * 15);
#if 0
	gFactory->RegDeviceChangeNotify((void *)this, 
				(FactoryDeviceChangeNotify)(VSCDeviceList::DeviceChangeCallbackFunc));

#endif
	//ui.pbRecorder->hide();
	//ui.lbRecorder->hide();
	
	//ui.pbDmining->hide();
	//ui.lbMining->hide();
	
	//ui.pbEmap->hide();
	//ui.lbEmap->hide();
	//ui.pbVIPC->hide();
	//ui.lbVIPC->hide();


}

void VSCDeviceList::SetupConnections()
{
	//connect(ui.pbSurveillance, SIGNAL(clicked()), this, SLOT(SurveillanceClick()));
	//connect(ui.pbCamera, SIGNAL(clicked()), this, SLOT(CameraAddClick()));
	//connect(ui.pbEmap, SIGNAL(clicked()), this, SLOT(EmapClick()));
	//connect(ui.pbRecordPlan, SIGNAL(clicked()), this, SLOT(RecordPlanClick()));
	//connect(ui.pbDmining, SIGNAL(clicked()), this, SLOT(DminingClick()));
	//connect(ui.pbSearch, SIGNAL(clicked()), this, SLOT(SearchClick()));
	//connect(ui.pbRecorder, SIGNAL(clicked()), this, SLOT(RecorderClick()));
	//connect(ui.pbVIPC, SIGNAL(clicked()), this, SLOT(VIPCAddClick()));

	connect(ui.treeWidget, SIGNAL(CameraAddClicked()), this, SIGNAL(CameraAddClicked()));
	connect(ui.treeWidget, SIGNAL(CameraEditClicked(int)), this, SIGNAL(CameraEditClicked(int)));
	connect(ui.treeWidget, SIGNAL(CameraDeleteClicked(int)), this, SIGNAL(CameraDeleteClicked(int)));
	connect(ui.treeWidget, SIGNAL(CameraDoubleClicked(int)), this, SIGNAL(CameraDoubleClicked(int)));


	connect(ui.treeWidget, SIGNAL(StartRecordAllClicked()), this, SLOT(StartRecordAll()));
	connect(ui.treeWidget, SIGNAL(StopRecordAllClicked()), this, SLOT(StopRecordAll()));
	connect(ui.treeWidget, SIGNAL(StartHdfsRecordAllClicked()), this, SLOT(StartHdfsRecordAll()));
	connect(ui.treeWidget, SIGNAL(StopHdfsRecordAllClicked()), this, SLOT(StopHdfsRecordAll()));


	/* VIPC */
	connect(ui.treeWidget, SIGNAL(VIPCAddClicked()), this, SIGNAL(VIPCAddClicked()));
	connect(ui.treeWidget, SIGNAL(VIPCEditClicked(int)), this, SIGNAL(VIPCEditClicked(int)));
	connect(ui.treeWidget, SIGNAL(VIPCDeleteClicked(int)), this, SIGNAL(VIPCDeleteClicked(int)));

	/* View */
	connect(ui.treeWidget, SIGNAL(ViewDeleteClicked(int)), this, SIGNAL(ViewDeleteClicked(int)));

	connect(ui.treeWidget, SIGNAL(DiskEditClicked()), this, SIGNAL(DiskEditClicked()));
	//connect(ui.pbTrash, SIGNAL(CameraDeleted()), this, SLOT(CameraTreeUpdated()));

	/* Group */
	connect(ui.treeWidget, SIGNAL(VGroupAddClicked()), this, SIGNAL(VGroupAddClicked()));
	connect(ui.treeWidget, SIGNAL(VGroupEditClicked(int)), this, SIGNAL(VGroupEditClicked(int)));
	connect(ui.treeWidget, SIGNAL(VGroupDeleteClicked(int)), this, SIGNAL(VGroupDeleteClicked(int)));
	connect(ui.treeWidget, SIGNAL(VGroupMapClicked()), this, SIGNAL(VGroupMapClicked()));

	/* VMS */
	connect(ui.treeWidget, SIGNAL(VMSDeleteClicked(int)), this, SIGNAL(VMSDeleteClicked(int)));

	
}



bool VSCDeviceList::DeviceChangeCallbackFunc(void* pData, FactoryCameraChangeData change)
{
	if (pData)
	{
		VSCDeviceList * pDeviceList = (VSCDeviceList *)pData;
		pDeviceList->DeviceChangeCallbackFunc1(change);
	}
	return true;
}

bool VSCDeviceList::DeviceChangeCallbackFunc1(FactoryCameraChangeData change)
{
#if 0
	VDC_DEBUG( " %s Device Change Callback %d type %d\n", __FUNCTION__, change.id, change.type);
	switch (change.type)
	{
	    case FACTORY_DEVICE_DEL:
	    {
		 NotifyDelCamera(change.id);
	        break;
	    }
	    case FACTORY_DEVICE_ADD:
	    {
	        NotifyAddCamera(change.id);
	        break;
	    }
	    case FACTORY_DEVICE_GROUP_CHANGE:
	    {
		NotifyDelCamera(change.id);
		NotifyAddCamera(change.id);
		BOOL bStatus = FALSE;
		gFactory->GetDeviceOnline(change.id, bStatus);
		NotifyCameraOnlineOffline(change.id, bStatus);
	        break;
	    }
	    case FACTORY_DEVICE_OFFLINE:
	    {
	        NotifyCameraOnlineOffline(change.id, FALSE);
	        break;
	    }
	    case FACTORY_DEVICE_ONLINE:
	    {
	        NotifyCameraOnlineOffline(change.id, TRUE);
	        break;
	    }
	    case FACTORY_DEVICE_RECORDING_OFF:
	    {
	        NotifyCameraRecording(change.id, FALSE);
	        break;
	    }
	    case FACTORY_DEVICE_RECORDING_ON:
	    {
	        NotifyCameraRecording(change.id, TRUE);
	        break;
	    }
	    case FACTORY_VMS_ADD:
	    {
	        NotifyAddVms(change.id);
	        break;
	    }
	    case FACTORY_VMS_DEL:
	    {
	        NotifyDelVms(change.id);
	        break;
	    }
	    case FACTORY_VIEW_ADD:
	    {
	        NotifyAddView(change.id);
	        break;
	    }
	    case FACTORY_VIEW_DEL:
	    {
	        NotifyDelView(change.id);
	        break;
	    }
	    case FACTORY_VIPC_ADD:
	    {
	        NotifyAddVIPC(change.id);
	        break;
	    }
	    case FACTORY_VIPC_DEL:
	    {
	        NotifyDelVIPC(change.id);
	        break;
	    }
	    case FACTORY_VGROUP_ADD:
	    {
	        NotifyAddVGroup(change.id);
	        break;
	    }
	    case FACTORY_VGROUP_DEL:
	    {
	        NotifyDelVGroup(change.id);
	        break;
	    }
	    default:
	    {

	        break;
	    }
	}
#endif
	return true;
}

void VSCDeviceList::NotifyAddCamera(int nId)
{
#if 0
	DeviceParam  pParam;
	if (gFactory->GetDeviceParamById(pParam, nId) == TRUE)
	{
		AddIPCameraByGroup(pParam);
	}
#endif
	return;
}

void VSCDeviceList::NotifyDelCamera(int nId)
{
#if 0
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);

	int cnt = qtreewidgetitem->childCount();
	//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
		VSCDeviceIPC *pIPC = dynamic_cast<VSCDeviceIPC*>(pChild);
		if (pIPC && pIPC->GetDeviceId() == nId)
		{
			qtreewidgetitem->removeChild(pChild);
		}
	}
#else
	//DelIPCameraByGroup(nId);
#endif
	return;
}

void VSCDeviceList::NotifyCameraOnlineOffline(int nId, BOOL bOnline)
{
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);
	bool setting = false;
	
	int cnt = qtreewidgetitem->childCount();
	//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
		VSCDeviceIPC *pIPC = dynamic_cast<VSCDeviceIPC*>(pChild);
		if (pIPC && pIPC->GetDeviceId() == nId)
		{
		 	pIPC->UpdateOnline(bOnline);
			setting = true;
		}
	}

	if (setting == false)
	{
		/* loop for del to the group */
		int cnt = qtreewidgetitem->childCount();
		for (int i = 0; i < cnt; i ++)
		{
			QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
			VSCDeviceIPCGroup *pGroup = dynamic_cast<VSCDeviceIPCGroup*>(pChild);
			if (pGroup)
			{
				int cnt2 = pChild->childCount();
				//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
				for (int j = 0; j < cnt2; j ++)
				{
					QTreeWidgetItem * pChild2 = pChild->child(j);
					VSCDeviceIPC *pIPC = dynamic_cast<VSCDeviceIPC*>(pChild2);
					if (pIPC && pIPC->GetDeviceId() == nId)
					{
						pIPC->UpdateOnline(bOnline);
						setting = true;
					}
				}
			}
		}
	}

	return;
}

void VSCDeviceList::NotifyCameraRecording(int nId, BOOL bRecording)
{
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);
	bool setting = false;
	
	int cnt = qtreewidgetitem->childCount();
	//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
		VSCDeviceIPC *pIPC = dynamic_cast<VSCDeviceIPC*>(pChild);
		if (pIPC && pIPC->GetDeviceId() == nId)
		{
		 	pIPC->UpdateRecord(bRecording);
			setting = true; 
		}
	}


	if (setting == false)
	{
		/* loop for del to the group */
		int cnt = qtreewidgetitem->childCount();
		for (int i = 0; i < cnt; i ++)
		{
			QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
			VSCDeviceIPCGroup *pGroup = dynamic_cast<VSCDeviceIPCGroup*>(pChild);
			if (pGroup)
			{
				int cnt2 = pChild->childCount();
				//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
				for (int j = 0; j < cnt2; j ++)
				{
					QTreeWidgetItem * pChild2 = pChild->child(j);
					VSCDeviceIPC *pIPC = dynamic_cast<VSCDeviceIPC*>(pChild2);
					if (pIPC && pIPC->GetDeviceId() == nId)
					{
						pIPC->UpdateRecord(bRecording);
						setting = true;
					}
				}
			}
		}
	}

	return;
}

void VSCDeviceList::NotifyAddVms(int nId)
{
#if 0
	VSCVmsDataItem  pParam;
	if (gFactory->GetVmsById(pParam, nId) == TRUE)
	{
		switch (pParam.nType)
		{
		    case VSC_VMS_SITE:
		    {
		        AddSite(pParam);
		        break;
		    }
		    default:
		    {

		        break;
		    }
		}
	}
#endif
	return;
}


void VSCDeviceList::NotifyDelVms(int nId)
{	
	/* Site */
	{
		QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_SITE);
#if 0
		int cnt = qtreewidgetitem->childCount();
		//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
		for (int i = 0; i < cnt; i ++)
		{
			QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
			VSCVms *pVms = dynamic_cast<VSCVms*>(pChild);
			if (pVms && pVms->GetDeviceId() == nId)
			{
				qtreewidgetitem->removeChild(pChild);
			}
		}
#endif
	}

	return;
}

void VSCDeviceList::NotifyAddView(int nId)
{
#if 0
	VSCViewDataItem  pParam;
	if (gFactory->GetViewById(pParam, nId) == TRUE)
	{
		AddView(pParam);
	}
#endif
	return;
}


void VSCDeviceList::NotifyDelView(int nId)
{	
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_VIEW);

	int cnt = qtreewidgetitem->childCount();
	//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
		VSCViewTree *pView = dynamic_cast<VSCViewTree*>(pChild);
		if (pView && pView->GetDeviceId() == nId)
		{
			qtreewidgetitem->removeChild(pChild);
		}
	}
	return;
}

void VSCDeviceList::NotifyAddVGroup(int nId)
{
#if 0
	VSCVGroupDataItem  pParam;
	if (gFactory->GetVGroupById(pParam, nId) == TRUE)
	{
		AddVGroup(pParam);
	}
#endif
	return;
}


void VSCDeviceList::NotifyDelVGroup(int nId)
{	
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);

	int cnt = qtreewidgetitem->childCount();
	//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
		VSCDeviceIPCGroup *pGroup = dynamic_cast<VSCDeviceIPCGroup*>(pChild);
		if (pGroup && pGroup->GetDeviceId() == nId)
		{
			qtreewidgetitem->removeChild(pChild);
		}
	}
	return;
}


void VSCDeviceList::NotifyAddVIPC(int nId)
{
#if 0
	VIPCDeviceParam  pParam;
	if (gFactory->GetVIPCParamById(pParam, nId) == TRUE)
	{
		AddVIPC(pParam);
	}
#endif
	return;
}

void VSCDeviceList::NotifyDelVIPC(int nId)
{
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_V_IPC);
#if 0
	int cnt = qtreewidgetitem->childCount();
	//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
		VSCVIPC *pVIPC = dynamic_cast<VSCVIPC*>(pChild);
		if (pVIPC && pVIPC->GetDeviceId() == nId)
		{
			qtreewidgetitem->removeChild(pChild);
		}
	}
#endif
	return;
}

void VSCDeviceList::StartRecordAll()
{
	//gFactory->StartRecordAll();

	return;
}
void VSCDeviceList::StopRecordAll()
{
	//gFactory->StopRecordAll();

	return;
}
void VSCDeviceList::StartHdfsRecordAll()
{
	//gFactory->StartHdfsRecordAll();

	return;
}
void VSCDeviceList::StopHdfsRecordAll()
{
	//gFactory->StopHdfsRecordAll();

	return;
}

void VSCDeviceList::VmsTreeUpdated()
{
	/* Delete all the site */
	{
		QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_SITE);

		qDeleteAll(qtreewidgetitem->takeChildren());
	}
#if 0
	VSCVmsData pData;
	gFactory->GetVms(pData);
	for (s32 i = 1; i < CONF_VMS_NUM_MAX; i ++)
	{
	    	if (pData.data.conf.vms[i].Used == 1)
	    	{
		        switch (pData.data.conf.vms[i].nType)
		        {
		            case VSC_VMS_SITE:
		            {
		                AddSite(pData.data.conf.vms[i]);
		                break;
		            }
		            default:
		            {

		                break;
		            }
		        }
	    	}else
	    	{
	    		continue;
	    	}
	}	
#endif
}


void VSCDeviceList::ViewTreeUpdated()
{
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_VIEW);

	qDeleteAll(qtreewidgetitem->takeChildren());
#if 0
	VSCViewData pData;
	gFactory->GetView(pData);
	for (s32 i = 1; i < CONF_VIEW_NUM_MAX; i ++)
	{
	    	if (pData.data.conf.view[i].Used == 1)
	    	{
	    		AddView(pData.data.conf.view[i]);
	    	}else
	    	{
	    		continue;
	    	}
	}	
#endif
}

void VSCDeviceList::VGroupTreeUpdated()
{
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);

	qDeleteAll(qtreewidgetitem->takeChildren());
#if 0
	VSCVGroupData pData;
	gFactory->GetVGroup(pData);
	for (s32 i = 1; i < CONF_VGROUP_NUM_MAX; i ++)
	{
	    	if (pData.data.conf.group[i].Used == 1)
	    	{
	    		AddVGroup(pData.data.conf.group[i]);
	    	}else
	    	{
	    		continue;
	    	}
	}	
#endif
}


void VSCDeviceList::DiskTreeUpdated()
{
	/* Delete all the disk */
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_DSIK);

	qDeleteAll(qtreewidgetitem->takeChildren());
#if 0
	VDBDiskStatus pStatus;
	gFactory->GetDiskStatus(pStatus);


	VDBDiskStatus::iterator it = pStatus.begin();

	for(; it!=pStatus.end(); ++it)
	{
		astring strName = (*it).first;
		VdbDiskStatus diskStatus = (*it).second;
#ifdef WIN32
		strName = diskStatus.path;
#else
		strName = diskStatus.hdd;
#endif
		AddDisk(strName, ((diskStatus.totalSize  - diskStatus.freeSize)* 100)/diskStatus.totalSize);
	}

	ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_DSIK)->setExpanded(true);
#endif
	
}

void VSCDeviceList::CameraTreeUpdated()
{
#if 0
    //RemoveAllCamera();
    DeviceParamMap pMap;
    gFactory->GetDeviceParamMap(pMap);


    DeviceParamMap::iterator it = pMap.begin();

    for(; it!=pMap.end(); ++it)
    {
        u32 nId = (*it).first;
        DeviceParam pParam = (*it).second;
        switch (pParam.m_Conf.data.conf.nType)
        {
            case VSC_DEVICE_CAM:
            {
                AddIPCameraByGroup(pParam);
                break;
            }
            default:
            {

                break;
            }
        }

    }
#endif
    ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC)->setExpanded(true);

}


void VSCDeviceList::VIPCTreeUpdated()
{
    QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_V_IPC);
#if 0
    qDeleteAll(qtreewidgetitem->takeChildren());
    VIPCDeviceParamMap pMap;
    gFactory->GetVIPCDeviceParamMap(pMap);


    VIPCDeviceParamMap::iterator it = pMap.begin();

    for(; it!=pMap.end(); ++it)
    {
        u32 nId = (*it).first;
        VIPCDeviceParam pParam = (*it).second;
	 AddVIPC(pParam);
    }
#endif

    ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC)->setExpanded(true);
}


#if 0
void VSCDeviceList::AddDisk(astring strTitle, int usage)
{
    QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_DSIK);
    QIcon icon1;
    icon1.addFile(QStringLiteral(":/device/resources/harddisk.png"), QSize(), QIcon::Normal, QIcon::Off);
    //icon1.addFile(QStringLiteral(":/device/resources/camera-record.png"), QSize(), QIcon::Normal, QIcon::Off);

    QTreeWidgetItem *qtreewidgetitemChild = new QTreeWidgetItem(qtreewidgetitem);

    qtreewidgetitemChild->setIcon(0, icon1);

    //qtreewidgetitemChild->setText(0, QApplication::translate("Disk",  strTitle.c_str(), 0));

	ui.treeWidget->setItemWidget(qtreewidgetitemChild, 0, new VSCDiskDevice(strTitle, usage, this));

    qtreewidgetitem->setExpanded(true);
    qtreewidgetitem->sortChildren(0, Qt::AscendingOrder);
}

void VSCDeviceList::AddIPCameraByGroup(DeviceParam &pParam)
{
    	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);
	bool added = false;
	/* Default group */
	if (pParam.m_Conf.data.conf.GroupId == 0)
	{
		AddIPCamera(pParam, qtreewidgetitem);
		added = true;
	}else
	{
		/* loop for add to the group */
		int cnt = qtreewidgetitem->childCount();
		for (int i = 0; i < cnt; i ++)
		{
			QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
			VSCDeviceIPCGroup *pGroup = dynamic_cast<VSCDeviceIPCGroup*>(pChild);
			if (pGroup && pGroup->GetDeviceId() 
						== pParam.m_Conf.data.conf.GroupId)
			{
				AddIPCamera(pParam, pChild);
				added = true;
			}
		}		
	}

	if (added == false)
	{
		AddIPCamera(pParam, qtreewidgetitem);
	}

	return;
}
void VSCDeviceList::DelIPCameraByGroup(s32 nId)
{
	DeviceParam pParam;
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);
	bool deled = false;
    	gFactory->GetDeviceParamById(pParam, nId);
	if (pParam.m_Conf.data.conf.GroupId == 0)
	{
		int cnt = qtreewidgetitem->childCount();
		//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
		for (int i = 0; i < cnt; i ++)
		{
			QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
			VSCDeviceIPC *pIPC = dynamic_cast<VSCDeviceIPC*>(pChild);
			if (pIPC && pIPC->GetDeviceId() == nId)
			{
				qtreewidgetitem->removeChild(pChild);
				deled = true;
			}
		}
	}
	if (deled == false)
	{
		/* loop for del to the group */
		int cnt = qtreewidgetitem->childCount();
		for (int i = 0; i < cnt; i ++)
		{
			QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
			VSCDeviceIPCGroup *pGroup = dynamic_cast<VSCDeviceIPCGroup*>(pChild);
			if (pGroup)
			{
				int cnt2 = pChild->childCount();
				//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
				for (int j = 0; j < cnt2; j ++)
				{
					QTreeWidgetItem * pChild2 = pChild->child(j);
					VSCDeviceIPC *pIPC = dynamic_cast<VSCDeviceIPC*>(pChild2);
					if (pIPC && pIPC->GetDeviceId() == nId)
					{
						pChild->removeChild(pChild2);
						deled = true;
					}
				}
			}
		}
	}

	if (deled == false)
	{
		int cnt = qtreewidgetitem->childCount();
		//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
		for (int i = 0; i < cnt; i ++)
		{
			QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
			VSCDeviceIPC *pIPC = dynamic_cast<VSCDeviceIPC*>(pChild);
			if (pIPC && pIPC->GetDeviceId() == nId)
			{
				qtreewidgetitem->removeChild(pChild);
				deled = true;
			}
		}		
	}

	return;
}

void VSCDeviceList::AddIPCamera(DeviceParam &pParam, QTreeWidgetItem *qtreewidgetitem)
{
    //QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);
    QIcon icon1;
    icon1.addFile(QStringLiteral(":/device/resources/dome.png"), QSize(), QIcon::Normal, QIcon::Off);

    QTreeWidgetItem *qtreewidgetitemChild = new VSCDeviceIPC(qtreewidgetitem, pParam);

    qtreewidgetitemChild->setIcon(0, icon1);

    qtreewidgetitemChild->setText(0, QApplication::translate("",
            pParam.m_Conf.data.conf.Name, 0));

    qtreewidgetitem->setExpanded(true);
    VSCDeviceIPC *pIPC = dynamic_cast<VSCDeviceIPC*>(qtreewidgetitemChild);
    if (pParam.m_Conf.data.conf.Recording == 1)
    {
		pIPC->UpdateRecord(TRUE);
    }else
    {
    	pIPC->UpdateRecord(FALSE);
    }
}


void VSCDeviceList::AddSite(VSCVmsDataItem &pParam)//添加site
{
    QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_SITE);//所有的
    QIcon icon1;
    icon1.addFile(QStringLiteral(":/action/resources/computer.png"), QSize(), QIcon::Normal, QIcon::Off);
    //icon1.addFile(QStringLiteral(":/device/resources/camera-record.png"), QSize(), QIcon::Normal, QIcon::Off);

	QTreeWidgetItem *qtreewidgetitemChild = new VSCVmsOAPI(qtreewidgetitem, pParam);

    qtreewidgetitemChild->setIcon(0, icon1);

    qtreewidgetitemChild->setText(0, QApplication::translate("Recorder",
            pParam.Name, 0));

    qtreewidgetitem->setExpanded(true);
}

void VSCDeviceList::AddView(VSCViewDataItem &pParam)//添加site
{
    QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_VIEW);//所有的
    QIcon icon1;
    switch(pParam.Mode)
    {
        case LAYOUT_MODE_2X2:
            icon1.addFile(QStringLiteral(":/view/resources/2x2.png"), 
                QSize(), QIcon::Normal, QIcon::Off);
            break;
        case LAYOUT_MODE_3X3:
            icon1.addFile(QStringLiteral(":/view/resources/3x3.png"), 
                QSize(), QIcon::Normal, QIcon::Off);
            break;
        case LAYOUT_MODE_4X4:
            icon1.addFile(QStringLiteral(":/view/resources/4x4.png"), 
                QSize(), QIcon::Normal, QIcon::Off);
            break;
        case LAYOUT_MODE_1:
            icon1.addFile(QStringLiteral(":/view/resources/1x1.png"), 
                QSize(), QIcon::Normal, QIcon::Off);
            break;
        case LAYOUT_MODE_6:
            icon1.addFile(QStringLiteral(":/view/resources/6.png"), 
                QSize(), QIcon::Normal, QIcon::Off);
            break;
        case LAYOUT_MODE_12p1:
            icon1.addFile(QStringLiteral(":/view/resources/12+1.png"), 
                QSize(), QIcon::Normal, QIcon::Off);
            break;
        case LAYOUT_MODE_5x5:
            icon1.addFile(QStringLiteral(":/view/resources/5x5.png"), 
                QSize(), QIcon::Normal, QIcon::Off);
            break;
        case LAYOUT_MODE_6x6:
            icon1.addFile(QStringLiteral(":/view/resources/6x6.png"), 
                QSize(), QIcon::Normal, QIcon::Off);
            break;
        case LAYOUT_MODE_8x8:
            icon1.addFile(QStringLiteral(":/view/resources/8x8.png"), 
                QSize(), QIcon::Normal, QIcon::Off);
            break;
        case LAYOUT_MODE_ONE:
            icon1.addFile(QStringLiteral(":/view/resources/1x1.png"), 
                QSize(), QIcon::Normal, QIcon::Off);
            break;
            break;
        default:
            break;
    }
    QTreeWidgetItem *qtreewidgetitemChild = new VSCViewTree(qtreewidgetitem, pParam);

    qtreewidgetitemChild->setIcon(0, icon1);

    qtreewidgetitemChild->setText(0, QApplication::translate("View",
            pParam.Name, 0));

    qtreewidgetitem->setExpanded(true);
}

void VSCDeviceList::AddVGroup(VSCVGroupDataItem &pParam)
{
    QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);
    QIcon icon1;
    icon1.addFile(QStringLiteral(":/device/resources/camgroup.png"), 
        QSize(), QIcon::Normal, QIcon::Off);

    QTreeWidgetItem *qtreewidgetitemChild = new VSCDeviceIPCGroup(qtreewidgetitem, pParam);

    qtreewidgetitemChild->setIcon(0, icon1);

    qtreewidgetitemChild->setText(0, QApplication::translate("Group",
            pParam.Name, 0));

    qtreewidgetitem->setExpanded(true);
}


void VSCDeviceList::AddVIPC(VIPCDeviceParam &pParam)
{
    QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_V_IPC);//所有的
    QIcon icon1;
    icon1.addFile(QStringLiteral(":/device/resources/virtualipc.png"), QSize(), QIcon::Normal, QIcon::Off);

    QTreeWidgetItem *qtreewidgetitemChild = new VSCVIPC(qtreewidgetitem, pParam);

    qtreewidgetitemChild->setIcon(0, icon1);

    qtreewidgetitemChild->setText(0, QApplication::translate("Virutal IPC",
            pParam.m_Conf.data.conf.Name, 0));

    qtreewidgetitem->setExpanded(true);
}
#endif

void VSCDeviceList::UpdateOnline()
{

}


void VSCDeviceList::RemoveAllCamera()
{
    QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);

    qDeleteAll(qtreewidgetitem->takeChildren());
}

VSCDeviceList::~VSCDeviceList()
{
	m_Timer->stop();
	delete m_Timer;
}

void VSCDeviceList::SurveillanceClick()
{
    emit SurveillanceClicked();
    return;
}

void VSCDeviceList::CameraAddClick()
{
    emit CameraAddClicked();
    return;
}

void VSCDeviceList::EmapClick()
{
    emit EmapClicked();
    return;
}

void VSCDeviceList::RecordPlanClick()
{
    emit RecordPlanClicked();
    return;
}

void VSCDeviceList::DminingClick()
{
    emit DminingClicked();
    return;
}

void VSCDeviceList::SearchClick()
{
    emit SearchClicked();
    return;
}

void VSCDeviceList::RecorderClick()
{
    emit RecorderClicked();
    return;
}

void VSCDeviceList::VIPCAddClick()
{
    emit VIPCAddClicked();
    return;
}
