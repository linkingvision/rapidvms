#include "systemdateandtime.h"
#include "message.h"
using namespace ONVIF;

SystemDateAndTime::SystemDateAndTime(QObject *parent) : QObject(parent) {
}

void SystemDateAndTime::setUtcTime(int year, int month, int day, int hour, int minute, int second) {
    QDate date;
    date.setDate(year, month, day);
    QTime time;
    time.setHMS(hour, minute, second);
    this->m_utcTime = QDateTime(date, time, Qt::UTC);
}

void SystemDateAndTime::setLocalTime(int year, int month, int day, int hour, int minute, int second) {
    QDate date;
    date.setDate(year, month, day);
    QTime time;
    time.setHMS(hour, minute, second);
    this->m_localTime = QDateTime(date, time);
}

QDomElement SystemDateAndTime::toxml(){
    QDomElement setSystemDateAndTime,dateTimeType,daylightSavings,timeZone,tz,utcDateTime,time,hour,minute,second,date,year
            ,month,day;
    setSystemDateAndTime = newElement("wsdl:SetSystemDateAndTime");
    dateTimeType = newElement("wsdl:DateTimeType","Manual");
    daylightSavings = newElement("wsdl:DaylightSavings",this->daylightSavings() == true?"true":"false");
    timeZone = newElement("wsdl:TimeZone");
    tz = newElement("sch:TZ",this->tz());
    utcDateTime = newElement("wsdl:UTCDateTime");
    time = newElement("sch:Time");
    hour = newElement("sch:Hour",QString::number(this->localTime().time().hour()));
    minute = newElement("sch:Minute",QString::number(this->localTime().time().minute()));
    second = newElement("sch:Second",QString::number(this->localTime().time().second()));
    date = newElement("sch:Date");
    year = newElement("sch:Year",QString::number(this->localTime().date().year()));
    month = newElement("sch:Month",QString::number(this->localTime().date().month()));
    day = newElement("sch:Day",QString::number(this->localTime().date().day()));
    setSystemDateAndTime.appendChild(dateTimeType);
    setSystemDateAndTime.appendChild(daylightSavings);
    setSystemDateAndTime.appendChild(timeZone);
    setSystemDateAndTime.appendChild(utcDateTime);
    timeZone.appendChild(tz);
    utcDateTime.appendChild(time);
    utcDateTime.appendChild(date);
    time.appendChild(hour);
    time.appendChild(minute);
    time.appendChild(second);
    date.appendChild(year);
    date.appendChild(month);
    date.appendChild(day);
    return setSystemDateAndTime;
}
