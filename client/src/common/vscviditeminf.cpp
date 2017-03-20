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
#include "common/vscviditeminf.h"

VSCVidItemInf::VSCVidItemInf(ClientFactory &pFactory, QTreeWidgetItem *parent)
: QTreeWidgetItem(parent), m_pFactory(pFactory), m_bOnline(false), m_bRec(false), 
m_bSetChecked(false)
{
	UpdateOnline(false);
}
VSCVidItemInf::~VSCVidItemInf()
{
	
}

void VSCVidItemInf::VidSetChecked(QTreeWidgetItem * item, bool bChecked)
{
	setFlags(flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	//setCheckState(Qt::Unchecked);
	/* The event is send from myself */
	if (item == this)
	{
		return;
	}
	if (bChecked == true)
	{
		setCheckState(0, Qt::Checked);
	}else
	{
		setCheckState(0, Qt::Unchecked);
	}

	return;
}

bool VSCVidItemInf::VidGetChecked()
{
	if (checkState(0) == Qt::Checked)
	{
		return true;
	}else
	{
		return false;
	}
}

void VSCVidItemInf::UpdateOnline(bool bOnline)
{
	if (bOnline == true)
	{
		this->setTextColor(0, QColor(0, 170, 0));
		m_bOnline = true;
		if (m_bRec == true)
		{
			this->setTextColor(0, QColor(255, 0, 0));
		}
	}else
	{
	    	this->setTextColor(0, QColor(194, 194, 194));
		m_bOnline = false;
	}
}

void VSCVidItemInf::UpdateRec(bool bRec)
{
	m_bRec = bRec;

	if (bRec == true && m_bOnline == true)
	{
		this->setTextColor(0, QColor(255, 0, 0));
	}

	if (bRec == false && m_bOnline == true)
	{
		this->setTextColor(0, QColor(0, 170, 0));
	}
	
}
