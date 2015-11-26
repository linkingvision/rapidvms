#include "vsccameraadd.h"
#include "vscvwidget.h"
#include <QFileDialog>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include "vscloading.hpp"
#include "utility/videotype.hpp"

#include "devicemanagement.h"
#include "ptzmanagement.h"
#include "media_management/profiles.h"
#include "media_management/streamuri.h"
#include "device_management/capabilities.h"
#include "mediamanagement.h"

extern Factory *gFactory;

VSCCameraAdd::VSCCameraAdd(DeviceParam &Param, QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	m_Param = Param;
	m_bUpdateProfiles = FALSE;

	BOOL bHWAccel;
	BOOL bMining;

	m_nId = m_Param.m_Conf.data.conf.nId;
	if (m_nId == 0)
	{
		gFactory->GetDefaultHWAccel(bHWAccel);
		if (bHWAccel == TRUE)
		{
			VDC_DEBUG( "%s  Enable default the HWAccel\n",__FUNCTION__);
			m_Param.m_Conf.data.conf.HWAccel = 1;
		}

		gFactory->GetDefaultMining(bMining);
		if (bMining == TRUE)
		{
			VDC_DEBUG( "%s  Enable default the Mining\n",__FUNCTION__);
			m_Param.m_Conf.data.conf.Mining = 1;
		}
	}
	
#if 0
    m_pFloating = new QAction(QIcon(tr("images/open.ico")), tr("Floating"), this);
    //m_pUnFloating = new QAction(QIcon(tr("images/open.ico")), tr("UnFloating"), this);
    connect(m_pFloating, SIGNAL(triggered()), this, SLOT(floatingAction()));
    //connect(m_pUnFloating, SIGNAL(triggered()), this, SLOT(unFloatingAction()));
    createContentMenu();
#endif
    //connect(ui.lineFilePath, SIGNAL(triggered()), this, SLOT(floatingAction()));
    //ui.radioButtonFile->setChecked(true);
    setupDefaultValue();
    radioButtonClicked();
    m_pVideo = new VSCVWidget(m_nId, this);

    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(m_pVideo);
    layout->setMargin(0);

    ui.widget->setLayout(layout);

    PreView();

    SetupConnections();
}

VSCCameraAdd::~VSCCameraAdd()
{

}

void VSCCameraAdd::SetupConnections()
{
	connect( this->ui.radioButtonFile, SIGNAL( clicked() ), this, SLOT(radioButtonClicked()));
	connect( this->ui.radioButtonRtsp, SIGNAL( clicked() ), this, SLOT(radioButtonClicked()));
	connect( this->ui.radioButtonOnvif, SIGNAL( clicked() ), this, SLOT(radioButtonClicked()));
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
	connect( this->ui.pushButtonFile, SIGNAL( clicked() ), this, SLOT(fileSelect()));
	connect( this->ui.pushButtonProfile, SIGNAL( clicked() ), this, SLOT(profileRefresh()));

}

