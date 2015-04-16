
#include "vscdevicelist.h"
#include "vscdeviceipc.h"
#include "factory.hpp"
#include "vscvms.h"
#include "vscviewtree.h"

extern Factory *gFactory;

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
	gFactory->RegDeviceChangeNotify((void *)this, 
				(FactoryDeviceChangeNotify)(VSCDeviceList::DeviceChangeCallbackFunc));


	//ui.pbRecorder->hide();
	//ui.lbRecorder->hide();
	
	//ui.pbDmining->hide();
	//ui.lbMining->hide();
	
	//ui.pbEmap->hide();
	//ui.lbEmap->hide();


}

void VSCDeviceList::SetupConnections()
{
	connect(ui.pbSurveillance, SIGNAL(clicked()), this, SLOT(SurveillanceClick()));
	connect(ui.pbCamera, SIGNAL(clicked()), this, SLOT(CameraAddClick()));
	connect(ui.pbEmap, SIGNAL(clicked()), this, SLOT(EmapClick()));
	connect(ui.pbDmining, SIGNAL(clicked()), this, SLOT(DminingClick()));
	connect(ui.pbSearch, SIGNAL(clicked()), this, SLOT(SearchClick()));
	connect(ui.pbRecorder, SIGNAL(clicked()), this, SLOT(RecorderClick()));
	connect(ui.pbVIPC, SIGNAL(clicked()), this, SLOT(VIPCAddClick()));

	connect(ui.treeWidget, SIGNAL(CameraAddClicked()), this, SIGNAL(CameraAddClicked()));
	connect(ui.treeWidget, SIGNAL(CameraEditClicked(int)), this, SIGNAL(CameraEditClicked(int)));
	connect(ui.treeWidget, SIGNAL(CameraDeleteClicked(int)), this, SIGNAL(CameraDeleteClicked(int)));


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
	connect(ui.pbTrash, SIGNAL(CameraDeleted()), this, SLOT(CameraTreeUpdated()));

	/* Group */
	connect(ui.treeWidget, SIGNAL(VGroupAddClicked()), this, SIGNAL(VGroupAddClicked()));
	connect(ui.treeWidget, SIGNAL(VGroupEditClicked(int)), this, SIGNAL(VGroupEditClicked(int)));
	connect(ui.treeWidget, SIGNAL(VGroupDeleteClicked(int)), this, SIGNAL(VGroupDeleteClicked(int)));
	connect(ui.treeWidget, SIGNAL(VGroupMapClicked()), this, SIGNAL(VGroupMapClicked()));

	/* VMS */
	connect(ui.treeWidget, SIGNAL(VMSDeleteClicked(int)), this, SIGNAL(VMSDeleteClicked(int)));

	
}



bool VSCDeviceList::DeviceChangeCallbackFunc(void* pData, FactoryDeviceChangeData change)
{
	if (pData)
	{
		VSCDeviceList * pDeviceList = (VSCDeviceList *)pData;
		pDeviceList->DeviceChangeCallbackFunc1(change);
	}
	return true;
}

bool VSCDeviceList::DeviceChangeCallbackFunc1(FactoryDeviceChangeData change)
{
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

	return true;
}

void VSCDeviceList::NotifyAddCamera(int nId)
{
	DeviceParam  pParam;
	if (gFactory->GetDeviceParamById(pParam, nId) == TRUE)
	{
		AddIPCameraByGroup(pParam);
	}
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
	DelIPCameraByGroup(nId);
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
	return;
}


void VSCDeviceList::NotifyDelVms(int nId)
{	
	/* Site */
	{
		QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_SITE);

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
	}

	return;
}

void VSCDeviceList::NotifyAddView(int nId)
{
	VSCViewDataItem  pParam;
	if (gFactory->GetViewById(pParam, nId) == TRUE)
	{
		AddView(pParam);
	}
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
	VSCVGroupDataItem  pParam;
	if (gFactory->GetVGroupById(pParam, nId) == TRUE)
	{
		AddVGroup(pParam);
	}
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
	VIPCDeviceParam  pParam;
	if (gFactory->GetVIPCParamById(pParam, nId) == TRUE)
	{
		AddVIPC(pParam);
	}
	return;
}

void VSCDeviceList::NotifyDelVIPC(int nId)
{
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_V_IPC);

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
	return;
}

