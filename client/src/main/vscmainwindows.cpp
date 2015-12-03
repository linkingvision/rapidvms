

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


Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

VSCMainWindows::VSCMainWindows(ClientFactory &pFactory, QWidget *parent)
    : m_pFactory(pFactory), QMainWindow(parent), m_VidIdx(VSC_VID_IDX_LAST)
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

	/* Show the Live default */
	m_pVidLive = new VSCVidLive(m_pFactory, this);
	m_pVidConf = new VSCVidInf(m_pFactory, this);//TODO
	m_pVidPb = new VSCVidInf(m_pFactory, this);//TODO
	ShowVidLive();

	delete loading;
	SetupConnections();
	setWindowTitle(QApplication::translate("VSCMainWindowsClass", VE_INFO, 0));
}

VSCMainWindows::~VSCMainWindows()
{

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
	m_VidIdx = VSC_VID_IDX_LIVE;
}
void VSCMainWindows::ShowVidConf()
{
	switch (m_VidIdx)
	{
		case VSC_VID_IDX_LIVE:
		{
			m_pVidLive->VidHide();
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
}
void VSCMainWindows::ShowVidPb()
{
	switch (m_VidIdx)
	{
		case VSC_VID_IDX_LIVE:
		{
			m_pVidLive->VidHide();
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
            "  <br><a href=\"http://veyesys.com/\">http://veyesys.com/</a>"));
}

void VSCMainWindows::UserStatus()
{
#if 0
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
#endif
}

void VSCMainWindows::ShowLogin()
{
#if 0
	VSCLogin login;

	hide();
again:
	login.SetDefault();
	login.show();

    QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	userStatus.setGeometry(rect.width()/2 -userStatus.width()/2 , 
					rect.height()/2 - userStatus.height()/2, 
					userStatus.width(), userStatus.height());

	login.exec();
	if (login.GetIsLogin() == TRUE)
	{	
		astring strUser;
		astring strPasswd;
		login.GetUserPasswd(strUser, strPasswd);
		//if (gFactory->AuthUser(strUser, strPasswd) == TRUE)
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
#endif
	
}

void VSCMainWindows::SetFullScreen()
{
    if(isFullScreen()) {
        this->setWindowState(Qt::WindowMaximized);
    } else {
        this->setWindowState(Qt::WindowFullScreen);
    }
}


