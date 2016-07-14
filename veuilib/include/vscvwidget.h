#ifndef QT_VSC_V_WIDGET_H
#define QT_VSC_V_WIDGET_H

#include <QWidget>

#define NOMINMAX
#define NOMINMAX 

#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "debug.hpp"
#include "server/factory.hpp"
#include "client/clientfactory.hpp"
#include "client/storstream.hpp"
#include <QTimer>
#include <QMenu>
#include <QMimeData>
using  namespace tthread;

namespace Ui {
class VSCVWidget;
}

class VSCPTZControl;
class VSCVideoInfo;

class VE_LIBRARY_API VSCVWidget : public QWidget
{
	Q_OBJECT

public:
	VSCVWidget(ClientFactory &pFactory, s32 nId, QWidget *parent = 0, bool bSingle = false, Qt::WindowFlags flags = 0);
	~VSCVWidget();
	void mouseDoubleClickEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void wheelEvent ( QWheelEvent * event );
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void contextMenuEvent(QContextMenuEvent* e) ;
	void ReCreateVideoWidget();

    Qt::DropActions supportedDropActions () const
    {
            return Qt::MoveAction;
    }
    void dropEvent(QDropEvent *event);
    void createContentMenu();
    void SetVideoFocus(BOOL bFocus);

    BOOL SetVideoUpdate(BOOL update);
    BOOL ChangeLayout();
    void DeviceEvent(astring deviceId, VscEventType type);

    void VideoSetGeometry();
    void ShowVideoInfo(bool bEnable);

public slots:
	void stopAction();
	void MotionDetectAction();
	void showDisplay1();
	void showDisplay2();
	void showDisplay3();
	void showDisplay4();
	void Help();
	QCursor GetPTZCursor(QHoverEvent *e);
	u32 GetPTZAction(int x, int y, float &speed);
	
	void videoMouseMove(QMouseEvent *e);
	void videoHoverMove(QHoverEvent *e);
	void videoHoverEnter(QHoverEvent *e);
	void videoHoverLeave(QHoverEvent *e);

	void videoResizeEventTimer();

	void videoResizeEvent();
	void PTZEnable();
	void PTZPanel();
	void AutoFocus();
	void LiveDelCallback();
	void PlaybackClick();

signals:
    void ShowDisplayClicked(int nId);
    void ShowFloatingClicked();
    void ShowTabbedClicked();
    void ShowControlPanelClicked();
    void ShowFocusClicked(int nId);
    void Layout1Clicked(int nId);
    void VideoSwitchWith(int nSrcId, int nDstId);
    void ShowViewClicked(std::string strId);
    void PlaybackClicked(std::string strStor, std::string strId, std::string strName);
    void MotionDetectClicked(std::string strStor, std::string strId, std::string strName);


public:
	BOOL StartPlay(astring strStorId, astring strCamId, astring strCamName, 
			bool bMotion = false);
	BOOL StopPlay();
	bool GetPlayParam(astring &strStorId, astring &strCamId);

public:
	void setId(int nId){ m_nId = nId; }

private:
	HWND m_videoWindow;
	BOOL m_bFocus;

	BOOL m_pStarted;
	int m_nId;
private:
	QWidget  * m_pVideo;
	
private:
	QTimer *m_TimerResize;
	time_t m_lastMoveTime;
	struct timeval m_lastPressed;
	struct timeval m_lastPtzZoom;
	
private:
	BOOL m_DragStart;
	s32 m_lastDragX;
	s32 m_lastWidth;
	s32 m_lastHeight;

	BOOL m_PtzEnable;
	RenderType m_render;

public:
	Ui::VSCVWidget *p_ui;
	Ui::VSCVWidget &ui;
	
private:
	QAction *m_pStop;
	QAction *m_pMotion;
	QAction *m_pHelp;
	QAction *m_pRecord;
	QAction *m_pPTZ;
	QAction *m_pPTZPanel;
	QAction *m_pPlayback;
	QAction *m_pDisplay1;
	QAction *m_pDisplay2;
	QAction *m_pDisplay3;
	QAction *m_pDisplay4;

	QAction *m_pFloating;
	QAction *m_pTabbed;
	QAction *m_pControlEnable;
	QMenu * popupMenu;

	ClientFactory &m_pFactory;
	StorStream *m_StorStream;
	astring m_strCamName;
	astring m_strStor;
	astring m_strCam;
	bool m_bSingle;
    
};

class VE_LIBRARY_API VSCQMimeVid : public QMimeData
{
public:
	VSCQMimeVid(){}
	~VSCQMimeVid(){}
};

class VE_LIBRARY_API VSCQMimeVidCam : public VSCQMimeVid
{
public:
	VSCQMimeVidCam(astring strStorId, astring strCamId, astring strCamName)
	: m_strCamId(strCamId),m_strStorId(strStorId), m_strCamName(strCamName)
	{
	}
	~VSCQMimeVidCam(){}
public:
	astring m_strCamId;
	astring m_strStorId;
	astring m_strCamName;
};

class VE_LIBRARY_API VSCQMimeVidWindowSwitch : public VSCQMimeVid
{
public:
	VSCQMimeVidWindowSwitch(int nDstId)
	: m_nDstId(nDstId)
	{
	}
	~VSCQMimeVidWindowSwitch(){}
public:
	int m_nDstId;
};

class VE_LIBRARY_API VSCQMimeVidView : public VSCQMimeVid
{
public:
	VSCQMimeVidView(astring strViewId)
	: m_strViewId(strViewId)
	{
	}
	~VSCQMimeVidView(){}
public:
	astring m_strViewId;
};

#endif // QT_VSC_V_WIDGET_H
