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
#include <QMessageBox>
#include "config/vidstor/camsetting/vsccamrec.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"

VSCCamRec::VSCCamRec(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory), m_pStor(stor), m_strCam(strCam)
{
	ui.setupUi(this);
	//VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	
	syncInf.Connect();

	VidCamera pCam;
	bool bGetSched = false;
	astring strSched;
	
	syncInf.GetVidCamera(m_strCam, pCam);	

	int current = pCam.nrecordstream() -1 ;
	if (current <= 0 || current > 1)
	{
		current = 0;/* Default use the stream 2 */
	}

	ui.stream->setCurrentIndex(current);

	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));

	//delete pLoading;
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
	int current = ui.stream->currentIndex();
	current = current + 1;
	if (current < 1 || current > 2)
	{
		current = 2;
	}

	pCam.set_nrecordstream(current);

	
	syncInf.AddCam(pCam);
	
	delete pLoading;

	return;

}


