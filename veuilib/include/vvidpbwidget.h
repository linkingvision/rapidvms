/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#ifndef __VSC_V_VID_PB_WIDGET_H__
#define __VSC_V_VID_PB_WIDGET_H__

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
#include "vscvwidget.h"
#include "vvidpbcontrol.h"
using  namespace tthread;

namespace Ui {
class VVidPBWidget;
}

class VVidPBWidgetDelegate;

class VE_LIBRARY_API VVidPBWidget : public QWidget
{
	Q_OBJECT

public:
	VVidPBWidget(VVidPBControl &pControl, ClientFactory &pFactory, s32 nId, QWidget *parent = 0, bool bSingle = false, Qt::WindowFlags flags = 0);
	~VVidPBWidget();
	void mouseDoubleClickEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void contextMenuEvent(QContextMenuEvent* e) ;

	Qt::DropActions supportedDropActions () const
	{
	        return Qt::MoveAction;
	}
	void dropEvent(QDropEvent *event);
	void createContentMenu();
	void SetVideoFocus(BOOL bFocus);

	BOOL SetVideoUpdate(BOOL update);
	void DeviceEvent(astring deviceId, VscEventType type);

	void VideoSetGeometry();
	void ShowVideoInfo(bool bEnable);

public slots:
	void stopAction();
	void videoMouseMove(QMouseEvent *e);

	void videoResizeEventTimer();

	void videoResizeEvent();
	void AutoFocus();

public slots:
	void SlotSeekTime(unsigned int nTime);
	void SlotPlay();
	void SlotPause();
	void SlotTimerInfoUpdate();	

signals:
	void ShowFocusClicked(int nId);
	void Layout1Clicked(int nId);
	void VideoSwitchWith(int nSrcId, int nDstId);

public:
	BOOL StartPlay(astring strStorId, astring strCamId, astring strCamName, 
		u32 nPlaytime);
	BOOL StopPlay();
	bool SetupPlayConnection();
	bool DestoryPlayConnection();

public:
	void setId(int nId){ m_nId = nId; }

private:
	HWND m_videoWindow;
	BOOL m_bFocus;

	BOOL m_pStarted;
	int m_nId;
private:
	VSCVideoInfo * m_pVideoInfo;
	QWidget  * m_pVideo;
	
private:
	QTimer *m_TimerResize;
	QTimer *m_TimerInfo;
	time_t m_lastMoveTime;
	struct timeval m_lastPressed;
	
private:
	BOOL m_DragStart;
	s32 m_lastDragX;
	s32 m_lastWidth;
	s32 m_lastHeight;

public:
	Ui::VVidPBWidget *p_ui;
	Ui::VVidPBWidget &ui;
	
private:
	QAction *m_pStop;
	QMenu * popupMenu;

	ClientFactory &m_pFactory;
	VVidPBControl &m_pControl;
	StorStream *m_StorStream;
	VVidPBWidgetDelegate *m_Delegate;
	astring m_strCamName;
	astring m_strStor;
	astring m_strCam;
	bool m_bSingle;
};


#endif // __VSC_V_VID_PB_WIDGET_H
