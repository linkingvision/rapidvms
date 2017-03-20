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
#include "common/vscvidtreeinf.h"
#include "common/vscviditeminf.h"


VSCVidTreeInf::VSCVidTreeInf(ClientFactory &pFactory, QWidget *parent)
: QTreeWidget(parent), m_pFactory(pFactory), m_bSetChecked(false)
{
	setHeaderHidden(true);
	setIconSize(QSize(20, 20));
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	connect(this, SIGNAL(itemClicked(QTreeWidgetItem * , int )), 
		this, SLOT(SlotItemClicked(QTreeWidgetItem * , int )));
}
VSCVidTreeInf::~VSCVidTreeInf()
{
	
}

void VSCVidTreeInf::SlotItemClicked(QTreeWidgetItem * item, int column)
{
	VDC_DEBUG("%s QTreeWidget %d \n", __FUNCTION__, column);
	/* Unselect all the other items, TODO add a bool to control how to proccess */
	VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf * >(item);
	if (pItem )
	{
		if (pItem->VidGetChecked() == true)
		{
			VidSetCheckedChild(item, false);	
		}
	}
	
}
