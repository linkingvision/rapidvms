

#include "vscmainwindows.h"

#include <QAction>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSignalMapper>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <qdebug.h>
#include <QDockWidget>
#include <QToolBar>
#include <QTabWidget>
#include <QListWidget>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QApplication>

#include "vscdevicelist.h"
#include "vscview.h"
#include "vscplayback.h"
#include "vscvwidget.h"
#include "vscvideowall.h"
#include "vsccameraadd.h"
#include "vscvipcadd.h"
#include "factory.hpp"
#include "vscsearch.h"
#include "vsctoolbar.h"
#include "vscsiteadd.h"
#include "vscsetting.h"
#include "vschddedit.h"
#include "vscloading.hpp"
#include "vscipcgroupconf.h"
#include "vscipcgroupmap.h"
#include "vscuserstatus.h"
#include "vemap.hpp"
#include "vdmining.hpp"
#include "vsclogin.h"
#include "vscpanel.h"

extern Factory *gFactory;

Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

VSCMainWindows::VSCMainWindows(QWidget *parent)
    : m_pEMap(NULL), m_pPanel(NULL), QMainWindow(parent)
{

	ui.setupUi(this);
#ifdef WIN32
	setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif


	QWidget *widget = new QWidget;
	setCentralWidget(widget);
	m_pToolBar = new VSCToolBar(this);
	//m_pEvent->hide();
	CreateActions();
	SetupMenuBar();
	SetupToolBar();
	CreateDockWindows();

	m_pMainArea = new QTabWidget(this);

	m_pMainArea->setTabsClosable(true);
	m_pMainArea->setMovable(true);
	m_pEventThread = VEventThread::CreateObject();
	m_pEventThread->start();

	VSCLoading *loading = new VSCLoading(NULL);
	loading->show();
	   QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	loading->setGeometry(rect.width()/2, rect.height()/2, 64, 64);
	QCoreApplication::processEvents();
#if 1	
	m_pView = new VSCView(m_pMainArea, *m_pMainArea, tr("Main View"));
	m_pView->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint  );
	m_pMainArea->addTab(m_pView,QIcon(tr(":/view/resources/3x3.png")), tr("Main View"));
	connect(m_pEventThread, SIGNAL(EventNotify(int, int)), 
			m_pView, SLOT(DeviceEvent(int, int)));
#endif

	setCentralWidget(m_pMainArea);

	QString message = tr("VS Cloud Client");

	delete loading;
	SetupConnections();
	setWindowTitle(QApplication::translate("VSCMainWindowsClass", VE_INFO, 0));
}

VSCMainWindows::~VSCMainWindows()
{

}

void VSCMainWindows::ViewHideFocus()
{
	m_pView->ViewHideFocus();
}

void VSCMainWindows::closeEvent(QCloseEvent *event)
{
	ExitOpenCVR();
	event->ignore(); 
}

