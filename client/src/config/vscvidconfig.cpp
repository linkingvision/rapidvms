#include "config/vscvidconfig.h"
#include "config/vidstor/vidstoradd.h"
#include "config/vidstor/vidcamadd.h"
#include "config/vidstor/vidstorsetting.h"
#include "config/vidclientsetting.h"

VSCVidConf::VSCVidConf(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent)
: VSCVidInf(pFactory, pTab, parent)
{
	m_pConfTree = new VSCVidTreeConf(m_pFactory);

	connect(m_pConfTree, SIGNAL(SignalStorAddSelectd()), this, SLOT(SlotVidStorAdd()));
	connect(m_pConfTree, SIGNAL(SignalCamAddSelectd(std::string)), this, 
								SLOT(SlotVidCamAdd(std::string)));
	connect(m_pConfTree, SIGNAL(SignalClientConfSelectd()), this, SLOT(SlotVidClientConf()));
	connect(m_pConfTree, SIGNAL(SignalStorConfSelectd(std::string)), this, 
								SLOT(SlotVidStorConf(std::string)));
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
void VSCVidConf::SlotVidStorConf(std::string strStorId)
{
	VidStor pStor;
	m_pFactory.GetConfDB().GetStorConf(strStorId, pStor);
	VidStorSetting *pConf = new VidStorSetting(pStor, m_pFactory, &m_pMainArea);
	m_pMainArea.addTab(pConf,QIcon(tr(":/action/resources/control_panel.png")), tr("Stor"));
	m_pMainArea.setCurrentWidget(pConf);	
}
