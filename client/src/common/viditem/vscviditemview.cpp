#include "common/viditem/vscviditemview.h"

VSCVidItemView::VSCVidItemView(VidView cView, ClientFactory &pFactory, QTreeWidgetItem *parent)
: m_cView(cView), VSCVidItemInf(pFactory, parent)
{
	QIcon icon1;
	VideoWallLayoutMode nMode = (VideoWallLayoutMode)(m_cView.clayout());

	switch(nMode)
	{
	    case LAYOUT_MODE_2X2:
	        icon1.addFile(QStringLiteral(":/view/resources/2x2.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);
	        break;
	    case LAYOUT_MODE_3X3:
	        icon1.addFile(QStringLiteral(":/view/resources/3x3.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);
	        break;
	    case LAYOUT_MODE_4X4:
	        icon1.addFile(QStringLiteral(":/view/resources/4x4.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);
	        break;
	    case LAYOUT_MODE_1:
	        icon1.addFile(QStringLiteral(":/view/resources/1x1.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);
	        break;
	    case LAYOUT_MODE_6:
	        icon1.addFile(QStringLiteral(":/view/resources/6.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);
	        break;
	    case LAYOUT_MODE_12p1:
	        icon1.addFile(QStringLiteral(":/view/resources/12+1.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);
	        break;
	    case LAYOUT_MODE_5x5:
	        icon1.addFile(QStringLiteral(":/view/resources/5x5.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);
	        break;
	    case LAYOUT_MODE_6x6:
	        icon1.addFile(QStringLiteral(":/view/resources/6x6.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);
	        break;
	    case LAYOUT_MODE_8x8:
	        icon1.addFile(QStringLiteral(":/view/resources/8x8.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);
	        break;
	    case LAYOUT_MODE_1x3:
	        icon1.addFile(QStringLiteral(":/view/resources/1x3.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);
	        break;
	    case LAYOUT_MODE_1p6:
	        icon1.addFile(QStringLiteral(":/view/resources/1p6.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);
	        break;
	    case LAYOUT_MODE_ONE:
	        break;
	    default:
	        break;
	}

	setIcon(0, icon1);

	setText(0, QApplication::translate(" ", m_cView.strname().c_str(), 0));
	this->setTextColor(0, QColor(41, 42, 43));
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