#include "config/viditem/vscviditemclientconf.h"

VSCVidItemClientConf::VSCVidItemClientConf(ClientFactory &pFactory, QTreeWidgetItem *parent)
:VSCVidItemInf(pFactory, parent)
{
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/action/resources/control_panel.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	setIcon(0, icon1);

	setText(0, QApplication::translate(" ", "Client", 0));	
	this->setTextColor(0, QColor(41, 42, 43));
	//setHidden(true);
}
VSCVidItemClientConf::~VSCVidItemClientConf()
{
	
}