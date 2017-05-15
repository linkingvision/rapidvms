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
#include "vscvwidget.h"
#include "server/factory.hpp"
#include <QResizeEvent>
#include <QMimeData>
#include <QDrag>
#include <QAction>
#include <QDesktopWidget>
#include <QApplication>
#include <QDateTime>
#include <QTime>
#include <QMessageBox>
#include <time.h>
#include "QMimeData"
#include "QPainter"
#include "QPixmap"
#include "ui_vscvwidget.h"
#include "vscvideoinfo.h"
#include "vscptzcontrol.h"
#include "client/storstream.hpp"
#include "rapidmedia/rapidmedia.hpp"
#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"

//background-color:rgb(42, 43, 44)


VSCVWidget::VSCVWidget(ClientFactory &pFactory, s32 nId, QWidget *parent, 
	bool bSingle, Qt::WindowFlags flags, VVidGetHWDec *pGetHWDec)
	: m_pFactory(pFactory), m_bSingle(bSingle), QWidget(parent, flags), p_ui(new Ui::VSCVWidget), ui(*p_ui), 
	m_StorStream(NULL), m_pGetHWDec(pGetHWDec), m_bHWAccel(false)
{
	m_pStarted = FALSE;
	m_nId = nId;
	
	m_bFocus = FALSE;

	m_DragStart = FALSE;

	m_PtzEnable = FALSE;
#ifdef WIN32
	m_render = RENDER_TYPE_D3D;
#else
	m_render = RENDER_TYPE_SDL;
#endif

	//gettimeofday(&m_lastPtz, NULL);
	//gettimeofday(&m_lastPtzZoom, NULL);
	this->setAcceptDrops(true);
	m_pStop = new QAction(QIcon(tr(":/action/resources/libuistop.png")), tr("Stop"), this);
	connect(m_pStop, SIGNAL(triggered()), this, SLOT(stopAction()));

	m_pMotion = new QAction(QIcon(tr(":/action/resources/motion.png")), tr("Motion Detect"), this);
	connect(m_pMotion, SIGNAL(triggered()), this, SLOT(MotionDetectAction()));

	m_pHelp = new QAction(QIcon(tr(":/action/resources/help.png")), tr("Help"), this);
	connect(m_pHelp, SIGNAL(triggered()), this, SLOT(Help()));

	m_pFloating = new QAction(QIcon(tr(":/action/resources/libuifloat.png")), tr("Float"), this);
	connect(m_pFloating, SIGNAL(triggered()), this, SIGNAL(ShowFloatingClicked()));


	m_pPTZ = new QAction(QIcon(tr(":/action/resources/libuidome.png")), tr("PTZ"), this);
	connect(m_pPTZ, SIGNAL(triggered()), this, SLOT(PTZEnable()));   

	m_pPTZPanel = new QAction(QIcon(tr(":/action/resources/libuidome.png")), tr("PTZ Panel"), this);
	connect(m_pPTZPanel, SIGNAL(triggered()), this, SLOT(PTZPanel()));   

	m_pPlayback = new QAction(QIcon(tr(":/action/resources/libuiinstantpb.png")), tr("Playback"), this);
	connect(m_pPlayback, SIGNAL(triggered()), this, SLOT(PlaybackClick()));  

	m_pTabbed = new QAction(QIcon(tr(":/action/resources/libuitabbed.png")), tr("Tabbed"), this);
	connect(m_pTabbed, SIGNAL(triggered()), this, SIGNAL(ShowTabbedClicked()));

	m_pControlEnable = new QAction(QIcon(tr(":/action/resources/libuicontrol_panel.png")), tr("Control"), this);
	connect(m_pControlEnable, SIGNAL(triggered()), this, SIGNAL(ShowControlPanelClicked()));

	m_pDisplay1 = new QAction(QIcon(tr(":/action/resources/libuidisplay.png")), tr("DISPLAY1"), this);
	connect(m_pDisplay1, SIGNAL(triggered()), this, SLOT(showDisplay1()));

	m_pDisplay2 = new QAction(QIcon(tr(":/action/resources/libuidisplay.png")), tr("DISPLAY2"), this);
	connect(m_pDisplay2, SIGNAL(triggered()), this, SLOT(showDisplay2()));

	m_pDisplay3 = new QAction(QIcon(tr(":/action/resources/libuidisplay.png")), tr("DISPLAY3"), this);
	connect(m_pDisplay3, SIGNAL(triggered()), this, SLOT(showDisplay3()));

	m_pDisplay4 = new QAction(QIcon(tr(":/action/resources/libuidisplay.png")), tr("DISPLAY4"), this);
	connect(m_pDisplay4, SIGNAL(triggered()), this, SLOT(showDisplay4()));

	ui.setupUi(this);

	m_pVideo = NULL;

	ReCreateVideoWidget();
	m_TimerResize = new QTimer(this);
	m_TimerInfo = new QTimer(this);
	connect(m_TimerResize, SIGNAL(timeout()),this,SLOT(videoResizeEventTimer())); 
	connect(m_TimerInfo, SIGNAL(timeout()),this,SLOT(SlotTimerInfoUpdate()));
	
	m_pStop->setEnabled(false);
	m_pPlayback->setEnabled(false);
	m_pPTZ->setEnabled(false);
	m_pPTZPanel->setEnabled(false);
	
#ifdef WIN32
   	setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif

	gettimeofday(&m_lastPtzZoom, NULL);
}

