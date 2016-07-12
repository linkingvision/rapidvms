#ifndef __VSC_ABOUT_H__
#define __VSC_ABOUT_H__

#include <QDialog>
#include "ui_vscabout.h"
#include "utility.hpp"
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>

class VSCAbout : public QDialog
{
    Q_OBJECT

public:
    VSCAbout(QWidget *parent = 0);
    ~VSCAbout(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

public slots: 
	void CancelClicked();

public:
	Ui::VSCAbout ui;
private:
	QPoint m_DragPosition;
	bool m_Drag;
	
};

#endif // __VSC_USER_STATUS_H__
