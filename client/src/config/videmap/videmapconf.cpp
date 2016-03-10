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
	p.set_strname("New Emap");
	
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

	for (s32 j = 0; j < nRowCnt; j ++)
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