void VSCDeviceList::StartRecordAll()
{
	gFactory->StartRecordAll();

	return;
}
void VSCDeviceList::StopRecordAll()
{
	gFactory->StopRecordAll();

	return;
}
void VSCDeviceList::StartHdfsRecordAll()
{
	gFactory->StartHdfsRecordAll();

	return;
}
void VSCDeviceList::StopHdfsRecordAll()
{
	gFactory->StopHdfsRecordAll();

	return;
}

void VSCDeviceList::VmsTreeUpdated()
{
	/* Delete all the site */
	{
		QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_SITE);

		qDeleteAll(qtreewidgetitem->takeChildren());
	}

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
}


void VSCDeviceList::ViewTreeUpdated()
{
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_VIEW);

	qDeleteAll(qtreewidgetitem->takeChildren());
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
}

void VSCDeviceList::VGroupTreeUpdated()
{
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);

	qDeleteAll(qtreewidgetitem->takeChildren());
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
}


void VSCDeviceList::DiskTreeUpdated()
{
	/* Delete all the disk */
	QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_DSIK);

	qDeleteAll(qtreewidgetitem->takeChildren());

	VDBDiskStatus pStatus;
	gFactory->GetDiskStatus(pStatus);


	VDBDiskStatus::iterator it = pStatus.begin();

	for(; it!=pStatus.end(); ++it)
	{
		astring strName = (*it).first;
		VdbDiskStatus diskStatus = (*it).second;
		s8 DiskTitle[1024];
		memset(DiskTitle, 0, sizeof(DiskTitle));
		sprintf(DiskTitle, "  %lld%%",   
				((diskStatus.totalSize  - diskStatus.freeSize)* 100)/diskStatus.totalSize);
		strName = diskStatus.path + DiskTitle;
		AddDisk(strName);
	}

	ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_DSIK)->setExpanded(true);

	
}

void VSCDeviceList::CameraTreeUpdated()
{
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

    ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC)->setExpanded(true);

}


void VSCDeviceList::VIPCTreeUpdated()
{
    QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_V_IPC);

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

    ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC)->setExpanded(true);
}



void VSCDeviceList::AddDisk(astring strTitle)
{
    QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_DSIK);
    QIcon icon1;
    icon1.addFile(QStringLiteral(":/device/resources/harddisk.png"), QSize(), QIcon::Normal, QIcon::Off);
    //icon1.addFile(QStringLiteral(":/device/resources/camera-record.png"), QSize(), QIcon::Normal, QIcon::Off);

    QTreeWidgetItem *qtreewidgetitemChild = new QTreeWidgetItem(qtreewidgetitem);

    qtreewidgetitemChild->setIcon(0, icon1);

    qtreewidgetitemChild->setText(0, QApplication::translate("Disk",  strTitle.c_str(), 0));

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


void VSCDeviceList::UpdateOnline()
{

}

BOOL VSCDeviceList::GetRecorderMap(RecorderMap &pMap)
{
	//TODO get all the recorder from VSC_DEVICE_INDEX_SITE
	return true;

	/* Site */
	{
		QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_SITE);

		int cnt = qtreewidgetitem->childCount();
		//VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
		for (int i = 0; i < cnt; i ++)
		{
			QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
			VSCVms *pVms = dynamic_cast<VSCVms*>(pChild);
			
				pVms->GetRecorderMap(pMap);

		}
	}
	/*//fake
	VSCRecorder zb1(0, "192.168.12.12", VSC_SUB_VMS_ZB);
	VSCRecorder zb2(0, "192.168.12.32", VSC_SUB_VMS_ZB);
	VSCRecorder zb3(0, "192.168.12.33", VSC_SUB_VMS_ZB);
	pMap.insert( std::map<int, VSCRecorder>::value_type(0, zb1) );
	pMap.insert( std::map<int, VSCRecorder>::value_type(1, zb2) );
	pMap.insert( std::map<int, VSCRecorder>::value_type(2, zb3) );
	//pMap.insert( std::map<int, VSCRecorder>::value_type(3, zb3) );*/
	return TRUE;
}

void VSCDeviceList::RemoveAllCamera()
{
    QTreeWidgetItem *qtreewidgetitem = ui.treeWidget->topLevelItem(VSC_DEVICE_INDEX_IPC);

    qDeleteAll(qtreewidgetitem->takeChildren());
#if 0
    int cnt = qtreewidgetitem->childCount();
    VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, cnt);
    for (int i = 0; i < cnt; i ++)
    {
        VDC_DEBUG( "%s cnt %d\n",__FUNCTION__, i);
        QTreeWidgetItem * pChild = qtreewidgetitem->child(i);
        delete pChild;
    }
#endif
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
