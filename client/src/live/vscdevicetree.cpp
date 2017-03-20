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
#include "vscdevicetree.h"
#include <QMimeData>
#include <QDrag>
#include "server/factory.hpp"
#include "vscdeviceipc.h"
#include <QPainter>
#include "vscdevicelist.h"
#include "vscvwidget.h"

extern Factory *gFactory;


VSCDeviceTree::VSCDeviceTree(QWidget *parent)
    : QTreeWidget(parent)
{
	createActions();
}

void VSCDeviceTree::createActions()
{
	/*Ipc*/
	pActAddCamera = new QAction(tr("&New"), this);
	pActAddCamera->setIcon(QIcon(":/action/resources/new.png"));
	pActEditCamera = new QAction(tr("&Edit"), this);
	pActEditCamera->setIcon(QIcon(":/action/resources/edit.png"));
	pActDeleteCamera = new QAction(tr("&Delete"), this);
	pActDeleteCamera->setIcon(QIcon(":/action/resources/user-trash-2.png"));
	pActRecord= new QAction(tr("&Start Record"), this);
	pActRecord->setIcon(QIcon(":/action/resources/recordstart.png"));
	pActStopRecord = new QAction(tr("&Stop Record"), this);
	pActStopRecord->setIcon(QIcon(":/action/resources/recordstop.png"));
	pActHdfsRecord= new QAction(tr("&Start HDFS Record"), this);
	pActHdfsRecord->setIcon(QIcon(":/action/resources/hdfsstartrecord.png"));
	pActStopHdfsRecord = new QAction(tr("&Stop HDFS Record"), this);
	pActStopHdfsRecord->setIcon(QIcon(":/action/resources/hdfsstoprecord.png"));


	pActRecordAll= new QAction(tr("&Start All Record"), this);
	pActRecordAll->setIcon(QIcon(":/action/resources/recordstart.png"));
	pActStopRecordAll = new QAction(tr("&Stop All Record"), this);
	pActStopRecordAll->setIcon(QIcon(":/action/resources/recordstop.png"));
	pActHdfsRecordAll= new QAction(tr("&Start All HDFS Record"), this);
	pActHdfsRecordAll->setIcon(QIcon(":/action/resources/hdfsstartrecord.png"));
	pActStopHdfsRecordAll = new QAction(tr("&Stop All HDFS Record"), this);
	pActStopHdfsRecordAll->setIcon(QIcon(":/action/resources/hdfsstoprecord.png"));


	/* Cam group */
	pActAddCamGroup = new QAction(tr("&New Group"), this);
	pActAddCamGroup->setIcon(QIcon(":/action/resources/new.png"));
	pActEditCamGroup = new QAction(tr("&Edit Group"), this);
	pActEditCamGroup->setIcon(QIcon(":/action/resources/edit.png"));
	pActDeleteCamGroup = new QAction(tr("&Delete Group"), this);
	pActDeleteCamGroup->setIcon(QIcon(":/action/resources/user-trash-2.png"));

	pActCamGroupMap = new QAction(tr("&Group"), this);
	pActCamGroupMap->setIcon(QIcon(":/device/resources/camgroup.png"));

	/*disk*/
	pActDiskEdit = new QAction(tr("&Edit"), this);
	pActDiskEdit->setIcon(QIcon(":/action/resources/edit.png"));

	/* VIPC */
	pActEditVIPC = new QAction(tr("&Edit"), this);
	pActEditVIPC->setIcon(QIcon(":/action/resources/edit.png"));
	pActAddVIPC = new QAction(tr("&New"), this);
	pActAddVIPC->setIcon(QIcon(":/action/resources/new.png"));
	pActDelVIPC= new QAction(tr("&Delete"), this);
	pActDelVIPC->setIcon(QIcon(":/action/resources/user-trash-2.png"));

	/* View */
	pActDelView= new QAction(tr("&Delete"), this);
	pActDelView->setIcon(QIcon(":/action/resources/user-trash-2.png"));

	/* VMS */
	pActDelVMS= new QAction(tr("&Delete"), this);
	pActDelVMS->setIcon(QIcon(":/action/resources/user-trash-2.png"));
	pActRefreshVMS= new QAction(tr("&Refresh"), this);
	pActRefreshVMS->setIcon(QIcon(":/action/resources/refresh.png"));

	SetupConnections();
	pPopMenu = new QMenu(this);
	
}

