#ifndef __VSC_ZB_TREE_ITEM_H__
#define __VSC_ZB_TREE_ITEM_H__

#include <QWidget>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QMimeData>
#include <QObject>
#include "factory.hpp"

class VSCQMimeDataIPC : public QMimeData
{
public:
    VSCQMimeDataIPC();//add param of IPC and NVR here
    ~VSCQMimeDataIPC(){}
private:
	//TODO add what the IPC info and NVR info the here
};

class VSCZbTreeItemNVR : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);
	//void DeleteIPC(){}//TODO
public:
    VSCZbTreeItemNVR(QTreeWidgetItem *parent);
    ~VSCZbTreeItemNVR(){}

};

class VSCZbTreeItemIPC : public QObject, public QTreeWidgetItem
{
	Q_OBJECT;
public:
	void mousePressEvent(QMouseEvent *event);
	
	u32 GetDeviceId()
	{
	    //return m_IpcId;
		return m_Param.m_Conf.data.conf.nId;
	}

	DeviceParam GetDeviceParam()
	{
		return m_Param;
	}

public:
    VSCZbTreeItemIPC(QTreeWidgetItem *parent, DeviceParam &pParam, QString ServerIp);
	//VSCZbTreeItemIPC(QTreeWidgetItem *parent, u32 TreeItemIPCid , QString TreeItemIPCip, QString ServerIp);
    ~VSCZbTreeItemIPC(){}

public:
	BOOL DeleteTreeItemIPC();
	void GetResult(const QJsonObject& json);
	void VSCZbIPCRecord();
	void VSCZbIPCStopRecord();
	void VSCZbIPCUpdateOnline(BOOL bonline);//huang
    void VSCZbIPCUpdateRecord(BOOL bRecording);//huang

private:
	//u32 m_IpcId;
	//QString m_IpcIp;
	QString mServerIp;
	DeviceParam m_Param;
};


#endif // __VSC_VMS_H__
