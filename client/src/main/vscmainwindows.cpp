

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

#include "server/factory.hpp"
#include "vsctoolbar.h"
#include "vscloading.hpp"
#include "vscuserstatus.h"
#include "vsclogin.h"


Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

VSCMainWindows::VSCMainWindows(ClientFactory &pFactory, QWidget *parent)
    : m_pFactory(pFactory), QMainWindow(parent), m_VidIdx(VSC_VID_IDX_LAST), 
    m_pMainView(NULL), m_pDashBoard(NULL)
{

	ui.setupUi(this);
#ifdef WIN32
	setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif
	m_pToolBar = new VSCToolBar(this);
	SetupToolBar();

	VSCLoading *loading = new VSCLoading(NULL);
	loading->show();
	QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	loading->setGeometry(rect.width()/2, rect.height()/2, 64, 64);
	QCoreApplication::processEvents();

	//QDockWidget *m_pDockDevicelist = new QDockWidget(tr("Devices"), this);
	m_pDockDevicelist = new QDockWidget(this);
	m_pDockDevicelist->setFeatures(QDockWidget::DockWidgetMovable);

	m_pDockDevicelist->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	//m_pDockDevicelist->setAllowedAreas(Qt::LeftDockWidgetArea);
	m_pDockDevicelist->setTitleBarWidget(new QWidget(this));

	m_pVidList = new VSCVidList(m_pDockDevicelist);
	m_pDockDevicelist->setWidget(m_pVidList);

	addDockWidget(Qt::LeftDockWidgetArea, m_pDockDevicelist);
	//addDockWidget(Qt::RightDockWidgetArea, m_pDockDevicelist);

	m_pMainArea = new QTabWidget(this);

	m_pMainArea->setTabsClosable(true);
	m_pMainArea->setMovable(true);

	setCentralWidget(m_pMainArea);
	connect(m_pMainArea, SIGNAL(tabCloseRequested(int)), this, SLOT(MainCloseTab(int)));
	

	/* Show the Live default */
	m_pVidLive = new VSCVidLive(m_pFactory, *m_pMainArea, this);
	m_pMainView = m_pVidLive->GetMainView();
	m_pVidConf = new VSCVidConf(m_pFactory, *m_pMainArea, this);
	m_pVidPb = new VSCVidSearchPB(m_pFactory, *m_pMainArea, this);

	/* First hide all */
	m_pVidLive->VidHide();
	m_pVidConf->VidHide();
	m_pVidPb->VidHide();
	ShowVidLive();

	delete loading;

	setWindowTitle(QApplication::translate("VSCMainWindowsClass", VE_INFO, 0));

	connect(m_pToolBar->ui.pbFullScreen, SIGNAL(clicked()), this, SLOT(SetFullScreen()));
	connect(m_pToolBar->ui.pbAbout, SIGNAL(clicked()), this, SLOT(about()));
	connect(m_pToolBar->ui.pbUser, SIGNAL(clicked()), this, SLOT(UserStatus()));

	connect(m_pToolBar->ui.pbVidLive, SIGNAL(clicked()), this, SLOT(ShowVidLive()));
	connect(m_pToolBar->ui.pbVidLiveView, SIGNAL(clicked()), m_pVidLive, SLOT(SlotNewLiveView()));
	connect(m_pToolBar->ui.pbVidPb, SIGNAL(clicked()), m_pVidLive, SLOT(SlotNewLivePB()));

	connect(m_pToolBar->ui.pbVidSearch, SIGNAL(clicked()), this, SLOT(ShowVidPb()));
	connect(m_pToolBar->ui.pbVidConf, SIGNAL(clicked()), this, SLOT(ShowVidConf()));
	connect(m_pToolBar->ui.pbVidDashBoard, SIGNAL(clicked()), this, SLOT(ShowDashBoard()));

}

VSCMainWindows::~VSCMainWindows()
{

}

