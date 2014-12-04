#include "vsclicense.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCLicense::VSCLicense(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	UpdateUI();
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));

}

void VSCLicense::UpdateUI()
{
	astring strLicense = "";
	astring strHostId = ""; 
	char channel[256];
	int ch = 0;
	int type = 0;
	gFactory->GetLicense(strLicense, strHostId, ch, type);
	memset(channel, 0, 256);
	sprintf(channel, "%d", ch);
	ui.lic->setText(strLicense.c_str());
	ui.hostId->setText(strHostId.c_str());
	ui.channel->setText(channel);

	return;
}



void VSCLicense::applyConfig()
{
	std::string strLicense = ui.lic->toPlainText().toStdString();
	gFactory->SetLicense(strLicense);
	UpdateUI();
	return;

}


