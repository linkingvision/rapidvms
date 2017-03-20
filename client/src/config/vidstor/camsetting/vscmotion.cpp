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
#include <QVariant>
#include "config/vidstor/camsetting/vscmotion.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"
#include "vmotconf.hpp"
#include "config/vidstor/camsetting/vscmotreg.h"

VSCMotion::VSCMotion(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory), m_pStor(stor), m_strCam(strCam)
{
	ui.setupUi(this);
	//VSCLoading * pLoading = VSCLoading::Create();
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
		ui.enable->setToggle(false);
	}else
	{
		ui.enable->setToggle(true);
	}
	
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
	connect( this->ui.setupMot, SIGNAL( clicked() ), this, SLOT(MotConf()));

	

	//delete pLoading;
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
	if (ui.enable->isToggled() == true)
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


void VSCMotion::MotConf()
{
	VSCMotRegConf *pConf = new VSCMotRegConf(m_pFactory, m_pStor, m_strCam);
	pConf->show();
}


