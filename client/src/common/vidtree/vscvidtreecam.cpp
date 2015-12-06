#include "common/vidtree/vscvidtreecam.h"
#include "common/viditem/vscviditemcam.h"
#include "common/viditem/vscviditemvidstor.h"

VSCVidTreeCam::VSCVidTreeCam(ClientFactory &pFactory, QWidget *parent)
: VSCVidTreeInf(pFactory, parent)
{
	m_pRoot = new  VSCVidItemCam((QTreeWidgetItem *)this);
	
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/device/resources/camera.png"), QSize(), QIcon::Normal, QIcon::Off);
	
    m_pRoot->setIcon(0, icon1);

    m_pRoot->setText(0, QApplication::translate("Camera",
            "Camera", 0));
	
	addTopLevelItem(m_pRoot);
	m_pRoot->setExpanded(true);
	
	TreeUpdate();
	
	/* Register the device callback TODO */
	
}
VSCVidTreeCam::~VSCVidTreeCam()
{
	
}

void VSCVidTreeCam::TreeUpdate()
{
	/* Delete all the child */
	qDeleteAll(m_pRoot->takeChildren());
	VidStorList storList;
	m_pFactory.GetConfDB().GetStorListConf(storList);
	int storSize = storList.cvidstor_size();
	
	for (s32 i = 0; i < storList.cvidstor_size(); i ++)
	{
		VidStor pStor = storList.cvidstor(i);
		VSCVidItemVidStor *pItemStor = new  VSCVidItemVidStor(m_pRoot);
		
		QIcon icon1;
		icon1.addFile(QStringLiteral(":/device/resources/computer.png"), QSize(), QIcon::Normal, QIcon::Off);
		
		pItemStor->setIcon(0, icon1);

		pItemStor->setText(0, QApplication::translate(" ",
			pStor.strname().c_str(), 0));
	}
	
}