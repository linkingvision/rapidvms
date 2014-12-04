#include "vscrecorderone.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCRecorderOne::VSCRecorderOne(VSCRecorder &recorder, QWidget *parent)
    : QWidget(parent), m_Recorder(recorder)
{
	ui.setupUi(this);//yuanen
	ui.lineEditIP->setText(recorder.m_Ip);
	ui.lineEditName->setText(QString("%1").arg(recorder.m_Id));		
	ui.pushButton->SetRecorder(&m_Recorder);
}




void VSCRecorderOne::applyConfig()
{

	return;

}
