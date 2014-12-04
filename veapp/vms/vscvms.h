#ifndef __VSC_VMS_H__
#define __VSC_VMS_H__

#include <QWidget>
#include <QTimer>
#include <QTreeWidgetItem>
#include "factory.hpp"
#include "vscrecorder.h"
#include <QObject>

class VSCVms : public QObject , public QTreeWidgetItem //多继承，qobject必须在继承的类的第一个
{
	Q_OBJECT;

public:
	void mousePressEvent(QMouseEvent *event);

public:
    VSCVms(QTreeWidgetItem *parent, VSCVmsDataItem &pParam);
    ~VSCVms();
	
public:
	u32 GetDeviceId()
	{
	    return m_Param.nId;
	}
	virtual BOOL GetRecorderMap(RecorderMap & pMap){return FALSE;}
	virtual BOOL Refresh(){return FALSE;}


private:
	VSCVmsDataItem m_Param;
	QTreeWidgetItem *m_pParent;

};

class VSCVmsPg : public VSCVms
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
    VSCVmsPg(QTreeWidgetItem *parent, VSCVmsDataItem &pParam);
    ~VSCVmsPg();
public:
	virtual BOOL Refresh(){return FALSE;}


private:

};

class VSCVmsZb : public VSCVms
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
    VSCVmsZb(QTreeWidgetItem *parent, VSCVmsDataItem &pParam);
    ~VSCVmsZb();
public:
	/* Reconnect site to refresh the data */
	virtual BOOL Refresh();
	virtual BOOL GetRecorderMap(RecorderMap & pMap);
	void printNVRList(const QJsonObject& json);//	
	void ShowRefresh(const QJsonObject& json);
private:
	RecorderMap mMap;
	QString mIp;
};

class VSCVmsVirtualIPC : public VSCVms
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
	VSCVmsVirtualIPC(QTreeWidgetItem *parent, VSCVmsDataItem &pParam):
		VSCVms(parent, pParam){}
	~VSCVmsVirtualIPC(){}
public:
	/* Reconnect site to refresh the data */
	virtual BOOL Refresh(){return true;}
	virtual BOOL GetRecorderMap(RecorderMap & pMap){return true;}
private:

};


#endif // __VSC_VMS_H__
