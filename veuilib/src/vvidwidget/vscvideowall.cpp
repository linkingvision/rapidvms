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
#include "vscvideowall.h"
#include "vscvwidget.h"
#include "server/factory.hpp"
#include <QLineEdit>
#include <QAction>
#include <QtWidgets/QApplication>

VSCVideoWall::VSCVideoWall(ClientFactory &pFactory, QWidget *parent, VVidGetHWDec *pGetHWDec)
: m_pFactory(pFactory), m_bFloated(FALSE), QWidget(parent)
{
    //setMinimumWidth(800);
    //setMinimumHeight(600);
    m_VideoWallMode = LAYOUT_MODE_4X4;
    m_LastVideoWallMode = LAYOUT_MODE_4X4;
    for (int i = 0; i < VIDEO_WALL_WIDGET_MAX; i ++)
    {
    	//if (i%1 == 0)
    	{
    		QCoreApplication::processEvents();
    	}
        m_VideoMap[i] = new VSCVWidget(m_pFactory, i, this, false, 0, pGetHWDec);
        connect(m_VideoMap[i], SIGNAL(ShowDisplayClicked(int)), this,
                SIGNAL(ShowDisplayClicked(int)));
        connect(m_VideoMap[i], SIGNAL(ShowFloatingClicked()), this,
                SIGNAL(ShowFloatingClicked()));
        connect(m_VideoMap[i], SIGNAL(ShowTabbedClicked()), this,
                SIGNAL(ShowTabbedClicked()));
        connect(m_VideoMap[i], SIGNAL(ShowControlPanelClicked()), this,
                SIGNAL(ShowControlPanelClicked()));
        connect(m_VideoMap[i], SIGNAL(ShowFocusClicked(int)), this,
                SIGNAL(ShowFocusClicked(int)));
        connect(m_VideoMap[i], SIGNAL(Layout1Clicked(int)), this,
                SIGNAL(Layout1Clicked(int)));
        connect(m_VideoMap[i], SIGNAL(VideoSwitchWith(int, int)), this,
                SLOT(VideoSwitchWith(int, int))); 
        connect(m_VideoMap[i], SIGNAL(ShowViewClicked(std::string)), this,
                SIGNAL(ShowViewClicked(std::string))); 
        connect(m_VideoMap[i], SIGNAL(PlaybackClicked(std::string, std::string, std::string)), this,
                SIGNAL(PlaybackClicked(std::string, std::string, std::string)));
        connect(m_VideoMap[i], SIGNAL(MotionDetectClicked(std::string, std::string, std::string)), this,
                SIGNAL(MotionDetectClicked(std::string, std::string, std::string)));
    }

    m_pLayout = new QGridLayout;
    m_pLayout->setSpacing(0);
    m_pLayout->setMargin(0);
    m_pLayout->setVerticalSpacing(0);
    m_pLayout->setHorizontalSpacing(0);
    
    setLayout(m_pLayout);

    UpdateVideoWallLayout();
    setAcceptDrops(true);
    setMouseTracking(true);
}

VSCVideoWall::~VSCVideoWall()
{
    VDC_DEBUG( "%s ~VSCVideoWall\n",__FUNCTION__);
}


void VSCVideoWall::VideoSwitchWith(int nSrcId, int nDstId)
{
    VSCVWidget * src = m_VideoMap[nSrcId];
    VSCVWidget * dst = m_VideoMap[nDstId];
    m_VideoMap[nSrcId] = dst;
    m_VideoMap[nDstId] = src;
    dst->setId(nSrcId);
    src->setId(nDstId);
    UpdateVideoWallLayout2();

}