void VSCDeviceTree::SetupConnections()
{
	connect(pActAddCamera, SIGNAL(triggered()), this, SLOT(CameraAddClick()));
	connect(pActEditCamera, SIGNAL(triggered()), this, SLOT(CameraEditClick()));
	connect(pActDeleteCamera, SIGNAL(triggered()), this, SLOT(CameraDeleteClick()));
	connect(pActRecord, SIGNAL(triggered()), this, SLOT(CameraRecordClick()));
	connect(pActStopRecord, SIGNAL(triggered()), this, SLOT(CameraStopRecordClick()));

	connect(pActHdfsRecord, SIGNAL(triggered()), this, SLOT(CameraHdfsRecordClick()));
	connect(pActStopHdfsRecord, SIGNAL(triggered()), this, SLOT(CameraStopHdfsRecordClick()));

	connect(pActRecordAll, SIGNAL(triggered()), this, SLOT(StartRecordAllClick()));
	connect(pActStopRecordAll, SIGNAL(triggered()), this, SLOT(StopRecordAllClick()));
	connect(pActHdfsRecordAll, SIGNAL(triggered()), this, SLOT(StartHdfsRecordAllClick()));
	connect(pActStopHdfsRecordAll, SIGNAL(triggered()), this, SLOT(StopHdfsRecordAllClick()));


	connect(pActDiskEdit, SIGNAL(triggered()), this, SIGNAL(DiskEditClicked()));

	connect(pActAddVIPC, SIGNAL(triggered()), this, SLOT(VIPCAddClick()));
	connect(pActEditVIPC, SIGNAL(triggered()), this, SLOT(VIPCEditClick()));
	connect(pActDelVIPC, SIGNAL(triggered()), this, SLOT(VIPCDeleteClick()));

	connect(pActDelView, SIGNAL(triggered()), this, SLOT(ViewDeleteClick()));

	connect(pActAddCamGroup, SIGNAL(triggered()), this, SLOT(VGroupAddClick()));
	connect(pActEditCamGroup, SIGNAL(triggered()), this, SLOT(VGroupEditClick()));
	connect(pActDeleteCamGroup, SIGNAL(triggered()), this, SLOT(VGroupDeleteClick()));
	connect(pActCamGroupMap, SIGNAL(triggered()), this, SLOT(VGroupMapClick()));

	/* VMS */
	connect(pActDelVMS, SIGNAL(triggered()), this, SLOT(VMSDeleteClick()));
	connect(pActRefreshVMS, SIGNAL(triggered()), this, SLOT(VMSRefreshClick()));
	
}