void VSCVWidget::ReCreateVideoWidget()
{
	if (m_pVideo != NULL)
	{
		delete m_pVideo;
		m_pVideo = NULL;
	}
	m_pVideo = new QWidget(ui.video);

	VideoSetGeometry();

	m_videoWindow = (HWND)m_pVideo->winId();
	setMouseTracking(true);

	connect(ui.video, SIGNAL(videoMouseMove(QMouseEvent *)), this, SLOT(videoMouseMove(QMouseEvent *)));
	connect(ui.video, SIGNAL(videoHoverMove(QHoverEvent *)), this, SLOT(videoHoverMove(QHoverEvent *)));
	connect(ui.video, SIGNAL(videoHoverEnter(QHoverEvent *)), this, SLOT(videoHoverEnter(QHoverEvent *)));
	connect(ui.video, SIGNAL(videoHoverLeave(QHoverEvent *)), this, SLOT(videoHoverLeave(QHoverEvent *)));

	connect(ui.video, SIGNAL(videoResize()), this, SLOT(videoResizeEvent()));

	m_lastWidth = ui.video->width();
	m_lastHeight = ui.video->height();
}

VSCVWidget::~VSCVWidget()
{
	VDC_DEBUG( "%s ~VSCVWidget id %d\n",__FUNCTION__,
	        m_nId);

	StopPlay();

#ifdef WIN32
	QCoreApplication::processEvents();
#endif
}

void VSCVWidget::PlaybackClick()
{
	emit PlaybackClicked(m_strStor, m_strCam, m_strCamName);
}

void VSCVWidget::DeviceEvent(astring deviceId, VscEventType type)
{
	
	return;
}

BOOL VSCVWidget::SetVideoUpdate(BOOL update)
{
    m_pVideo->setUpdatesEnabled(update);
    if (update == TRUE)
        repaint();

    return TRUE;
}

void VSCVWidget::dragEnterEvent(QDragEnterEvent *event)
{
    VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__, m_nId);

    event->accept();
}

void VSCVWidget::dragMoveEvent(QDragMoveEvent *event)
{
    VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__, m_nId);

    event->accept();
}

