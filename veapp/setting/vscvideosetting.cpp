#include "vscvideosetting.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCVideoSetting::VSCVideoSetting(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	//ui.user->setText("admin");
	//ui.passwd->setText("admin");
}




void VSCVideoSetting::applyConfig()
{

	return;

}


