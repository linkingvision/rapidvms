#include "vscuser.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCUser::VSCUser(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	ui.user->setText("admin");
	ui.passwd->setText("admin");
}




void VSCUser::applyConfig()
{

	return;

}


