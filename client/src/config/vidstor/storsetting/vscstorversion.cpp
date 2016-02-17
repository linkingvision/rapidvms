#include "config/vidstor/storsetting/vscstorversion.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include <QDesktopWidget>
#include "client/storsyncinf.hpp"



VSCStorVersion::VSCStorVersion(ClientFactory &pFactory, VidStor &stor, QWidget *parent)
    : m_pFactory(pFactory), m_pStor(stor), QWidget(parent)
{
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;
	syncInf.Connect();
	syncInf.GetVer(pVer, strInfo);
	
	ui.setupUi(this);
	ui.ver->setText(pVer.c_str());
	ui.info->setText(strInfo.c_str());

	delete pLoading;
}




void VSCStorVersion::applyConfig()
{

	return;

}


