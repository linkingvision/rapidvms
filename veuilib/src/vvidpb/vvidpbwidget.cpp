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
#include "vvidpbwidget.h"
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
#include "ui_vvidpbwidget.h"
#include "vscvideoinfo.h"
#include "vscptzcontrol.h"
#include "client/storstream.hpp"
#include "rapidmedia/rapidmedia.hpp"
#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "client/storsyncinf.hpp"
#include "vscpbexport.h"

//background-color:rgb(42, 43, 44)

class VVidPBWidgetDelegate : public VVidPBDelegate
{
public:
	VVidPBWidgetDelegate(ClientFactory &pFactory, VidStor &stor, string strId, 
		astring strCamName);
	~VVidPBWidgetDelegate(){}
public:
	virtual bool SearchRec(u32 nStart, u32 nEnd, RecordItemMap &pMap);
	virtual bool SearchHasRec(HasRecordItemMap &pMap);
	virtual VTaskItem * CreateExportTask(u32 nStart, u32 nEnd);
private:
	/* two more ui may use this delegate */
	XMutex m_cMutex;//XGuard guard(m_cMutex);
	VidStor m_stor;
	astring m_strId;
	astring m_strCamName;
	StorSyncInf m_StorInf;
	ClientFactory &m_pFactory;
};

VVidPBWidgetDelegate::VVidPBWidgetDelegate(ClientFactory &pFactory, VidStor &stor, string strId, 
	astring strCamName)
: m_stor(stor), m_strId(strId), m_StorInf(stor), m_pFactory(pFactory), 
 m_strCamName(strCamName)
{
	m_StorInf.Connect();
}

bool VVidPBWidgetDelegate::SearchRec(u32 nStart, u32 nEnd, 
					RecordItemMap &pMap)
{
	XGuard guard(m_cMutex);
	/* The Type is 0xffffffff */
	return m_StorInf.SearchRec(m_strId, nStart, nEnd, 0xffffffff, pMap);
}

bool VVidPBWidgetDelegate::SearchHasRec(HasRecordItemMap &pMap)
{
	XGuard guard(m_cMutex);
	return m_StorInf.SearchHasRec(m_strId, pMap);
}

VTaskItem * VVidPBWidgetDelegate::CreateExportTask(u32 nStart, u32 nEnd)
{
	astring strExportPath;
	m_pFactory.GetExportPath(strExportPath);
	return new VSCPbExportTask(strExportPath, m_strCamName, m_stor, m_strId, nStart, nEnd);
}

VVidPBWidget::VVidPBWidget(VVidPBControl &pControl, ClientFactory &pFactory, s32 nId, QWidget *parent, bool bSingle, Qt::WindowFlags flags)
	: m_pControl(pControl), m_pFactory(pFactory), m_bSingle(bSingle), QWidget(parent, flags), p_ui(new Ui::VVidPBWidget), ui(*p_ui), 
	m_StorStream(NULL), m_Delegate(NULL)
{
	m_pStarted = FALSE;
	m_nId = nId;
	
	m_bFocus = FALSE;

	//gettimeofday(&m_lastPtz, NULL);
	//gettimeofday(&m_lastPtzZoom, NULL);
	this->setAcceptDrops(true);
	m_pStop = new QAction(QIcon(tr(":/action/resources/libuistop.png")), tr("STOP"), this);
	connect(m_pStop, SIGNAL(triggered()), this, SLOT(stopAction()));

	ui.setupUi(this);
	m_pVideo = new QWidget(ui.video);

	VideoSetGeometry();

	m_videoWindow = (HWND)m_pVideo->winId();
	setMouseTracking(true);

	connect(ui.video, SIGNAL(videoMouseMove(QMouseEvent *)), this, SLOT(videoMouseMove(QMouseEvent *)));
	connect(ui.video, SIGNAL(videoResize()), this, SLOT(videoResizeEvent()));

	m_TimerResize = new QTimer(this);
	connect(m_TimerResize, SIGNAL(timeout()),this,SLOT(videoResizeEventTimer())); 

	m_TimerInfo = new QTimer(this);
	connect(m_TimerInfo, SIGNAL(timeout()),this,SLOT(SlotTimerInfoUpdate()));

	m_lastWidth = ui.video->width();
	m_lastHeight = ui.video->height();

	m_pStop->setEnabled(false);
	
#ifdef WIN32
   	setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif
}

