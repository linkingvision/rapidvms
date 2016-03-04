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
	connect(ui.pbNew, SIGNAL(clicked()), this, SLOT(SlotNewStor()));
	connect(ui.pbDelete, SIGNAL(clicked()), this, SLOT(SlotDeleteStor()));
	connect(ui.pbApply, SIGNAL(clicked()), this, SLOT(SlotApplyStor()));
	connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(SlotCancelStor()));

	connect(this, SIGNAL(SignalSectionClicked(int, int)), ui.tableWidget, SIGNAL(cellClicked(int, int)));
	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(SlotSectionClicked(int, int)));
}

VidViewConf::~VidViewConf()
{

}

void VidViewConf::SlotNewStor()
{
	VidStor pStor;
	UUIDGenerator uuidCreator;
	astring strId  = uuidCreator.createRandom().toString();
	pStor.set_strid(strId);
	pStor.set_strname("New Stor");
	pStor.set_strip("192.168.0.1");
	pStor.set_strport("9080");
	pStor.set_struser("admin");
	pStor.set_strpasswd("admin");
	
	int insertRow = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(insertRow);
	QTableWidgetItem *firstCheck = new VidViewConfTableItem(pStor, false);
	firstCheck->setCheckState(Qt::Checked);
	ui.tableWidget->setItem(insertRow, 0, firstCheck);
	ui.tableWidget->setItem(insertRow, 1, new QTableWidgetItem(pStor.strname().c_str()));
	ui.tableWidget->setItem(insertRow, 2, new QTableWidgetItem(pStor.strip().c_str()));
	ui.tableWidget->setItem(insertRow, 3, new QTableWidgetItem(pStor.struser().c_str()));

	ui.tableWidget->selectRow(insertRow);

	emit(SignalSectionClicked(insertRow, 0));
}

void VidViewConf::SlotDeleteStor()
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidViewConfTableItem *pStor = dynamic_cast<VidViewConfTableItem * >(firstCheck);
	/* Only the new is false need call the factory */
	if (pStor && pStor->IsNew() == false)
	{
		m_pFactory.GetStorFactory().DeleteStor(pStor->GetStor().strid());
	}

	ui.tableWidget->removeRow(ui.tableWidget->currentRow());
}
void VidViewConf::SlotApplyStor()
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidViewConfTableItem *pStor = dynamic_cast<VidViewConfTableItem * >(firstCheck);
	/* Only the new is false need call the factory */
	if (pStor)
	{
		VidStor sStorConf = pStor->GetStor();
		GetStorUI(sStorConf);
		m_pFactory.GetStorFactory().AddStor(sStorConf);
	}
	TreeWidgetUpdate();
}
void VidViewConf::SlotCancelStor()
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidViewConfTableItem *pStor = dynamic_cast<VidViewConfTableItem * >(firstCheck);
	/* Only the new is false need call the factory */
	if (pStor)
	{
		SetStorUI(pStor->GetStor());
	}
}


/* Stor Clicked, show the stor on the right */
void VidViewConf::SlotSectionClicked(int row, int column)
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidViewConfTableItem *pStor = dynamic_cast<VidViewConfTableItem * >(firstCheck);
	if (pStor)
	{
		SetStorUI(pStor->GetStor());
	}
	
}

void VidViewConf::SetStorUI(VidStor pStor)
{
    ui.lineEditName->setText(pStor.strname().c_str());
    ui.lineEditIP->setText(pStor.strip().c_str());
    ui.lineEditPort->setText(pStor.strport().c_str());
    ui.lineEditUser->setText(pStor.struser().c_str());
    ui.lineEditPassword->setText(pStor.strpasswd().c_str());
}

void VidViewConf::GetStorUI(VidStor &pStor)
{
	pStor.set_strname(ui.lineEditName->text().toStdString());
	pStor.set_strip(ui.lineEditIP->text().toStdString());
	pStor.set_strport(ui.lineEditPort->text().toStdString());
	pStor.set_struser(ui.lineEditUser->text().toStdString());
	pStor.set_strpasswd(ui.lineEditPassword->text().toStdString());
}


void VidViewConf::TreeWidgetUpdate()
{
	VidStorList storList;
	m_pFactory.GetConfDB().GetStorListConf(storList);
	int storSize = storList.cvidstor_size();
	
	int nRowCnt = ui.tableWidget->rowCount();

	for (s32 j = 0; j < nRowCnt; j ++)
	{
		ui.tableWidget->removeRow(j);
	}
	ui.tableWidget->clear();

	
	for (s32 i = 0; i < storList.cvidstor_size(); i ++)
	{
		VidStor pStor = storList.cvidstor(i);
		//int insertRow = ui.tableWidget->rowCount();
		int insertRow = i;
    		ui.tableWidget->insertRow(insertRow);
    		QTableWidgetItem *firstCheck = new VidViewConfTableItem(pStor, false);
    		firstCheck->setCheckState(Qt::Checked);
		ui.tableWidget->setItem(insertRow, 0, firstCheck);
		ui.tableWidget->setItem(insertRow, 1, new QTableWidgetItem(pStor.strname().c_str()));
		ui.tableWidget->setItem(insertRow, 2, new QTableWidgetItem(pStor.strip().c_str()));
		ui.tableWidget->setItem(insertRow, 3, new QTableWidgetItem(pStor.struser().c_str()));
	}
}



