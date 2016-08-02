#include <QMessageBox>
#include <QVariant>
#include "config/vidstor/camsetting/vscmotion.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"

VSCMotion::VSCMotion(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory), m_pStor(stor), m_strCam(strCam)
{
	ui.setupUi(this);
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	
	syncInf.Connect();


	VidCamera pCam;
	
	syncInf.GetVidCamera(m_strCam, pCam);
	
	ui.stream->setEditable(false);

	int current = pCam.nservermotionstream() -1 ;
	if (current <= 0 || current > 1)
	{
		current = 1;/* Default use the stream 2 */
	}

	ui.stream->setCurrentIndex(current);

	if (pCam.bservermotion()== false)
	{
		ui.enable->setChecked(false);
	}else
	{
		ui.enable->setChecked(true);
	}
	
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));

	delete pLoading;
}

void VSCMotion::applyConfig()
{
	VSCLoading loading(NULL);
	StorSyncInf syncInf(m_pStor);
	
	syncInf.Connect();


	VidCamera pCam;
	
	syncInf.GetVidCamera(m_strCam, pCam);

	int current = ui.stream->currentIndex();
	current = current + 1;
	if (current < 1 || current > 2)
	{
		current = 2;
	}
	if (ui.enable->isChecked() == true)
	{
		pCam.set_bservermotion(true);
	}else
	{
		pCam.set_bservermotion(false);
	}
	pCam.set_nservermotionstream(current);

	
	syncInf.AddCam(pCam);
	return;

}


