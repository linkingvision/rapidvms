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
#include "vmotconf.hpp"
#include "ui_vmotconf.h"
#include "recordwrapper.hpp"
#include <QRubberBand>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>

class VMotConfPri
{
public:
	VMotConfPri();
	~VMotConfPri();
public:
};

VMotConfPri::VMotConfPri()
{
}
VMotConfPri::~VMotConfPri()
{
}

VMotConf::VMotConf(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags), p_ui(new Ui::VMotConf), ui(*p_ui), 
	m_pPri(NULL)
{
	m_pPri = new VMotConfPri();
	ui.setupUi(this);


	connect(ui.pbOff, SIGNAL(clicked()), this, SLOT(SlotMotOff()));
	connect(ui.pbOn, SIGNAL(clicked()), this, SLOT(SlotMotOn()));
	connect(ui.pbAll, SIGNAL(clicked()), this, SLOT(SlotMotAll()));
	connect(ui.pbClear, SIGNAL(clicked()), this, SLOT(SlotMotClear()));
}

VMotConf::~VMotConf()
{
	if (m_pPri)
	{
		delete m_pPri;
		m_pPri = NULL;
	}
	
}

bool VMotConf::GetCurrentMot(VVidMotReg &pMot)
{	
	ui.widget->GetCurrentMot(pMot);
	return true;
}

bool VMotConf::SetCurrentMot(VVidMotReg &pMot)
{
	ui.widget->SetCurrentMot(pMot);
	return true;
}

bool VMotConf::UpdateImage(astring strUrl)
{
	ui.widget->UpdateImage(strUrl);
	return true;
}

void VMotConf::SlotMotOff()
{
	ui.widget->UpdateMotType(false);
}
void VMotConf::SlotMotOn()
{
	ui.widget->UpdateMotType(true);
}

void VMotConf::SlotMotAll()
{
	ui.widget->SetMotAll();
}

void VMotConf::SlotMotClear()
{
	ui.widget->SetMotClear();
}