void VSCVWidget::dropEvent(QDropEvent *event)
{
	QMimeData * pMime = (QMimeData *)event->mimeData();

	/* Check if it is view drop */
	VSCQMimeVidView *pView = dynamic_cast<VSCQMimeVidView * > (pMime);
	if (pView)
	{
		VDC_DEBUG("%s View in dropEvent id %s\n", __FUNCTION__, pView->m_strViewId.c_str());
		emit ShowViewClicked(pView->m_strViewId);

	}
	/* Check if it is remote camera drop */
	VSCQMimeVidCam *pVidCam = dynamic_cast<VSCQMimeVidCam * > (pMime);
	if (pVidCam)
	{
		/* Just stop and start a new camera */
		StopPlay();
		StartPlay(pVidCam->m_strStorId, pVidCam->m_strCamId, pVidCam->m_strCamName);
		emit ShowFocusClicked(m_nId);
		emit SignalUpdateMainView();
		return;		
	}

	/* Check if it is Window switch */
	VSCQMimeVidWindowSwitch *pWinSwitch = dynamic_cast<VSCQMimeVidWindowSwitch * > (pMime);
	if (pWinSwitch)
	{
		if (pWinSwitch->m_nDstId == m_nId)
		{
			VDC_DEBUG("%s Enter in dropEvent Same with me %d\n",
				__FUNCTION__, pWinSwitch->m_nDstId);
			return;
		}
		emit VideoSwitchWith(m_nId, pWinSwitch->m_nDstId);
	}
}

void VSCVWidget::stopAction()
{
    StopPlay();
}

void VSCVWidget::MotionDetectAction()
{
	emit MotionDetectClicked(m_strStor, m_strCam, m_strCamName);
}

void VSCVWidget::PTZEnable()
{
	//m_pProxy->PTZEnable();
	if (m_PtzEnable == FALSE)
	{
		m_PtzEnable = TRUE;
	}else
	{
		this->setCursor(Qt::ArrowCursor);
		m_PtzEnable = FALSE;
	}
	
}

void VSCVWidget::PTZPanel()
{

	VSCPTZControl ptzControl(m_pFactory, this);

	ptzControl.show();

	QPoint pos = mapTo(this, QCursor::pos());
	ptzControl.move(pos.x() , pos.y());
	/* Move the PTZ Control to mouse pos */
	ptzControl.exec();
}

void VSCVWidget::VideoSetGeometry()
{
	if (m_pVideo == NULL)
	{
		return;
	}
	m_pVideo->setGeometry(QRect(0, 0, ui.video->width(), ui.video->height()));

	//m_pVideoControl->resize(ui.video->width(), 45);
	QPoint pos = ui.video->mapToGlobal(QPoint(0, 0));
	//m_pVideoControl->move(pos.x(),  pos.y() + ui.video->height() - 45);}
}

void VSCVWidget::ShowVideoInfo(bool bEnable)
{
	if (bEnable == true)
	{
		ui.videoInfo->show();
	}else
	{
		ui.videoInfo->hide();
	}
}

void VSCVWidget::videoResizeEventTimer()
{
	VideoSetGeometry();
	if (m_pStarted == TRUE && m_StorStream)
	{
		if (m_lastWidth != ui.video->width() || m_lastHeight != ui.video->height())
		{
			m_StorStream->UpdateWidget(m_videoWindow, ui.video->width(), 
				ui.video->height());
		}else
		{
			return;
		}
		m_lastWidth = ui.video->width();
		m_lastHeight = ui.video->height();
	}
}

void VSCVWidget::SlotTimerInfoUpdate()
{
	if (m_pStarted == TRUE)
	{
		astring strName;
		VideoStreamInfo pInfo;

		m_StorStream->GetStreamInfo(pInfo);
		std::string strCodec = RapidMedia::GetCodecName(pInfo.codec);
		std::string strWidth = ck_string::from_uint16(pInfo.width, 10); 
		std::string strHeight = ck_string::from_uint16(pInfo.height, 10); 

		strName =  m_strCamName + " ["  + strCodec + " " + strWidth + " " + "x" + 
		+ " " + strHeight + "]";

		ui.videoInfo->ui.labelName->setText(strName.c_str());
	}
}

void VSCVWidget::videoResizeEvent()
{    
	m_TimerResize->setSingleShot(true);
	m_TimerResize->start(20); 
}