void VSCMainWindows::SetupConnections()
{
	connect(m_pMainArea, SIGNAL(tabCloseRequested(int)), this, SLOT(MainCloseTab(int)));
	connect(m_pDeviceList, SIGNAL(SurveillanceClicked()), this, SLOT(AddSurveillance()));
	connect(m_pDeviceList, SIGNAL(CameraAddClicked()), this, SLOT(AddCamera()));
	connect(m_pDeviceList, SIGNAL(EmapClicked()), this, SLOT(AddEmap()));
	connect(m_pDeviceList, SIGNAL(DminingClicked()), this, SLOT(AddDmining()));
	connect(m_pDeviceList, SIGNAL(SearchClicked()), this, SLOT(Search()));
	connect(m_pDeviceList, SIGNAL(RecorderClicked()), this, SLOT(AddRecorder()));


	connect(m_pDeviceList, SIGNAL(CameraEditClicked(int)), this, SLOT(EditCamera(int)));
	connect(m_pDeviceList, SIGNAL(CameraDeleteClicked(int)), this, SLOT(DeleteCamera(int)));

	/* VIPC */
	connect(m_pDeviceList, SIGNAL(VIPCAddClicked()), this, SLOT(AddVIPC()));
	connect(m_pDeviceList, SIGNAL(VIPCEditClicked(int)), this, SLOT(EditVIPC(int)));
	connect(m_pDeviceList, SIGNAL(VIPCDeleteClicked(int)), this, SLOT(DeleteVIPC(int)));

	/* Camera Group */
	connect(m_pDeviceList, SIGNAL(VGroupAddClicked()), this, SLOT(AddVGroup()));
	connect(m_pDeviceList, SIGNAL(VGroupEditClicked(int)), this, SLOT(EditVGroup(int)));
	connect(m_pDeviceList, SIGNAL(VGroupDeleteClicked(int)), this, SLOT(DeleteVGroup(int)));
	connect(m_pDeviceList, SIGNAL(VGroupMapClicked()), this, SLOT(MapVGroup()));


	/* Disk edit */
	connect(m_pDeviceList, SIGNAL(DiskEditClicked()), this, SLOT(EditDisk()));

	/* View */
	connect(m_pDeviceList, SIGNAL(ViewDeleteClicked(int)), this, SLOT(DeleteView(int)));

	/* VMS */
	connect(m_pDeviceList, SIGNAL(VMSDeleteClicked(int)), this, SLOT(DeleteVMS(int)));


	//connect(this, SIGNAL(CameraDeleted()), m_pDeviceList, SLOT(CameraTreeUpdated()));
	connect(m_pToolBar->ui.pbFullScreen, SIGNAL(clicked()), this, SLOT(SetFullScreen()));
	connect(m_pToolBar->ui.pbAbout, SIGNAL(clicked()), this, SLOT(about()));
	connect(m_pToolBar->ui.pbAlarm, SIGNAL(clicked()), this, SLOT(AddEvent()));
	connect(m_pToolBar->ui.pbSetting, SIGNAL(clicked()), this, SLOT(Setting()));
	connect(m_pToolBar->ui.pbUser, SIGNAL(clicked()), this, SLOT(UserStatus()));
	connect(m_pToolBar->ui.pbPanel, SIGNAL(clicked()), this, SLOT(Panel()));
	connect(m_pEventThread, SIGNAL(EventNotifyNoParam()), m_pToolBar, SLOT(NewAlarm()));

}

void VSCMainWindows::AddEvent()
{
	VEvent *pEvent = VEvent::CreateObject(m_pMainArea);

	m_pMainArea->addTab(pEvent, QIcon(tr(":/action/resources/alarmno.png")), tr("Alarm"));
	m_pMainArea->setCurrentWidget(pEvent);
	ViewHideFocus();
}

void VSCMainWindows::AddSurveillance()
{
	char Name[256];
	int tabNum = m_pMainArea->count();
	int currentNum = -1;
	for (int i=0; i<tabNum; i++)
	{
		QWidget *qwidget = m_pMainArea->widget(i);
		VSCView *pView = dynamic_cast<VSCView* >(qwidget);
		if (pView)
		{
			currentNum++;
		}
	}
	sprintf(Name, "%s %d", "View", currentNum+1);
	VSCLoading *loading = new VSCLoading(NULL);
	loading->setGeometry(width()/2, height()/2, 64, 64);
	QCoreApplication::processEvents();
	VSCView *pView = new VSCView(m_pMainArea,  *m_pMainArea, tr("View %1").arg(currentNum+1));
	pView->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint  );
    	m_pMainArea->addTab(pView, QIcon(tr(":/view/resources/3x3.png")), tr("View %1").arg(currentNum+1));

	m_pMainArea->setCurrentWidget(pView);
	ViewHideFocus();
	delete loading;
	connect(m_pEventThread, SIGNAL(EventNotify(int, VscEventType)), pView, SLOT(DeviceEvent(int, VscEventType)));
}

void VSCMainWindows::AddEmap()
{
	if (m_pEMap == NULL)
	{
		m_pEMap = VEMap::CreateObject(*m_pMainArea, m_pMainArea);
		connect(m_pEventThread, SIGNAL(EventNotify(int, int)), 
			m_pEMap, SLOT(DeviceEvent(int, int)));
		m_pEMap->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint  );

	}
	
	m_pMainArea->addTab(m_pEMap, QIcon(tr(":/action/resources/map.png")), tr("eMap"));
	m_pMainArea->setCurrentWidget(m_pEMap);
	m_pEMap->show();
	ViewHideFocus();
}

