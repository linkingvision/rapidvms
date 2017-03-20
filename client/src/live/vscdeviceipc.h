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
#ifndef VSCDEVICE_IPC_H
#define VSCDEVICE_IPC_H

#include <QWidget>
#include <QTimer>
#include <QTreeWidgetItem>
#include "server/factory.hpp"

class VSCDeviceIPC : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
    //VSCDeviceIPC(QTreeWidgetItem *parent, DeviceParam &pParam);
	VSCDeviceIPC(QTreeWidgetItem *parent);
    ~VSCDeviceIPC();
	
public:
	u32 GetDeviceId()
	{
	    return 1;
	}
	
public:
    void UpdateOnline(BOOL bonline);
    void UpdateRecord(BOOL bRecording);	


private:
	//DeviceParam m_Param;

};

class VSCDeviceIPCOAPI : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
    //VSCDeviceIPCOAPI(QTreeWidgetItem *parent, oapi::Device &pParam, 
		//VSCVmsDataItem &pVMSParam);
	VSCDeviceIPCOAPI(QTreeWidgetItem *parent);
    ~VSCDeviceIPCOAPI();
	
public:
	u32 GetDeviceId()
	{
		return 1;
	}
	u32 GetVMSId()
	{
		return 1;
	}
	
	
public:
    void UpdateOnline(BOOL bonline);
    void UpdateRecord(BOOL bRecording);	


private:
	//oapi::Device m_Param;
	//VSCVmsDataItem m_pVMSParam;

};


class VSCDeviceIPCGroup : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
    //VSCDeviceIPCGroup(QTreeWidgetItem *parent, VSCVGroupDataItem &pParam);
	VSCDeviceIPCGroup(QTreeWidgetItem *parent);
    ~VSCDeviceIPCGroup();
	
public:
	u32 GetDeviceId()
	{
	    return 1;
	}
	
public:
    void UpdateOnline(BOOL bonline);
    void UpdateRecord(BOOL bRecording);	


private:
	//VSCViewDataItem m_Param;

};

#endif // VSCDEVICE_IPC_H
