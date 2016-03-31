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
#include <QMessageBox>
#include <QTableWidgetItem>

using namespace Poco;

VidCamAdd::VidCamAdd(VidStor &pStor, ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
    : m_sStor(pStor), m_pFactory(pFactory), m_bStarted(false), m_syncInfSearch(NULL),
    m_bSelectedAll(false), QWidget(parent, flags)
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
	connect( this->ui.radioButtonMJPEG, SIGNAL( clicked() ), this, SLOT(SlotRadioButtonClicked()));

	connect( this->ui.pushButtonStart, SIGNAL( clicked() ), this, SLOT(SlotStartSearch()));	
	connect( this->ui.pushButtonStop, SIGNAL( clicked() ), this, SLOT(SlotStopSearch()));	
	connect( this->ui.pushButtonAdd, SIGNAL( clicked() ), this, SLOT(SlotAddAll()));	
	connect( this->ui.pushButtonSelect, SIGNAL( clicked() ), this, SLOT(SlotSelectAll()));
	/* Set Stor Name */
	ui.storName->setText(m_sStor.strname().c_str());

	m_Timer = new QTimer(this);
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(SlotSearchRecv()));
}

void VidCamAdd::SlotRadioButtonClicked()
{
	ui.lineEditIP->setDisabled(1);
	ui.lineEditPort->setDisabled(1);
	ui.lineEditUser->setDisabled(1);
	ui.lineEditPassword->setDisabled(1);
	ui.lineEditRtspAddr->setDisabled(1);
	ui.lineEditFile->setDisabled(1);
	//ui.pbMotionJPEG->setDisabled(1);

	if(this->ui.radioButtonFile->isChecked())
	{
		ui.lineEditFile->setDisabled(0);
	}


	if(this->ui.radioButtonRtsp->isChecked()
	|| this->ui.radioButtonMJPEG->isChecked())
	{
		//ui.lineEditIP->setDisabled(0);
		//ui.lineEditPort->setDisabled(0);
		ui.lineEditUser->setDisabled(0);
		ui.lineEditPassword->setDisabled(0);
		ui.lineEditRtspAddr->setDisabled(0);
		//ui.pbMotionJPEG->setDisabled(0);
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
	pCam.set_bmotionjpeg(false);


	pCam.set_strrtspurl("rtsp://192.168.0.1:554/Streaming");
	astring filePath = "camera.mp4";
	pCam.set_strfile(filePath.c_str());
	astring *pSched = pCam.add_crecsched();
	*pSched = REC_SCHED_ALL_DAY;
	
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
	
	QTableWidgetItem *firstCheck = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);

	VidCamTableItem *pCam = dynamic_cast<VidCamTableItem * >(firstCheck);
	/* Only the new is false need call the factory */
	if (pCam)
	{
		VidCamera sCamConf = pCam->GetCam();
		GetCamUI(sCamConf);
		syncInf.AddCam(sCamConf);
		//m_pFactory.GetStorFactory().AddCam(m_sStor.strid(), sCamConf);
		TreeWidgetUpdate();
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
		case VID_MJPEG:
			ui.radioButtonMJPEG->setChecked(true);
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
	//ui.pbMotionJPEG->setChecked(pCam.bmotionjpeg());
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
	}else if (ui.radioButtonMJPEG->isChecked() == true)
	{
		pCam.set_ntype(VID_MJPEG);
	}
	pCam.set_stronvifaddress("/onvif/device_service");
	pCam.set_strname(ui.lineEditName->text().toStdString());
	pCam.set_strip(ui.lineEditIP->text().toStdString());
	pCam.set_strport(ui.lineEditPort->text().toStdString());
	pCam.set_struser(ui.lineEditUser->text().toStdString());
	pCam.set_strpasswd(ui.lineEditPassword->text().toStdString());
	pCam.set_strfile(ui.lineEditFile->text().toStdString());
	pCam.set_strrtspurl(ui.lineEditRtspAddr->text().toStdString());
	//pCam.set_bmotionjpeg(ui.pbMotionJPEG->isChecked());
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
	int nRowCnt = ui.tableWidget->rowCount();

	for (s32 j = 0; j < nRowCnt; j ++)
	{
		ui.tableWidget->removeRow(j);
	}
	ui.tableWidget->clear();

	
	for (s32 i = 0; i < camList.cvidcamera_size(); i ++)
	{
		VidCamera pCam = camList.cvidcamera(i);
		//int insertRow = ui.tableWidget->rowCount();
		int insertRow = i;
    		ui.tableWidget->insertRow(insertRow);
    		QTableWidgetItem *firstCheck = new VidCamTableItem(pCam, false);
    		firstCheck->setCheckState(Qt::Checked);
		ui.tableWidget->setItem(insertRow, 0, firstCheck);
		ui.tableWidget->setItem(insertRow, 1, new QTableWidgetItem(pCam.strname().c_str()));
		ui.tableWidget->setItem(insertRow, 2, new QTableWidgetItem(pCam.strip().c_str()));
		ui.tableWidget->setItem(insertRow, 3, new QTableWidgetItem(pCam.struser().c_str()));
	}

}