void VSCDeviceTree::contextMenuEvent(QContextMenuEvent * event)
{
	QTreeWidgetItem *item = NULL;

	item = currentItem();
	pPopMenu->removeAction(pActAddCamera);
	pPopMenu->removeAction(pActEditCamera);
	pPopMenu->removeAction(pActDeleteCamera);
	pPopMenu->removeAction(pActRecord);
	pPopMenu->removeAction(pActStopRecord);
	pPopMenu->removeAction(pActHdfsRecord);
	pPopMenu->removeAction(pActStopHdfsRecord);
	pPopMenu->removeAction(pActRecordAll);
	pPopMenu->removeAction(pActStopRecordAll);
	pPopMenu->removeAction(pActHdfsRecordAll);
	pPopMenu->removeAction(pActStopHdfsRecordAll);

	/* Camera Group */
	pPopMenu->removeAction(pActAddCamGroup);
	pPopMenu->removeAction(pActEditCamGroup);
	pPopMenu->removeAction(pActDeleteCamGroup);
	pPopMenu->removeAction(pActCamGroupMap);


	/* Disk */
	pPopMenu->removeAction(pActDiskEdit);



	/* VIPC */
	pPopMenu->removeAction(pActEditVIPC);
	pPopMenu->removeAction(pActAddVIPC);
	pPopMenu->removeAction(pActDelVIPC);

	/* View  */
	pPopMenu->removeAction(pActDelView);

	/* VMS */
	pPopMenu->removeAction(pActDelVMS);
	pPopMenu->removeAction(pActRefreshVMS);

	if(item != NULL)
	{
#if 0
		    VSCDeviceIPC *pIpc = dynamic_cast<VSCDeviceIPC * >(item);
		    if (pIpc)
		    {
			pPopMenu->addAction(pActEditCamera);
			pPopMenu->addAction(pActDeleteCamera);
			pPopMenu->addSeparator();
			pPopMenu->addAction(pActRecord);
			pPopMenu->addAction(pActStopRecord);
			pPopMenu->addSeparator();
			pPopMenu->addAction(pActHdfsRecord);
			pPopMenu->addAction(pActStopHdfsRecord);
		    }
		    /* Camera group */
		    VSCDeviceIPCGroup *pIpcGroup = dynamic_cast<VSCDeviceIPCGroup * >(item);
		    if (pIpcGroup)
		    {
		        pPopMenu->addAction(pActEditCamGroup);
		        pPopMenu->addAction(pActDeleteCamGroup);
		    }

		    VSCVIPC *pVIPC = dynamic_cast<VSCVIPC * >(item);
		    if (pVIPC)
		    {
			pPopMenu->addAction(pActEditVIPC);
			pPopMenu->addAction(pActDelVIPC);
		    }

		VSCViewTree *pView = dynamic_cast<VSCViewTree * >(item);
		if (pView)
		{
			pPopMenu->addAction(pActDelView);
		}

		VSCVms *pVMS = dynamic_cast<VSCVms * >(item);
		if (pVMS)
		{
			pPopMenu->addAction(pActRefreshVMS);
			pPopMenu->addAction(pActDelVMS);
		}

#endif
		/* This is Top level Camera Item */
		if (item == topLevelItem(VSC_DEVICE_INDEX_IPC))
		{
			pPopMenu->addAction(pActAddCamera);
			pPopMenu->addSeparator();
			pPopMenu->addAction(pActAddCamGroup);
			pPopMenu->addAction(pActCamGroupMap);
			pPopMenu->addSeparator();
			pPopMenu->addAction(pActRecordAll);
			pPopMenu->addAction(pActStopRecordAll);
			pPopMenu->addSeparator();
			pPopMenu->addAction(pActHdfsRecordAll);
			pPopMenu->addAction(pActStopHdfsRecordAll);
		}
		if (item == topLevelItem(VSC_DEVICE_INDEX_DSIK))
		{
			pPopMenu->addAction(pActDiskEdit);
		}
		if (item == topLevelItem(VSC_DEVICE_INDEX_V_IPC))
		{
			pPopMenu->addAction(pActAddVIPC);
		}
	}
	pPopMenu->exec(QCursor::pos());
	event->accept();
}

void VSCDeviceTree::mousePressEvent(QMouseEvent *event)
{
	QTreeWidgetItem *selectedItem = currentItem();

	if (event->buttons() & Qt::RightButton)
	{
		return;
	}
	// If the selected Item exists
	if (selectedItem)
	{
		VSCDeviceIPC *pIpc = dynamic_cast<VSCDeviceIPC * >(selectedItem);
		if (pIpc)
		{
			u32 nId = pIpc->GetDeviceId();
			VDC_DEBUG( "%s id %d\n",__FUNCTION__, nId);
			QMimeData *mimeData = new QMimeData();
			mimeData->setText(QString::number(nId));

			// Create drag
			QPixmap pixmap(":/device/resources/camera1.png");
			QPainter painter(&pixmap);

			QDrag *drag = new QDrag(this);
			drag->setMimeData(mimeData);
			drag->setPixmap(pixmap);
			      drag->setHotSpot(QPoint(drag->pixmap().width()/2,
			             drag->pixmap().height()/2));
			drag->exec();
			QTreeWidget::mousePressEvent(event);
			return ;
		}

		VSCDeviceIPCOAPI *pIPCOAPI = dynamic_cast<VSCDeviceIPCOAPI * >(selectedItem);
		if (pIPCOAPI)
		{
			u32 nId = pIPCOAPI->GetDeviceId();
			u32 nOAPIId = pIPCOAPI->GetVMSId();
			VDC_DEBUG( "%s id %d oapi id\n",__FUNCTION__, nId, nOAPIId);
			QMimeData *mimeData = new VSCQMimeOAPI(nOAPIId, nId);

			// Create drag
			QPixmap pixmap(":/device/resources/camera1.png");
			QPainter painter(&pixmap);

			QDrag *drag = new QDrag(this);
			drag->setMimeData(mimeData);
			drag->setPixmap(pixmap);
			      drag->setHotSpot(QPoint(drag->pixmap().width()/2,
			             drag->pixmap().height()/2));
			drag->exec();
			QTreeWidget::mousePressEvent(event);
			return ;
		}

		VSCViewTree *pView = dynamic_cast<VSCViewTree * >(selectedItem);
		if (pView)
		{
			VSCQMimeView *pMimeView = new VSCQMimeView(pView->GetDeviceId());
			
			// Create drag
			QPixmap pixmap(":/device/resources/view1.png");
			QPainter painter(&pixmap);

			QDrag *drag = new QDrag(this);
			drag->setMimeData(pMimeView);
			drag->setPixmap(pixmap);
                      drag->setHotSpot(QPoint(drag->pixmap().width()/2,
                             drag->pixmap().height()/2));
			drag->exec();
			QTreeWidget::mousePressEvent(event);
			return ;
		}

	}
	
	QTreeWidget::mousePressEvent(event);
}

