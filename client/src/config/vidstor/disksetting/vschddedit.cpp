#include "config/vidstor/disksetting/vschddedit.h"
#include "debug.hpp"
#include "config/vidstor/disksetting/vschddoneedit.h"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"

VSCHddEdit::VSCHddEdit(VidStor &stor, QWidget *parent)
    : QWidget(parent), m_pStor(stor)
{
	ui.setupUi(this);
	ui.storName->setText(m_pStor.strname().c_str());
	ui.scrollArea->setBackgroundRole(QPalette::Light);
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;
	syncInf.Connect();

	VidDiskList sysDiskList = syncInf.GetSysVidDiskList();

	VidDiskList diskList = syncInf.GetVidDiskList();

	for (s32 i = 0; i < sysDiskList.cviddisk_size(); i ++)
	{
		VidDisk disk = sysDiskList.cviddisk(i);
		m_Disks[i] = new VSCHddOneEdit(m_pStor, disk, diskList, this);
		ui.verticalLayout->addWidget(m_Disks[i] );

	}
	delete pLoading;
}



