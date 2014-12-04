#include "vscvipcadd.h"
#include "devicesearcher.h"
#include <QMessageBox>
#include <QApplication>

extern Factory *gFactory;

VSCVIPCAdd::VSCVIPCAdd(VIPCDeviceParam &Param, QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	m_Param = Param;
	m_nId = m_Param.m_Conf.data.conf.nId;

	SetupConnections();

	/* Add all the IP address */
	QList<QHostAddress> hostAddr = DeviceSearcher::getHostAddress();
	QList<QHostAddress>::iterator i;
	for(i=hostAddr.begin();i!=hostAddr.end();i++)
	{
		
		ui.listIP->addItem((*i).toString());
		
	}
	ui.listIP->addItem("none");

	/* Add all the IP Camera */
	DeviceParamMap DeviceMap;
	gFactory->GetDeviceParamMap(DeviceMap);
        DeviceParamMap::iterator it = DeviceMap.begin(); 
        for(; it!=DeviceMap.end(); ++it)
        {
            ui.listCamera->addItem((*it).second.m_Conf.data.conf.Name);
        }
	ui.listCamera->addItem("none");

	setupDefaultValue();
	radioButtonClicked();
}

VSCVIPCAdd::~VSCVIPCAdd()
{

}

void VSCVIPCAdd::SetupConnections()
{
    connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}

