#include "common/vscvidtreeinf.h"
#include "common/vscviditeminf.h"


VSCVidTreeInf::VSCVidTreeInf(ClientFactory &pFactory, QWidget *parent)
: QTreeWidget(parent), m_pFactory(pFactory), m_bSetChecked(false)
{
	setHeaderHidden(true);
	setIconSize(QSize(20, 20));
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	connect(this, SIGNAL(itemClicked(QTreeWidgetItem * , int )), 
		this, SLOT(SlotItemClicked(QTreeWidgetItem * , int )));
}
VSCVidTreeInf::~VSCVidTreeInf()
{
	
}

void VSCVidTreeInf::SlotItemClicked(QTreeWidgetItem * item, int column)
{
	VDC_DEBUG("%s QTreeWidget %d \n", __FUNCTION__, column);
	/* Unselect all the other items, TODO add a bool to control how to proccess */
	VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf * >(item);
	if (pItem )
	{
		if (pItem->VidGetChecked() == true)
		{
			VidSetCheckedChild(item, false);	
		}
	}
	
}
