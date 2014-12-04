#include "vsccameraadd.h"
#include "vscvwidget.h"

extern Factory *gFactory;

VSCCameraAdd::VSCCameraAdd(DeviceParam &Param, QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    m_Param = Param;
    m_nId = m_Param.m_Conf.data.conf.nId;
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
    //connect( this->ui.radioButtonFile, SIGNAL( clicked() ), this, SLOT(radioButtonClicked()));
    connect( this->ui.radioButtonRtsp, SIGNAL( clicked() ), this, SLOT(radioButtonClicked()));
    connect( this->ui.radioButtonOnvif, SIGNAL( clicked() ), this, SLOT(radioButtonClicked()));
    connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}

void VSCCameraAdd::PreView()
{
    if (m_nId <= 0)
    {
        return;
    }
    m_pVideo->StopPlay();
    m_pVideo->SetPlayId(m_nId);
    m_pVideo->StartPlay("fake");

}

void VSCCameraAdd::setupDefaultValue()
{
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
    updateParamValue(ui.lineOnvifAddr, m_Param.m_Conf.data.conf.OnvifAddress);
    updateParamValue(ui.lineEditProfileToken, m_Param.m_Conf.data.conf.OnvifProfileToken);
    m_Param.m_Conf.data.conf.nType = VSC_DEVICE_CAM;
    if (ui.radioButtonRtsp->isChecked() == true)
    {
        m_Param.m_Conf.data.conf.nSubType = VSC_SUB_DEVICE_RTSP;
    }else if (ui.radioButtonOnvif->isChecked() == true)
    {
        m_Param.m_Conf.data.conf.nSubType = VSC_SUB_DEVICE_ONVIF;
    }

    if (ui.checkBoxProfileToken->isChecked() == true)
    {
        m_Param.m_Conf.data.conf.UseProfileToken = 1;
    }else
    {
        m_Param.m_Conf.data.conf.UseProfileToken = 0;
    }

    if (m_nId > 0)
    {
        gFactory->DelDevice(m_nId);
    }
    VDC_DEBUG( "%s  Line %d\n",__FUNCTION__, __LINE__);
    /* m_nId <= 0 is Add Camera Device */
    m_nId = gFactory->AddDevice(m_Param);

    VDC_DEBUG( "%s  ID %d\n",__FUNCTION__, m_nId);
    //PreView();
    VDC_DEBUG( "%s  Line %d\n",__FUNCTION__, __LINE__);
    //emit CameraTreeUpdated();
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
}
