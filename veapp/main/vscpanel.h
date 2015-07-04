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

using  namespace tthread;

class VSCPanel : public QWidget
{
    Q_OBJECT

public:
    VSCPanel(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~VSCPanel();
public:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
        void dropEvent(QDropEvent *event);
public:
    Ui::VSCPanel ui;
private:
	VSCVGroupDataItem m_pParam;
};


#endif // __VIDEO_PANEL_H__
