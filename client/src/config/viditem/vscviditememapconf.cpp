#include "config/viditem/vscviditememapconf.h"

VSCVidItemEmapConf::VSCVidItemEmapConf(ClientFactory &pFactory, QTreeWidgetItem *parent)
: VSCVidItemInf(pFactory, parent)
{
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/action/resources/map.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	setIcon(0, icon1);
	setText(0, QApplication::translate(" ", "Emap",0));	
	this->setTextColor(0, QColor(41, 42, 43));
	//setHidden(true);
}
VSCVidItemEmapConf::~VSCVidItemEmapConf()
{
	
}
