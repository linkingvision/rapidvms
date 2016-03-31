#include "config/vidstor/camsetting/vsccaminfo.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include <QDesktopWidget>
#include "client/storsyncinf.hpp"



VSCCamInfo::VSCCamInfo(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent)
    : m_pFactory(pFactory), m_pStor(stor), m_strCam(strCam), QWidget(parent)
{
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	syncInf.Connect();
	
	ui.setupUi(this);
	
	VidCamera pCam;
	
	syncInf.GetVidCamera(m_strCam, pCam);
	switch(pCam.ntype())
	{
		case VID_FILE:
			ui.radioButtonFile->setChecked(true);
			break;
		case VID_RTSP:
			ui.radioButtonRtsp->setChecked(true);
			break;
		case VID_ONVIF_S:
			ui.radioButtonOnvif->setChecked(true);
			break;
		case VID_MJPEG:
			ui.radioButtonMJPEG->setChecked(true);
			break;
		default:
			break;
	}

	ui.lineEditName->setText(pCam.strname().c_str());
	ui.lineEditIP->setText(pCam.strip().c_str());
	ui.lineEditPort->setText(pCam.strport().c_str());
	ui.lineEditFile->setText(pCam.strfile().c_str());
	ui.lineEditRtspAddr->setText(pCam.strrtspurl().c_str());

	delete pLoading;
}




void VSCCamInfo::applyConfig()
{

	return;

}