void VSCDeviceTree::mouseDoubleClickEvent(QMouseEvent *event)
{
	VDC_DEBUG( "%s \n",__FUNCTION__);
	QTreeWidgetItem *selectedItem = currentItem();

	if (event->buttons() & Qt::RightButton)
	{
		return;
	}

	if (selectedItem)
	{
		VSCDeviceIPC *pIpc = dynamic_cast<VSCDeviceIPC * >(selectedItem);

		if (pIpc)
		{
			u32 nId = pIpc->GetDeviceId();
			VDC_DEBUG( "%s id %d\n",__FUNCTION__, nId);

			emit CameraDoubleClicked(nId);

			//QTreeWidget::mouseDoubleClickEvent(event);
			return ;
		}
	}

	//QTreeWidget::mouseDoubleClickEvent(event);
}

void VSCDeviceTree::CameraAddClick()
{
    VDC_DEBUG( "%s \n",__FUNCTION__);
    emit CameraAddClicked();
    return;
}

void VSCDeviceTree::CameraEditClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s \n",__FUNCTION__);
    if(item != NULL)
    {
        VSCDeviceIPC *pIpc = dynamic_cast<VSCDeviceIPC * >(item);
        if (pIpc)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pIpc->GetDeviceId();
            emit CameraEditClicked(nId);
        }
    }

    return;
}

void VSCDeviceTree::CameraDeleteClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s \n",__FUNCTION__);
    if(item != NULL)
    {
        VSCDeviceIPC *pIpc = dynamic_cast<VSCDeviceIPC * >(item);
        if (pIpc)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pIpc->GetDeviceId();
            emit CameraDeleteClicked(nId);
        }
    }

    return;
}

void VSCDeviceTree::CameraRecordClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s Record\n",__FUNCTION__);
    if(item != NULL)
    {
        VSCDeviceIPC *pIpc = dynamic_cast<VSCDeviceIPC * >(item);
        if (pIpc)
        {
            VDC_DEBUG( "%s Record\n",__FUNCTION__);
            u32 nId = pIpc->GetDeviceId();
            //gFactory->StartRecord(nId);
        }
    }

    return;
}


void VSCDeviceTree::CameraStopRecordClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s Stop Record\n",__FUNCTION__);
    if(item != NULL)
    {
        VSCDeviceIPC *pIpc = dynamic_cast<VSCDeviceIPC * >(item);
        if (pIpc)
        {
            VDC_DEBUG( "%s Stop Record\n",__FUNCTION__);
            u32 nId = pIpc->GetDeviceId();
            //gFactory->StopRecord(nId);
        }
    }

    return;
}

void VSCDeviceTree::CameraHdfsRecordClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s Hdfs Record\n",__FUNCTION__);
    if(item != NULL)
    {
        VSCDeviceIPC *pIpc = dynamic_cast<VSCDeviceIPC * >(item);
        if (pIpc)
        {
            VDC_DEBUG( "%s Hdfs Record\n",__FUNCTION__);
            u32 nId = pIpc->GetDeviceId();
            //gFactory->StartHdfsRecord(nId);
        }
    }

    return;
}