bool VidCamAdd::CheckIPPort(s8 * ipAddr, s8 * Port)
{
	int nId = 0;
	int insertRow = ui.tableSearch->rowCount();
	s8 rowip[1024];
	s8 rowPort[1024];
       VDC_DEBUG( "[ONVIF]: Searched %d", insertRow);
	/* Loop to add device */
	for (int i = 0; i < insertRow; i ++)
	{

		/* Update  from UI  */

		updateParamValue(ui.tableSearch->item(i, 1), rowip);
		updateParamValue(ui.tableSearch->item(i, 2), rowPort);
		if (strcmp(ipAddr, rowip) == 0 && strcmp(Port, rowPort) == 0)
		{
		    VDC_DEBUG( "%s  Find IP %s\n",__FUNCTION__, rowip);
		    return true;
		}
	}

	return false;
}

void VidCamAdd::updateParamValue(QTableWidgetItem *item, s8 * pParam)
{
    if (pParam && item)
    {
        strcpy(pParam, item->text().toStdString().c_str());
    }
}

void VidCamAdd::SlotSearchRecv()
{
	XGuard guard(m_cMutex);
	astring strIP;
	astring strPort;
	astring strModel;
	astring strOnvifAddr;
	if (m_syncInfSearch == NULL)
	{
		return;
	}

	if (m_syncInfSearch->CamSearchGet(strIP, strPort, strModel, strOnvifAddr) == false)
	{
		return ;
	}
	
	if (CheckIPPort((s8 *)strIP.c_str(), (s8 *)strPort.c_str()) == true)
	{
		VDC_DEBUG(" AddItem === > %s exist\n", strIP.c_str());
		return;
	}
	VDC_DEBUG( " AddItem === >\n");
	int insertRow = ui.tableSearch->rowCount();
	ui.tableSearch->insertRow(insertRow);
	QTableWidgetItem *firstCheck = new QTableWidgetItem("");
	firstCheck->setCheckState(Qt::Unchecked);

	ui.tableSearch->setItem(insertRow, 0, firstCheck);
	ui.tableSearch->setItem(insertRow, 1, new QTableWidgetItem(strIP.c_str()));
	ui.tableSearch->setItem(insertRow, 2, new QTableWidgetItem(strPort.c_str()));
	ui.tableSearch->setItem(insertRow, 3, new QTableWidgetItem(strModel.c_str()));
	ui.tableSearch->setItem(insertRow, 4, new QTableWidgetItem(strOnvifAddr.c_str()));
	ui.tableSearch->setItem(insertRow, 5, new QTableWidgetItem(strIP.c_str()));
	ui.tableSearch->setItem(insertRow, 6, new QTableWidgetItem("admin"));
	ui.tableSearch->setItem(insertRow, 7, new QTableWidgetItem("admin"));
}