void VSCMainWindows::AddDmining()
{
    VDMining *pDMining = VDMining::CreateObject(m_pMainArea);
	
    m_pMainArea->addTab(pDMining, QIcon(tr(":/action/resources/mining.png")), tr("Mining"));
    m_pMainArea->setCurrentWidget(pDMining);
	ViewHideFocus();
}


void VSCMainWindows::Setting()
{
    VSCSetting *pSetting = new VSCSetting(this, *m_pDeviceList);

    m_pMainArea->addTab(pSetting, QIcon(tr(":/action/resources/setting.png")), tr("Setting"));  
    m_pMainArea->setCurrentWidget(pSetting);
	ViewHideFocus();
}

void VSCMainWindows::Panel()
{
	if (m_pPanel == NULL)
	{
    		m_pPanel = new VSCPanel(this);
		connect(m_pPanel, SIGNAL(AddRecorder()), this, SLOT(AddRecorder()));
		connect(m_pPanel, SIGNAL(AddCamera()), this, SLOT(AddCamera()));
		connect(m_pPanel, SIGNAL(AddSurveillance()), this, SLOT(AddSurveillance()));
		connect(m_pPanel, SIGNAL(Search()), this, SLOT(Search()));
		connect(m_pPanel, SIGNAL(AddEmap()), this, SLOT(AddEmap()));
		connect(m_pPanel, SIGNAL(AddDmining()), this, SLOT(AddDmining()));
		connect(m_pPanel, SIGNAL(Setting()), this, SLOT(Setting()));
		connect(m_pPanel, SIGNAL(AddEvent()), this, SLOT(AddEvent()));
	}

	m_pMainArea->addTab(m_pPanel, QIcon(tr(":/action/resources/panel.png")), tr("Control Panel"));  
	m_pMainArea->setCurrentWidget(m_pPanel);

	ViewHideFocus();
}

void VSCMainWindows::AddCamera()
{
    DeviceParam mParam;
    VSCCameraAdd *pCameraadd = new VSCCameraAdd(mParam, this);

    m_pMainArea->addTab(pCameraadd, QIcon(tr(":/device/resources/camera.png")), tr("Camera"));  
    m_pMainArea->setCurrentWidget(pCameraadd);
	ViewHideFocus();
    //connect(pCameraadd, SIGNAL(CameraTreeUpdated()), m_pDeviceList, SLOT(CameraTreeUpdated()));
}

void VSCMainWindows::AddRecorder()
{
	AddSite();
}

void VSCMainWindows::AddSite()
{
    VSCVmsDataItem mParam;
    memset(&mParam, 0, sizeof(mParam));
    VSCVmsDataItemDefault(mParam);
    VSCSiteAdd *pSiteadd = new VSCSiteAdd(mParam, this);

    m_pMainArea->addTab(pSiteadd, QIcon(tr(":/action/resources/computer.png")), tr("Recorder"));  
    m_pMainArea->setCurrentWidget(pSiteadd);
	ViewHideFocus();
}

void VSCMainWindows::Search()
{
    if (VSCSearch::m_bStarted == TRUE)
    {
        QMessageBox msgBox(this);
        //Set text
        msgBox.setText(tr("Search is In Processing ..."));
            //Set predefined icon, icon is show on left side of text.
        msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
            //Set focus of ok button
        msgBox.setDefaultButton(QMessageBox::Ok);

        int ret = msgBox.exec();

        return;
    }
    VSCSearch *pSearch = new VSCSearch(this);

    m_pMainArea->addTab(pSearch, QIcon(tr(":/action/resources/search.png")), tr("Search"));
    m_pMainArea->setCurrentWidget(pSearch);
	ViewHideFocus();
	
    //connect(pSearch, SIGNAL(CameraTreeUpdated()), m_pDeviceList, SLOT(CameraTreeUpdated()));
}


