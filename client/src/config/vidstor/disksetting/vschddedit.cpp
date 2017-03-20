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
#include "config/vidstor/disksetting/vschddedit.h"
#include "debug.hpp"
#include "config/vidstor/disksetting/vschddoneedit.h"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"

VSCHddEdit::VSCHddEdit(VidStor &stor, QWidget *parent)
    : QWidget(parent), m_pStor(stor)
{
	ui.setupUi(this);
	ui.storName->setText(m_pStor.strname().c_str());
	ui.scrollArea->setBackgroundRole(QPalette::Light);
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;
	syncInf.Connect();

	VidDiskList sysDiskList = syncInf.GetSysVidDiskList();

	VidDiskList diskList = syncInf.GetVidDiskList();

	for (s32 i = 0; i < sysDiskList.cviddisk_size(); i ++)
	{
		VidDisk disk = sysDiskList.cviddisk(i);
		m_Disks[i] = new VSCHddOneEdit(m_pStor, disk, diskList, this);
		ui.verticalLayout->addWidget(m_Disks[i] );

	}
	delete pLoading;
}



