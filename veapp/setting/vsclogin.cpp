#include "vsclogin.h"
#include "debug.hpp"


VSCLogin::VSCLogin(QWidget *parent)
    : QDialog(parent)
{
   ui.setupUi(this);
   //setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

   //connect(ui.pbSave, SIGNAL(clicked()), this, SLOT(SaveClicked()));
   //connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(CancelClicked()));


}

void VSCLogin::SaveClicked()
{
	//m_Type = VSC_IPCGROUP_CONF_SAVE;
}
void VSCLogin::CancelClicked()
{
	//m_Type = VSC_IPCGROUP_CONF_CANCEL;
}


void VSCLogin::mousePressEvent(QMouseEvent *event)  
{  
    if (event->button() == Qt::LeftButton) {  
        m_Drag = true;  
        m_DragPosition = event->globalPos() - this->pos();  
        event->accept();  
    }  
}  
  
void VSCLogin::mouseMoveEvent(QMouseEvent *event)  
{  
    if (m_Drag && (event->buttons() && Qt::LeftButton)) {  
        move(event->globalPos() - m_DragPosition);  
        event->accept();  
    }  
}  
  
void VSCLogin::mouseReleaseEvent(QMouseEvent *)  
{  
    m_Drag = false;  
} 



