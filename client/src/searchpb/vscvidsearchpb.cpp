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
#include "searchpb/vscvidsearchpb.h"
#include "searchpb/vscvideventsearch.h"
#include "common/vidtree/vscvidtreecam.h"

VSCVidSearchPB::VSCVidSearchPB(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent)
	: m_pCameraTree(NULL), m_pSearch(NULL), VSCVidInf(pFactory, pTab, parent)
{
	m_pCameraTree = new VSCVidTreeCam(m_pFactory, parent);
	m_pCameraTree->VidSetCheckedChild(NULL, true);
	m_pCameraTree->Init();
	m_pCameraTree->VidSetCheckedChild(NULL, true);
	m_pCameraTree->VidSetCheckedChild(NULL, false);
	m_pCameraTree->hide();
}
VSCVidSearchPB::~VSCVidSearchPB()
{
	
}

void VSCVidSearchPB::VidShow()
{

}
void VSCVidSearchPB::VidHide()
{

}

void VSCVidSearchPB::VidNewEventSearch()
{
	if (m_pSearch == NULL)
	{
		m_pSearch = new VSCVidEventSearch(m_pFactory, *m_pCameraTree, &m_pMainArea);
		m_pSearch->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint  );
	}

	m_pSearch->show();
	m_pMainArea.addTab(m_pSearch, QIcon(tr(":/action/resources/alarm_search.png")), tr("EVENT SEARCH"));
	m_pMainArea.setCurrentWidget(m_pSearch);
}

bool VSCVidSearchPB::CheckClosed(QWidget * pTab)
{
	if (m_pSearch == pTab)
	{
		return true;
	}else
	{
		return false;
	}
}

void VSCVidSearchPB::VidNewMotionSearch()
{
}

VSCVidTreeInf *VSCVidSearchPB::GetCameraTree()
{
	return m_pCameraTree;
}
