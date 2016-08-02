#include "searchpb/vscvidsearchpb.h"
#include "searchpb/vscvideventsearch.h"
#include "common/vidtree/vscvidtreecam.h"

VSCVidSearchPB::VSCVidSearchPB(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent)
	: m_pCameraTree(NULL), m_pSearch(NULL), VSCVidInf(pFactory, pTab, parent)
{
	m_pCameraTree = new VSCVidTreeCam(m_pFactory, parent);
	m_pCameraTree->Init();
	//m_pCameraTree->VidSetCheckedChild(NULL, true);
	m_pCameraTree->VidSetCheckedChild(NULL, false);
	m_pCameraTree->hide();
}
VSCVidSearchPB::~VSCVidSearchPB()
{
	
}

void VSCVidSearchPB::VidShow()
{

}
void VSCVidSearchPB::VidHide()
{

}

void VSCVidSearchPB::VidNewEventSearch()
{
	if (m_pSearch == NULL)
	{
		m_pSearch = new VSCVidEventSearch(m_pFactory, *m_pCameraTree, &m_pMainArea);
		m_pSearch->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint  );
	}

	m_pSearch->show();
	m_pMainArea.addTab(m_pSearch, QIcon(tr(":/action/resources/alarm_search.png")), tr("Event Search"));
	m_pMainArea.setCurrentWidget(m_pSearch);
}

bool VSCVidSearchPB::CheckClosed(QWidget * pTab)
{
	if (m_pSearch == pTab)
	{
		return true;
	}else
	{
		return false;
	}
}

void VSCVidSearchPB::VidNewMotionSearch()
{
}

VSCVidTreeInf *VSCVidSearchPB::GetCameraTree()
{
	return m_pCameraTree;
}
