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
#include "vscdeviceipc.h"
#include "server/factory.hpp"
#include "QColor"

extern Factory *gFactory;

VSCDeviceIPC::VSCDeviceIPC(QTreeWidgetItem *parent)
    : QTreeWidgetItem(parent)
{
	//m_Param = pParam;
    UpdateOnline(FALSE);
	UpdateRecord(FALSE);
}

void VSCDeviceIPC::mousePressEvent(QMouseEvent *event)
{
	//VDC_DEBUG( "%s \n",__FUNCTION__);
}


void VSCDeviceIPC::UpdateOnline(BOOL bonline)
{
	if (bonline == TRUE)
	{
	    this->setTextColor(0, QColor(0, 170, 0));
	      //this->setTextColor(0, QColor(205, 0, 0));
	      //this->setTextColor(0, QColor(255, 0, 0));
	      //VDC_DEBUG( "%s Set to Green %d\n",__FUNCTION__, m_Param.m_Conf.data.conf.nId);
	}else
	{
	    this->setTextColor(0, QColor(194, 194, 194));
	      //VDC_DEBUG( "%s Set to black %d\n",__FUNCTION__, m_Param.m_Conf.data.conf.nId);
	}
}

void VSCDeviceIPC::UpdateRecord(BOOL bRecording)
{
	QIcon icon1;
#if 0
	DeviceParam pParam;
	gFactory->GetDeviceParamById(pParam, GetDeviceId());
	if (pParam.m_Conf.data.conf.HdfsRecording == 1 
		&& pParam.m_Conf.data.conf.Recording == 1)
	{
                icon1.addFile(QStringLiteral(":/action/resources/dome-recordhdfs.png"), QSize(), QIcon::Normal, QIcon::Off);
                setIcon(0, icon1);	
		 return ;
	}
	if (pParam.m_Conf.data.conf.HdfsRecording == 0 
		&& pParam.m_Conf.data.conf.Recording == 0)
	{
                icon1.addFile(QStringLiteral(":/device/resources/dome.png"), QSize(), QIcon::Normal, QIcon::Off);
                setIcon(0, icon1);		
		 return ;
	}
	if (pParam.m_Conf.data.conf.HdfsRecording == 1 
		&& pParam.m_Conf.data.conf.Recording == 0)
	{
                icon1.addFile(QStringLiteral(":/action/resources/hdfsstartrecord.png"), QSize(), QIcon::Normal, QIcon::Off);
                setIcon(0, icon1);		
		 return ;
	}
	if (pParam.m_Conf.data.conf.HdfsRecording == 0 
		&& pParam.m_Conf.data.conf.Recording == 1)
	{
                icon1.addFile(QStringLiteral(":/device/resources/dome-record.png"), QSize(), QIcon::Normal, QIcon::Off);

                setIcon(0, icon1);	
		 return ;
	}
#endif
}

VSCDeviceIPC::~VSCDeviceIPC()
{

}

VSCDeviceIPCOAPI::VSCDeviceIPCOAPI(QTreeWidgetItem *parent)
    : QTreeWidgetItem(parent)
{
#if 0
	m_Param = pParam;
	memcpy(&m_pVMSParam, &pVMSParam, sizeof(VSCVmsDataItem));
	if (pParam.Online == 1)
	{
		UpdateOnline(TRUE);
	}else
	{
		UpdateOnline(FALSE);
	}
	UpdateRecord(FALSE);
#endif
}

void VSCDeviceIPCOAPI::mousePressEvent(QMouseEvent *event)
{
	//VDC_DEBUG( "%s \n",__FUNCTION__);
}


void VSCDeviceIPCOAPI::UpdateOnline(BOOL bonline)
{
	if (bonline == TRUE)
	{
	    this->setTextColor(0, QColor(0, 170, 0));
	      //this->setTextColor(0, QColor(205, 0, 0));
	      //this->setTextColor(0, QColor(255, 0, 0));
	      //VDC_DEBUG( "%s Set to Green %d\n",__FUNCTION__, m_Param.m_Conf.data.conf.nId);
	}else
	{
	    this->setTextColor(0, QColor(194, 194, 194));
	      //VDC_DEBUG( "%s Set to black %d\n",__FUNCTION__, m_Param.m_Conf.data.conf.nId);
	}
}

void VSCDeviceIPCOAPI::UpdateRecord(BOOL bRecording)
{
#if 0
	QIcon icon1;
	DeviceParam pParam;
	gFactory->GetDeviceParamById(pParam, GetDeviceId());
	if (pParam.m_Conf.data.conf.HdfsRecording == 1 
		&& pParam.m_Conf.data.conf.Recording == 1)
	{
                icon1.addFile(QStringLiteral(":/action/resources/dome-recordhdfs.png"), QSize(), QIcon::Normal, QIcon::Off);
                setIcon(0, icon1);	
		 return ;
	}
	if (pParam.m_Conf.data.conf.HdfsRecording == 0 
		&& pParam.m_Conf.data.conf.Recording == 0)
	{
                icon1.addFile(QStringLiteral(":/device/resources/dome.png"), QSize(), QIcon::Normal, QIcon::Off);
                setIcon(0, icon1);		
		 return ;
	}
	if (pParam.m_Conf.data.conf.HdfsRecording == 1 
		&& pParam.m_Conf.data.conf.Recording == 0)
	{
                icon1.addFile(QStringLiteral(":/action/resources/hdfsstartrecord.png"), QSize(), QIcon::Normal, QIcon::Off);
                setIcon(0, icon1);		
		 return ;
	}
	if (pParam.m_Conf.data.conf.HdfsRecording == 0 
		&& pParam.m_Conf.data.conf.Recording == 1)
	{
                icon1.addFile(QStringLiteral(":/device/resources/dome-record.png"), QSize(), QIcon::Normal, QIcon::Off);

                setIcon(0, icon1);	
		 return ;
	}
#endif
}

VSCDeviceIPCOAPI::~VSCDeviceIPCOAPI()
{

}

VSCDeviceIPCGroup::VSCDeviceIPCGroup(QTreeWidgetItem *parent)
    : QTreeWidgetItem(parent)
{
	//memcpy(&m_Param, &pParam, sizeof(VSCVGroupDataItem));
}

void VSCDeviceIPCGroup::mousePressEvent(QMouseEvent *event)
{
	//VDC_DEBUG( "%s \n",__FUNCTION__);
}


void VSCDeviceIPCGroup::UpdateOnline(BOOL bonline)
{

}

void VSCDeviceIPCGroup::UpdateRecord(BOOL bRecording)
{

}

VSCDeviceIPCGroup::~VSCDeviceIPCGroup()
{

}

