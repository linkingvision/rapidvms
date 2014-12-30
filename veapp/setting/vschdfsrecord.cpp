#include "vschdfsrecord.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCHdfsRecord::VSCHdfsRecord(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	gFactory->GetHdfsRecordConf(m_Param);
	/* Setup the default value */
	setupDefaultValue();
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}

void VSCHdfsRecord::setupDefaultValue()
{
    ui.lineEditNameNode->setText(m_Param.data.conf.NameNode);
    ui.lineEditPort->setText(m_Param.data.conf.Port);
    ui.lineEditUser->setText(m_Param.data.conf.User);
}




void VSCHdfsRecord::applyConfig()
{
    /* Update m_Param from UI  */
    updateParamValue(ui.lineEditNameNode, m_Param.data.conf.NameNode);
    updateParamValue(ui.lineEditPort, m_Param.data.conf.Port);
    updateParamValue(ui.lineEditUser, m_Param.data.conf.User);

    VDC_DEBUG( "%s  Line %d\n",__FUNCTION__, __LINE__);
    gFactory->SetHdfsRecordConf(m_Param);


}