bool VSCVWidget::GetPlayParam(astring &strStorId, astring &strCamId, bool &bHWAccel)
{
	if (m_pStarted == true)
	{
		strStorId = m_strStor;
		strCamId = m_strCam;
		bHWAccel = m_bHWAccel;
		return true;
	}
	return false;
}

BOOL VSCVWidget::StartPlay(astring strStorId, astring strCamId, 
				astring strCamName, bool bMotion, bool bByView, bool bWithHWAccel)
{

	
	//m_pProxy->StartPlayLive(ui.video->width(), ui.video->height());
	//ui.video->width();
	VidStor sStor;
	m_pFactory.GetConfDB().GetStorConf(strStorId, sStor);

	VidCamera sCam;
	if (m_pFactory.GetStorFactory().GetCameraConf(strStorId, strCamId, sCam) == false)
	{
		return false;
	}
	m_strCamName = sCam.strname();

	m_strStor = strStorId;
	m_strCam = strCamId;
	bool bHWAccel = false;
	if (bByView == false)
	{
		if (m_pGetHWDec)
		{
			bHWAccel = m_pGetHWDec->GetHWDec();
		}
	}else
	{
		bHWAccel = bWithHWAccel;
	}
	m_bHWAccel = bHWAccel;
	m_StorStream = new StorStream(sStor, strCamId, 1, false, 0, bHWAccel);
	if (bMotion == true)
	{
		m_render = RENDER_TYPE_SDL;
	}else
	{
#ifdef WIN32
		m_render = RENDER_TYPE_D3D;
#else
		m_render = RENDER_TYPE_SDL;
#endif		
	}
	m_StorStream->AttachWidget(m_videoWindow, ui.video->width(), ui.video->height(), m_render);
	if (bMotion == true)
	{
		m_StorStream->EnableMot(m_videoWindow, true, sCam.strmotreg());
	}
	m_StorStream->StartStorStream();

	m_pStarted = TRUE;
	

	astring strName;
	VideoStreamInfo pInfo;

	m_StorStream->GetStreamInfo(pInfo);
	std::string strCodec = RapidMedia::GetCodecName(pInfo.codec);
	std::string strWidth = ck_string::from_uint16(pInfo.width, 10); 
	std::string strHeight = ck_string::from_uint16(pInfo.height, 10); 

	strName =  m_strCamName + " ["  + strCodec + " " + strWidth + " " + "x" + 
		+ " " + strHeight + "]";
	
	ui.videoInfo->ui.labelName->setText(strName.c_str());

	SetVideoUpdate(false);

	m_pStop->setEnabled(true);
	m_pPlayback->setEnabled(true);
	m_pPTZ->setEnabled(true);
	m_pPTZPanel->setEnabled(true);
	m_pMotion->setEnabled(true);
	ui.video->setRunning(true);
	videoResizeEvent();
	m_TimerInfo->start(1000);
	
	return TRUE;
}

BOOL VSCVWidget::StopPlay()
{
	
	if (m_pStarted == true)
    	{
    		m_TimerInfo->stop();
    		m_StorStream->DetachWidget(m_videoWindow);
    		m_StorStream->StopStorStream();
		m_StorStream = NULL;
		m_strCamName = "";
        	VDC_DEBUG( "%s StopPlay begin\n",__FUNCTION__);
        	m_pStarted = false;
		/* Fix this for motion detect delete */
		//ReCreateVideoWidget();
		ui.video->setRunning(false);
		SetVideoUpdate(true);
		ui.videoInfo->ui.labelName->setText("");
    	}

	m_pStop->setEnabled(false);
	m_pMotion->setEnabled(false);
	m_pPlayback->setEnabled(false);
	m_pPTZ->setEnabled(false);
	m_pPTZPanel->setEnabled(false);
	m_PtzEnable = FALSE;

	/* Clear the PTZ control Proxy */	
    return TRUE;
}


void VSCVWidget::LiveDelCallback()
{
	StopPlay();
	return ;
}

void VSCVWidget::AutoFocus()
{
	SetVideoFocus(TRUE);
}


