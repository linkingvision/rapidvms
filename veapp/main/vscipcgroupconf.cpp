#include "vscipcgroupconf.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"


VSCIPCGroupConf::VSCIPCGroupConf(QWidget *parent)
    : QDialog(parent), m_Type(VSC_IPCGROUP_CONF_LAST)
{
   ui.setupUi(this);
   connect(ui.pbSave, SIGNAL(clicked()), this, SLOT(SaveClicked()));
   connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(CancelClicked()));

}

void VSCIPCGroupConf::SaveClicked()
{
	m_Type = VSC_IPCGROUP_CONF_SAVE;
}
void VSCIPCGroupConf::CancelClicked()
{
	m_Type = VSC_IPCGROUP_CONF_CANCEL;
}