void VidCamAdd::SlotStartSearch()
{
	XGuard guard(m_cMutex);
	if (m_bStarted == true)
	{
		QMessageBox msgBox(this);
		//Set text
		msgBox.setWindowTitle(tr("Warning"));
		msgBox.setText(tr("Search is In Processing ..."));
		//Set predefined icon, icon is show on left side of text.
		msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));
		//set inforative text
		//msgBox.setInformativeText("Just show infornation.");
		//Add ok and cancel button.
		msgBox.setStandardButtons(QMessageBox::Ok);
		//Set focus of ok button
		msgBox.setDefaultButton(QMessageBox::Ok);

		//execute message box. method exec() return the button value of cliecke button
		int ret = msgBox.exec();

		return;
	}

	m_bStarted = true;
    
	m_syncInfSearch = new StorSyncInf(m_sStor);
	
	m_syncInfSearch->Connect();
	m_syncInfSearch->CamSearchStart();
	m_syncInfSearch->SetRecvTimeout(200);
	m_Timer->start(1000);
	
}
void VidCamAdd::SlotStopSearch()
{
	XGuard guard(m_cMutex);
	if (m_bStarted == false)
	{
		QMessageBox msgBox(this);
		//Set text
		msgBox.setWindowTitle(tr("Warning"));
		msgBox.setText(tr("Search is not Started ..."));
		    //Set predefined icon, icon is show on left side of text.
		msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));
		    //set inforative text
		//msgBox.setInformativeText("Just show infornation.");
		    //Add ok and cancel button.
		msgBox.setStandardButtons(QMessageBox::Ok);
		    //Set focus of ok button
		msgBox.setDefaultButton(QMessageBox::Ok);

		    //execute message box. method exec() return the button value of cliecke button
		int ret = msgBox.exec();

		return;
	}

	m_syncInfSearch->CamSearchStop();
	m_Timer->stop();
	
	delete m_syncInfSearch;
	m_syncInfSearch = NULL;
	m_bStarted = false;
    
}

void VidCamAdd::SlotAddAll()
{
	int insertRow = ui.tableSearch->rowCount();
    	VDC_DEBUG( "[ONVIF]: Searched %d", insertRow);
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

	/* Loop to add device */
	for (int i = 0; i < insertRow; i ++)
	{
		if (ui.tableSearch->item(i, 0)->checkState() != Qt::Checked)
		{
			continue;
		}
		
		VidCamera pCam;
		UUIDGenerator uuidCreator;
		astring strId  = uuidCreator.createRandom().toString();
		pCam.set_strid(strId);
		pCam.set_strname(ui.tableSearch->item(i, 5)->text().toStdString());
		pCam.set_strip(ui.tableSearch->item(i, 1)->text().toStdString());
		pCam.set_strport(ui.tableSearch->item(i, 2)->text().toStdString());
		pCam.set_struser(ui.tableSearch->item(i, 6)->text().toStdString());
		pCam.set_strpasswd(ui.tableSearch->item(i, 7)->text().toStdString());
		pCam.set_ntype(VID_ONVIF_S);
		pCam.set_stronvifaddress(ui.tableSearch->item(i, 4)->text().toStdString());


		pCam.set_strrtspurl("/streamaddress");
		astring filePath = "/camera.mp4";
		pCam.set_strfile(filePath.c_str());
		astring *pSched = pCam.add_crecsched();
		*pSched = REC_SCHED_ALL_DAY;
		syncInf.AddCam(pCam);
		//m_pFactory.GetStorFactory().AddCam(m_sStor.strid(), pCam);
	}

	TreeWidgetUpdate();
}

void VidCamAdd::SlotSelectAll()
{
	int insertRow = ui.tableSearch->rowCount();
    VDC_DEBUG( "[ONVIF]: Searched %d", insertRow);

	if (m_bSelectedAll == true)
	{
		for (int i = 0; i < insertRow; i ++)
		{
			ui.tableSearch->item(i, 0)->setCheckState(Qt::Unchecked);
		}
		m_bSelectedAll = false;
	}else
	{
		for (int i = 0; i < insertRow; i ++)
		{
			ui.tableSearch->item(i, 0)->setCheckState(Qt::Checked);
		}	
		m_bSelectedAll = true;
	}
}





