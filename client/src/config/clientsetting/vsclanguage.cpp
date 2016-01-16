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


