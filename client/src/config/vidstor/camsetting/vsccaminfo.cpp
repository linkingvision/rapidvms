/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#include "config/vidstor/camsetting/vsccaminfo.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include <QDesktopWidget>
#include "client/storsyncinf.hpp"



VSCCamInfo::VSCCamInfo(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent)
    : m_pFactory(pFactory), m_pStor(stor), m_strCam(strCam), QWidget(parent), 
    m_strCamName("Loading...")
{
	//VSCLoading * pLoading = VSCLoading::Create();
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
	m_strCamName = pCam.strname();
	ui.lineEditName->setText(pCam.strname().c_str());
	ui.lineEditIP->setText(pCam.strip().c_str());
	ui.lineEditPort->setText(pCam.strport().c_str());
	ui.lineEditFile->setText(pCam.strfile().c_str());
	ui.lineEditRtspAddr->setText(pCam.strrtspurl().c_str());

	//delete pLoading;
}




void VSCCamInfo::applyConfig()
{

	return;

}


