

#ifndef __VSC_RECORDER_PANEL_H__
#define __VSC_RECORDER_PANEL_H__

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
#include "vscrecorder.h"

class VSCRecorderPanel : public QPushButton
{
    Q_OBJECT
public:
    VSCRecorderPanel(QWidget *parent = 0);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	/* This must be called after new this object */
	BOOL SetRecorder(VSCRecorder *pRecorder)
	{
		m_Recorder = pRecorder;
		return TRUE;
	}
private:
	VSCRecorder *m_Recorder;

};

#endif // VSCDEVICE_TRASH_H
