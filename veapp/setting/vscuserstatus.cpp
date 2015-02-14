#include "vscuserstatus.h"
#include "debug.hpp"


VSCUserStatus::VSCUserStatus(QWidget *parent)
    : QDialog(parent), m_quit(FALSE)
{
   ui.setupUi(this);
   //setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

   connect(ui.pbLogout, SIGNAL(clicked()), this, SLOT(QuitClicked()));
   //connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(CancelClicked()));
#ifdef WIN32
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::Tool);
#else
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::ToolTip);
#endif

}

void VSCUserStatus::QuitClicked()
{
	m_quit = TRUE;
}
void VSCUserStatus::CancelClicked()
{
}


void VSCUserStatus::mousePressEvent(QMouseEvent *event)  
{  
    if (event->button() == Qt::LeftButton) {  
        m_Drag = true;  
        m_DragPosition = event->globalPos() - this->pos();  
        event->accept();  
    }  
}  
  
void VSCUserStatus::mouseMoveEvent(QMouseEvent *event)  
{  
    if (m_Drag && (event->buttons() && Qt::LeftButton)) {  
        move(event->globalPos() - m_DragPosition);  
        event->accept();  
    }  
}  
  
void VSCUserStatus::mouseReleaseEvent(QMouseEvent *)  
{  
    m_Drag = false;  
} 



