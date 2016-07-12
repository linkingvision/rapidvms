#include <QMessageBox>
#include <QVariant>
#include "config/vidstor/camsetting/vscschedule.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"

VSCSchedule::VSCSchedule(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory), m_pStor(stor), m_strCam(strCam)
{

	ui.setupUi(this);
	//VSCLoading * pLoading = VSCLoading::Create();
	//StorSyncInf syncInf(m_pStor);
	
	//syncInf.Connect();


	
	//connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));

	//delete pLoading;
#ifdef WIN32
	ui.sched->setStyleSheet(QString::fromUtf8("font: 8pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif
}

void VSCSchedule::applyConfig()
{
	//VSCLoading loading(NULL);
	//StorSyncInf syncInf(m_pStor);
	
	//syncInf.Connect();



	return;

}


