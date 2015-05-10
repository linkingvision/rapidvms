#ifndef QT_VSC_V_WIDGET_H
#define QT_VSC_V_WIDGET_H

#include <QWidget>

#define NOMINMAX
#define NOMINMAX 

#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "debug.hpp"
#include "device.hpp"
#include <QTimer>
#include <QMenu>
#include <QMimeData>
using  namespace tthread;

namespace Ui {
class VSCVWidget;
}
class VSCVideoControl;
class VSCPbThread;
class VSCVideoInfo;
#define  VWIDGET_ID_OFFSET 1000 * 1000

class VSCVWidgetProxy;
class VE_LIBRARY_API VSCVWidget : public QWidget
{
	Q_OBJECT

public:
	VSCVWidget(s32 nId, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSCVWidget();
	void mouseDoubleClickEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void wheelEvent ( QWheelEvent * event );
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void contextMenuEvent(QContextMenuEvent* e) ;

    Qt::DropActions supportedDropActions () const
    {
            return Qt::MoveAction;
    }
    void dropEvent(QDropEvent *event);
    void createContentMenu();
    void setId(s32 nId)
    {
        m_nId = nId;
    }
    void SetVideoFocus(BOOL bFocus);

    BOOL SetVideoUpdate(BOOL update);
    BOOL SetVideoUpdateNoPaint(BOOL update);
    BOOL ChangeLayout();
    void DeviceEvent(int deviceId, VscEventType type);
    void PtzAction(int x1, int y1, int x2, int y2);
    void PtzActionStop();
    void VideoSetGeometry();

public slots:
    void stopAction();
    void showDisplay1();
    void showDisplay2();
    void showDisplay3();
    void showDisplay4();
    void Help();
    void videoMouseMove(QMouseEvent *e);
    void UpdateVideoControl();
    void videoResizeEventTimer();
    void InstantPbClick();
    void TimeLineChanged(int nStatus );
    void InstantPbPause();
    void InstantPbPlay();
    void TimeLineLengthChanged(int index);
    void videoResizeEvent();
    void PTZEnable();
    void AutoFocus();
    void LiveDelCallback();

signals:
    void ShowDisplayClicked(int nId);
    void ShowFloatingClicked();
    void ShowTabbedClicked();
    void ShowControlPanelClicked();
    void ShowFocusClicked(int nId);
    void Layout1Clicked(int nId);
    void VideoSwitchWith(int nSrcId, int nDstId);
    void ShowViewClicked(int nId);


public:
	BOOL StartPlay(int nId);
	BOOL StartPlay(int OAPIId, int nId);
	BOOL StartPlayById(int nId);
	BOOL StopPlay(BOOL bForce = FALSE);
	BOOL StartPlayLive();
	BOOL StopPlayLive(BOOL bForce = FALSE);
    
    int GetPlayId();
    void LivePlayControlUI();
    void InstantPbControlUI();
    void InstantPbControlUISimple();
    void StopPlayUI();
    
private:
	HWND m_videoWindow;
	BOOL m_bFocus;
	s32 m_nId;

	BOOL m_pStarted;


	int m_lastTime;
	BOOL m_InstantPbMode;
private:
	VSCVideoControl *m_pVideoControl;
	VSCVideoInfo * m_pVideoInfo;
	QWidget  * m_pVideo;
	
private:
	QTimer *m_Timer;
	QTimer *m_TimerResize;
	time_t m_lastMoveTime;
	struct timeval m_lastPressed;
	
private:
	BOOL m_DragStart;
	s32 m_lastDragX;
	s32 m_lastWidth;
	s32 m_lastHeight;

	BOOL m_PtzStart;
	s32 m_lastPtzX;
	s32 m_lastPtzY;
	BOOL m_PtzEnable;
	struct timeval m_lastPtz;
	struct timeval m_lastPtzZoom;
private:
	VSCVWidgetProxy *m_pProxy;

public:
	Ui::VSCVWidget *p_ui;
	Ui::VSCVWidget &ui;
	
private:
	QAction *m_pStop;
	QAction *m_pHelp;
	QAction *m_pRecord;
	QAction *m_pPTZ;
	QAction *m_pDisplay1;
	QAction *m_pDisplay2;
	QAction *m_pDisplay3;
	QAction *m_pDisplay4;

	QAction *m_pFloating;
	QAction *m_pTabbed;
	QAction *m_pControlEnable;
	QMenu * popupMenu;
    
};

class VE_LIBRARY_API VSCQMimeOAPI : public QMimeData
{
public:
	VSCQMimeOAPI(int OAPIId, int DeviceId)
	: nOAPIId(OAPIId),nDeviceId(DeviceId)
	{
	}
	~VSCQMimeOAPI(){}
public:
	int nDeviceId;/* Remote Camera ID */
	int nOAPIId;/* Remote OpenCVR ID */
	
};

#endif // QT_VSC_V_WIDGET_H