void VSCVideoWall::createContentMenu()
{
    //this->addAction(m_pFloating);
    //this->addAction(m_pUnFloating);
    //this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void VSCVideoWall::OffAllFocus()
{
    for (int i = 0; i < VIDEO_WALL_WIDGET_MAX; i ++)
    {
        m_VideoMap[i]->SetVideoFocus(FALSE);
    }
    return;
}

void VSCVideoWall::ShowVideoInfo(bool bEnable)
{
    for (int i = 0; i < VIDEO_WALL_WIDGET_MAX; i ++)
    {
        m_VideoMap[i]->ShowVideoInfo(bEnable);
    }
    return;
}

void VSCVideoWall::ResizeAllVideo()
{
    for (int i = 0; i < VIDEO_WALL_WIDGET_MAX; i ++)
    {
        m_VideoMap[i]->videoResizeEvent();
    }
    return;
}

void VSCVideoWall::DeviceEvent(int deviceId, VscEventType type)
{
    for (int i = 0; i < VIDEO_WALL_WIDGET_MAX; i ++)
    {
        //m_VideoMap[i]->DeviceEvent(deviceId, type);
    }
    return;
}

void VSCVideoWall::ClearVideoLayout()
{
    VDC_DEBUG( "%s Layout count before clearing it: %d\n",__FUNCTION__,
            m_pLayout->count());

    int count = m_pLayout->count();
    int colums = m_pLayout->columnCount();
    int rows = m_pLayout->rowCount();

    int i=0;
    for(int j=0; j<rows; j++)
    {
        for(int k=0; k<colums && i<count; k++)
        {
            i++;
            VDC_DEBUG( "%s Removing item at: %d %d\n",__FUNCTION__,
                    j, k);
            QLayoutItem* item = m_pLayout->itemAtPosition(j, k);

            if (!item) continue;

            if (item->widget()) {
                m_pLayout->removeWidget(item->widget());
            } else {
                m_pLayout->removeItem(item);
            }
        }
    }
    VDC_DEBUG( "%s Layout count after clearing it: %d\n",__FUNCTION__,
            m_pLayout->count());
}


void VSCVideoWall::SetupVideoLayout4x4()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0);
    m_pLayout->addWidget(m_VideoMap[1], 0, 1);
    m_pLayout->addWidget(m_VideoMap[2], 0, 2);
    m_pLayout->addWidget(m_VideoMap[3], 0, 3);

    m_pLayout->addWidget(m_VideoMap[4], 1, 0);
    m_pLayout->addWidget(m_VideoMap[5], 1, 1);
    m_pLayout->addWidget(m_VideoMap[6], 1, 2);
    m_pLayout->addWidget(m_VideoMap[7], 1, 3);

    m_pLayout->addWidget(m_VideoMap[8], 2, 0);
    m_pLayout->addWidget(m_VideoMap[9], 2, 1);
    m_pLayout->addWidget(m_VideoMap[10], 2, 2);
    m_pLayout->addWidget(m_VideoMap[11], 2, 3);

    m_pLayout->addWidget(m_VideoMap[12], 3, 0);
    m_pLayout->addWidget(m_VideoMap[13], 3, 1);
    m_pLayout->addWidget(m_VideoMap[14], 3, 2);
    m_pLayout->addWidget(m_VideoMap[15], 3, 3);
}

void VSCVideoWall::SetupVideoLayout2x2()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0);
    m_pLayout->addWidget(m_VideoMap[1], 0, 1);

    m_pLayout->addWidget(m_VideoMap[2], 1, 0);
    m_pLayout->addWidget(m_VideoMap[3], 1, 1);
}

void VSCVideoWall::SetupVideoLayout3x3()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0);
    m_pLayout->addWidget(m_VideoMap[1], 0, 1);
    m_pLayout->addWidget(m_VideoMap[2], 0, 2);


    m_pLayout->addWidget(m_VideoMap[3], 1, 0);
    m_pLayout->addWidget(m_VideoMap[4], 1, 1);
    m_pLayout->addWidget(m_VideoMap[5], 1, 2);

    m_pLayout->addWidget(m_VideoMap[6], 2, 0);
    m_pLayout->addWidget(m_VideoMap[7], 2, 1);
    m_pLayout->addWidget(m_VideoMap[8], 2, 2);
}

void VSCVideoWall::SetupVideoLayout1()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0);
}
void VSCVideoWall::SetupVideoLayout12p1()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0);
    m_pLayout->addWidget(m_VideoMap[1], 0, 1);
    m_pLayout->addWidget(m_VideoMap[2], 0, 2);
    m_pLayout->addWidget(m_VideoMap[3], 0, 3);

    m_pLayout->addWidget(m_VideoMap[4], 1, 0);
    m_pLayout->addWidget(m_VideoMap[5], 1, 1, 2, 2);
    m_pLayout->addWidget(m_VideoMap[6], 1, 3);

    m_pLayout->addWidget(m_VideoMap[7], 2, 0);
    m_pLayout->addWidget(m_VideoMap[8], 2, 3);

    m_pLayout->addWidget(m_VideoMap[9], 3, 0);
    m_pLayout->addWidget(m_VideoMap[10], 3, 1);
    m_pLayout->addWidget(m_VideoMap[11], 3, 2);
    m_pLayout->addWidget(m_VideoMap[12], 3, 3);
}
void VSCVideoWall::SetupVideoLayout6()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0, 2, 2);
    m_pLayout->addWidget(m_VideoMap[1], 0, 2);
    m_pLayout->addWidget(m_VideoMap[2], 1, 2);
    m_pLayout->addWidget(m_VideoMap[3], 2, 0);
    m_pLayout->addWidget(m_VideoMap[4], 2, 1);
    m_pLayout->addWidget(m_VideoMap[5], 2, 2);
}