void VSCVWidget::SetVideoFocus(BOOL bFocus)
{

    if (bFocus == FALSE && m_bFocus == TRUE)
    {
		ui.videoWrapper->setStyleSheet(QStringLiteral("background-color:rgb(100, 100, 100)"));
		//VDC_DEBUG( "%s SetVideoFocus %d false\n",__FUNCTION__, m_nId);
		m_bFocus = bFocus;
    }else if (bFocus == TRUE && m_bFocus == FALSE)
    {
		ui.videoWrapper->setStyleSheet(QStringLiteral("background-color:rgb(255, 0, 0)"));
		//VDC_DEBUG( "%s SetVideoFocus %d true\n",__FUNCTION__, m_nId);
		m_bFocus = bFocus;
    }
    videoResizeEvent();
    
}

void VSCVWidget::mousePressEvent(QMouseEvent *e)
{
	VDC_DEBUG( "%s mousePressEvent %d\n",__FUNCTION__, m_nId);
	QWidget::mousePressEvent(e);
	Qt::MouseButtons mouseButtons = e->buttons();
	if( mouseButtons != Qt::LeftButton )
	{
	    return;
	}

	emit ShowFocusClicked(m_nId);

	/* PTZ is enabled just process the PTZ action */
	if (m_PtzEnable == true)
	{
		float speed = 0.5;
		u32 nAction = GetPTZAction(e->x(), e->y(), speed);
		m_pFactory.GetStorFactory().PtzCmd(m_strStor, m_strCam,
			nAction, speed);	
		return;
	}

	if (m_pStarted == TRUE)
	{
	    if (m_DragStart == FALSE)
	    {
	        m_lastDragX = e->x();
	        VDC_DEBUG( "%s mousePressEvent %d start x %d\n",__FUNCTION__, m_nId, m_lastDragX);
	        m_DragStart = TRUE;
			e->accept();
	        return;
	    }
	}

	if (m_pStarted == TRUE)
	{
	   videoResizeEvent();
	}
	e->accept();

}

void VSCVWidget::mouseReleaseEvent(QMouseEvent *event)
{
    VDC_DEBUG( "%s id %d Release \n",__FUNCTION__, m_nId);
    if(event->button() == Qt::LeftButton)
    {
        m_DragStart = false;
    }

	if (m_PtzEnable == true)
	{
		m_pFactory.GetStorFactory().PtzCmd(m_strStor, m_strCam, F_PTZ_STOP, 0);
	}

    event->accept();
}

/* Add http://doc.qt.io/qt-5/qhoverevent.html QHoverEvent 
    this->setCursor(Qt::WaitCursor);
    http://doc.qt.io/qt-5/qcursor.html
	QCursor leftPTZ(QPixmap(":/action/resources/libuileft.png"));
	this->setCursor(leftPTZ);
	this->setCursor(Qt::ArrowCursor);
*/

u32 VSCVWidget::GetPTZAction(int x, int y, float &speed)
{
	u32 nAction;
	int w = width();
	int h = height();
	/* new axis based on the  width()/2 and height()/2 is center*/
	int x1 = x - w/2;
	int y1 = h/2 - y;

	if (x1 >= 0)
	{
		if (abs(x1) >= abs(y1))
		{
			speed =  (double)abs(x1) /(double)(w/2);
			return F_PTZ_RIGHT;
		}
	}

	if (y1 >= 0)
	{
		if (abs(x1) <= abs(y1))
		{
			speed =  (double)abs(y1) /(double)(h/2);
			return F_PTZ_UP;
		}
	}

	if (x1 <= 0)
	{
		if (abs(x1) >= abs(y1))
		{
			speed =  (double)abs(x1) /(double)(w/2);
			return F_PTZ_LEFT;
		}		
	}

	if (y1 <= 0)
	{
		if (abs(x1) <= abs(y1))
		{
			speed =  (double)abs(y1) /(double)(h/2);
			return F_PTZ_DOWN;
		}		
	}

	return F_PTZ_LEFT;
}