void VSCDeviceTree::CameraStopHdfsRecordClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s Stop Hdfs Record\n",__FUNCTION__);
    if(item != NULL)
    {
        VSCDeviceIPC *pIpc = dynamic_cast<VSCDeviceIPC * >(item);
        if (pIpc)
        {
            VDC_DEBUG( "%s Stop Hdfs Record\n",__FUNCTION__);
            u32 nId = pIpc->GetDeviceId();
            //gFactory->StopHdfsRecord(nId);
        }
    }

    return;
}


void VSCDeviceTree::VIPCAddClick()
{
    VDC_DEBUG( "%s \n",__FUNCTION__);
    emit VIPCAddClicked();
    return;
}

void VSCDeviceTree::VIPCEditClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s \n",__FUNCTION__);
    if(item != NULL)
    {
#if 0
        VSCVIPC *pVIPC = dynamic_cast<VSCVIPC * >(item);
        if (pVIPC)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pVIPC->GetDeviceId();
            emit VIPCEditClicked(nId);
        }
#endif
    }

    return;
}

void VSCDeviceTree::VIPCDeleteClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s \n",__FUNCTION__);
    if(item != NULL)
    {
#if 0
        VSCVIPC *pVIPC = dynamic_cast<VSCVIPC * >(item);
        if (pVIPC)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pVIPC->GetDeviceId();
            emit VIPCDeleteClicked(nId);
        }
#endif
    }

    return;
}

void VSCDeviceTree::ViewDeleteClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s \n",__FUNCTION__);
    if(item != NULL)
    {
#if 0
        VSCViewTree *pView = dynamic_cast<VSCViewTree * >(item);
        if (pView)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pView->GetDeviceId();
            emit ViewDeleteClicked(nId);
        }
#endif
    }

    return;
}

void VSCDeviceTree::VMSDeleteClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s \n",__FUNCTION__);
    if(item != NULL)
    {
#if 0
        VSCVms *pVMS = dynamic_cast<VSCVms * >(item);
        if (pVMS)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pVMS->GetDeviceId();
            emit VMSDeleteClicked(nId);
        }
#endif
    }

    return;
}

void VSCDeviceTree::VMSRefreshClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s \n",__FUNCTION__);
    if(item != NULL)
    {
#if 0
        VSCVms *pVMS = dynamic_cast<VSCVms * >(item);
        if (pVMS)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            pVMS->Refresh();
        }
#endif
    }

    return;
}


void VSCDeviceTree::VGroupAddClick()
{
    VDC_DEBUG( "%s \n",__FUNCTION__);
    emit VGroupAddClicked();
    return;
}

void VSCDeviceTree::VGroupEditClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s \n",__FUNCTION__);
    if(item != NULL)
    {
        VSCDeviceIPCGroup *pVGroup = dynamic_cast<VSCDeviceIPCGroup * >(item);
        if (pVGroup)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pVGroup->GetDeviceId();
            emit VGroupEditClicked(nId);
        }
    }

    return;
}

void VSCDeviceTree::VGroupDeleteClick()
{
    QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s \n",__FUNCTION__);
    if(item != NULL)
    {
        VSCDeviceIPCGroup *pVGroup = dynamic_cast<VSCDeviceIPCGroup * >(item);
		if (pVGroup)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pVGroup->GetDeviceId();
            emit VGroupDeleteClicked(nId);
        }
    }

    return;
}

void VSCDeviceTree::VGroupMapClick()
{
    VDC_DEBUG( "%s \n",__FUNCTION__);
    emit VGroupMapClicked();
    return;
}

void VSCDeviceTree::StartRecordAllClick()
{
	VDC_DEBUG( "%s \n",__FUNCTION__);
	emit StartRecordAllClicked();
	return;
}
void VSCDeviceTree::StartHdfsRecordAllClick()
{
	VDC_DEBUG( "%s \n",__FUNCTION__);
	emit StartHdfsRecordAllClicked();
	return;
}

void VSCDeviceTree::StopRecordAllClick()
{
	VDC_DEBUG( "%s \n",__FUNCTION__);
	emit StopRecordAllClicked();
	return;
}
void VSCDeviceTree::StopHdfsRecordAllClick()
{
	VDC_DEBUG( "%s \n",__FUNCTION__);
	emit StopHdfsRecordAllClicked();
	return;
}