void VSCVideoWall::SetupVideoLayout5x5()
{
    s32 w, h;
    int i = 0;
    for (w = 0; w < 5 ; w ++)
    {
        for (h = 0; h < 5; h ++)
        {
            m_pLayout->addWidget(m_VideoMap[i++], w, h);
	         VDC_DEBUG( "%s 5x5 w %d h %d\n",__FUNCTION__, w, h);
        }
    }
}
void VSCVideoWall::SetupVideoLayout6x6()
{
    s32 w, h;
    int i = 0;
    for (w = 0; w < 6 ; w ++)
    {
        for (h = 0; h < 6; h ++)
        {
            m_pLayout->addWidget(m_VideoMap[i++], w, h);
        }
    }
}
void VSCVideoWall::SetupVideoLayout8x8()
{
    s32 w, h;
    int i = 0;
    for (w = 0; w < 8 ; w ++)
    {
        for (h = 0; h < 8; h ++)
        {
            m_pLayout->addWidget(m_VideoMap[i++], w, h);
        }
    }
}

void VSCVideoWall::SetupVideoLayout1x3()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0);
    m_pLayout->addWidget(m_VideoMap[1], 0, 1);
    m_pLayout->addWidget(m_VideoMap[2], 0, 2);
}


void VSCVideoWall::SetupVideoLayout1p6()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0, 3, 1);
    m_pLayout->addWidget(m_VideoMap[1], 0, 1);
    m_pLayout->addWidget(m_VideoMap[2], 0, 2);
    m_pLayout->addWidget(m_VideoMap[3], 1, 1);
    m_pLayout->addWidget(m_VideoMap[4], 1, 2);
    m_pLayout->addWidget(m_VideoMap[5], 2, 1);
    m_pLayout->addWidget(m_VideoMap[6], 2, 2);
}

void VSCVideoWall::SetLayoutMode(VideoWallLayoutMode nMode)
{
    if (nMode != m_VideoWallMode)
    {
        m_VideoWallMode = nMode;
        UpdateVideoWallLayout();
    }

    return;
}

void VSCVideoWall::SetLayout1Mode(int nId)
{
    if (m_VideoWallMode == LAYOUT_MODE_ONE)
    {
        SetLayoutMode(m_LastVideoWallMode);
	 m_LastVideoWallMode = LAYOUT_MODE_2X2;
    }else
    {
        m_LastVideoWallMode = m_VideoWallMode;
        m_VideoWallMode = LAYOUT_MODE_ONE;
        StopVideoBeforeSetLayout();
        ClearVideoLayout();
        m_pLayout->addWidget(m_VideoMap[nId], 0, 0);
        m_VideoMap[nId]->show();
        //m_VideoMap[nId]->ChangeLayout();
    }
}

void VSCVideoWall::StopVideoBeforeSetLayout()
{
    int videoCnt = 0;
    switch(m_VideoWallMode)
    {
        case LAYOUT_MODE_2X2:
            videoCnt = 4;
            break;
        case LAYOUT_MODE_3X3:
            videoCnt = 9;
            break;
        case LAYOUT_MODE_4X4:
            videoCnt = 16;
            break;
        case LAYOUT_MODE_1:
            videoCnt = 1;
            break;
        case LAYOUT_MODE_6:
            videoCnt = 6;
            break;
        case LAYOUT_MODE_12p1:
            videoCnt = 13;
            break;
        case LAYOUT_MODE_5x5:
            videoCnt = 25;
            break;
        case LAYOUT_MODE_6x6:
            videoCnt = 36;
            break;
        case LAYOUT_MODE_8x8:
            videoCnt = 64;
            break;
        case LAYOUT_MODE_1x3:
            videoCnt = 3;
            break;
        case LAYOUT_MODE_1p6:
            videoCnt = 7;
            break;
        case LAYOUT_MODE_ONE:
            videoCnt = 0;
            break;
        default:
            break;
    }
    for (int i = videoCnt; i < VIDEO_WALL_WIDGET_MAX; i ++)
    {
        //m_VideoMap[i]->StopPlay();
        m_VideoMap[i]->hide();
    }
    for (int i = 0; i < videoCnt; i ++)
    {
        m_VideoMap[i]->show();
        //m_VideoMap[i]->ChangeLayout();
    }

}

