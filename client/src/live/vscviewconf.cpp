#include "vscviewconf.h"
#include "debug.hpp"


VSCViewConf::VSCViewConf(QWidget *parent)
    : QDialog(parent), m_Type(VSC_VIEW_CONF_LAST)
{
   ui.setupUi(this);
   connect(ui.pbNew, SIGNAL(clicked()), this, SLOT(NewClicked()));
   connect(ui.pbModify, SIGNAL(clicked()), this, SLOT(ModifyClicked()));
#ifdef WIN32
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::Tool);
#else
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::ToolTip);
#endif

}

void VSCViewConf::NewClicked()
{
	m_Type = VSC_VIEW_CONF_NEW;
}
void VSCViewConf::ModifyClicked()
{
	m_Type = VSC_VIEW_CONF_MODIFY;
}

void VSCViewConf::mousePressEvent(QMouseEvent *event)  
{  
    if (event->button() == Qt::LeftButton) {  
        m_Drag = true;  
        m_DragPosition = event->globalPos() - this->pos();  
        event->accept();  
    }  
}  
  
void VSCViewConf::mouseMoveEvent(QMouseEvent *event)  
{  
    if (m_Drag && (event->buttons() && Qt::LeftButton)) {  
        move(event->globalPos() - m_DragPosition);  
        event->accept();  
    }  
}  
  
void VSCViewConf::mouseReleaseEvent(QMouseEvent *)  
{  
    m_Drag = false;  
} 



