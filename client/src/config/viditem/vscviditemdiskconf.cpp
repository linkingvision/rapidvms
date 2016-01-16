#include "config/viditem/vscviditemdiskconf.h"

VSCVidItemDiskConf::VSCVidItemDiskConf(VidStor cStor, ClientFactory &pFactory, QTreeWidgetItem *parent)
:VSCVidItemInf(pFactory, parent), m_cStor(cStor)
{
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/action/resources/disksetting.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	setIcon(0, icon1);

	setText(0, QApplication::translate(" ", "Disk", 0));
	this->setTextColor(0, QColor(41, 42, 43));
	//setHidden(true);
}
VSCVidItemDiskConf::~VSCVidItemDiskConf()
{
	
}