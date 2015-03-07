#include "vscvideosetting.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCVideoSetting::VSCVideoSetting(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	gFactory->GetDefaultHWAccel(m_bHWAccel);

	if (m_bHWAccel  == 1)
	{
	        ui.hwaccel->setChecked(true);
	}else
	{
		ui.hwaccel->setChecked(false);
	}

	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}




void VSCVideoSetting::applyConfig()
{
	if (ui.hwaccel->isChecked() == true)
	{
		m_bHWAccel  = 1;
	}else
	{
		m_bHWAccel  = 0;
	}

	gFactory->SetDefaultHWAccel(m_bHWAccel);

	return;

}


