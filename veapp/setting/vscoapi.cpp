#include "vscoapi.h"
#include "debug.hpp"
#include "factory.hpp"

extern Factory *gFactory;

VSCOAPI::VSCOAPI(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	//ui.user->setText("admin");
	//ui.passwd->setText("admin");
}




void VSCOAPI::applyConfig()
{

	return;

}


