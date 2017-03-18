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
#include "vvidpbwall.h"
#include "vvidpbcontrol.h"
#include "vvidpbwidget.h"
#include "server/factory.hpp"
#include <QLineEdit>
#include <QAction>
#include <QtWidgets/QApplication>

VVidPBWall::VVidPBWall(ClientFactory &pFactory, QWidget *parent)
: m_pFactory(pFactory), QWidget(parent)
{
	m_VideoWallMode = LAYOUT_MODE_3X3;
	m_LastVideoWallMode = LAYOUT_MODE_3X3;
	/* Create the control widget */

	m_pVLayout = new QVBoxLayout();
	m_pLayout = new QGridLayout;

	m_pVLayout->addLayout(m_pLayout);
	m_pVLayout->setSpacing(0);
	m_pVLayout->setMargin(0);

	for (int i = 0; i < PB_WALL_WIDGET_MAX; i ++)
	{
		QCoreApplication::processEvents();
		m_VideoControlMap[i] = new VVidPBControl(this);
		m_VideoControlMap[i]->hide();

		m_VideoMap[i] = new VVidPBWidget(*(m_VideoControlMap[i]), m_pFactory, i, this);
		connect(m_VideoMap[i], SIGNAL(ShowFocusClicked(int)), this,
		        SIGNAL(ShowFocusClicked(int)));
		connect(m_VideoMap[i], SIGNAL(Layout1Clicked(int)), this,
		        SIGNAL(Layout1Clicked(int)));
		connect(m_VideoMap[i], SIGNAL(VideoSwitchWith(int, int)), this,
		        SLOT(VideoSwitchWith(int, int))); 
	}

    m_pLayout->setSpacing(0);
    m_pLayout->setMargin(0);
    m_pLayout->setVerticalSpacing(0);
    m_pLayout->setHorizontalSpacing(0);
    
    setLayout(m_pVLayout);

    UpdateVideoWallLayout();
    setAcceptDrops(true);
    setMouseTracking(true);

}

VVidPBWall::~VVidPBWall()
{
    VDC_DEBUG( "%s ~VVidPBWall\n",__FUNCTION__);
}

void VVidPBWall::VideoSwitchWith(int nSrcId, int nDstId)
{
    VVidPBWidget * src = m_VideoMap[nSrcId];
	VVidPBWidget * dst = m_VideoMap[nDstId];
    m_VideoMap[nSrcId] = dst;
    m_VideoMap[nDstId] = src;
    dst->setId(nSrcId);
    src->setId(nDstId);
    UpdateVideoWallLayout2();

}

void VVidPBWall::createContentMenu()
{
    //this->addAction(m_pFloating);
    //this->addAction(m_pUnFloating);
    //this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void VVidPBWall::OffAllFocus()
{
    for (int i = 0; i < PB_WALL_WIDGET_MAX; i ++)
    {
        m_VideoMap[i]->SetVideoFocus(FALSE);
    }
    return;
}

void VVidPBWall::ResizeAllVideo()
{
    for (int i = 0; i < PB_WALL_WIDGET_MAX; i ++)
    {
        m_VideoMap[i]->videoResizeEvent();
    }
    return;
}

void VVidPBWall::DeviceEvent(int deviceId, VscEventType type)
{
    for (int i = 0; i < PB_WALL_WIDGET_MAX; i ++)
    {
        //m_VideoMap[i]->DeviceEvent(deviceId, type);
    }
    return;
}

void VVidPBWall::ClearVideoLayout()
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

void VVidPBWall::ClearControlLayout()
{
    VDC_DEBUG( "%s Layout count before clearing it: %d\n",__FUNCTION__,
            m_pVLayout->count());

	QLayoutItem  *item = m_pVLayout->itemAt(1);
	if (item == NULL)
	{
		return;
	}

	if (item->widget()) {
	    	m_pVLayout->removeWidget(item->widget());
	} else {
	    	m_pVLayout->removeItem(item);
	}

}


void VVidPBWall::SetupVideoLayout4x4()
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

void VVidPBWall::SetupVideoLayout2x2()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0);
    m_pLayout->addWidget(m_VideoMap[1], 0, 1);

    m_pLayout->addWidget(m_VideoMap[2], 1, 0);
    m_pLayout->addWidget(m_VideoMap[3], 1, 1);
}

