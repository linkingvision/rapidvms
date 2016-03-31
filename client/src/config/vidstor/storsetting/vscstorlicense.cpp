#include "debug.hpp"
#include "config/vidstor/storsetting/vscstorlicense.h"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"

VSCStorLicense::VSCStorLicense(ClientFactory &pFactory, VidStor &stor, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory), m_pStor(stor)
{
	ui.setupUi(this);
	UpdateUI();
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));

}

void VSCStorLicense::UpdateUI()
{
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;
	syncInf.Connect();
	
	astring strLicense = "";
	astring strHostId = "";
	astring strExpire = "";
	char channel[256];
	int ch = 0;
	astring type = " ";
	syncInf.GetLic(strLicense, strHostId, ch, type, strExpire);
	memset(channel, 0, 256);
	sprintf(channel, "%d", ch);
	ui.lic->setText(strLicense.c_str());
	ui.hostId->setText(strHostId.c_str());
	ui.channel->setText(channel);
	ui.type->setText(type.c_str());
	
	delete pLoading;

	return;
}



void VSCStorLicense::applyConfig()
{
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;
	syncInf.Connect();
	
	std::string strLicense = ui.lic->toPlainText().toStdString();
	syncInf.ConfLic(strLicense);
	UpdateUI();
	if (pLoading)
	{
		delete pLoading;
		pLoading = NULL;
	}
	return;

}


