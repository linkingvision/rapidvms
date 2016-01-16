#include "config/viditem/vscviditemaddcam.h"

VSCVidItemAddCam::VSCVidItemAddCam(VidStor cStor, ClientFactory &pFactory, QTreeWidgetItem *parent)
: VSCVidItemInf(pFactory, parent), m_cStor(cStor)
{
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/device/resources/addcam.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	setIcon(0, icon1);
	setText(0, QApplication::translate(" ", "Camera Add",0));	
	this->setTextColor(0, QColor(41, 42, 43));

}
VSCVidItemAddCam::~VSCVidItemAddCam()
{
	
}
