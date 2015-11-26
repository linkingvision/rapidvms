#ifndef __VIDEO_IPCGROUP_MAP_H__
#define __VIDEO_IPCGROUP_MAP_H__

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
#include "ui_vscipcgroupmap.h"
#include "factory.hpp"

using  namespace tthread;

class VSCVGroupMap : public QWidget
{
    Q_OBJECT

public:
    VSCVGroupMap(VSCVGroupDataItem &pParam, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~VSCVGroupMap();
public:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
        void dropEvent(QDropEvent *event);
public:
    Ui::VSCVGroupMap ui;
private:
	VSCVGroupDataItem m_pParam;
};


class VSCVGroupWall : public QWidget
{
    Q_OBJECT

public:
    VSCVGroupWall(QWidget *parent = 0);
    ~VSCVGroupWall();
private:
	QGridLayout *m_pLayout;
	QScrollArea *m_pArea;
};

#endif // __VIDEO_IPCGROUP_MAP_H__
