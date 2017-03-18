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
#include "vvidpb1.h"
#include "server/factory.hpp"
#include <QResizeEvent>
#include <QMimeData>
#include <QDrag>
#include <QAction>
#include <QDesktopWidget>
#include <QApplication>
#include <QDateTime>
#include <QTime>
#include <time.h>
#include <QStyleOption>
#include <QPainter>
#include <QDialog>
#include "ui_vvidpb1.h"

//background-color:rgb(42, 43, 44)


VVidPB1::VVidPB1(ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags), m_pFactory(pFactory), p_ui(new Ui::VVidPB1), ui(*p_ui)
{
	ui.setupUi(this);
	setWindowTitle(tr("Rapidvms Playback"));
#ifdef WIN32
	setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif

	QVBoxLayout* layout = new QVBoxLayout();
	m_pVideo = new VVidPBWidget(*(ui.pbControl), m_pFactory, 0, this->ui.video, true);
	connect(m_pVideo, SIGNAL(ShowFocusClicked(int)), this,
	                                    SLOT(ShowFocusClicked(int)));	
	m_pVideo->hide();

	layout->addWidget(m_pVideo);
	layout->setMargin(0);

	this->ui.video->setLayout(layout);
	m_pVideo->ShowVideoInfo(false);
	m_pVideo->show();

}

void VVidPB1::ShowFocusClicked(int nId)
{
	//m_pVideo->SetVideoFocus(TRUE);
}

bool VVidPB1::StartPlay(astring strStorId, astring strCamId, astring strCamName, 
	 u32 nPlaytime)
{
	m_strStor = strStorId;
	m_strCam = strCamId;
	m_strCamName = strCamName;
	setWindowTitle(tr(m_strCamName.c_str()));
	return m_pVideo->StartPlay(strStorId, strCamId, strCamName, nPlaytime);
}

bool VVidPB1::StopPlay()
{	
	m_pVideo->StopPlay();
	return true;
}

VVidPB1::~VVidPB1()
{

}
