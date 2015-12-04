#include "live/vscvidlive.h"
#include "live/vscdevicelist.h"
#include "live/vscview.h"
#include "vscvwidget.h"
#include "vscvideowall.h"

Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

VSCVidLive::VSCVidLive(ClientFactory &pFactory, QMainWindow *parent)
: VSCVidInf(pFactory, parent)
{
	m_pMainArea = new QTabWidget(m_parent);

	m_pMainArea->setTabsClosable(true);
	m_pMainArea->setMovable(true);
	
#if 1	
	m_pView = new VSCView(m_pMainArea, *m_pMainArea, tr("Main View"));
	m_pView->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint  );
	connect(m_pDeviceList, SIGNAL(CameraDoubleClicked(int)), m_pView, SLOT(CameraDoubleClicked(int)));

	m_pMainArea->addTab(m_pView,QIcon(tr(":/view/resources/3x3.png")), tr("Main View"));
	//connect(m_pEventThread, SIGNAL(EventNotify(int, int)), 
		//	m_pView, SLOT(DeviceEvent(int, int)));
#endif
	connect(m_pMainArea, SIGNAL(tabCloseRequested(int)), this, SLOT(MainCloseTab(int)));
	
}
VSCVidLive::~VSCVidLive()
{
	
}

void VSCVidLive::MainCloseTab(int index)
{
    QWidget *wdgt = m_pMainArea->widget(index);
    m_pMainArea->setCurrentWidget(wdgt);
    /* Frist view can not be closed */
    if (wdgt == m_pView)
    {
        return;
    }
#if 0
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
#endif
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

void VSCVidLive::VidShow()
{
	m_parent->addDockWidget(Qt::LeftDockWidgetArea, m_pDockDevicelist);
	m_parent->setCentralWidget(m_pMainArea);
}
void VSCVidLive::VidHide()
{
	m_parent->removeDockWidget(m_pDockDevicelist);
}

void VSCVidLive::VidNewLiveView()
{
}

void VSCVidLive::VidNewEmap()
{
}

QTreeWidget *VSCVidLive::GetCameraTree()
{
}
QTreeWidget *VSCVidLive::GetGroupTree()
{
}
QTreeWidget *VSCVidLive::GetEmapTree()
{
}
QTreeWidget *VSCVidLive::GetViewTree()
{
}