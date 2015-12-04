
#include "main/vscvidlist.h"
#include "server/factory.hpp"


VSCVidList::VSCVidList(QWidget *parent)
    : QWidget(parent), m_pVidInf(NULL)
{
	ui.setupUi(this);

	m_treeLayout = new QVBoxLayout();

	//m_treeLayout->addWidget(m_pVideo);
	m_treeLayout->setMargin(0);

	ui.ListWidget->setLayout(m_treeLayout);
}
VSCVidList::~VSCVidList()
{

}

void VSCVidList::SetCurrVidInf(VSCVidInf *pVidInf)
{
}