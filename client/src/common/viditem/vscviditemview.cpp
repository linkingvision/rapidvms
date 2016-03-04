#include "common/viditem/vscviditemview.h"

VSCVidItemView::VSCVidItemView(VidView cView, ClientFactory &pFactory, QTreeWidgetItem *parent)
: m_cView(cView), VSCVidItemInf(pFactory, parent)
{
	QIcon icon1;
	//TODO
	icon1.addFile(QStringLiteral(":/view/resources/3x3.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	setIcon(0, icon1);

	setText(0, QApplication::translate(" ", m_cView.strname().c_str(), 0));
	//setHidden(true);
}
VSCVidItemView::~VSCVidItemView()
{
	
}

void VSCVidItemView::VidFilter(astring strFilter)
{
	VDC_DEBUG( "%s %d Filter %s name %s\n",__FUNCTION__, __LINE__, 
		strFilter.c_str(), m_cView.strname().c_str());
	if (strFilter.size() == 0)
	{
		setHidden(false);
		return;
	}
	std::size_t found = m_cView.strname().find(strFilter);
	if (found != std::string::npos)
	{
		setHidden(false);
	}else
	{
		setHidden(true);
	}
}