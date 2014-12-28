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
#ifdef WIN32
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::Tool);
#else
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::ToolTip);
#endif
}

void VSCIPCGroupConf::SaveClicked()
{
	m_Type = VSC_IPCGROUP_CONF_SAVE;
}
void VSCIPCGroupConf::CancelClicked()
{
	m_Type = VSC_IPCGROUP_CONF_CANCEL;
}

	
void VSCIPCGroupConf::mousePressEvent(QMouseEvent *event)  
{  
    if (event->button() == Qt::LeftButton) {  
        m_Drag = true;  
        m_DragPosition = event->globalPos() - this->pos();  
        event->accept();  
    }  
}  
  
void VSCIPCGroupConf::mouseMoveEvent(QMouseEvent *event)  
{  
    if (m_Drag && (event->buttons() && Qt::LeftButton)) {  
        move(event->globalPos() - m_DragPosition);  
        event->accept();  
    }  
}  
  
void VSCIPCGroupConf::mouseReleaseEvent(QMouseEvent *)  
{  
    m_Drag = false;  
} 