void VSCMainWindows::EditDisk()
{
    VDC_DEBUG( "%s\n",__FUNCTION__);
    VSCHddEdit *pDiskEdit = new VSCHddEdit(this);

    m_pMainArea->addTab(pDiskEdit, QIcon(tr(":/device/resources/harddisk.png")), tr("Disk"));
    m_pMainArea->setCurrentWidget(pDiskEdit);
	ViewHideFocus();
    //connect(pDiskEdit, SIGNAL(DiskTreeUpdated()), m_pDeviceList, SLOT(DiskTreeUpdated()));
}

void VSCMainWindows::EditCamera(int nId)
{
    VDC_DEBUG( "%s %d\n",__FUNCTION__, nId);
    DeviceParam mParam;
    gFactory->GetDeviceParamById(mParam, nId);
    VSCCameraAdd *pCameraadd = new VSCCameraAdd(mParam, this);

    m_pMainArea->addTab(pCameraadd, QIcon(tr(":/device/resources/camera.png")), tr("Camera"));
    m_pMainArea->setCurrentWidget(pCameraadd);
	ViewHideFocus();
    //connect(pCameraadd, SIGNAL(CameraTreeUpdated()), m_pDeviceList, SLOT(CameraTreeUpdated()));
}
void VSCMainWindows::DeleteCamera(int nId)
{
    VDC_DEBUG( "%s %d\n",__FUNCTION__, nId);
    QMessageBox msgBox(this);
    //Set text
    msgBox.setText(tr("Delete the Camera ..."));
        //Set predefined icon, icon is show on left side of text.
    msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        //Set focus of ok button
    msgBox.setDefaultButton(QMessageBox::Ok);

        //execute message box. method exec() return the button value of cliecke button
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
       gFactory->DelDevice(nId);
       emit CameraDeleted();
       break;
    default:
       // should never be reached
       break;
    }

    return;
}

void VSCMainWindows::ExitOpenCVR()
{
    QMessageBox msgBox(this);
    //Set text
    msgBox.setText(tr("OpenCVR Exit ..."));
        //Set predefined icon, icon is show on left side of text.
    msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        //Set focus of ok button
    msgBox.setDefaultButton(QMessageBox::Ok);

        //execute message box. method exec() return the button value of cliecke button
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
	QApplication::quit();
	exit(0);
       break;
    default:
       // should never be reached
       break;
    }

    return;
}

void VSCMainWindows::AddVIPC()
{
    VIPCDeviceParam mParam;
    VSCVIPCAdd *pVIPCadd = new VSCVIPCAdd(mParam, this);

    m_pMainArea->addTab(pVIPCadd, QIcon(tr(":/device/resources/virtualipc.png")), tr("Virutal IPC"));  
    m_pMainArea->setCurrentWidget(pVIPCadd);
	ViewHideFocus();
}

void VSCMainWindows::EditVIPC(int nId)
{
    VDC_DEBUG( "%s %d\n",__FUNCTION__, nId);
    VIPCDeviceParam mParam;
    gFactory->GetVIPCParamById(mParam, nId);
    VSCVIPCAdd *pVIPCadd = new VSCVIPCAdd(mParam, this);

    m_pMainArea->addTab(pVIPCadd, QIcon(tr(":/device/resources/virtualipc.png")), tr("Virutal IPC"));  
    m_pMainArea->setCurrentWidget(pVIPCadd);
	ViewHideFocus();
}
void VSCMainWindows::DeleteVIPC(int nId)
{
    VDC_DEBUG( "%s %d\n",__FUNCTION__, nId);
    QMessageBox msgBox(this);
    //Set text
    msgBox.setText(tr("Delete the Virtual IPC ..."));
        //Set predefined icon, icon is show on left side of text.
    msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        //Set focus of ok button
    msgBox.setDefaultButton(QMessageBox::Ok);

        //execute message box. method exec() return the button value of cliecke button
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
       gFactory->DelVIPC(nId);
       break;
    default:
       // should never be reached
       break;
    }

    return;
}

