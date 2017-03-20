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
#ifndef __VSC_VID_SEARCH_PB_H__
#define __VSC_VID_SEARCH_PB_H__

#include "common/vscvidinf.h"
#include "searchpb/vscvideventsearch.h"

class VSCVidSearchPB : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidSearchPB(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent);
    ~VSCVidSearchPB();
	
public:
	virtual void VidShow();
	virtual void VidHide();
	virtual void VidNewEventSearch();
	virtual void VidNewMotionSearch();
	virtual VSCVidTreeInf *GetCameraTree();
	virtual bool CheckClosed(QWidget * pTab);

private:
	VSCVidTreeInf *m_pCameraTree;
	VSCVidEventSearch *m_pSearch;

};
#endif
	