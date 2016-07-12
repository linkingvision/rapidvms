#include "VSCAbout.h"
#include "debug.hpp"


VSCAbout::VSCAbout(QWidget *parent)
    : QDialog(parent)
{
   ui.setupUi(this);

#ifdef WIN32
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::Tool);
#else
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::ToolTip);
#endif

}

void VSCAbout::CancelClicked()
{
}


void VSCAbout::mousePressEvent(QMouseEvent *event)  
{  
    if (event->button() == Qt::LeftButton) {  
        m_Drag = true;  
        m_DragPosition = event->globalPos() - this->pos();  
        event->accept();  
    }  
}  
  
void VSCAbout::mouseMoveEvent(QMouseEvent *event)  
{  
    if (m_Drag && (event->buttons() && Qt::LeftButton)) {  
        move(event->globalPos() - m_DragPosition);  
        event->accept();  
    }  
}  
  
void VSCAbout::mouseReleaseEvent(QMouseEvent *)  
{  
    m_Drag = false;  
} 