QCursor VSCVWidget::GetPTZCursor(QHoverEvent *e)
{
	float nSpeed;
	u32 action = GetPTZAction(e->pos().x(), e->pos().y(), nSpeed);

	switch (action)
	{
		case F_PTZ_RIGHT:
		{
			QCursor PTZCursor(QPixmap(":/action/resources/ptz-right.png"));
			return PTZCursor;
			break;
		}
		case F_PTZ_LEFT:
		{
			QCursor PTZCursor(QPixmap(":/action/resources/ptz-left.png"));
			return PTZCursor;
			break;
		}
		case F_PTZ_UP:
		{
			QCursor PTZCursor(QPixmap(":/action/resources/ptz-up.png"));
			return PTZCursor;
			break;
		}
		case F_PTZ_DOWN:
		{
			QCursor PTZCursor(QPixmap(":/action/resources/ptz-down.png"));
			return PTZCursor;
			break;
		}
		default:
		{
			QCursor PTZCursor(QPixmap(":/action/resources/ptz-left.png"));
			return PTZCursor;
		}
	};
	
	QCursor PTZCursor(QPixmap(":/action/resources/ptz-left.png"));
	return PTZCursor;
}

void VSCVWidget::videoHoverMove(QHoverEvent *e)
{
	if (m_PtzEnable == false)
	{
		return;
	}

	this->setCursor(GetPTZCursor(e));
}
void VSCVWidget::videoHoverEnter(QHoverEvent *e)
{
	if (m_PtzEnable == false)
	{
		return;
	}
	this->setCursor(GetPTZCursor(e));
}
void VSCVWidget::videoHoverLeave(QHoverEvent *e)
{
	if (m_PtzEnable == false)
	{
		return;
	}
	this->setCursor(Qt::ArrowCursor);
}



void VSCVWidget::videoMouseMove(QMouseEvent *e)
{
    if (e->y() > ui.video->height() - 45)
    {
		return;
    }
  
    if (m_pStarted == TRUE)
    {
        if (m_DragStart == TRUE)
        {
           VDC_DEBUG( "%s mouseMoveEvent %d event x %d\n",__FUNCTION__, m_nId, e->x());
           if (abs(e->x() - m_lastDragX) < 5)
           {
               return;
           }
        }
        else
        {
            return;
        }
        u32 nId = m_nId;
        m_DragStart = FALSE;
        VDC_DEBUG( "%s id %d\n",__FUNCTION__, nId);
		QMimeData *mimeData = new VSCQMimeVidWindowSwitch(nId);
    	
    	// Create drag
    	QPixmap pixmap(":/action/resources/buttonhover.png");
    	QPainter painter(&pixmap);
    
    	QDrag *drag = new QDrag(this);
    	drag->setMimeData(mimeData);
    	drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(drag->pixmap().width()/2,
                         drag->pixmap().height()/2));
    	drag->exec();
        
    }
}

void VSCVWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    VDC_DEBUG( "%s mouseDoubleClickEvent %d\n",__FUNCTION__, m_nId);
    QWidget::mouseDoubleClickEvent(e);
    m_DragStart = false;

    emit Layout1Clicked(m_nId);
    e->accept();
}


void VSCVWidget::wheelEvent ( QWheelEvent * event )
{
	struct timeval current;
	gettimeofday(&current, NULL);
	if (current.tv_sec == m_lastPtzZoom.tv_sec)
	{
		if (current.tv_usec - m_lastPtzZoom.tv_usec < 200000)
		{
			return;
		}
	}
	if (current.tv_sec -  m_lastPtzZoom.tv_sec == 1)
	{
		if (current.tv_usec + 1000000 - m_lastPtzZoom.tv_usec < 200000)
		{
			return;
		}		
	}
	if (m_PtzEnable == false)
	{
		return;
	}
	gettimeofday(&m_lastPtzZoom, NULL);
    
	VDC_DEBUG( "%s wheelEvent %d %d\n",__FUNCTION__, m_nId, event->delta());
	float scale =(event->delta()/120); //or use any other step for zooming

	//m_pProxy->PtzAction(action, speed);

	if(scale > 0)
	{
		m_pFactory.GetStorFactory().PtzCmd(m_strStor, m_strCam,
					F_PTZ_ZOOM_IN, 0.2);//TODO Change//speed is 0.0 to 1.0
	}else
	{
		m_pFactory.GetStorFactory().PtzCmd(m_strStor, m_strCam,
					F_PTZ_ZOOM_OUT, 0.2);//TODO Change//speed is 0.0 to 1.0
	}
#ifdef WIN32
	Sleep(100);//TODO change 
#else
	usleep(100 * 1000);
#endif
	m_pFactory.GetStorFactory().PtzCmd(m_strStor, m_strCam, F_PTZ_STOP, 0);
	return;
}

