
#include "vscdevicetree.h"
#include <QMimeData>
#include <QDrag>
#include "factory.hpp"
#include "vscdeviceipc.h"
#include <QPainter>
#include "vscdevicelist.h"
#include "vsczbtreeitem.h"

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
	
    /*VSCZbIpc*/
    pActDelVSCZbIpc = new QAction(tr("&Delete"), this);//hjy
    pActDelVSCZbIpc->setIcon(QIcon(":/action/resources/user-trash-2.png"));
    pActRecordVSCZbIpc= new QAction(tr("&Start Recording"), this);
    pActRecordVSCZbIpc->setIcon(QIcon(":/action/resources/recordstart.png"));
    pActStopRecordVSCZbIpc = new QAction(tr("&Stop Recording"), this);
    pActStopRecordVSCZbIpc->setIcon(QIcon(":/action/resources/recordstop.png"));

    SetupConnections();
    pPopMenu = new QMenu(this);
    //pPopMenu->addAction(pActAddCamera);
    //pPopMenu->addAction(pActEditCamera);
    //pPopMenu->addAction(pActDeleteCamera);
}

void VSCDeviceTree::SetupConnections()
{
	connect(pActAddCamera, SIGNAL(triggered()), this, SLOT(CameraAddClick()));
	connect(pActEditCamera, SIGNAL(triggered()), this, SLOT(CameraEditClick()));
	connect(pActDeleteCamera, SIGNAL(triggered()), this, SLOT(CameraDeleteClick()));
	connect(pActRecord, SIGNAL(triggered()), this, SLOT(CameraRecordClick()));
	connect(pActStopRecord, SIGNAL(triggered()), this, SLOT(CameraStopRecordClick()));

	connect(pActDiskEdit, SIGNAL(triggered()), this, SIGNAL(DiskEditClicked()));

	connect(pActAddVIPC, SIGNAL(triggered()), this, SLOT(VIPCAddClick()));
	connect(pActEditVIPC, SIGNAL(triggered()), this, SLOT(VIPCEditClick()));
	connect(pActDelVIPC, SIGNAL(triggered()), this, SLOT(VIPCDeleteClick()));

	connect(pActDelView, SIGNAL(triggered()), this, SLOT(ViewDeleteClick()));

	connect(pActDelVSCZbIpc, SIGNAL(triggered()), this, SLOT(VSCZbIpcDeleteClick()));//
	connect(pActRecordVSCZbIpc, SIGNAL(triggered()), this, SLOT(VSCZbIpcRecordClick()));
	connect(pActStopRecordVSCZbIpc, SIGNAL(triggered()), this, SLOT(VSCZbIpcStopRecordClick()));

	connect(pActAddCamGroup, SIGNAL(triggered()), this, SLOT(VGroupAddClick()));
	connect(pActEditCamGroup, SIGNAL(triggered()), this, SLOT(VGroupEditClick()));
	connect(pActDeleteCamGroup, SIGNAL(triggered()), this, SLOT(VGroupDeleteClick()));
	connect(pActCamGroupMap, SIGNAL(triggered()), this, SLOT(VGroupMapClick()));
	
}

