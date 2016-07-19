#include "common/vscvidtreeinf.h"

VSCVidTreeInf::VSCVidTreeInf(ClientFactory &pFactory, QWidget *parent)
: QTreeWidget(parent), m_pFactory(pFactory)
{
	setHeaderHidden(true);
	setIconSize(QSize(20, 20));
	setSelectionMode(QAbstractItemView::ExtendedSelection);
}
VSCVidTreeInf::~VSCVidTreeInf()
{
	
}