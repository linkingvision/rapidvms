#ifndef __VIDEO_PANEL_H__
#define __VIDEO_PANEL_H__

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QMimeData>
#include <QDrag>

#define NOMINMAX
#define NOMINMAX 

#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "ui_vscpanel.h"
#include "factory.hpp"
#include "vtaskmgr.hpp"

using  namespace tthread;

typedef std::map<std::string, QWidget *> VTaskItemWidgetList;

class VSCPanel : public QWidget
{
    Q_OBJECT

public:
	VSCPanel(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSCPanel();

signals:
	void AddRecorder();
	void AddCamera();
	
	void AddSurveillance();
	void Search();
	void AddEmap();
	void AddDmining();
	
	void Setting();
	void AddEvent();
	void AddVIPC();

public slots:
	void UpdateTaskList();
public:
    	Ui::VSCPanel ui;
private:
	VTaskItemList m_pTaskList;
	VTaskItemWidgetList m_pTaskWidget;

};


#endif // __VIDEO_PANEL_H__