void VSCDeviceTree::contextMenuEvent(QContextMenuEvent * event)
{
    QTreeWidgetItem *item = NULL;
#if 0
    QPoint point = QCursor::pos();
    point = mapFromGlobal(point);
    item = this->itemAt(point);
    //pActAddCamera->setEnabled (false);
    //pActEditCamera->setEnabled (false);
    //pActDeleteCamera->setEnabled (false);
#endif
    item = currentItem();//获取当前被点击的节点
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

	pPopMenu->removeAction(pActDelVSCZbIpc);
	pPopMenu->removeAction(pActRecordVSCZbIpc);
	pPopMenu->removeAction(pActStopRecordVSCZbIpc);

    if(item != NULL)
    {
        VSCDeviceIPC *pIpc = dynamic_cast<VSCDeviceIPC * >(item);
        if (pIpc)//指向IP camera里面的项
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

	VSCZbTreeItemIPC *pVSCZbIpc = dynamic_cast<VSCZbTreeItemIPC * >(item);
        if (pVSCZbIpc)//指向NVR的ipc
        {
            pPopMenu->addAction(pActDelVSCZbIpc);
            pPopMenu->addAction(pActRecordVSCZbIpc);
            pPopMenu->addAction(pActStopRecordVSCZbIpc);	
         }

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
    pPopMenu->exec(QCursor::pos());//菜单出现的位置为当前鼠标的位置
    event->accept();
}

void VSCDeviceTree::mousePressEvent(QMouseEvent *event)
{
	VDC_DEBUG( "%s \n",__FUNCTION__);
	// Get current selection
	QTreeWidgetItem *selectedItem = currentItem();
	
    if (event->buttons() & Qt::RightButton)
    {
        return;
    }
	// If the selected Item exists
	if (selectedItem)
	{
		VSCDeviceIPC *pIpc = dynamic_cast<VSCDeviceIPC * >(selectedItem);
	    //VSCDeviceIPC *pIpc = (VSCDeviceIPC * )(selectedItem);
        // Create data
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
		VSCZbTreeItemIPC *pSiteIpc = dynamic_cast<VSCZbTreeItemIPC * >(selectedItem);
	    //VSCDeviceIPC *pIpc = (VSCDeviceIPC * )(selectedItem);
        // Create data
		if (pSiteIpc)
		{
			VSCQMimeDataIPC *pMimeIpc = new VSCQMimeDataIPC();
			
			// Create drag
			QPixmap pixmap(":/device/resources/camera1.png");
			QPainter painter(&pixmap);

			QDrag *drag = new QDrag(this);
			drag->setMimeData(pMimeIpc);
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
            gFactory->StartRecord(nId);
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
            gFactory->StopRecord(nId);
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
        VSCVIPC *pVIPC = dynamic_cast<VSCVIPC * >(item);
        if (pVIPC)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pVIPC->GetDeviceId();
            emit VIPCEditClicked(nId);
        }
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
        VSCVIPC *pVIPC = dynamic_cast<VSCVIPC * >(item);
        if (pVIPC)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pVIPC->GetDeviceId();
            emit VIPCDeleteClicked(nId);
        }
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
        VSCViewTree *pView = dynamic_cast<VSCViewTree * >(item);
        if (pView)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pView->GetDeviceId();
            emit ViewDeleteClicked(nId);
        }
    }

    return;
}

void VSCDeviceTree::VSCZbIpcDeleteClick()
{
	QTreeWidgetItem* child = currentItem();
	QTreeWidgetItem* parent = currentItem()->parent();
	VSCZbTreeItemIPC *pVSCZbIpc = dynamic_cast<VSCZbTreeItemIPC * >(child);
	u32 nId = pVSCZbIpc->GetDeviceId();
	pVSCZbIpc->DeleteTreeItemIPC();
	if(parent) 
	{
		parent->removeChild(child);
	}
	emit VSCZbIpcDeleteClicked(nId);

    return;
}

void VSCDeviceTree::VSCZbIpcRecordClick()//nur的IPC的startrecordclick
{
	QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s \n",__FUNCTION__);
    if(item != NULL)
    {
        VSCZbTreeItemIPC *pVSCZbIpc = dynamic_cast<VSCZbTreeItemIPC * >(item);
        if (pVSCZbIpc)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pVSCZbIpc->GetDeviceId();
			//qDebug() << QString("hahahaa2: %1").arg(pVSCZbIpc->GetDeviceId());
			gFactory->StartRecord(nId);
			//TODO  VSCZbIPCRecord()?
			//if(pVSCZbIpc->GetDeviceParam().m_Conf.data.conf.Recording==1)
				pVSCZbIpc->VSCZbIPCUpdateRecord(true);
        }
    }

	return;
}

void VSCDeviceTree::VSCZbIpcStopRecordClick()
{
	QTreeWidgetItem *item = NULL;
    item = currentItem();

    VDC_DEBUG( "%s \n",__FUNCTION__);
    if(item != NULL)
    {
        VSCZbTreeItemIPC *pVSCZbIpc = dynamic_cast<VSCZbTreeItemIPC * >(item);
        if (pVSCZbIpc)
        {
            VDC_DEBUG( "%s \n",__FUNCTION__);
            u32 nId = pVSCZbIpc->GetDeviceId();
            gFactory->StartRecord(nId);
			//TODO VSCZbIPCRecord()?
			//if(pVSCZbIpc->GetDeviceParam().m_Conf.data.conf.Recording==0)
				pVSCZbIpc->VSCZbIPCUpdateRecord(false);
        }
		
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

