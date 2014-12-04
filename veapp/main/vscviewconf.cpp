#include "vscviewconf.h"
#include "hdddevice.hpp"
#include "debug.hpp"


VSCViewConf::VSCViewConf(QWidget *parent)
    : QDialog(parent), m_Type(VSC_VIEW_CONF_LAST)
{
   ui.setupUi(this);
   connect(ui.pbNew, SIGNAL(clicked()), this, SLOT(NewClicked()));
   connect(ui.pbModify, SIGNAL(clicked()), this, SLOT(ModifyClicked()));

}

void VSCViewConf::NewClicked()
{
	m_Type = VSC_VIEW_CONF_NEW;
}
void VSCViewConf::ModifyClicked()
{
	m_Type = VSC_VIEW_CONF_MODIFY;
}