void VSCVideoWall::GetPlayMap(std::map<int, VidViewWindow> &playMap)
{
	int videoCnt = 0;

	switch(m_VideoWallMode)
	{
	    case LAYOUT_MODE_2X2:
	        videoCnt = 4;
	        break;
	    case LAYOUT_MODE_3X3:
	        videoCnt = 9;
	        break;
	    case LAYOUT_MODE_4X4:
	        videoCnt = 16;
	        break;
	    case LAYOUT_MODE_1:
	        videoCnt = 1;
	        break;
	    case LAYOUT_MODE_6:
	        videoCnt = 6;
	        break;
	    case LAYOUT_MODE_12p1:
	        videoCnt = 13;
	        break;
	    case LAYOUT_MODE_5x5:
	        videoCnt = 25;
	        break;
	    case LAYOUT_MODE_6x6:
	        videoCnt = 36;
	        break;
	    case LAYOUT_MODE_8x8:
	        videoCnt = 64;
	        break;
	    case LAYOUT_MODE_1x3:
	        videoCnt = 3;
	        break;
	    case LAYOUT_MODE_1p6:
	        videoCnt = 7;
	        break;
	    case LAYOUT_MODE_ONE:
	        videoCnt = 0;
	        break;
	    default:
	        break;
	}
	for (int i = 0; i < videoCnt; i ++)
	{
		astring strStorId;
		astring strCamId;
		bool bHWAccel = false;
		if (m_VideoMap[i]->GetPlayParam(strStorId, strCamId, bHWAccel) == true)
		{
			VidViewWindow win;
			VidCameraId * pCam = new VidCameraId;
			pCam->set_strstorid(strStorId);
			pCam->set_strcameraid(strCamId);
	
			win.set_nwindowsid(i);
			win.set_allocated_cid(pCam);
			win.set_bhwaccel(bHWAccel);
			playMap[i] = win;
		}
	}
}

void VSCVideoWall::PlayVideoByWindow(u32 nWindow, astring strStor, 
						astring strCam, astring strCamName)
{
	if (nWindow >= VIDEO_WALL_WIDGET_MAX)
	{
		return;
	}
	m_VideoMap[nWindow]->StopPlay();
	m_VideoMap[nWindow]->StartPlay(strStor, strCam, strCamName);

	return;
}

void VSCVideoWall::SetPlayMap(std::map<int, VidViewWindow> &playMap, VideoWallLayoutMode nMode)
{
	int videoCnt = 0;
	
	SetLayoutMode(nMode);
	switch(nMode)
	{
	    case LAYOUT_MODE_2X2:
	        videoCnt = 4;
	        break;
	    case LAYOUT_MODE_3X3:
	        videoCnt = 9;
	        break;
	    case LAYOUT_MODE_4X4:
	        videoCnt = 16;
	        break;
	    case LAYOUT_MODE_1:
	        videoCnt = 1;
	        break;
	    case LAYOUT_MODE_6:
	        videoCnt = 6;
	        break;
	    case LAYOUT_MODE_12p1:
	        videoCnt = 13;
	        break;
	    case LAYOUT_MODE_5x5:
	        videoCnt = 25;
	        break;
	    case LAYOUT_MODE_6x6:
	        videoCnt = 36;
	        break;
	    case LAYOUT_MODE_8x8:
	        videoCnt = 64;
	        break;
	    case LAYOUT_MODE_1x3:
	        videoCnt = 3;
	        break;
	    case LAYOUT_MODE_1p6:
	        videoCnt = 7;
	        break;
	    case LAYOUT_MODE_ONE:
	        videoCnt = 0;
	        break;
	    default:
	        break;
	}
	for (int i = 0; i < VIDEO_WALL_WIDGET_MAX; i ++)
	{
	    m_VideoMap[i]->StopPlay();
	}

	std::map<int, VidViewWindow>::iterator it = playMap.begin(); 
	for(; it!=playMap.end(); ++it)
	{
		VidViewWindow win = (*it).second;
		//TODO Get the name of the Camera
		m_VideoMap[win.nwindowsid()]->StartPlay(win.cid().strstorid(), 
		win.cid().strcameraid(), "", false, true, win.bhwaccel());
	}
}

