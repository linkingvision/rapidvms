#include "config/clientsetting/vscversion.h"
#include "debug.hpp"


VSCVersion::VSCVersion(ClientFactory &pFactory, QWidget *parent)
    : m_pFactory(pFactory), QWidget(parent)
{
	
	ui.setupUi(this);
	ui.ver->setText(VE_VERSION);
	ui.info->setText(VE_INFO);
}




void VSCVersion::applyConfig()
{

	return;

}


