#include "config/vidstor/vidcamadd.h"
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
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"

#include <QDesktopWidget>
#include <QApplication>

using namespace Poco;

VidCamAdd::VidCamAdd(VidStor &pStor, ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
    : m_sStor(pStor), m_pFactory(pFactory), QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);
	ui.scrollArea->setBackgroundRole(QPalette::Light);

	TreeWidgetUpdate();

	/* Setup connection */	
	connect(ui.pbNew, SIGNAL(clicked()), this, SLOT(SlotNewCam()));
	connect(ui.pbDelete, SIGNAL(clicked()), this, SLOT(SlotDeleteCam()));
	connect(ui.pbApply, SIGNAL(clicked()), this, SLOT(SlotApplyCam()));
	connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(SlotCancelCam()));

	connect(this, SIGNAL(SignalSectionClicked(int, int)), ui.tableWidget, SIGNAL(cellClicked(int, int)));
	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(SlotSectionClicked(int, int)));
	connect( this->ui.radioButtonFile, SIGNAL( clicked() ), this, SLOT(SlotRadioButtonClicked()));
	connect( this->ui.radioButtonRtsp, SIGNAL( clicked() ), this, SLOT(SlotRadioButtonClicked()));
	connect( this->ui.radioButtonOnvif, SIGNAL( clicked() ), this, SLOT(SlotRadioButtonClicked()));
}

void VidCamAdd::SlotRadioButtonClicked()
{
	ui.lineEditIP->setDisabled(1);
	ui.lineEditPort->setDisabled(1);
	ui.lineEditUser->setDisabled(1);
	ui.lineEditPassword->setDisabled(1);
	ui.lineEditRtspAddr->setDisabled(1);
	ui.lineEditFile->setDisabled(1);

	if(this->ui.radioButtonFile->isChecked())
	{
		ui.lineEditFile->setDisabled(0);
	}


	if(this->ui.radioButtonRtsp->isChecked())
	{
		ui.lineEditIP->setDisabled(0);
		ui.lineEditPort->setDisabled(0);
		ui.lineEditUser->setDisabled(0);
		ui.lineEditPassword->setDisabled(0);
		ui.lineEditRtspAddr->setDisabled(0);
	}
	
	if(this->ui.radioButtonOnvif->isChecked())
	{
		ui.lineEditIP->setDisabled(0);
		ui.lineEditPort->setDisabled(0);
		ui.lineEditUser->setDisabled(0);
		ui.lineEditPassword->setDisabled(0);
	}
}

VidCamAdd::~VidCamAdd()
{

}

void VidCamAdd::SlotNewCam()
{
	VidCamera pCam;
	UUIDGenerator uuidCreator;
	astring strId  = uuidCreator.createRandom().toString();
	pCam.set_strid(strId);
	pCam.set_strname("New Camera");
	pCam.set_strip("192.168.0.1");
	pCam.set_strport("80");
	pCam.set_struser("admin");
	pCam.set_strpasswd("admin");
	pCam.set_ntype(VID_ONVIF_S);
	pCam.set_stronvifaddress("/onvif/device_service");


	pCam.set_strrtspurl("/streamaddress");
	astring filePath = "/camera.mp4";
	pCam.set_strfile(filePath.c_str());
	
	int insertRow = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(insertRow);
	QTableWidgetItem *firstCheck = new VidCamTableItem(pCam, false);
	firstCheck->setCheckState(Qt::Checked);
	ui.tableWidget->setItem(insertRow, 0, firstCheck);
	ui.tableWidget->setItem(insertRow, 1, new QTableWidgetItem(pCam.strname().c_str()));
	ui.tableWidget->setItem(insertRow, 2, new QTableWidgetItem(pCam.strip().c_str()));
	ui.tableWidget->setItem(insertRow, 3, new QTableWidgetItem(pCam.struser().c_str()));

	ui.tableWidget->selectRow(insertRow);

	emit(SignalSectionClicked(insertRow, 0));
}