void VVidPBWall::SetupVideoLayout3x3()
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

void VVidPBWall::SetupVideoLayout1()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0);
}
void VVidPBWall::SetupVideoLayout12p1()
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
void VVidPBWall::SetupVideoLayout6()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0, 2, 2);
    m_pLayout->addWidget(m_VideoMap[1], 0, 2);
    m_pLayout->addWidget(m_VideoMap[2], 1, 2);
    m_pLayout->addWidget(m_VideoMap[3], 2, 0);
    m_pLayout->addWidget(m_VideoMap[4], 2, 1);
    m_pLayout->addWidget(m_VideoMap[5], 2, 2);
}

void VVidPBWall::SetupVideoLayout5x5()
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
void VVidPBWall::SetupVideoLayout6x6()
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
void VVidPBWall::SetupVideoLayout8x8()
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

void VVidPBWall::SetupVideoLayout1x3()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0);
    m_pLayout->addWidget(m_VideoMap[1], 0, 1);
    m_pLayout->addWidget(m_VideoMap[2], 0, 2);
}


void VVidPBWall::SetupVideoLayout1p6()
{
    m_pLayout->addWidget(m_VideoMap[0], 0, 0, 3, 1);
    m_pLayout->addWidget(m_VideoMap[1], 0, 1);
    m_pLayout->addWidget(m_VideoMap[2], 0, 2);
    m_pLayout->addWidget(m_VideoMap[3], 1, 1);
    m_pLayout->addWidget(m_VideoMap[4], 1, 2);
    m_pLayout->addWidget(m_VideoMap[5], 2, 1);
    m_pLayout->addWidget(m_VideoMap[6], 2, 2);
}

void VVidPBWall::SetLayoutMode(VideoWallLayoutMode nMode)
{
    if (nMode != m_VideoWallMode)
    {
        m_VideoWallMode = nMode;
        UpdateVideoWallLayout();
    }

    return;
}

void VVidPBWall::SetLayout1Mode(int nId)
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

void VVidPBWall::StopVideoBeforeSetLayout()
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
    for (int i = videoCnt; i < PB_WALL_WIDGET_MAX; i ++)
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

void VVidPBWall::GetPlayMap(u32 *Map, int length)
{
	int videoCnt = 0;
	for (int i = 0; i < length; i ++)
	{
		Map[i] = 0;
	}
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
	    //Map[i] = m_VideoMap[i]->GetPlayId();
	}
}

void VVidPBWall::PlayVideoByWindow(u32 nWindow, astring strStor, 
						astring strCam, astring strCamName)
{
	if (nWindow >= PB_WALL_WIDGET_MAX)
	{
		return;
	}
	s32 currentTime = time(NULL) - 5 * 60;
	if (currentTime < 0)
	{
		currentTime = 0;
	}

	m_VideoMap[nWindow]->StopPlay();
	m_VideoMap[nWindow]->StartPlay(strStor, strCam, strCamName, currentTime);

	return;
}

void VVidPBWall::SetPlayMap(u32 *Map, int length, VideoWallLayoutMode nMode)
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
	for (int i = 0; i < PB_WALL_WIDGET_MAX; i ++)
	{
	    m_VideoMap[i]->StopPlay();
	}
	for (int i = 0; i < videoCnt; i ++)
	{
	    if (Map[i] > 0)
	    {
	        //m_VideoMap[i]->StartPlayById(Map[i]);
	    }
	}
}

void VVidPBWall::UpdateVideoWallLayout2()
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


void VVidPBWall::UpdateVideoWallLayout()
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

void VVidPBWall::SetVideoFocus(int nId, BOOL on)
{
	if (nId > PB_WALL_WIDGET_MAX || nId < 0)
	{
	    return;
	}
	ClearControlLayout();
	for (int i = 0; i < PB_WALL_WIDGET_MAX; i ++)
	{
		m_VideoControlMap[i]->hide();
	}
    	m_VideoMap[nId]->SetVideoFocus(on);
	m_pVLayout->addWidget(m_VideoControlMap[nId]);
	m_VideoControlMap[nId]->show();
}



void VVidPBWall::mouseDoubleClickEvent(QMouseEvent *e)
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

