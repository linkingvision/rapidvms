#include "vsclanguage.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCLanguage::VSCLanguage(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	gFactory->GetLang(m_lang);
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
	SetupValue();
}
void VSCLanguage::SetupValue()
{
    switch(m_lang)
    {
	case VSC_LANG_AUTO:
        ui.radioButtonAuto->setChecked(true);
        break;
	case VSC_LANG_EN:
        ui.radioButtonEN->setChecked(true);
        break;
	case VSC_LANG_ZH:
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
		m_lang = VSC_LANG_AUTO;
	}
	else if (ui.radioButtonEN->isChecked() == true)
	{
		m_lang = VSC_LANG_EN;
	}
	else if (ui.radioButtonZH->isChecked() == true)
	{
		m_lang = VSC_LANG_ZH;
	}

	gFactory->SetLang(m_lang);
	return;

}


