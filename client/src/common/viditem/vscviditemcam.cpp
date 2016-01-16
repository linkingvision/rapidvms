#include "common/viditem/vscviditemcam.h"

VSCVidItemCam::VSCVidItemCam(VidCamera cCam, ClientFactory &pFactory, QTreeWidgetItem *parent)
: m_cCam(cCam), VSCVidItemInf(pFactory, parent)
{
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/device/resources/camera.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	setIcon(0, icon1);

	setText(0, QApplication::translate(" ", m_cCam.strname().c_str(), 0));
	//setHidden(true);
}
VSCVidItemCam::~VSCVidItemCam()
{
	
}

void VSCVidItemCam::VidFilter(astring strFilter)
{
	if (strFilter.size() == 0)
	{
		setHidden(false);
		return;
	}
	std::size_t found = m_cCam.strname().find(strFilter);
	if (found != std::string::npos)
	{
		setHidden(false);
	}else
	{
		setHidden(true);
	}
}