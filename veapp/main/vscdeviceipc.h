

#ifndef VSCDEVICE_IPC_H
#define VSCDEVICE_IPC_H

#include <QWidget>
#include <QTimer>
#include <QTreeWidgetItem>
#include "factory.hpp"

class VSCDeviceIPC : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
    VSCDeviceIPC(QTreeWidgetItem *parent, DeviceParam &pParam);
    ~VSCDeviceIPC();
	
public:
	u32 GetDeviceId()
	{
	    return m_Param.m_Conf.data.conf.nId;
	}
	
public:
    void UpdateOnline(BOOL bonline);
    void UpdateRecord(BOOL bRecording);	


private:
	DeviceParam m_Param;

};

class VSCDeviceIPCOAPI : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
    VSCDeviceIPCOAPI(QTreeWidgetItem *parent, oapi::Device &pParam);
    ~VSCDeviceIPCOAPI();
	
public:
	u32 GetDeviceId()
	{
	    return m_Param.nId;
	}
	
public:
    void UpdateOnline(BOOL bonline);
    void UpdateRecord(BOOL bRecording);	


private:
	oapi::Device m_Param;

};


class VSCDeviceIPCGroup : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
    VSCDeviceIPCGroup(QTreeWidgetItem *parent, VSCVGroupDataItem &pParam);
    ~VSCDeviceIPCGroup();
	
public:
	u32 GetDeviceId()
	{
	    return m_Param.nId;
	}
	
public:
    void UpdateOnline(BOOL bonline);
    void UpdateRecord(BOOL bRecording);	


private:
	VSCViewDataItem m_Param;

};

#endif // VSCDEVICE_IPC_H
