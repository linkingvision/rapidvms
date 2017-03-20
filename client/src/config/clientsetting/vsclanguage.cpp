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
#include "debug.hpp"
#include "config/clientsetting/vsclanguage.h"

VSCLanguage::VSCLanguage(ClientFactory &pFactory, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory)
{
	ui.setupUi(this);

	m_pFactory.GetLang(m_nLang);
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
	SetupValue();
}
void VSCLanguage::SetupValue()
{
	switch (m_nLang)
    {
	case VID_LANG_AUTO:
        ui.radioButtonAuto->setChecked(true);
        break;
	case VID_EN:
        ui.radioButtonEN->setChecked(true);
        break;
	case VID_ZH_CN:
        ui.radioButtonZH->setChecked(true);
        break;
    default:
        return;
    }
}

void VSCLanguage::applyConfig()
{
	if (ui.radioButtonAuto->isChecked() == true)
	{
		m_nLang = VID_LANG_AUTO;
	}
	else if (ui.radioButtonEN->isChecked() == true)
	{
		m_nLang = VID_EN;
	}
	else if (ui.radioButtonZH->isChecked() == true)
	{
		m_nLang = VID_ZH_CN;
	}

	m_pFactory.SetLang(m_nLang);
	return;

}


