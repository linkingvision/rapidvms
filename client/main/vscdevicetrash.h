

#ifndef VSCDEVICE_TRASH_H
#define VSCDEVICE_TRASH_H

#include <QWidget>
#include <QTreeWidget>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QPushButton>
#include <QResizeEvent>
#include <QMimeData>
#include <QDrag>
#include <QAction>

#include "factory.hpp"

class VSCDeviceTrash : public QPushButton
{
    Q_OBJECT
public:
    VSCDeviceTrash(QWidget *parent = 0);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
signals:
    void CameraDeleted();
    //~VSCDeviceTrash();

};

#endif // VSCDEVICE_TRASH_H
