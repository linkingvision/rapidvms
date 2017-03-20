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
#include "config/vidview/vidviewconf.h"
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

using namespace Poco;

VidViewConf::VidViewConf(ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
	: m_pFactory(pFactory), QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);
	ui.scrollArea->setBackgroundRole(QPalette::Light);

	TreeWidgetUpdate();

	/* Setup connection */
	connect(ui.pbDelete, SIGNAL(clicked()), this, SLOT(SlotDeleteView()));
}

VidViewConf::~VidViewConf()
{

}

void VidViewConf::SlotDeleteView()
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidViewConfTableItem *pView = dynamic_cast<VidViewConfTableItem * >(firstCheck);
	/* Only the new is false need call the factory */
	if (pView)
	{
		m_pFactory.DelView(pView->GetView().strid());
	}

	ui.tableWidget->removeRow(ui.tableWidget->currentRow());
}

void VidViewConf::TreeWidgetUpdate()
{
	VidViewList viewList;
	m_pFactory.GetConfDB().GetViewListConf(viewList);
	int storSize = viewList.cvidview_size();
	
	int nRowCnt = ui.tableWidget->rowCount();

	for (s32 j = 0; j < nRowCnt; j ++)
	{
		ui.tableWidget->removeRow(j);
	}
	ui.tableWidget->clear();

	
	for (s32 i = 0; i < viewList.cvidview_size(); i ++)
	{
		VidView pView = viewList.cvidview(i);
		//int insertRow = ui.tableWidget->rowCount();
		int insertRow = i;
    		ui.tableWidget->insertRow(insertRow);
    		QTableWidgetItem *firstCheck = new VidViewConfTableItem(pView, false);
    		firstCheck->setCheckState(Qt::Checked);
		ui.tableWidget->setItem(insertRow, 0, firstCheck);
		ui.tableWidget->setItem(insertRow, 2, new QTableWidgetItem(pView.strname().c_str()));

		VideoWallLayoutMode nMode = (VideoWallLayoutMode)(pView.clayout());
		QIcon icon1;
		switch(nMode)
		{
		    case LAYOUT_MODE_2X2:
		        icon1.addFile(QStringLiteral(":/view/resources/2x2.png"), 
		            QSize(), QIcon::Normal, QIcon::Off);
		        break;
		    case LAYOUT_MODE_3X3:
		        icon1.addFile(QStringLiteral(":/view/resources/3x3.png"), 
		            QSize(), QIcon::Normal, QIcon::Off);
		        break;
		    case LAYOUT_MODE_4X4:
		        icon1.addFile(QStringLiteral(":/view/resources/4x4.png"), 
		            QSize(), QIcon::Normal, QIcon::Off);
		        break;
		    case LAYOUT_MODE_1:
		        icon1.addFile(QStringLiteral(":/view/resources/1x1.png"), 
		            QSize(), QIcon::Normal, QIcon::Off);
		        break;
		    case LAYOUT_MODE_6:
		        icon1.addFile(QStringLiteral(":/view/resources/6.png"), 
		            QSize(), QIcon::Normal, QIcon::Off);
		        break;
		    case LAYOUT_MODE_12p1:
		        icon1.addFile(QStringLiteral(":/view/resources/12+1.png"), 
		            QSize(), QIcon::Normal, QIcon::Off);
		        break;
		    case LAYOUT_MODE_5x5:
		        icon1.addFile(QStringLiteral(":/view/resources/5x5.png"), 
		            QSize(), QIcon::Normal, QIcon::Off);
		        break;
		    case LAYOUT_MODE_6x6:
		        icon1.addFile(QStringLiteral(":/view/resources/6x6.png"), 
		            QSize(), QIcon::Normal, QIcon::Off);
		        break;
		    case LAYOUT_MODE_8x8:
		        icon1.addFile(QStringLiteral(":/view/resources/8x8.png"), 
		            QSize(), QIcon::Normal, QIcon::Off);
		        break;
		    case LAYOUT_MODE_1x3:
		        icon1.addFile(QStringLiteral(":/view/resources/1x3.png"), 
		            QSize(), QIcon::Normal, QIcon::Off);
		        break;
		    case LAYOUT_MODE_1p6:
		        icon1.addFile(QStringLiteral(":/view/resources/1p6.png"), 
		            QSize(), QIcon::Normal, QIcon::Off);
		        break;
		    case LAYOUT_MODE_ONE:
		        break;
		    default:
		        break;
		}
		ui.tableWidget->setItem(insertRow, 1, new QTableWidgetItem(icon1, ""));

	}
}



