#include "debug.hpp"
#include "config/clientsetting/vsclicense.h"

VSCLicense::VSCLicense(ClientFactory &pFactory, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory)
{
	ui.setupUi(this);
	UpdateUI();
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));

}

void VSCLicense::UpdateUI()
{
	astring strLicense = "";
	astring strHostId = "";
	astring strExpire = "";
	char channel[256];
	int ch = 0;
	astring type = " ";
	m_pFactory.GetLicense(strLicense, strHostId, ch, type, strExpire);
	memset(channel, 0, 256);
	sprintf(channel, "%d", ch);
	ui.lic->setText(strLicense.c_str());
	ui.hostId->setText(strHostId.c_str());
	ui.channel->setText(channel);
	ui.type->setText(type.c_str());

	return;
}



void VSCLicense::applyConfig()
{
	std::string strLicense = ui.lic->toPlainText().toStdString();
	m_pFactory.SetLicense(strLicense);
	UpdateUI();
	return;

}


