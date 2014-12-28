#ifndef __VSC_VIEW_CONF_H__
#define __VSC_VIEW_CONF_H__

#include <QDialog>
#include "ui_vscviewconf.h"
#include "utility.hpp"
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>

typedef enum
{
    VSC_VIEW_CONF_NEW = 1,
    VSC_VIEW_CONF_MODIFY,
    VSC_VIEW_CONF_LAST
} VSCViewConfType;

class VSCViewConf : public QDialog
{
    Q_OBJECT

public:
    VSCViewConf(QWidget *parent = 0);
    ~VSCViewConf(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
public:
	VSCViewConfType GetType()
	{
		return m_Type;
	}
	bool SetName(string strName)
	{
		ui.lineEditName->setText(strName.c_str());
		return true;
	}
	bool GetName(string &strName)
	{
	       strName = ui.lineEditName->text().toStdString().c_str();
		return true;
		   	
	}

public slots: 
	void NewClicked();
	void ModifyClicked();

public:
	Ui::VSCViewConf ui;
	
private:
    VSCViewConfType m_Type;
private:
	QPoint m_DragPosition;
	bool m_Drag;

    
};

#endif // __VSC_VIEW_CONF_H__
