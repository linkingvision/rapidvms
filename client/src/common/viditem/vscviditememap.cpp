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
#include "common/viditem/vscviditememap.h"

VSCVidItemEmap::VSCVidItemEmap(VidEmap cEmap, ClientFactory &pFactory, QTreeWidgetItem *parent)
	: m_cEmap(cEmap), VSCVidItemInf(pFactory, parent)
{
	QIcon icon1;

	icon1.addFile(QStringLiteral(":/action/resources/map.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);

	setIcon(0, icon1);

	setText(0, QApplication::translate(" ", m_cEmap.strname().c_str(), 0));
	this->setTextColor(0, QColor(41, 42, 43));
	//setHidden(true);
}
VSCVidItemEmap::~VSCVidItemEmap()
{
	
}

void VSCVidItemEmap::VidFilter(astring strFilter)
{
	VDC_DEBUG( "%s %d Filter %s name %s\n",__FUNCTION__, __LINE__, 
		strFilter.c_str(), m_cEmap.strname().c_str());
	if (strFilter.size() == 0)
	{
		setHidden(false);
		return;
	}
	std::size_t found = m_cEmap.strname().find(strFilter);
	if (found != std::string::npos)
	{
		setHidden(false);
	}else
	{
		setHidden(true);
	}
}