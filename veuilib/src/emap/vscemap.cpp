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
#include <QThread>

#include "vscemap.h"
#include "vscloading.hpp"
#include "vscemapcam.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QApplication>
#include <QPainter>
#include <QColor>
#include <QFileDialog>
#include <QImage>
#include <QPushButton>
#include <QMimeData>
#include <QDesktopWidget>
#include <QApplication>
#include <QBuffer>
#include "vscvwidget.h"

static Factory *m_pFactory = NULL;

VSCEMap::VSCEMap(std::string strId, ClientFactory &pFactory, QTabWidget &pTabbed, QWidget *parent)
: m_pTabWidget(pTabbed), m_bFloated(FALSE), m_pParent(parent), VEMap(parent), 
  m_strId(strId), m_pFactory(pFactory)
{
	ui.setupUi(this);
	SetupConnections();

	this->setAcceptDrops(true);

	m_pLoadFile = new QAction(QIcon(tr(":/action/resources/libuimap.png")), tr("LOAD EMAP"), this);
	connect(m_pLoadFile, SIGNAL(triggered()), this, SLOT(LoadEmapAction()));
	
	m_pHelp = new QAction(QIcon(tr(":/action/resources/help.png")), tr("HELP"), this);
	connect(m_pHelp, SIGNAL(triggered()), this, SLOT(Help()));

	m_pFloating = new QAction(QIcon(tr(":/action/resources/libuifloat.png")), tr("FLOAT"), this);
	connect(m_pFloating, SIGNAL(triggered()), this, SLOT(FloatingClicked()));

	m_pTabbed = new QAction(QIcon(tr(":/action/resources/libuitabbed.png")), tr("TABBED"), this);
	connect(m_pTabbed, SIGNAL(triggered()), this, SLOT(TabbedClicked()));

	m_pDisplay1 = new QAction(QIcon(tr(":/action/resources/libuidisplay.png")), tr("DISPLAY 1"), this);
	connect(m_pDisplay1, SIGNAL(triggered()), this, SLOT(showDisplay1()));

	m_pDisplay2 = new QAction(QIcon(tr(":/action/resources/libuidisplay.png")), tr("DISPLAY 2"), this);
	connect(m_pDisplay2, SIGNAL(triggered()), this, SLOT(showDisplay2()));

	m_pDisplay3 = new QAction(QIcon(tr(":/action/resources/libuidisplay.png")), tr("DISPLAY 3"), this);
	connect(m_pDisplay3, SIGNAL(triggered()), this, SLOT(showDisplay3()));

	m_pDisplay4 = new QAction(QIcon(tr(":/action/resources/libuidisplay.png")), tr("DISPLAY 4"), this);
	connect(m_pDisplay4, SIGNAL(triggered()), this, SLOT(showDisplay4()));

	
	astring strImage;
	
	if (m_pFactory.GetConfDB().GetCmnParam(m_strId, strImage) == FALSE)
	{
		QImage image;
		image.load(":/action/resources/defaultmap2.jpg");
		ui.widget->setImage(image);
		astring strImage;
		QBuffer bufImage;
		image.save(&bufImage, "JPG", 100);
		QByteArray data = bufImage.data();
		strImage.assign((const char *)data.data(), data.size());
		m_pFactory.GetConfDB().SetCmnParam(m_strId, strImage);
	}
	
	QImage image;
	if (strImage.length()  > 0)
	{
		bool ret = image.loadFromData((const uchar*)strImage.c_str(), strImage.length(), "JPEG");
		ui.widget->setImage(image);
	}
	//VSCEmapCam * pCamera = new  VSCEmapCam(this);
	//pCamera->setGeometry(100, 100, pCamera->width(), pCamera->height());
	updateCameras();

	m_TimerResize = new QTimer(this);
	connect(m_TimerResize, SIGNAL(timeout()),this,SLOT(ResizeEventTimer())); 
	m_TimerResize->start(100); 
#ifdef WIN32
   	setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif	
}

VSCEMap::~VSCEMap()
{
	m_TimerResize->stop(); 
	delete m_TimerResize;
}

void VSCEMap::ResizeEventTimer()
{
	updateCameras();
	m_TimerResize->stop(); 
}

void VSCEMap::updateCameras()
{
	VidEmap pData;
	if (m_pFactory.GetConfDB().GetEmapConf(m_strId, pData) == false)
	{
		return;
	}

	int camSize = pData.ccamera_size();

	for (s32 i = 0; i < pData.ccamera_size(); i ++)
	{
		VidEmapCamera cam = pData.ccamera(i);

		updateCameraPos(cam);
	}
}

void VSCEMap::dragEnterEvent(QDragEnterEvent *event)
{
    VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__);
    event->accept();
}

void VSCEMap::dragMoveEvent(QDragMoveEvent *event)
{
    VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__);

    event->accept();
}

