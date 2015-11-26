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
