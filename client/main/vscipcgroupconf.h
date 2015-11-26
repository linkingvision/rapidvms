#ifndef __VSC_IPCGROUP_CONF_H__
#define __VSC_IPCGROUP_CONF_H__

#include <QDialog>
#include "ui_vscipcgroupconf.h"
#include "utility.hpp"
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>

typedef enum
{
    VSC_IPCGROUP_CONF_SAVE = 1,
    VSC_IPCGROUP_CONF_CANCEL,
    VSC_IPCGROUP_CONF_LAST
} VSCIPCGroupConfType;

class VSCIPCGroupConf : public QDialog
{
    Q_OBJECT

public:
    VSCIPCGroupConf(QWidget *parent = 0);
    ~VSCIPCGroupConf(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
public:
	VSCIPCGroupConfType GetType()
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
	void SaveClicked();
	void CancelClicked();

public:
	Ui::VSCIPCGroupConf ui;
	
private:
    VSCIPCGroupConfType m_Type;
private:
	QPoint m_DragPosition;
	bool m_Drag;
    
};

#endif // __VSC_IPCGROUP_CONF_H__
