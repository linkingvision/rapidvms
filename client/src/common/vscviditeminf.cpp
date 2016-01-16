#include "common/vscviditeminf.h"

VSCVidItemInf::VSCVidItemInf(ClientFactory &pFactory, QTreeWidgetItem *parent)
: QTreeWidgetItem(parent), m_pFactory(pFactory)
{
	UpdateOnline(false);
}
VSCVidItemInf::~VSCVidItemInf()
{
	
}
void VSCVidItemInf::UpdateOnline(bool bOnline)
{
	if (bOnline == TRUE)
	{
	    this->setTextColor(0, QColor(0, 170, 0));
	}else
	{
	    this->setTextColor(0, QColor(194, 194, 194));
	}
}