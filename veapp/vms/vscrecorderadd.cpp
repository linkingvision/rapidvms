#include "vscrecorderadd.h"
#include "vscvwidget.h"

extern Factory *gFactory;

VSCRecorderAdd::VSCRecorderAdd(DeviceParam &Param, QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    m_Param = Param;
    m_nId = m_Param.m_Conf.data.conf.nId;

    setupDefaultValue();
    radioButtonClicked();
    SetupConnections();
}

VSCRecorderAdd::~VSCRecorderAdd()
{

}

void VSCRecorderAdd::SetupConnections()
{
    connect( this->ui.radioButtonRtsp, SIGNAL( clicked() ), this, SLOT(radioButtonClicked()));
    connect( this->ui.radioButtonOnvif, SIGNAL( clicked() ), this, SLOT(radioButtonClicked()));
    connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}


void VSCRecorderAdd::setupDefaultValue()
{
    switch(m_Param.m_Conf.data.conf.nSubType)
    {
    case VSC_SUB_DEVICE_FILE:
        //ui.radioButtonFile->setChecked(true);
        break;
    case VSC_SUB_DEVICE_RTSP:
        ui.radioButtonRtsp->setChecked(true);
        break;
    case VSC_SUB_DEVICE_ONVIF_RECODER:
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


    ui.lineOnvifAddr->setText(m_Param.m_Conf.data.conf.OnvifAddress);

}

void VSCRecorderAdd::updateParamValue(QLineEdit *ld, s8 * pParam)
{
    if (pParam && ld)
    {
        strcpy(pParam, ld->text().toStdString().c_str());
    }
}

void VSCRecorderAdd::applyConfig()
{
    VDC_DEBUG( "%s  ID %d\n",__FUNCTION__, m_nId);
    /* Update m_Param from UI  */
    updateParamValue(ui.lineEditName, m_Param.m_Conf.data.conf.Name);

    updateParamValue(ui.lineEditIP, m_Param.m_Conf.data.conf.IP);
    updateParamValue(ui.lineEditPort, m_Param.m_Conf.data.conf.Port);
    updateParamValue(ui.lineEditUser, m_Param.m_Conf.data.conf.User);
    updateParamValue(ui.lineEditPassword, m_Param.m_Conf.data.conf.Password);

    //updateParamValue(ui.lineFileLoc, m_Param.m_Conf.data.conf.FileLocation);
    updateParamValue(ui.lineOnvifAddr, m_Param.m_Conf.data.conf.OnvifAddress);
    m_Param.m_Conf.data.conf.nType = VSC_DEVICE_RECORDER;
    if (ui.radioButtonRtsp->isChecked() == true)
    {
    }else if (ui.radioButtonOnvif->isChecked() == true)
    {
        m_Param.m_Conf.data.conf.nSubType = VSC_SUB_DEVICE_ONVIF_RECODER;
    }

    if (m_nId > 0)
    {
        gFactory->DelDevice(m_nId);
    }
    /* m_nId <= 0 is Add Camera Device */
    m_nId = gFactory->AddDevice(m_Param);
    emit RecorderTreeUpdated();
}

void VSCRecorderAdd::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void VSCRecorderAdd::createContentMenu()
{

}

void VSCRecorderAdd::radioButtonClicked()
{
    ui.lineEditIP->setDisabled(0);
    ui.lineEditPort->setDisabled(0);
    ui.lineEditUser->setDisabled(0);
    ui.lineEditPassword->setDisabled(0);

    ui.lineOnvifAddr->setDisabled(0);
}
