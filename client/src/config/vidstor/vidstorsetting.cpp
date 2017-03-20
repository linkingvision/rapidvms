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
#include "config/vidstor/vidstorsetting.h"
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
#include <QScrollArea>
#include "Poco/UUIDGenerator.h"
#include "config/vidstor/storsetting/vscstorversion.h"
#include "config/vidstor/storsetting/vscstoruser.h"
#include "config/vidstor/storsetting/vscstorlicense.h"

using namespace Poco;

VidStorSetting::VidStorSetting(VidStor pStor, ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
	: m_pStor(pStor), m_pFactory(pFactory), QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);
	
	ui.tabWidget->addTab(new VSCStorVersion(pFactory, m_pStor, this), tr("Version"));
	ui.tabWidget->addTab(new VSCStorUser(pFactory, m_pStor, this), tr("User"));
	ui.tabWidget->addTab(new VSCStorLicense(pFactory, m_pStor, this), tr("License"));

	//connect(this, SIGNAL(SignalSectionClicked(int, int)), ui.tableWidget, SIGNAL(cellClicked(int, int)));
	ui.storName->setText(m_pStor.strname().c_str());
}

VidStorSetting::~VidStorSetting()
{

}



