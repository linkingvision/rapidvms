#include "vschdfsrecord.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCHdfsRecord::VSCHdfsRecord(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	//ui.user->setText("admin");
	//ui.passwd->setText("admin");
}




void VSCHdfsRecord::applyConfig()
{

	return;

}


