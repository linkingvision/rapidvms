#include "vscoapi.h"
#include "debug.hpp"
#include "factory.hpp"
#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"

using namespace cppkit;
using namespace std;

extern Factory *gFactory;

VSCOAPI::VSCOAPI(QWidget *parent)
    : QWidget(parent), m_port(9080)
{
	ui.setupUi(this);
	
	gFactory->GetOAPIPort(m_port);

	ck_string ckPort = ck_string::from_uint16(m_port, 10); 

	ui.OAPIPort->setText(ckPort.c_str());

	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}




void VSCOAPI::applyConfig()
{
	ck_string ckPort = ui.OAPIPort->text().toStdString();

	m_port = ckPort.to_uint16(10);

	gFactory->SetOAPIPort(m_port);

	return;

}


