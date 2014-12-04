#include "vscsiteadd.h"
#include "vscvwidget.h"

extern Factory *gFactory;

VSCSiteAdd::VSCSiteAdd(VSCVmsDataItem &Param, QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    m_Param = Param;
    memcpy(&m_Param, &Param, sizeof(m_Param));
    m_nId = m_Param.nId;

    setupDefaultValue();
    radioButtonClicked();
    SetupConnections();
}

VSCSiteAdd::~VSCSiteAdd()
{

}

void VSCSiteAdd::SetupConnections()
{
    connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}


void VSCSiteAdd::setupDefaultValue()
{
    ui.lineEditName->setText(m_Param.Name);
    ui.lineEditIP->setText(m_Param.IP);
    ui.lineEditPort->setText(m_Param.Port);
    ui.lineEditUser->setText(m_Param.User);
    ui.lineEditPassword->setText(m_Param.Password);
}

void VSCSiteAdd::updateParamValue(QLineEdit *ld, s8 * pParam)
{
    if (pParam && ld)
    {
        strcpy(pParam, ld->text().toStdString().c_str());
    }
}

void VSCSiteAdd::applyConfig()
{
	VDC_DEBUG( "%s  ID %d\n",__FUNCTION__, m_nId);
	/* Update m_Param from UI  */
	updateParamValue(ui.lineEditName, m_Param.Name);

	updateParamValue(ui.lineEditIP, m_Param.IP);
	updateParamValue(ui.lineEditPort, m_Param.Port);
	updateParamValue(ui.lineEditUser, m_Param.User);
	updateParamValue(ui.lineEditPassword, m_Param.Password);
	m_Param.nType = VSC_VMS_SITE;
       m_Param.nSubType = VSC_SUB_VMS_ZB;
	if (m_nId > 0)
	{
	    gFactory->DelVms(m_nId);
	}
	/* m_nId <= 0 is Add Camera Device */
	m_nId = gFactory->AddVms(m_Param);

	return ;
}

void VSCSiteAdd::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void VSCSiteAdd::createContentMenu()
{

}

void VSCSiteAdd::radioButtonClicked()
{

}