void VidCamAdd::SlotDeleteCam()
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidCamTableItem *pCam = dynamic_cast<VidCamTableItem * >(firstCheck);
	/* Only the new is false need call the factory */
	if (pCam && pCam->IsNew() == false)
	{
		m_pFactory.GetStorFactory().DeleteCam(m_sStor.strid(), pCam->GetCam().strid());
	}
	ui.tableWidget->removeRow(ui.tableWidget->currentRow());
}
void VidCamAdd::SlotApplyCam()
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidCamTableItem *pCam = dynamic_cast<VidCamTableItem * >(firstCheck);
	/* Only the new is false need call the factory */
	if (pCam)
	{
		VidCamera sCamConf = pCam->GetCam();
		GetCamUI(sCamConf);
		m_pFactory.GetStorFactory().AddCam(m_sStor.strid(), sCamConf);
	}
}
void VidCamAdd::SlotCancelCam()
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidCamTableItem *pCam = dynamic_cast<VidCamTableItem * >(firstCheck);
	/* Only the new is false need call the factory */
	if (pCam)
	{
		SetCamUI(pCam->GetCam());
	}
}


/* Stor Clicked, show the stor on the right */
void VidCamAdd::SlotSectionClicked(int row, int column)
{
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidCamTableItem *pCam = dynamic_cast<VidCamTableItem * >(firstCheck);
	if (pCam )
	{
		SetCamUI(pCam->GetCam());
	}
	
}

void VidCamAdd::SetCamUI(VidCamera pCam)
{
	switch(pCam.ntype())
	{
		case VID_FILE:
			ui.radioButtonFile->setChecked(true);
			break;
		case VID_RTSP:
			ui.radioButtonRtsp->setChecked(true);
			break;
		case VID_ONVIF_S:
			ui.radioButtonOnvif->setChecked(true);
			break;
		default:
			return;
	}
	SlotRadioButtonClicked();
	ui.lineEditName->setText(pCam.strname().c_str());
	ui.lineEditIP->setText(pCam.strip().c_str());
	ui.lineEditPort->setText(pCam.strport().c_str());
	ui.lineEditUser->setText(pCam.struser().c_str());
	ui.lineEditPassword->setText(pCam.strpasswd().c_str());
	ui.lineEditFile->setText(pCam.strfile().c_str());
	ui.lineEditRtspAddr->setText(pCam.strrtspurl().c_str());
}

void VidCamAdd::GetCamUI(VidCamera &pCam)
{
	if (ui.radioButtonRtsp->isChecked() == true)
	{
		pCam.set_ntype(VID_RTSP);
	}else if (ui.radioButtonOnvif->isChecked() == true)
	{
		pCam.set_ntype(VID_ONVIF_S);

	}else if (ui.radioButtonFile->isChecked() == true)
	{
		pCam.set_ntype(VID_FILE);
	}

	pCam.set_stronvifaddress("/onvif/device_service");
	pCam.set_strname(ui.lineEditName->text().toStdString());
	pCam.set_strip(ui.lineEditIP->text().toStdString());
	pCam.set_strport(ui.lineEditPort->text().toStdString());
	pCam.set_struser(ui.lineEditUser->text().toStdString());
	pCam.set_strpasswd(ui.lineEditPassword->text().toStdString());
	pCam.set_strfile(ui.lineEditFile->text().toStdString());
	pCam.set_strrtspurl(ui.lineEditRtspAddr->text().toStdString());
}


void VidCamAdd::TreeWidgetUpdate()
{

	VidCameraList camList;
	StorSyncInf syncInf(m_sStor, 5 * 1000);

	VSCLoading loading(NULL);
	loading.show();
	QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	loading.setGeometry(rect.width()/2, rect.height()/2, 64, 64);
	QCoreApplication::processEvents();

	if (syncInf.Connect() == false)
	{
		return;
	}

	camList = syncInf.GetVidCameraList();
	
	
	int storSize = camList.cvidcamera_size();
	ui.tableWidget->clearContents();

	for (s32 j = 0; j < ui.tableWidget->rowCount(); j ++)
	{
		ui.tableWidget->removeRow(j);
	}

	
	for (s32 i = 0; i < camList.cvidcamera_size(); i ++)
	{
		VidCamera pCam = camList.cvidcamera(i);
		int insertRow = ui.tableWidget->rowCount();
    		ui.tableWidget->insertRow(insertRow);
    		QTableWidgetItem *firstCheck = new VidCamTableItem(pCam, false);
    		firstCheck->setCheckState(Qt::Checked);
		ui.tableWidget->setItem(insertRow, 0, firstCheck);
		ui.tableWidget->setItem(insertRow, 1, new QTableWidgetItem(pCam.strname().c_str()));
		ui.tableWidget->setItem(insertRow, 2, new QTableWidgetItem(pCam.strip().c_str()));
		ui.tableWidget->setItem(insertRow, 3, new QTableWidgetItem(pCam.struser().c_str()));
	}

}



