#include "vscminingsetting.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCMiningSetting::VSCMiningSetting(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	gFactory->GetDefaultMining(m_bMining);

	if (m_bMining  == 1)
	{
	        ui.mining->setChecked(true);
	}else
	{
		ui.mining->setChecked(false);
	}

	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}




void VSCMiningSetting::applyConfig()
{
	if (ui.mining->isChecked() == true)
	{
		m_bMining  = 1;
	}else
	{
		m_bMining  = 0;
	}

	gFactory->SetDefaultMining(m_bMining);

	return;

}


