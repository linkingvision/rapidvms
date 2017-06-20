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
#include "config/videmap/videmapconf.h"
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

VidEmapConf::VidEmapConf(ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
	: m_pFactory(pFactory), QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);
	ui.scrollArea->setBackgroundRole(QPalette::Light);

	TreeWidgetUpdate();

	/* Setup connection */
	connect(ui.pbNew, SIGNAL(clicked()), this, SLOT(SlotNew()));
	connect(ui.pbDelete, SIGNAL(clicked()), this, SLOT(SlotDelete()));
	connect(ui.pbApply, SIGNAL(clicked()), this, SLOT(SlotApply()));
	connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(SlotCancel()));

	connect(this, SIGNAL(SignalSectionClicked(int, int)), ui.tableWidget, SIGNAL(cellClicked(int, int)));
	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(SlotSectionClicked(int, int)));
}

VidEmapConf::~VidEmapConf()
{

}

void VidEmapConf::SlotNew()
{
	VidEmap p;
	UUIDGenerator uuidCreator;
	astring strId  = uuidCreator.createRandom().toString();
	p.set_strid(strId);
	p.set_strname("NEW EMAP");
	
	int insertRow = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(insertRow);
	QTableWidgetItem *firstCheck = new VidEmapConfTableItem(p, false);
	firstCheck->setCheckState(Qt::Checked);
	ui.tableWidget->setItem(insertRow, 0, firstCheck);
	ui.tableWidget->setItem(insertRow, 1, new QTableWidgetItem(p.strname().c_str()));

	ui.tableWidget->selectRow(insertRow);

	emit(SignalSectionClicked(insertRow, 0));
}

void VidEmapConf::SlotDelete()
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidEmapConfTableItem *p = dynamic_cast<VidEmapConfTableItem * >(firstCheck);
	/* Only the new is false need call the factory */
	if (p && p->IsNew() == false)
	{
		m_pFactory.DelEmap(p->Get().strid());
	}

	ui.tableWidget->removeRow(ui.tableWidget->currentRow());
}
void VidEmapConf::SlotApply()
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidEmapConfTableItem *p = dynamic_cast<VidEmapConfTableItem * >(firstCheck);
	/* Only the new is false need call the factory */
	if (p)
	{
		VidEmap sConf = p->Get();
		GetEmapUI(sConf);
		m_pFactory.AddEmap(sConf);
	}
	TreeWidgetUpdate();
}
void VidEmapConf::SlotCancel()
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidEmapConfTableItem *p = dynamic_cast<VidEmapConfTableItem * >(firstCheck);
	/* Only the new is false need call the factory */
	if (p)
	{
		SetEmapUI(p->Get());
	}
}


/*  Clicked, show the  on the right */
void VidEmapConf::SlotSectionClicked(int row, int column)
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidEmapConfTableItem *p = dynamic_cast<VidEmapConfTableItem * >(firstCheck);
	if (p)
	{
		SetEmapUI(p->Get());
	}
	
}

void VidEmapConf::SetEmapUI(VidEmap p)
{
    ui.lineEditName->setText(p.strname().c_str());
}

void VidEmapConf::GetEmapUI(VidEmap &p)
{
	p.set_strname(ui.lineEditName->text().toStdString());
}


void VidEmapConf::TreeWidgetUpdate()
{
	VidEmapList emapList;
	m_pFactory.GetConfDB().GetEmapListConf(emapList);
	int Size = emapList.cvidemap_size();
	
	int nRowCnt = ui.tableWidget->rowCount();

	for (s32 j = nRowCnt -1 ; j >= 0; j --)
	{
		ui.tableWidget->removeRow(j);
	}
	ui.tableWidget->clear();

	
	for (s32 i = 0; i < emapList.cvidemap_size(); i++)
	{
		VidEmap p = emapList.cvidemap(i);
		//int insertRow = ui.tableWidget->rowCount();
		int insertRow = i;
    		ui.tableWidget->insertRow(insertRow);
    		QTableWidgetItem *firstCheck = new VidEmapConfTableItem(p, false);
    		firstCheck->setCheckState(Qt::Checked);
		ui.tableWidget->setItem(insertRow, 0, firstCheck);
		ui.tableWidget->setItem(insertRow, 1, new QTableWidgetItem(p.strname().c_str()));
	}
}