/* H264 1080 x 720 30fps 4000kbps */
void VSCCameraAdd::profileRefresh()
{
	/* Update the param  */
	updateParamValue(ui.lineEditName, m_Param.m_Conf.data.conf.Name);

	updateParamValue(ui.lineEditIP, m_Param.m_Conf.data.conf.IP);
	updateParamValue(ui.lineEditPort, m_Param.m_Conf.data.conf.Port);
	updateParamValue(ui.lineEditUser, m_Param.m_Conf.data.conf.User);
	updateParamValue(ui.lineEditPassword, m_Param.m_Conf.data.conf.Password);

	updateParamValue(ui.lineOnvifAddr, m_Param.m_Conf.data.conf.OnvifAddress);
	
	VSCLoading *loading = new VSCLoading(NULL);
	loading->show();
	QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	loading->setGeometry(rect.width()/2, rect.height()/2, 64, 64);
	QCoreApplication::processEvents();

	astring IP = m_Param.m_Conf.data.conf.IP;
	astring Port = m_Param.m_Conf.data.conf.Port;
	astring User = m_Param.m_Conf.data.conf.User;
	astring Password = m_Param.m_Conf.data.conf.Password;
	astring OnvifAddress = m_Param.m_Conf.data.conf.OnvifAddress;

	astring OnvifDeviceService = "http://" + IP + ":" + Port + OnvifAddress;
	astring url = "rtsp://" + IP + ":" + "554" + "/";
	astring urlSubStream = "rtsp://" + IP + ":" + "554" + "/";

	DeviceManagement *pDm = new DeviceManagement(OnvifDeviceService.c_str(), 
	                        User.c_str(), Password.c_str());

	if (pDm  == NULL)
	{
	    VDC_DEBUG( "%s new DeviceManagement error \n",__FUNCTION__);
	    goto end;
	}

	Capabilities * pMediaCap = pDm->getCapabilitiesMedia();
	if (pMediaCap == NULL)
	{
	    VDC_DEBUG( "%s getCapabilitiesMedia error \n",__FUNCTION__);
	    delete pDm;
	    goto end;
	}

	QCoreApplication::processEvents();

	MediaManagement *pMedia = new MediaManagement(pMediaCap->mediaXAddr(), 
	                            User.c_str(), Password.c_str());
	if (pMedia == NULL)
	{
	    VDC_DEBUG( "%s new MediaManagement error \n",__FUNCTION__);
	    delete pDm;
	    delete pMediaCap;
	    goto end;
	}

	QCoreApplication::processEvents();
	
	Profiles *pProfileS = pMedia->getProfiles();
	if (pProfileS == NULL)
	{
	    VDC_DEBUG( "%s new getProfiles error \n",__FUNCTION__);
	    delete pDm;
	    delete pMediaCap;
	    delete pMedia;
	    goto end;
	}

	QCoreApplication::processEvents();

	if (pProfileS != NULL && pProfileS->m_toKenPro.size() <= 0)
	{
	    VDC_DEBUG( "%s new getProfiles error \n",__FUNCTION__);
	    delete pDm;
	    delete pMediaCap;
	    delete pMedia;
	    goto end;
	}

	m_bUpdateProfiles = TRUE;

	for ( int i=0; i!=pProfileS->m_toKenPro.size(); ++i )
	{
		m_ProfilesMap[i] = pProfileS->m_toKenPro.at(i).toStdString();
		s8 profileDisplay[1024];
		sprintf(profileDisplay, "%s %dx%d %dfps %dbps", 
				pProfileS->m_encodingVec.at(i).toStdString().c_str(), pProfileS->m_widthVec.at(i), 
				pProfileS->m_heightVec.at(i), pProfileS->m_frameRateLimitVec.at(i), 
				pProfileS->m_bitrateLimitVec.at(i));

		m_ProfilesMapDisplay[i] = profileDisplay;
	}

	QCoreApplication::processEvents();

	delete pDm;
	delete pMediaCap;
	delete pMedia;
	delete pProfileS;

end:
	delete loading;

	/* update error, show a error message */
	if (m_bUpdateProfiles == FALSE)
	{
		QMessageBox msgBox(this);
		//Set text
		msgBox.setWindowTitle(tr("Warning"));
		msgBox.setText(tr("Update Profiles error ..."));
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

	int mainStreamIndex = 0;
	int subStreamIndex = 0;
	{
		ProfilesMap::iterator it = m_ProfilesMap.begin(); 
		for(; it!=m_ProfilesMap.end(); ++it)
		{
			std::string strMainStream = m_Param.m_Conf.data.conf.OnvifProfileToken;
			std::string strSubStream = m_Param.m_Conf.data.conf.OnvifProfileToken2;
			if ((*it).second == strMainStream)
			{
				mainStreamIndex = (*it).first;
			}

			if ((*it).second == strSubStream)
			{
				subStreamIndex = (*it).first;
			}
		}
	}

	/* update with current profile */
	ProfilesMap::iterator it = m_ProfilesMapDisplay.begin(); 
	for(; it!=m_ProfilesMapDisplay.end(); ++it)
	{
		ui.comboBoxMain->insertItem((*it).first, (*it).second.c_str());
		ui.comboBoxSub->insertItem((*it).first, (*it).second.c_str());
	}

	ui.comboBoxMain->setCurrentIndex(mainStreamIndex);
	ui.comboBoxSub->setCurrentIndex(subStreamIndex);

	

	
	return ;

}

void VSCCameraAdd::fileSelect()
{
	QFileDialog *fileDialog = new QFileDialog(this); 
	fileDialog->setWindowTitle(tr("Select File")); 
	fileDialog->setDirectory(ui.fileLoc->text()); 
	fileDialog->setNameFilter(tr("Video Files(*.mov *.mp4 *.avi *.ts *.mpg)")); 
	QIcon icon;
	icon.addFile(QStringLiteral(":/logo/resources/vscsmall.png"), QSize(), QIcon::Normal, QIcon::Off);
	fileDialog->setWindowIcon(icon);
	if(fileDialog->exec() == QDialog::Accepted) { 
		QString path = fileDialog->selectedFiles()[0]; 
		ui.fileLoc->setText(path);
		VDC_DEBUG( "%s  QFileDialog %s\n",__FUNCTION__, path.toStdString().c_str());
	} else 
	{ 
	
	}
}

void VSCCameraAdd::PreView()
{
	if (m_nId <= 0)
	{
	    return;
	}
	m_pVideo->StopPlay(TRUE);
	QCoreApplication::processEvents();
	m_pVideo->StartPlay(m_nId);
	QCoreApplication::processEvents();

}

void VSCCameraAdd::setupDefaultValue()
{
	switch(m_Param.m_Conf.data.conf.nSubType)
	{
	case VSC_SUB_DEVICE_FILE:
	    ui.radioButtonFile->setChecked(true);
	    break;
	case VSC_SUB_DEVICE_RTSP:
	    ui.radioButtonRtsp->setChecked(true);
	    break;
	case VSC_SUB_DEVICE_ONVIF:
	    ui.radioButtonOnvif->setChecked(true);
	    break;
	default:
	    return;
	}

	switch(m_Param.m_Conf.data.conf.ConnectType)
	{
	case VSC_CONNECT_TCP:
	    ui.comboBoxConnect->setCurrentIndex(1);
	    break;
	case VSC_CONNECT_UDP:
	    ui.comboBoxConnect->setCurrentIndex(0);
	    break;
	default:
	    ui.comboBoxConnect->setCurrentIndex(1);
	    break;
	}

	ui.lineEditName->setText(m_Param.m_Conf.data.conf.Name);
	ui.lineEditIP->setText(m_Param.m_Conf.data.conf.IP);
	ui.lineEditPort->setText(m_Param.m_Conf.data.conf.Port);
	ui.lineEditUser->setText(m_Param.m_Conf.data.conf.User);
	ui.lineEditPassword->setText(m_Param.m_Conf.data.conf.Password);

	ui.lineEditRtspLoc->setText(m_Param.m_Conf.data.conf.RtspLocation);

	//ui.lineFileLoc->setText(m_Param.m_Conf.data.conf.FileLocation);
	ui.fileLoc->setText(m_Param.m_Conf.data.conf.FileLocation);

	ui.lineOnvifAddr->setText(m_Param.m_Conf.data.conf.OnvifAddress);

	//ui.lineEditProfileToken->setText(m_Param.m_Conf.data.conf.OnvifProfileToken);
	//ui.lineEditProfileToken2->setText(m_Param.m_Conf.data.conf.OnvifProfileToken2);

	if (m_Param.m_Conf.data.conf.UseProfileToken == 1)
	{
	    ui.checkBoxProfileToken->setChecked(true);
	    //ui.lineEditProfileToken->setDisabled(0);
	}else
	{
	    ui.checkBoxProfileToken->setChecked(false);
	    //ui.lineEditProfileToken->setDisabled(1);
	}

	if (m_Param.m_Conf.data.conf.HWAccel == 1)
	{
		ui.checkBoxHWAccel->setChecked(true);
	}else
	{
		ui.checkBoxHWAccel->setChecked(false);
	}

	if (m_Param.m_Conf.data.conf.Mining == 1)
	{
		ui.checkBoxDataMining->setChecked(true);
	}else
	{
		ui.checkBoxDataMining->setChecked(false);
	}

}

void VSCCameraAdd::updateParamValue(QLineEdit *ld, s8 * pParam)
{
    if (pParam && ld)
    {
        strcpy(pParam, ld->text().toStdString().c_str());
    }
}

void VSCCameraAdd::applyConfig()
{
	VDC_DEBUG( "%s  ID %d\n",__FUNCTION__, m_nId);
	/* Update m_Param from UI  */
	updateParamValue(ui.lineEditName, m_Param.m_Conf.data.conf.Name);

	updateParamValue(ui.lineEditIP, m_Param.m_Conf.data.conf.IP);
	updateParamValue(ui.lineEditPort, m_Param.m_Conf.data.conf.Port);
	updateParamValue(ui.lineEditUser, m_Param.m_Conf.data.conf.User);
	updateParamValue(ui.lineEditPassword, m_Param.m_Conf.data.conf.Password);


	updateParamValue(ui.lineEditRtspLoc, m_Param.m_Conf.data.conf.RtspLocation);
	//updateParamValue(ui.lineFileLoc, m_Param.m_Conf.data.conf.FileLocation);
	/* Update the File location */
	strcpy(m_Param.m_Conf.data.conf.FileLocation, ui.fileLoc->text().toStdString().c_str());
	updateParamValue(ui.lineOnvifAddr, m_Param.m_Conf.data.conf.OnvifAddress);
	//updateParamValue(ui.lineEditProfileToken, m_Param.m_Conf.data.conf.OnvifProfileToken);
	//updateParamValue(ui.lineEditProfileToken2, m_Param.m_Conf.data.conf.OnvifProfileToken2);
	m_Param.m_Conf.data.conf.nType = VSC_DEVICE_CAM;
	if (ui.radioButtonRtsp->isChecked() == true)
	{
	    m_Param.m_Conf.data.conf.nSubType = VSC_SUB_DEVICE_RTSP;
	}else if (ui.radioButtonOnvif->isChecked() == true)
	{
	    m_Param.m_Conf.data.conf.nSubType = VSC_SUB_DEVICE_ONVIF;
	}else if (ui.radioButtonFile->isChecked() == true)
	{
	    m_Param.m_Conf.data.conf.nSubType = VSC_SUB_DEVICE_FILE;
	}

	if (ui.checkBoxProfileToken->isChecked() == true)
	{
	    m_Param.m_Conf.data.conf.UseProfileToken = 1;
	}else
	{
	    m_Param.m_Conf.data.conf.UseProfileToken = 0;
	}

	/* HWAccel */
	if (ui.checkBoxHWAccel->isChecked() == true)
	{
	    m_Param.m_Conf.data.conf.HWAccel = 1;
	}else
	{
	    m_Param.m_Conf.data.conf.HWAccel = 0;
	}

	/* Mining */
	if (ui.checkBoxDataMining->isChecked() == true)
	{
	    m_Param.m_Conf.data.conf.Mining = 1;
	}else
	{
	    m_Param.m_Conf.data.conf.Mining = 0;
	}

	switch(ui.comboBoxConnect->currentIndex())
	{
		case 1:
		{
			m_Param.m_Conf.data.conf.ConnectType = VSC_CONNECT_TCP;
			break;
		}
		case 0:
		{
			m_Param.m_Conf.data.conf.ConnectType = VSC_CONNECT_UDP;
			break;
		}
		default:
		{	
				m_Param.m_Conf.data.conf.ConnectType = VSC_CONNECT_TCP;
				break;
		}
	}

	/* Check the used set profiles */
	if (m_bUpdateProfiles == TRUE)
	{
		int mainStreamIndex = ui.comboBoxMain->currentIndex();
		int subStreamIndex = ui.comboBoxSub->currentIndex();

		strcpy(m_Param.m_Conf.data.conf.OnvifProfileToken, m_ProfilesMap[mainStreamIndex].c_str());
		strcpy(m_Param.m_Conf.data.conf.OnvifProfileToken2, m_ProfilesMap[subStreamIndex].c_str());
	}

	VSCLoading *loading = new VSCLoading(this);
	loading->Processing(1);
	
	if (m_nId > 0)
	{
	    gFactory->DelDevice(m_nId);
	}
	VDC_DEBUG( "%s  Line %d\n",__FUNCTION__, __LINE__);
	/* m_nId <= 0 is Add Camera Device */
	m_nId = gFactory->AddDevice(m_Param);
	
	QCoreApplication::processEvents();
	VDC_DEBUG( "%s  ID %d\n",__FUNCTION__, m_nId);
	PreView();
	VDC_DEBUG( "%s  Line %d\n",__FUNCTION__, __LINE__);
	delete loading;
}

void VSCCameraAdd::mouseDoubleClickEvent(QMouseEvent *e)
{
#if 0
    VDC_DEBUG( "%s mouseDoubleClickEvent\n",__FUNCTION__);
    QWidget::mouseDoubleClickEvent(e);
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

void VSCCameraAdd::floatingAction()
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

void VSCCameraAdd::unFloatingAction()
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


void VSCCameraAdd::createContentMenu()
{
    this->addAction(m_pFloating);
    //this->addAction(m_pUnFloating);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void VSCCameraAdd::radioButtonClicked()
{
    ui.lineEditIP->setDisabled(0);
    ui.lineEditPort->setDisabled(0);
    ui.lineEditUser->setDisabled(0);
    ui.lineEditPassword->setDisabled(0);

    ui.lineEditRtspLoc->setDisabled(0);

    //ui.lineFileLoc->setDisabled(0);

    ui.lineOnvifAddr->setDisabled(0);
    //ui.lineEditProfileToken->setDisabled(0);
    //ui.lineEditProfileToken2->setDisabled(0);
    ui.checkBoxProfileToken->setDisabled(0);

    if(this->ui.radioButtonFile->isChecked())
    {
        ui.lineEditIP->setDisabled(1);
        ui.lineEditPort->setDisabled(1);
        ui.lineEditUser->setDisabled(1);
        ui.lineEditPassword->setDisabled(1);

        ui.lineEditRtspLoc->setDisabled(1);
        ui.lineOnvifAddr->setDisabled(1);
        //ui.lineEditProfileToken->setDisabled(1);
		//ui.lineEditProfileToken2->setDisabled(1);
        ui.checkBoxProfileToken->setDisabled(1);
	 ui.pushButtonProfile->setDisabled(1);
    }


    if(this->ui.radioButtonRtsp->isChecked())
    {
        ui.lineOnvifAddr->setDisabled(1);

        //ui.lineFileLoc->setDisabled(1);
        //ui.lineEditProfileToken->setDisabled(1);
        //ui.lineEditProfileToken2->setDisabled(1);
	ui.checkBoxProfileToken->setDisabled(1);
	ui.pushButtonProfile->setDisabled(1);
    }
    if(this->ui.radioButtonOnvif->isChecked())
    {
        ui.lineEditRtspLoc->setDisabled(1);
        //ui.lineFileLoc->setDisabled(1);
        ui.pushButtonProfile->setDisabled(0);
    }
}