void VSCVIPCAdd::setupDefaultValue()
{
#if 1
    ui.radioButtonCamera->setChecked(true);
    ui.lineEditFile->setDisabled(1);
    ui.lineEditName->setText(m_Param.m_Conf.data.conf.Name);
    //ui.lineEditIP->setText(m_Param.m_Conf.data.conf.IP);
    ui.lineEditPort->setText(m_Param.m_Conf.data.conf.Port);
    ui.lineEditUser->setText(m_Param.m_Conf.data.conf.User);
    ui.lineEditPassword->setText(m_Param.m_Conf.data.conf.Password);
    int idxIP = ui.listIP->findText(m_Param.m_Conf.data.conf.IP);
    QString strNone = "none";
    if (idxIP == -1)
    {
    	idxIP = ui.listIP->findText(strNone);
    }
    ui.listIP->setCurrentIndex(idxIP);
    DeviceParam camParam;
    gFactory->GetDeviceParamById(camParam, m_Param.m_Conf.data.conf.nStreamId);
    int idxCam = ui.listCamera->findText(camParam.m_Conf.data.conf.Name);
    if (idxCam == -1)
    {
    	idxCam = ui.listCamera->findText(strNone);
    }
    ui.listCamera->setCurrentIndex(idxCam);
#else
    switch(m_Param.m_Conf.data.conf.nSubType)
    {
    case VSC_SUB_DEVICE_FILE:
        //ui.radioButtonFile->setChecked(true);
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

    ui.lineEditName->setText(m_Param.m_Conf.data.conf.Name);
    ui.lineEditIP->setText(m_Param.m_Conf.data.conf.IP);
    ui.lineEditPort->setText(m_Param.m_Conf.data.conf.Port);
    ui.lineEditUser->setText(m_Param.m_Conf.data.conf.User);
    ui.lineEditPassword->setText(m_Param.m_Conf.data.conf.Password);

    ui.lineEditRtspLoc->setText(m_Param.m_Conf.data.conf.RtspLocation);

    //ui.lineFileLoc->setText(m_Param.m_Conf.data.conf.FileLocation);

    ui.lineOnvifAddr->setText(m_Param.m_Conf.data.conf.OnvifAddress);

    ui.lineEditProfileToken->setText(m_Param.m_Conf.data.conf.OnvifProfileToken);

    if (m_Param.m_Conf.data.conf.UseProfileToken == 1)
    {
        ui.checkBoxProfileToken->setChecked(true);
        ui.lineEditProfileToken->setDisabled(0);
    }else
    {
        ui.checkBoxProfileToken->setChecked(false);
        ui.lineEditProfileToken->setDisabled(1);
    }
#endif

}

void VSCVIPCAdd::updateParamValue(QLineEdit *ld, s8 * pParam)
{
    if (pParam && ld)
    {
        strcpy(pParam, ld->text().toStdString().c_str());
    }
}

void VSCVIPCAdd::applyConfig()
{
    VDC_DEBUG( "%s  ID %d\n",__FUNCTION__, m_nId);
    /* Update m_Param from UI  */
    updateParamValue(ui.lineEditName, m_Param.m_Conf.data.conf.Name);
    astring strIP = ui.listIP->currentText().toStdString();
    astring strCamera = ui.listCamera->currentText().toStdString();
    if (strIP == "none" || strCamera == "none")
    {
	    QMessageBox msgBox(this);
	    //Set text
	    msgBox.setText(tr("Virtual IPC configuration error ..."));
	        //Set predefined icon, icon is show on left side of text.
	    msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

	    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	    msgBox.setDefaultButton(QMessageBox::Ok);

	    int ret = msgBox.exec();

	    switch (ret) {
	    case QMessageBox::Ok:
	       break;
	    default:
	       break;
	    }
	    return;
    }
    strcpy(m_Param.m_Conf.data.conf.IP, strIP.c_str());
    DeviceParamMap DeviceMap;
    gFactory->GetDeviceParamMap(DeviceMap);
    DeviceParamMap::iterator it = DeviceMap.begin(); 
    for(; it!=DeviceMap.end(); ++it)
    {
    	if (strcmp((*it).second.m_Conf.data.conf.Name, strCamera.c_str()) == 0)
    	{
    	    m_Param.m_Conf.data.conf.nStreamId = (*it).second.m_Conf.data.conf.nId;
	    break;
    	}
    }
    updateParamValue(ui.lineEditPort, m_Param.m_Conf.data.conf.Port);
    updateParamValue(ui.lineEditUser, m_Param.m_Conf.data.conf.User);
    updateParamValue(ui.lineEditPassword, m_Param.m_Conf.data.conf.Password);

    if (m_nId > 0)
    {
        gFactory->DelVIPC(m_nId);
    }
    VDC_DEBUG( "%s  Line %d\n",__FUNCTION__, __LINE__);
    /* m_nId <= 0 is Add VIPC Device */
    m_nId = gFactory->AddVIPC(m_Param);

    VDC_DEBUG( "%s  ID %d\n",__FUNCTION__, m_nId);
    VDC_DEBUG( "%s  Line %d\n",__FUNCTION__, __LINE__);
}

void VSCVIPCAdd::mouseDoubleClickEvent(QMouseEvent *e)
{

}



void VSCVIPCAdd::createContentMenu()
{

}

void VSCVIPCAdd::radioButtonClicked()
{
#if 0
    ui.lineEditIP->setDisabled(0);
    ui.lineEditPort->setDisabled(0);
    ui.lineEditUser->setDisabled(0);
    ui.lineEditPassword->setDisabled(0);

    ui.lineEditRtspLoc->setDisabled(0);

    //ui.lineFileLoc->setDisabled(0);

    ui.lineOnvifAddr->setDisabled(0);
    ui.lineEditProfileToken->setDisabled(0);
    ui.checkBoxProfileToken->setDisabled(0);
#if 0
    if(this->ui.radioButtonFile->isChecked())
    {
        ui.lineEditIP->setDisabled(1);
        ui.lineEditPort->setDisabled(1);
        ui.lineEditUser->setDisabled(1);
        ui.lineEditPassword->setDisabled(1);

        ui.lineEditRtspLoc->setDisabled(1);
        ui.lineOnvifAddr->setDisabled(1);
        ui.lineEditProfileToken->setDisabled(1);
        ui.checkBoxProfileToken->setDisabled(1);
    }
#endif

    if(this->ui.radioButtonRtsp->isChecked())
    {
        ui.lineOnvifAddr->setDisabled(1);

        //ui.lineFileLoc->setDisabled(1);
        ui.lineEditProfileToken->setDisabled(1);
        ui.checkBoxProfileToken->setDisabled(1);
    }
    if(this->ui.radioButtonOnvif->isChecked())
    {
        ui.lineEditRtspLoc->setDisabled(1);
        //ui.lineFileLoc->setDisabled(1);
    }
#endif
}