void VSCEMap::dropEvent(QDropEvent *event)
{
	QMimeData * pMime = (QMimeData *)event->mimeData();

	VSCQMimeVidCam *pVidCam = dynamic_cast<VSCQMimeVidCam * > (pMime);
	if (pVidCam)
	{
	    	QPoint position = event->pos();
		s32 x = position.x();
		s32 y = position.y();
		Lock();
		VSCCamMap::iterator it = m_CamMap.find(pVidCam->m_strCamId), ite = m_CamMap.end();
		if (it != ite)
		{
			//delCamera(nPlayId);
		}
		UnLock();

		VidCameraId cId;
		cId.set_strcameraid(pVidCam->m_strCamId);
		cId.set_strstorid(pVidCam->m_strStorId);
		
		addCamera(cId, x, y);
	       return;		
	}
}


void VSCEMap::addCamera(VidCameraId cId, s32 x, s32 y)
{
	s32 w; s32 h; s32 wScale; s32 hScale;

	ui.widget->getImageSize(w, h, wScale, hScale);

	s32 xImage = (width() - wScale)/2;
	s32 yImage = (height() - hScale)/2;

	if (x < xImage)
	{
		x = xImage;
	}

	if (x > xImage + wScale)
	{
		x = xImage + wScale;
	}

	if (y < yImage)
	{
		y = yImage;
	}

	if (y > yImage + hScale)
	{
		y = yImage + hScale;
	}

	float wRatio =(float) w / (float)wScale;
	float hRatio = (float) h / (float)hScale;

	s32 xReal = (float)(x - xImage) * wRatio;
	s32 yReal = (float)(y - yImage) * hRatio;

	VidCameraId * pCam = new VidCameraId;
	*pCam = cId;
	VidEmapCamera cCam;
	cCam.set_allocated_cid(pCam);
	cCam.set_x(xReal);
	cCam.set_y(yReal);
	cCam.set_w(w);
	cCam.set_h(h);

	m_pFactory.AddEmapCamera(m_strId, cCam);
	updateCameraPos(cCam);

	return;
	

}

void VSCEMap::delCamera(std::string strId)
{
	VSCEmapCam * pCamera = NULL;

	Lock();
        VSCCamMap::iterator it = m_CamMap.find(strId), ite = m_CamMap.end();
        if (it == ite)
        {
        	UnLock();
		return;
        }else
        {
        	pCamera = m_CamMap[strId];
        }

	m_pFactory.DelEmapCamera(m_strId, strId);
	m_CamMap.erase(strId);
	UnLock();

	delete pCamera;

	updateCameras();
	return;
}

void VSCEMap::updateCameraPos(VidEmapCamera &cCam)
{
	s32 w; s32 h; s32 wScale; s32 hScale;

	VSCEmapCam * pCamera = NULL;

	ui.widget->getImageSize(w, h, wScale, hScale);
	s32 xImage = (width() - wScale)/2;
	s32 yImage = (height() - hScale)/2;

	float wRatio = (float)wScale / (float) w ;
	float hRatio =  (float)hScale / (float) h;

	float ratio;
	if (xImage == 0)
	{
		ratio = wRatio;
	}else
	{
		ratio = hRatio;
	}

	u32 xReal; u32 yReal; u32 wReal; u32 hReal;

	xReal = cCam.x();
	yReal = cCam.y();
	wReal = cCam.w();
	hReal = cCam.h();

	if (w != wReal || h != hReal)
	{
		wReal = w;
		hReal = h;
	}
	Lock();
    VSCCamMap::iterator it = m_CamMap.find(cCam.cid().strcameraid()), ite = m_CamMap.end();
       if (it == ite)
       {
		pCamera = new  VSCEmapCam(cCam, m_pFactory, this);
		connect(pCamera, SIGNAL(EmapCamDel(std::string)), this, SLOT(delCamera(std::string)));
		m_CamMap[cCam.cid().strcameraid()] = pCamera;
       }else
       {
		   pCamera = m_CamMap[cCam.cid().strcameraid()];
       }
	UnLock();

	s32 x = xReal * wRatio + xImage; 
	s32 y = yReal * hRatio + yImage;
	pCamera->show();

	pCamera->setGeometry(x, y, pCamera->width(), pCamera->height());
	

	return;
	 
}

void VSCEMap::resizeEvent(QResizeEvent* event)
{
	updateCameras();
}

void VSCEMap::SetupConnections()
{

}
void VSCEMap::wheelEvent ( QWheelEvent * event )
{
#if 0
	float scale =(event->delta()/120); //or use any other step for zooming 
	if(scale > 0)
	{
		ui.widget->slotZoomIn();
	}else
	{
		ui.widget->slotZoomOut();
	}
#endif
}

void VSCEMap::paintEvent(QPaintEvent* event)
{

}

