#include <QMessageBox>
#include <QVariant>
#include "config/vidstor/camsetting/vscschedule.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"
#include "server/camera.hpp"

VSCSchedule::VSCSchedule(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory), m_pStor(stor), m_strCam(strCam)
{

	ui.setupUi(this);

	VSCLoading loading(NULL);
	StorSyncInf syncInf(m_pStor);
	
	syncInf.Connect();
	VidCamera pCam;
	if (syncInf.GetVidCamera(m_strCam, pCam)  == true)
	{
		//return ;
		
		astring strSched = pCam.strsched();
		if (RecordSchedWeek::CheckStringForSched(strSched) == true)
		{
			RecordSchedWeek sched(strSched);
			ui.conf->SetCurrentSched(sched);
		}
	}

	
	
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}

void VSCSchedule::applyConfig()
{
	RecordSchedWeek sched;
	if (ui.conf->GetCurrentSched(sched) == false)
	{
		return;
	}
	VSCLoading loading(NULL);
	StorSyncInf syncInf(m_pStor);
	
	syncInf.Connect();
	VidCamera pCam;
	if (syncInf.GetVidCamera(m_strCam, pCam)  == false)
	{
		return ;
	}
	pCam.set_strsched(sched.ToString());

	syncInf.SetCamSched(pCam);

	return;

}


