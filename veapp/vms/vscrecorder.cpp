
#include "vscrecorder.h"
#include "factory.hpp"
#include "QColor"
#include <QEventLoop>

extern Factory *gFactory;

VSCRecorder::VSCRecorder(u32 id ,QString ip, VSCVmsSubType Type, QString ServerIp)
	:m_Id(id),m_Ip(ip), 
	m_Type(Type), mServerIp(ServerIp)
{
	
}


VSCRecorder::VSCRecorder(VSCRecorder & pRecoder)
: m_Id(pRecoder.m_Id),  m_Ip(pRecoder.m_Ip), m_Type(pRecoder.m_Type), mServerIp(pRecoder.mServerIp)
{
}


VSCRecorder::~VSCRecorder()
{

}

void VSCRecorder::GetResult(const QJsonObject& json)//hjy
{
	//qDebug() << QString("GetResult");
}

BOOL VSCRecorder::AddAnIPCamera()
{

	return TRUE;
}
