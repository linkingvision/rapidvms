#ifndef __VSC_VMS_H__
#define __VSC_VMS_H__

#include <QWidget>
#include <QTimer>
#include <QTreeWidgetItem>
#include "factory.hpp"
#include <QObject>

class VSCVms : public QObject , public QTreeWidgetItem //多继承，qobject必须在继承的类的第一个
{
	Q_OBJECT

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
	virtual BOOL Refresh(){return FALSE;}


public:
	VSCVmsDataItem m_Param;
	QTreeWidgetItem *m_pParent;

};


/* The QThread will get the device status and update the data to UI */
class VSCVMSOAPIThread :public QThread
{
	Q_OBJECT
public:
    VSCVMSOAPIThread(VSCVmsDataItem &pParam)
    :m_Quit(FALSE)
    {
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
	memcpy(&m_pParam, &pParam, sizeof(VSCVmsDataItem));
    }
    ~VSCVMSOAPIThread()
    {
    }
public:
	/* Get the device list from oapi server */
	void run();
	void setQuit()
	{
	    m_Quit = TRUE;
           return;
	}
signals:
    void UpdateDeviceList(oapi::DeviceList plist);
private:
	VSCVmsDataItem m_pParam;
	BOOL m_Quit;
};

class VSCVmsOAPI : public VSCVms
{
	Q_OBJECT
public:
	void mousePressEvent(QMouseEvent *event);

public:
	VSCVmsOAPI(QTreeWidgetItem *parent, VSCVmsDataItem &pParam);
	~VSCVmsOAPI();
public slots:
	void UpdateDeviceList(oapi::DeviceList plist);
public:
	/* Reconnect site to refresh the data */
	virtual BOOL Refresh();
	void printNVRList(const QJsonObject& json);//	
	void ShowRefresh(const QJsonObject& json);
private:
	QString mIp;
	VSCVMSOAPIThread *m_pThread;
};

#endif // __VSC_VMS_H__
