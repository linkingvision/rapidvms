#ifndef __VSC_RECORDER_H__
#define __VSC_RECORDER_H__

#include <QWidget>
#include <QTimer>
#include <QTreeWidgetItem>
#include "factory.hpp"
#include <QObject>


class VSCRecorder: public QObject
{	
	Q_OBJECT;
public:
    //VSCRecorder(VSCVmsDataItem &pParam);
	VSCRecorder(u32 id ,QString ip, VSCVmsSubType Type, QString Ip);
	VSCRecorder(VSCRecorder & pRecoder);
    ~VSCRecorder();

	u32 GetDeviceId()
	{
	    //return m_Param.m_Conf.data.conf.nId;
		return 0;
	}

	BOOL AddAnIPCamera();
	void GetResult(const QJsonObject& json);//

	u32 m_Id;
	QString m_Ip;
	VSCVmsSubType m_Type;
private:
	//VSCVmsDataItem m_Param;
	QString mServerIp;

};

typedef std::map<int, VSCRecorder > RecorderMap;


#endif // __VSC_VMS_H__