VVidPBWidget::~VVidPBWidget()
{
	VDC_DEBUG( "%s ~VVidPBWidget id %d\n",__FUNCTION__,
	        m_nId);

	if (m_pStarted == true)
	{
		m_StorStream->DetachWidget(m_videoWindow);
    		m_StorStream->StopStorStream();
		delete m_StorStream;
		m_StorStream = NULL;
		delete m_Delegate;
		m_Delegate = NULL;
	}

#ifdef WIN32
	//QCoreApplication::processEvents();
#endif
}

void VVidPBWidget::DeviceEvent(astring deviceId, VscEventType type)
{
	
	return;
}

BOOL VVidPBWidget::SetVideoUpdate(BOOL update)
{
    m_pVideo->setUpdatesEnabled(update);
    if (update == TRUE)
        repaint();

    return TRUE;
}

void VVidPBWidget::dragEnterEvent(QDragEnterEvent *event)
{
    VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__, m_nId);

    event->accept();
}

void VVidPBWidget::dragMoveEvent(QDragMoveEvent *event)
{
    VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__, m_nId);

    event->accept();
}

void VVidPBWidget::dropEvent(QDropEvent *event)
{
	QMimeData * pMime = (QMimeData *)event->mimeData();


	/* Check if it is remote camera drop */
	VSCQMimeVidCam *pVidCam = dynamic_cast<VSCQMimeVidCam * > (pMime);
	if (pVidCam)
	{
		/* Just stop and start a new playback */
		StopPlay();
		s32 currentTime = time(NULL) - 5 * 60;
		if (currentTime < 0)
		{
			currentTime = 0;
		}
		StartPlay(pVidCam->m_strStorId, pVidCam->m_strCamId, 
										pVidCam->m_strCamName, currentTime);
		
		emit ShowFocusClicked(m_nId);
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

void VVidPBWidget::stopAction()
{
    StopPlay();
}

void VVidPBWidget::VideoSetGeometry()
{
	m_pVideo->setGeometry(QRect(0, 0, ui.video->width(), ui.video->height()));

	QPoint pos = ui.video->mapToGlobal(QPoint(0,0));
	//m_pVideoInfo->move(pos);
}

void VVidPBWidget::ShowVideoInfo(bool bEnable)
{
	if (bEnable == false)
	{
		ui.videoInfo->hide();
	}else
	{
		ui.videoInfo->show();
	}
}

void VVidPBWidget::videoResizeEventTimer()
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

void VVidPBWidget::videoResizeEvent()
{    
	m_TimerResize->setSingleShot(true);
	m_TimerResize->start(20); 
}

void VVidPBWidget::SlotSeekTime(unsigned int nTime)
{
	if (m_StorStream == NULL)
	{	
		return;
	}
	m_StorStream->SeekPlayback(nTime);
}
void VVidPBWidget::SlotPlay()
{
	if (m_StorStream == NULL)
	{	
		return;
	}
	m_StorStream->ResumePlayback();
}
void VVidPBWidget::SlotPause()
{
	if (m_StorStream == NULL)
	{	
		return;
	}
	m_StorStream->PausePlayback();
}

void VVidPBWidget::SlotTimerInfoUpdate()
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

bool VVidPBWidget::SetupPlayConnection()
{
	if (m_StorStream == NULL)
	{	
		return false;
	}
	/* Stream to Timeline update */
	connect(m_StorStream, SIGNAL(SignalPlayTime(unsigned int)), 
					&m_pControl, SLOT(SlotUpdateTime(unsigned int)));
	/* Control to Stream */
	connect(&m_pControl, SIGNAL(SignalSeekTime(unsigned int)), 
					this, SLOT(SlotSeekTime(unsigned int)));	
	connect(&m_pControl, SIGNAL(SignalPlay()),  this, SLOT(SlotPlay()));
	connect(&m_pControl, SIGNAL(SignalPause()),  this, SLOT(SlotPause()));
	
	return true;
}
bool VVidPBWidget::DestoryPlayConnection()
{
	if (m_StorStream == NULL)
	{	
		return false;
	}

	
	return true;
}


BOOL VVidPBWidget::StartPlay(astring strStorId, astring strCamId, astring strCamName, 
	u32 nPlaytime)
{

	m_pStarted = TRUE;
	//m_pProxy->StartPlayLive(ui.video->width(), ui.video->height());
	//ui.video->width();
	VidStor sStor;
	m_pFactory.GetConfDB().GetStorConf(strStorId, sStor);

	m_strStor = strStorId;
	m_strCam = strCamId;
	
	m_StorStream = new StorStream(sStor, strCamId, 1, true, nPlaytime);
	m_Delegate = new VVidPBWidgetDelegate(m_pFactory, sStor, strCamId, strCamName);
	m_StorStream->AttachWidget(m_videoWindow, ui.video->width(), ui.video->height());
	m_StorStream->StartStorStream();
	m_strCamName = strCamName;
	m_pControl.SetDelegate(m_Delegate);
	m_pControl.SetPlayParam(strStorId, strCamId, strCamName);
	m_pControl.SetPlaying(true);
	SetupPlayConnection();

	astring strName;
	VideoStreamInfo pInfo;

	m_StorStream->GetStreamInfo(pInfo);
	std::string strCodec = RapidMedia::GetCodecName(pInfo.codec);
	std::string strWidth = ck_string::from_uint16(pInfo.width, 10); 
	std::string strHeight = ck_string::from_uint16(pInfo.height, 10); 

	strName =  m_strCamName + " ["  + strCodec + " " + strWidth + " " + "x" + 
		+ " " + strHeight + "]";

	SetVideoUpdate(false);

	m_pStop->setEnabled(true);
	ui.video->setRunning(true);
	videoResizeEvent();
	m_TimerInfo->start(1000);
	
	return TRUE;
}

BOOL VVidPBWidget::StopPlay()
{
	
	if (m_pStarted == true)
    	{
    		m_TimerInfo->stop();
    		DestoryPlayConnection();
		m_pControl.SetPlaying(false);
		m_pControl.SetPlayParam(VVID_UUID_NULL, 
						VVID_UUID_NULL, VVID_UUID_NULL);
    		m_StorStream->DetachWidget(m_videoWindow);
    		m_StorStream->StopStorStream();
		delete m_StorStream;
		m_StorStream = NULL;
		m_pControl.SetDelegate(NULL);
		m_pControl.SlotClearPlay();
		delete m_Delegate;
		m_Delegate = NULL;
		m_StorStream = NULL;
		m_strCamName = "";
        	VDC_DEBUG( "%s StopPlay begin\n",__FUNCTION__);
        	m_pStarted = false;	
		ui.video->setRunning(false);
		SetVideoUpdate(true);
    	}

	ui.videoInfo->ui.labelName->setText("");
	m_pStop->setEnabled(false);	
 	return TRUE;
}

void VVidPBWidget::AutoFocus()
{
	SetVideoFocus(TRUE);
}


void VVidPBWidget::SetVideoFocus(BOOL bFocus)
{

    if (bFocus == FALSE && m_bFocus == TRUE)
    {
		ui.videoWrapper->setStyleSheet(QStringLiteral("background-color:rgb(100, 100, 100)"));
		m_bFocus = bFocus;
    }else if (bFocus == TRUE && m_bFocus == FALSE)
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

		ui.videoWrapper->setStyleSheet(QStringLiteral("background-color:rgb(255, 0, 0)"));
		//VDC_DEBUG( "%s SetVideoFocus %d true\n",__FUNCTION__, m_nId);
		m_bFocus = bFocus;
    }
    videoResizeEvent();
    
}

void VVidPBWidget::mousePressEvent(QMouseEvent *e)
{
    VDC_DEBUG( "%s mousePressEvent %d\n",__FUNCTION__, m_nId);
    QWidget::mousePressEvent(e);
    Qt::MouseButtons mouseButtons = e->buttons();
    if( mouseButtons != Qt::LeftButton )
    {
        return;
    }

    emit ShowFocusClicked(m_nId);

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

void VVidPBWidget::mouseReleaseEvent(QMouseEvent *event)
{
    VDC_DEBUG( "%s id %d Release \n",__FUNCTION__, m_nId);
    if(event->button() == Qt::LeftButton)
    {
        m_DragStart = false;
    }
    event->accept();
}

void VVidPBWidget::videoMouseMove(QMouseEvent *e)
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

void VVidPBWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    VDC_DEBUG( "%s mouseDoubleClickEvent %d\n",__FUNCTION__, m_nId);
    QWidget::mouseDoubleClickEvent(e);
    m_DragStart = false;

    emit Layout1Clicked(m_nId);
    e->accept();
}


void VVidPBWidget::contextMenuEvent(QContextMenuEvent* e) 
{
	QDesktopWidget *desktop = QApplication::desktop();
	popupMenu = new QMenu(NULL);
	popupMenu->addAction(m_pStop);

	popupMenu->exec(e->globalPos()); 
	delete popupMenu;
	popupMenu = NULL;
}

