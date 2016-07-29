#include <QMessageBox>
#include "config/vidstor/camsetting/vsccamrec.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"

VSCCamRec::VSCCamRec(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory), m_pStor(stor), m_strCam(strCam)
{
	ui.setupUi(this);
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	
	syncInf.Connect();

	VidCamera pCam;
	bool bGetSched = false;
	astring strSched;
	
	syncInf.GetVidCamera(m_strCam, pCam);	
#if 0
	int nSchedSize = pCam.crecsched_size();

	for (s32 i = 0; i < pCam.crecsched_size(); i ++)
	{
		/* Current only use the first one */
		strSched = pCam.crecsched(i);
		bGetSched = true;
		break;
	}
	if (bGetSched == false)
	{
		strSched = REC_SCHED_OFF;
	}

	if (strSched == REC_SCHED_ALL_DAY)
	{
		ui.recType->setCurrentIndex(0);
	}else if (strSched == REC_SCHED_WORK_DAY)
	{
		ui.recType->setCurrentIndex(1);
	}else if (strSched == REC_SCHED_OFF)
	{
		ui.recType->setCurrentIndex(2);
	}
#endif	
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));

	delete pLoading;
}

void VSCCamRec::applyConfig()
{
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;
	syncInf.Connect();

	VidCamera pCam;
	bool bGetSched = false;
	astring strSched;
	
	syncInf.GetVidCamera(m_strCam, pCam);	

	switch (ui.recType->currentIndex())
	{
		case 0:
		{
			strSched = REC_SCHED_ALL_DAY;
			break;
		}
		case 1:
		{
			strSched = REC_SCHED_WORK_DAY;
			break;
		}
		case 2:
		{
			strSched = REC_SCHED_OFF;
			break;
		}
		default:
		{
			strSched = REC_SCHED_ALL_DAY;
			break;
		}
	}
#if 0
	pCam.clear_crecsched();
	int nSchedSize = pCam.crecsched_size();
	if (nSchedSize == 0)
	{
		pCam.add_crecsched(strSched);
	}
#endif
	syncInf.SetCamSched(pCam);
	
	delete pLoading;

	return;

}


