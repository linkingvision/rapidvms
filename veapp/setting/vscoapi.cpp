#include "vscoapi.h"
#include "debug.hpp"
#include "factory.hpp"
#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"

using namespace cppkit;
using namespace std;

extern Factory *gFactory;

VSCOAPI::VSCOAPI(QWidget *parent)
    : QWidget(parent), m_port(9080), m_HLSPort(9001), 
    m_HTTPPort(9000)
{
	ui.setupUi(this);
	
	gFactory->GetOAPIPort(m_port);
	gFactory->GetVHTTPSPort(m_HTTPPort);
	gFactory->GetVHLSSPort(m_HLSPort);
	gFactory->GetVHTTPSSSLPort(m_HTTPSSLPort);

	ck_string ckPort = ck_string::from_uint16(m_port, 10); 
	ck_string ckHTTPPort = ck_string::from_uint16(m_HTTPPort, 10); 
	ck_string ckHLSPort = ck_string::from_uint16(m_HLSPort, 10); 
	ck_string ckHTTPSSLPort = ck_string::from_uint16(m_HTTPSSLPort, 10); 

	ui.OAPIPort->setText(ckPort.c_str());
	ui.HTTPPort->setText(ckHTTPPort.c_str());
	ui.HLSPort->setText(ckHLSPort.c_str());

	ui.HTTPSSLPort->setText(ckHTTPSSLPort.c_str());

	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}




void VSCOAPI::applyConfig()
{
	ck_string ckPort = ui.OAPIPort->text().toStdString();
	ck_string ckHTTPPort = ui.HTTPPort->text().toStdString();
	ck_string ckHLSPort = ui.HLSPort->text().toStdString();
	ck_string ckHTTPSSLPort = ui.HTTPSSLPort->text().toStdString();

	m_port = ckPort.to_uint16(10);
	m_HTTPPort = ckHTTPPort.to_uint16(10);
	m_HLSPort = ckHLSPort.to_uint16(10);
	m_HTTPSSLPort = ckHTTPSSLPort.to_uint16(10);

	gFactory->SetOAPIPort(m_port);
	gFactory->SetVHTTPSPort(m_HTTPPort);
	gFactory->SetVHLSSPort(m_HLSPort);
	gFactory->SetVHTTPSSSLPort(m_HTTPSSLPort);

	return;

}