void VSCVWidget::Help()
{
        QMessageBox msgBox(NULL);
#ifdef WIN32
   	msgBox.setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif
        //Set text
        msgBox.setText(tr("Drag and drop camera to here ..."));
            //Set predefined icon, icon is show on left side of text.
        msgBox.setWindowTitle(tr("Help"));
         msgBox.setWindowIcon(QIcon(tr(":/action/resources/libuivsc32.png")));
        msgBox.setIconPixmap(QPixmap(":/action/resources/libuivsc32.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
            //Set focus of ok button
        msgBox.setDefaultButton(QMessageBox::Ok);

        int ret = msgBox.exec();
}

void VSCVWidget::contextMenuEvent(QContextMenuEvent* e) 
{
	if (m_bSingle == true)
	{
		return;
	}
	QDesktopWidget *desktop = QApplication::desktop();
	popupMenu = new QMenu(NULL);
	QPalette Pal(palette());
 
	// set black background
	Pal.setColor(QPalette::Background, Qt::white);
	popupMenu->setAutoFillBackground(true);
	popupMenu->setPalette(Pal);
#ifdef WIN32
   	popupMenu->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif
	//popupMenu->setStyleSheet("background-color: rgb(240, 240, 240);");
	popupMenu->addAction(m_pStop);
#ifndef __APPLE__
	popupMenu->addAction(m_pMotion);
#endif
	popupMenu->addAction(m_pPTZ);
	popupMenu->addAction(m_pPTZPanel);
	popupMenu->addAction(m_pPlayback);
#ifndef __APPLE__
	popupMenu->addAction(m_pFloating);
	popupMenu->addAction(m_pTabbed);
#endif
	popupMenu->addSeparator();
	popupMenu->addAction(m_pControlEnable);


	int screenCnt = desktop->screenCount();
	if (screenCnt == 1)
	{
	    popupMenu->addAction(m_pDisplay1);
	}else if (screenCnt == 2)
	{
	    popupMenu->addAction(m_pDisplay1);
	    popupMenu->addAction(m_pDisplay2);
	}else if (screenCnt == 3)
	{
	    popupMenu->addAction(m_pDisplay1);
	    popupMenu->addAction(m_pDisplay2);
	    popupMenu->addAction(m_pDisplay3);
	}else if (screenCnt == 4)
	{
	    popupMenu->addAction(m_pDisplay1);
	    popupMenu->addAction(m_pDisplay2);
	    popupMenu->addAction(m_pDisplay3);
	    popupMenu->addAction(m_pDisplay4);
	}else
	{
	    popupMenu->addAction(m_pDisplay1);
	    popupMenu->addAction(m_pDisplay2);
	    popupMenu->addAction(m_pDisplay3);
	    popupMenu->addAction(m_pDisplay4);
	}


	popupMenu->addSeparator();
	popupMenu->addAction(m_pHelp);
	popupMenu->addSeparator();

	popupMenu->exec(e->globalPos()); 
	delete popupMenu;
	popupMenu = NULL;
}

void VSCVWidget::showDisplay1()
{
    emit ShowDisplayClicked(0);
}
void VSCVWidget::showDisplay2()
{
    emit ShowDisplayClicked(1);
}
void VSCVWidget::showDisplay3()
{
    emit ShowDisplayClicked(2);
}

void VSCVWidget::showDisplay4()
{
    emit ShowDisplayClicked(3);
}


