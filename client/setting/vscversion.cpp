#include "vscversion.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCVersion::VSCVersion(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	ui.ver->setText(VE_VERSION);
	ui.info->setText(VE_INFO);
}




void VSCVersion::applyConfig()
{

	return;

}


