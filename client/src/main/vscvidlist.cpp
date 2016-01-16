
#include "main/vscvidlist.h"
#include "server/factory.hpp"


VSCVidList::VSCVidList(QWidget *parent)
    : QWidget(parent), m_pVidInf(NULL), 
	m_pCameraTree(NULL), m_pGroupTree(NULL), 
	m_pViewTree(NULL), m_pEmapTree(NULL), m_pConfTree(NULL)
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
	connect(ui.pbGroup, SIGNAL(SignalClicked()), this, SLOT(SlotShowGroupTree()));
	connect(ui.filterText, SIGNAL(textChanged(const QString &)), this,
					SLOT(FilterChanged(const QString &)));
	connect(ui.pbFilterCancel, SIGNAL(clicked() ), this, SLOT(FilterCancel()));

	this->setAcceptDrops(true);
}
VSCVidList::~VSCVidList()
{

}

void VSCVidList::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void VSCVidList::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}


void VSCVidList::HideAll()
{
	if (m_pCameraTree)
		m_pCameraTree->hide();

	if (m_pGroupTree)
		m_pGroupTree->hide();

	if (m_pViewTree)
		m_pViewTree->hide();

	if (m_pEmapTree)
		m_pEmapTree->hide();

	if (m_pConfTree)
		m_pConfTree->hide();
}

void VSCVidList::FilterChanged(const QString & text)
{
	astring strFilter = text.toStdString();
	m_pCameraTree->VidFilter(strFilter);
}

void VSCVidList::FilterCancel()
{
	QString empty;
	ui.filterText->setText(empty);
}

void VSCVidList::SlotShowCameraTree()
{
	HideAll();
	
	if (m_pCameraTree)
	{
		m_pCameraTree->show();
	}
}
void VSCVidList::SlotShowGroupTree()
{
	HideAll();
	
	if (m_pGroupTree)
	{
		m_pGroupTree->show();
	}	
}
void VSCVidList::SlotShowViewTree()
{
	HideAll();
	if (m_pViewTree)
	{
		m_pViewTree->show();
	}
}
void VSCVidList::SlotShowEmapTree()
{
	HideAll();
	if (m_pEmapTree)
	{
		m_pEmapTree->show();
	}
}

void VSCVidList::SetCurrVidInf(VSCVidInf *pVidInf)
{
	HideAll();
	ui.pbCamera->hide();
	ui.pbGroup->hide();
	ui.pbView->hide();
	ui.pbEmap->hide();

	m_pConfTree = pVidInf->GetConfTree();
	if (m_pConfTree)
	{
		m_treeLayout->addWidget(m_pConfTree);
		m_pConfTree->show();
		return;
	}

	
	
	m_pCameraTree = pVidInf->GetCameraTree();
	m_pGroupTree = pVidInf->GetGroupTree();
	m_pViewTree = pVidInf->GetViewTree();
	m_pEmapTree = pVidInf->GetEmapTree();
	
	
	if (m_pCameraTree)
	{
		ui.pbCamera->show();
		/* Default is the Camera tree */
		m_treeLayout->addWidget(m_pCameraTree);
		m_pCameraTree->show();
	}
	
	if (m_pGroupTree)
	{
		ui.pbGroup->show();
		m_treeLayout->addWidget(m_pGroupTree);
		m_pGroupTree->hide();
	}

	if (m_pViewTree)
	{
		ui.pbView->show();
		m_treeLayout->addWidget(m_pViewTree);
		m_pViewTree->hide();
	}

	if (m_pEmapTree)
	{
		ui.pbEmap->show();
		m_treeLayout->addWidget(m_pEmapTree);
		m_pEmapTree->hide();
	}
}
