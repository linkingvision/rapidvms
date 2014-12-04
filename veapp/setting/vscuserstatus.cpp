#include "vscuserstatus.h"
#include "debug.hpp"


VSCUserStatus::VSCUserStatus(QWidget *parent)
    : QDialog(parent)
{
   ui.setupUi(this);
   //setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

   //connect(ui.pbSave, SIGNAL(clicked()), this, SLOT(SaveClicked()));
   //connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(CancelClicked()));

}

void VSCUserStatus::SaveClicked()
{
	//m_Type = VSC_IPCGROUP_CONF_SAVE;
}
void VSCUserStatus::CancelClicked()
{
	//m_Type = VSC_IPCGROUP_CONF_CANCEL;
}



