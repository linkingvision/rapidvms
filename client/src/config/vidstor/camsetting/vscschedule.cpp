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
#include "config/vidstor/camsetting/vscschedule.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"
#include "server/camera.hpp"

VSCSchedule::VSCSchedule(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory), m_pStor(stor), m_strCam(strCam)
{

	ui.setupUi(this);

	//VSCLoading loading(NULL);
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


