#ifndef __VSC_EVENT_CONSOLE_H__
#define __VSC_EVENT_CONSOLE_H__

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
#include "ui_vsceventconsole.h"

using  namespace tthread;

class VSCEventConsole : public QWidget
{
    Q_OBJECT

public:
	VSCEventConsole(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSCEventConsole();

public slots:
	//void UpdateTaskList();
public:
    	Ui::VSCEventConsole ui;
private:

};


#endif // __VSC_EVENT_CONSOLE_H__
