#ifndef __VSC_VID_LIST_FRAME_H__
#define __VSC_VID_LIST_FRAME_H__

#include <QFrame>
#include <QMouseEvent>
#include "common/vscvidinf.h"

class VSCVidListFrame : public QFrame
{
    Q_OBJECT
public:
    VSCVidListFrame(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~VSCVidListFrame();
signals:
	void SignalClicked();	
public:
	void mousePressEvent(QMouseEvent *event);
private:
};

#endif
	