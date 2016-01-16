#include "main/vscvidlistframe.h"

VSCVidListFrame::VSCVidListFrame(QWidget * parent, Qt::WindowFlags f)
: QFrame(parent, f)
{
	
}
VSCVidListFrame::~VSCVidListFrame()
{
	
}

void VSCVidListFrame::mousePressEvent(QMouseEvent *event)
{
	Qt::MouseButtons mouseButtons = event->buttons();
    if( mouseButtons != Qt::LeftButton )
    {
        return;
    }	
	
	emit SignalClicked();
	VDC_DEBUG( "%s mousePressEvent %d\n",__FUNCTION__);
}