void VSCVideoWall::UpdateVideoWallLayout2()
{
    ClearVideoLayout();
    switch(m_VideoWallMode)
    {
        case LAYOUT_MODE_2X2:
            SetupVideoLayout2x2();
            break;
        case LAYOUT_MODE_3X3:
            SetupVideoLayout3x3();
            break;
        case LAYOUT_MODE_4X4:
            SetupVideoLayout4x4();
            break;
        case LAYOUT_MODE_1:
            SetupVideoLayout1();
            break;
        case LAYOUT_MODE_6:
            SetupVideoLayout6();
            break;
        case LAYOUT_MODE_12p1:
            SetupVideoLayout12p1();
            break;
        case LAYOUT_MODE_5x5:
            SetupVideoLayout5x5();
            break;
        case LAYOUT_MODE_6x6:
            SetupVideoLayout6x6();
            break;
        case LAYOUT_MODE_8x8:
            SetupVideoLayout8x8();
            break;
        case LAYOUT_MODE_1x3:
            SetupVideoLayout1x3();
            break;
        case LAYOUT_MODE_1p6:
            SetupVideoLayout1p6();
            break;
        default:
            break;
    }

    return;
}


void VSCVideoWall::UpdateVideoWallLayout()
{
    StopVideoBeforeSetLayout();
    ClearVideoLayout();
    switch(m_VideoWallMode)
    {
        case LAYOUT_MODE_2X2:
            SetupVideoLayout2x2();
            break;
        case LAYOUT_MODE_3X3:
            SetupVideoLayout3x3();
            break;
        case LAYOUT_MODE_4X4:
            SetupVideoLayout4x4();
            break;
        case LAYOUT_MODE_1:
            SetupVideoLayout1();
            break;
        case LAYOUT_MODE_6:
            SetupVideoLayout6();
            break;
        case LAYOUT_MODE_12p1:
            SetupVideoLayout12p1();
            break;
        case LAYOUT_MODE_5x5:
            SetupVideoLayout5x5();
            break;
        case LAYOUT_MODE_6x6:
            SetupVideoLayout6x6();
            break;
        case LAYOUT_MODE_8x8:
            SetupVideoLayout8x8();
            break;
        case LAYOUT_MODE_1x3:
            SetupVideoLayout1x3();
            break;
        case LAYOUT_MODE_1p6:
            SetupVideoLayout1p6();
            break;
        default:
            break;
    }

    return;
}

void VSCVideoWall::SetVideoFocus(int nId, BOOL on)
{
    if (nId > VIDEO_WALL_WIDGET_MAX || nId < 0)
    {
        return;
    }
    m_VideoMap[nId]->SetVideoFocus(on);
    //m_VideoMap[nId]->repaint();
}

void VSCVideoWall::floatingAction()
{
    if (m_bFloated == TRUE)
    {
        return;
    }
    m_pParent = parentWidget();
    setParent(NULL);
    showMaximized();
    m_bFloated = TRUE;
}

void VSCVideoWall::unFloatingAction()
{
    if (m_bFloated == FALSE)
    {
        return;
    }
    showFullScreen();
    setParent(m_pParent);
    resize(m_pParent->width(), m_pParent->height());
    showFullScreen();
    m_bFloated = FALSE;
}

void VSCVideoWall::mouseDoubleClickEvent(QMouseEvent *e)
{
    VDC_DEBUG( "%s mouseDoubleClickEvent\n",__FUNCTION__);
    QWidget::mouseDoubleClickEvent(e);
#if 0
    if(isFullScreen()) {
        //setParent(m_pParent);
        //resize(m_pParent->width(), m_pParent->height());
        //showMaximized();
        this->setWindowState(Qt::WindowMaximized);
    } else {
        //m_pParent = parentWidget();
        //setParent(NULL);
        //showFullScreen();
        this->setWindowState(Qt::WindowFullScreen);
    }
#endif
}

bool VSCVideoWall::Start()
{   
    return true;
}

bool VSCVideoWall::Stop()
{
    return true;
}
