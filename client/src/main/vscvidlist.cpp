
#include "main/vscvidlist.h"
#include "server/factory.hpp"


VSCVidList::VSCVidList(QWidget *parent)
    : QWidget(parent), m_pVidInf(NULL), 
	m_pCameraTree(NULL), m_pGroupTree(NULL), 
	m_pViewTree(NULL), m_pEmapTree(NULL)
{
	ui.setupUi(this);

	m_treeLayout = new QVBoxLayout();

	//m_treeLayout->addWidget(m_pVideo);
	m_treeLayout->setMargin(0);

	ui.ListWidget->setLayout(m_treeLayout);
	ui.pbCamera->hide();
	ui.pbGroup->hide();
	ui.pbView->hide();
	ui.pbEmap->hide();
	
	connect(ui.pbCamera, SIGNAL(SignalClicked()), this, SLOT(SlotShowCameraTree()));
	connect(ui.pbGroup, SIGNAL(SignalClicked()), this, SLOT(SlotShowGroupTree()));
	
}
VSCVidList::~VSCVidList()
{

}

void VSCVidList::SlotShowCameraTree()
{
	m_pCameraTree->hide();
	m_pGroupTree->hide();
	//m_pViewTree->hide();
	//m_pEmapTree->hide();
	
	if (m_pCameraTree)
	{
		m_pCameraTree->show();
	}
}
void VSCVidList::SlotShowGroupTree()
{
	m_pCameraTree->hide();
	m_pGroupTree->hide();
	//m_pViewTree->hide();
	//m_pEmapTree->hide();
	
	if (m_pGroupTree)
	{
		m_pGroupTree->show();
	}	
}
void VSCVidList::SlotShowViewTree()
{
	
}
void VSCVidList::SlotShowEmapTree()
{
	
}

void VSCVidList::SetCurrVidInf(VSCVidInf *pVidInf)
{
	ui.pbCamera->hide();
	ui.pbGroup->hide();
	ui.pbView->hide();
	ui.pbEmap->hide();
	
	m_pCameraTree = pVidInf->GetCameraTree();
	m_pGroupTree = pVidInf->GetGroupTree();
	
	
	if (m_pCameraTree)
	{
		ui.pbCamera->show();
		/* Default is the Camera tree */
		m_treeLayout->addWidget(m_pCameraTree);
	}
	
	if (m_pGroupTree)
	{
		ui.pbGroup->show();
		m_treeLayout->addWidget(m_pGroupTree);
		m_pGroupTree->hide();
	}
	
	if (pVidInf->GetViewTree())
	{
		ui.pbView->show();
	}
	
	if (pVidInf->GetEmapTree())
	{
		ui.pbEmap->show();
	}
}