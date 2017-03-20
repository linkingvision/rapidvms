/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
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