void VSCMainWindows::AddVGroup()
{
	VSCIPCGroupConf group;
	VSCVGroupDataItem groupItem;
	group.SetName("Group");
	group.show();
       QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	group.setGeometry(rect.width()/2 -group.width()/2 , 
					rect.height()/2 - group.height()/2, 
					group.width(), group.height());
	group.exec();

	VSCIPCGroupConfType type = group.GetType();
	if (type == VSC_IPCGROUP_CONF_LAST 
		|| type == VSC_IPCGROUP_CONF_CANCEL)
	{
		return;
	}
	string strName = "Group";
	group.GetName(strName);
	strcpy(groupItem.Name, strName.c_str());
    	gFactory->AddVGroup(groupItem);
}

void VSCMainWindows::EditVGroup(int nId)
{
    VSCIPCGroupConf group;
	VSCVGroupDataItem groupItem;
	gFactory->GetVGroupById(groupItem, nId);
	group.SetName(groupItem.Name);
       group.show();
       QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	group.setGeometry(rect.width()/2 -group.width()/2 , 
					rect.height()/2 - group.height()/2, 
					group.width(), group.height());
       group.exec();

	VSCIPCGroupConfType type = group.GetType();
	if (type == VSC_IPCGROUP_CONF_LAST 
		|| type == VSC_IPCGROUP_CONF_CANCEL)
	{
		return;
	}
	string strName = "Group";
	group.GetName(strName);
	strcpy(groupItem.Name, strName.c_str());
    	gFactory->AddVGroup(groupItem);
}
void VSCMainWindows::DeleteVGroup(int nId)
{
    VDC_DEBUG( "%s %d\n",__FUNCTION__, nId);
    QMessageBox msgBox(this);
    //Set text
    msgBox.setText(tr("Delete the  Group ..."));
        //Set predefined icon, icon is show on left side of text.
    msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        //Set focus of ok button
    msgBox.setDefaultButton(QMessageBox::Ok);

        //execute message box. method exec() return the button value of cliecke button
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
       gFactory->DelVGroup(nId);
       break;
    default:
       // should never be reached
       break;
    }

    return;
}


void VSCMainWindows::MapVGroup()
{
   VDC_DEBUG( "%s\n",__FUNCTION__);
    VSCVGroupWall *pGroup = new VSCVGroupWall(this);

    m_pMainArea->addTab(pGroup, QIcon(tr(":/device/resources/camgroup.png")), tr("Camera Group"));
    m_pMainArea->setCurrentWidget(pGroup);
	ViewHideFocus();
}

void VSCMainWindows::DeleteView(int nId)
{
    VDC_DEBUG( "%s %d\n",__FUNCTION__, nId);
    QMessageBox msgBox(this);
    //Set text
    msgBox.setText(tr("Delete the View ..."));
        //Set predefined icon, icon is show on left side of text.
    msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        //Set focus of ok button
    msgBox.setDefaultButton(QMessageBox::Ok);

        //execute message box. method exec() return the button value of cliecke button
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
       gFactory->DelView(nId);
       break;
    default:
       // should never be reached
       break;
    }

    return;
}

void VSCMainWindows::DeleteVMS(int nId)
{
    VDC_DEBUG( "%s %d\n",__FUNCTION__, nId);
    QMessageBox msgBox(this);
    //Set text
    msgBox.setText(tr("Delete the Rcorder ..."));
        //Set predefined icon, icon is show on left side of text.
    msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        //Set focus of ok button
    msgBox.setDefaultButton(QMessageBox::Ok);

        //execute message box. method exec() return the button value of cliecke button
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
       gFactory->DelVms(nId);
       break;
    default:
       // should never be reached
       break;
    }

    return;
}


void VSCMainWindows::CreateActions()
{
    pActSurveillance = new QAction(tr("&Surveillance"), this);
    pActSurveillance->setIcon(QIcon(":/action/resources/surveillance.png"));

    pActSearch = new QAction(tr("&Search"), this);
    pActSearch->setIcon(QIcon(":/action/resources/search.png"));


    pActDeviceList = new QAction(tr("&Devices List"), this);
    pActDeviceList->setIcon(QIcon(":/action/resources/devicelist.png"));

    pActDeviceAdd = new QAction(tr("&Add"), this);
    pActDeviceAdd->setIcon(QIcon(":/action/resources/list-add.png"));

    pActDeviceDel = new QAction(tr("&Delete"), this);
    pActDeviceDel->setIcon(QIcon(":/action/resources/list-remove.png"));

    pActDeviceConf = new QAction(tr("&Configurate"), this);
    pActDeviceConf->setIcon(QIcon(":/action/resources/configure.png"));

}

