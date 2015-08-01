#include "vscrtsprtp.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"

using namespace cppkit;
using namespace std;

extern Factory *gFactory;

VSCRtspRtp::VSCRtspRtp(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	
	gFactory->GetRTSPServer(m_pAuth, m_pMultiAddr, m_pPort);

	if (m_pAuth  == 1)
	{
	        ui.auth->setChecked(true);
	}else
	{
		ui.auth->setChecked(false);
	}
	ck_string ckPort = ck_string::from_uint16(m_pPort, 10); 

	ui.rtspPort->setText(ckPort.c_str());

	//ui.multiCastStart->setText(m_pMultiAddr.c_str());

	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}




void VSCRtspRtp::applyConfig()
{

	ck_string ckPort = ui.rtspPort->text().toStdString();

	m_pPort = ckPort.to_uint16(10);

	if (ui.auth->isChecked() == true)
	{
		m_pAuth  = 1;
	}else
	{
		m_pAuth  = 0;
	}

	//m_pMultiAddr = ui.multiCastStart->text().toStdString();

	gFactory->SetRTSPServer(m_pAuth, m_pMultiAddr, m_pPort);
	return;

}


