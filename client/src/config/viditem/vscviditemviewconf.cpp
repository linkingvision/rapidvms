#include "config/viditem/vscviditemviewconf.h"

VSCVidItemViewConf::VSCVidItemViewConf(ClientFactory &pFactory, QTreeWidgetItem *parent)
: VSCVidItemInf(pFactory, parent)
{
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/device/resources/view.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	setIcon(0, icon1);
	setText(0, QApplication::translate(" ", "View",0));	
	this->setTextColor(0, QColor(41, 42, 43));
	//setHidden(true);
}
VSCVidItemViewConf::~VSCVidItemViewConf()
{
	
}