void VSCMainWindows::SetupToolBar()
{
    QToolBar *pToolBar = new QToolBar();
    pToolBar->addWidget(m_pToolBar);
    addToolBar(Qt::TopToolBarArea, pToolBar);

}
void VSCMainWindows::SetupMenuBar()
{

}
void VSCMainWindows::about()
{
   QMessageBox::about(this, tr("About OpenCVR"),
            tr("<b>OpenCVR</b>. <br>"
            "<a href=\"https://github.com/xsmart/opencvr\">https://github.com/xsmart/opencvr</a>"
            "  <br><a href=\"http://veyesys.com/\">http://veyesys.com/</a>"));
}

void VSCMainWindows::UserStatus()
{
	VSCUserStatus userStatus;

	userStatus.show();
       QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	userStatus.setGeometry(rect.width()/2 -userStatus.width()/2 , 
					rect.height()/2 - userStatus.height()/2, 
					userStatus.width(), userStatus.height());
	userStatus.exec();

	if (userStatus.IsQuit() == TRUE)
	{
		ShowLogin();
	}
}

void VSCMainWindows::ShowLogin()
{
	VSCLogin login;

	hide();
again:
	login.SetDefault();
	login.show();
#if 0
    QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	userStatus.setGeometry(rect.width()/2 -userStatus.width()/2 , 
					rect.height()/2 - userStatus.height()/2, 
					userStatus.width(), userStatus.height());
#endif
	login.exec();
	if (login.GetIsLogin() == TRUE)
	{	
		astring strUser;
		astring strPasswd;
		login.GetUserPasswd(strUser, strPasswd);
		if (gFactory->AuthUser(strUser, strPasswd) == TRUE)
		{
			showMaximized();
			return;
		}
		QMessageBox msgBox(this);
		//Set text
		msgBox.setText(tr("Username or Password not correct ..."));
		    //Set predefined icon, icon is show on left side of text.
		msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

		msgBox.setStandardButtons(QMessageBox::Ok);
		    //Set focus of ok button
		msgBox.setDefaultButton(QMessageBox::Ok);

		    //execute message box. method exec() return the button value of cliecke button
		int ret = msgBox.exec();
	}else
	{
		ExitOpenCVR();
	}

	goto again;
	
}

void VSCMainWindows::SetFullScreen()
{
    if(isFullScreen()) {
        this->setWindowState(Qt::WindowMaximized);
    } else {
        this->setWindowState(Qt::WindowFullScreen);
    }
}

void VSCMainWindows::CreateDockWindows()
{
    QDockWidget *pDockDevicelist = new QDockWidget(tr("Devices"), this);
    pDockDevicelist->setFeatures(QDockWidget::DockWidgetMovable);

    //pDockDevicelist->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pDockDevicelist->setAllowedAreas(Qt::LeftDockWidgetArea);

    m_pDeviceList = new VSCDeviceList(pDockDevicelist);
    pDockDevicelist->setWidget(m_pDeviceList);

    addDockWidget(Qt::LeftDockWidgetArea, pDockDevicelist);
}

void VSCMainWindows::MainCloseTab(int index)
{
    QWidget *wdgt = m_pMainArea->widget(index );
    m_pMainArea->setCurrentWidget(wdgt);
    /* Frist view can not be closed */
    if (wdgt == m_pView)
    {
        return;
    }

    if (wdgt == m_pEMap)	
    {
    	m_pMainArea->removeTab(index);
	m_pEMap->hide();
	return;
    }

    if (wdgt == m_pPanel)	
    {
    	m_pMainArea->removeTab(index);
	m_pPanel->hide();
	return;
    }
    m_pMainArea->removeTab(index);
    if (wdgt)
    {
	VSCLoading *loading = new VSCLoading(NULL);
	loading->setGeometry(width()/2, height()/2, 64, 64);
	QCoreApplication::processEvents();
        delete wdgt;
        wdgt = NULL;
	delete loading;
    }
}