void VSCEMap::LoadEmapAction()
{
	QFileDialog *fileDialog = new QFileDialog(this); 
	fileDialog->setWindowTitle(tr("Select Map")); 
	fileDialog->setOption( QFileDialog::DontUseNativeDialog, true );
	//fileDialog->setDirectory(ui.fileLoc->text()); 
	fileDialog->setNameFilter(tr("Video Files(*.jpg *.jpeg *.png)")); 
	QIcon icon;
	icon.addFile(QStringLiteral(":/action/resources/libuivsc32.png"), QSize(), QIcon::Normal, QIcon::Off);
	fileDialog->setWindowIcon(icon);
	if(fileDialog->exec() == QDialog::Accepted) { 
		QString path = fileDialog->selectedFiles()[0]; 
		QImage image(path);
		ui.widget->setImage(image);
		astring strImage;
		QBuffer bufImage;
		image.save(&bufImage, "JPG", 100);
		QByteArray data = bufImage.data();
		strImage.assign((const char *)data.data(), data.size());
		m_pFactory.GetConfDB().SetCmnParam(m_strId, strImage);
		
		VDC_DEBUG( "%s  QFileDialog %s\n",__FUNCTION__, path.toStdString().c_str());
	} else 
	{ 
	
	}
}


void VSCEMap::Help()
{
        QMessageBox msgBox(this);
        //Set text
        msgBox.setText(tr("Drag and drop camera to here ...\n"));
            //Set predefined icon, icon is show on left side of text.
	msgBox.setWindowTitle(tr("Help"));
        msgBox.setIconPixmap(QPixmap(":/action/resources/libuivsc32.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
            //Set focus of ok button
        msgBox.setDefaultButton(QMessageBox::Ok);

        int ret = msgBox.exec();
}

void VSCEMap::contextMenuEvent(QContextMenuEvent* e) 
{
    QDesktopWidget *desktop = QApplication::desktop();
	
    popupMenu = new QMenu(this);
    popupMenu->addAction(m_pLoadFile);

    popupMenu->addSeparator();

    popupMenu->addAction(m_pFloating);
    popupMenu->addAction(m_pTabbed);
    popupMenu->addSeparator();
    
    //this->addAction(m_pRecord);
    //this->addAction(m_pPlayback);

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
	
    popupMenu->exec(e->globalPos()); 
    delete popupMenu;
    popupMenu = NULL;
}

void VSCEMap::OnDeviceEvent(int deviceId, int type)
{
#if 0
	VSCEmapCam * pCamera = NULL;

	Lock();
        VSCCamMap::iterator it = m_CamMap.find(deviceId), ite = m_CamMap.end();
        if (it == ite)
        {
        	UnLock();
		return;
        }else
        {
        	pCamera = m_CamMap[deviceId];
        }

	pCamera->OnDeviceEvent(deviceId, type);
	UnLock();
#endif
	return;
}

void VSCEMap::FloatingClicked()
{
    if (m_bFloated == TRUE)
    {
        return;
    }

    m_pTabWidget.setCurrentWidget(this);
    int thisIndex = m_pTabWidget.currentIndex();

    m_pTabWidget.removeTab(thisIndex);
    setParent(NULL);
    showMaximized();
    m_bFloated = TRUE;
}

void VSCEMap::TabbedClicked()
{
    if (m_bFloated == FALSE)
    {
        return;
    }
	astring strEmapName;
	VidEmap cEmap;
	m_pFactory.GetConfDB().GetEmapConf(m_strId, cEmap);

	strEmapName = cEmap.strname();
    setParent(m_pParent);
    m_pTabWidget.addTab(this,QIcon(tr(":/action/resources/libuimap.png")), strEmapName.c_str());
    m_pTabWidget.setCurrentWidget(this);
    showMaximized();

    m_bFloated = FALSE;
}

void VSCEMap::showDisplay1()
{
    ShowDisplayClicked(0);
}
void VSCEMap::showDisplay2()
{
    ShowDisplayClicked(1);
}
void VSCEMap::showDisplay3()
{
    ShowDisplayClicked(2);
}

void VSCEMap::showDisplay4()
{
    ShowDisplayClicked(3);
}

void VSCEMap::ShowDisplayClicked(int nId)
{
    if (m_bFloated == FALSE)
    {
        return;
    }

    QDesktopWidget *desktop = QApplication::desktop();
    if ( 1==desktop->screenCount()  ) {
        // single monitor - use built in
        showFullScreen();
    } else {
        QRect rect = desktop->screenGeometry(nId);
        //move(rect.topLeft());
        setGeometry(desktop->screenGeometry(nId));
        setWindowState(Qt::WindowFullScreen | windowState());
        move(rect.topLeft());
        resize(rect.width(), rect.height());
    }

}


VEMap::VEMap(QWidget *parent)
:QWidget(parent)
{

}

VEMap::~VEMap()
{

}

VEMap * VEMap::CreateObject(std::string strId, ClientFactory &pFactory, QTabWidget &pTabbed, QWidget *parent)
{
	return new VSCEMap(strId, pFactory, pTabbed, parent);
}