void VSCMainWindows::MainCloseTab(int index)
{
    QWidget *wdgt = m_pMainArea->widget(index);
	m_pMainArea->setCurrentWidget(wdgt);
    /* Frist view can not be closed */
    if (wdgt == m_pMainView)
    {
        return;
    }


    if (wdgt == m_pDashBoard)	
    {
		m_pMainArea->removeTab(index);
		m_pDashBoard->hide();
		return;
    }

	m_pMainArea->removeTab(index);
    if (wdgt)
    {
		delete wdgt;
		wdgt = NULL;
    }
}

void VSCMainWindows::ShowDashBoard()
{
	if (m_pDashBoard == NULL)
	{
    		m_pDashBoard = new VSCDashBoard(this);
		//connect(m_pPanel, SIGNAL(AddVIPC()), this, SLOT(AddVIPC()));
	}

	m_pMainArea->addTab(m_pDashBoard, QIcon(tr(":/action/resources/dashboard.png")), tr("Dashboard"));  
	m_pMainArea->setCurrentWidget(m_pDashBoard);
}

void VSCMainWindows::ShowVidLive()
{
	switch (m_VidIdx)
	{
		case VSC_VID_IDX_LIVE:
		{
			return;
			break;
		}
		case VSC_VID_IDX_CONF:
		{
			m_pVidConf->VidHide();
			break;
		}
		case VSC_VID_IDX_PB:
		{
			m_pVidPb->VidHide();
			break;
		}
		default:
		{
			break;
		}
	}

	m_pVidLive->VidShow();
	m_pToolBar->ui.pbVidLiveView->show();
	m_pToolBar->ui.pbVidPb->show();
	m_VidIdx = VSC_VID_IDX_LIVE;
	m_pVidList->SetCurrVidInf(m_pVidLive);
}
void VSCMainWindows::ShowVidConf()
{
	switch (m_VidIdx)
	{
		case VSC_VID_IDX_LIVE:
		{
			m_pVidLive->VidHide();
			m_pToolBar->ui.pbVidLiveView->hide();
			m_pToolBar->ui.pbVidPb->hide();
			break;
		}
		case VSC_VID_IDX_CONF:
		{
			return;
			break;
		}
		case VSC_VID_IDX_PB:
		{
			m_pVidPb->VidHide();
			break;
		}
		default:
		{
			break;
		}
	}

	m_pVidConf->VidShow();
	m_VidIdx = VSC_VID_IDX_CONF;
	m_pVidList->SetCurrVidInf(m_pVidConf);
}
void VSCMainWindows::ShowVidPb()
{
	switch (m_VidIdx)
	{
		case VSC_VID_IDX_LIVE:
		{
			m_pVidLive->VidHide();
			m_pToolBar->ui.pbVidLiveView->hide();
			m_pToolBar->ui.pbVidPb->hide();
			break;
		}
		case VSC_VID_IDX_CONF:
		{
			m_pVidConf->VidHide();
			break;
		}
		case VSC_VID_IDX_PB:
		{
			return;
			break;
		}
		default:
		{
			break;
		}
	}

	m_pVidPb->VidShow();
	m_VidIdx = VSC_VID_IDX_PB;
	m_pVidList->SetCurrVidInf(m_pVidPb);

}

void VSCMainWindows::ViewHideFocus()
{
	//m_pView->ViewHideFocus();
}

void VSCMainWindows::closeEvent(QCloseEvent *event)
{
	ExitOpenCVR();
	event->ignore(); 
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


void VSCMainWindows::SetupToolBar()
{
    QToolBar *pToolBar = new QToolBar();
    pToolBar->addWidget(m_pToolBar);
    addToolBar(Qt::TopToolBarArea, pToolBar);

}

void VSCMainWindows::about()
{
   QMessageBox::about(this, tr("About OpenCVR"),
            tr("<b>OpenCVR</b>. <br>"
            "<a href=\"https://github.com/xsmart/opencvr\">https://github.com/xsmart/opencvr</a>"
            "  <br><a href=\"http://www.veyesys.com/\">http://www.veyesys.com/</a>"));
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

    QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);

	login.exec();
	if (login.GetIsLogin() == TRUE)
	{	
		astring strUser;
		astring strPasswd;
		login.GetUserPasswd(strUser, strPasswd);
		if (m_pFactory.AuthUser(strUser, strPasswd) == TRUE)
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


