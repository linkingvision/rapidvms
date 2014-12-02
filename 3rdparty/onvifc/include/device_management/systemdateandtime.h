#ifndef ONVIF_SYSTEMDATEANDTIME_H
#define ONVIF_SYSTEMDATEANDTIME_H

#include <QDateTime>
#include <QDomElement>
namespace ONVIF {
    class SystemDateAndTime : public QObject {
        Q_OBJECT
        Q_ENUMS(DateTimeType)
        Q_PROPERTY(DateTimeType dateTimeType READ dateTimeType WRITE setDateTimeType)
        Q_PROPERTY(bool daylightSavings READ daylightSavings WRITE setDaylightSavings)
        Q_PROPERTY(QString tz READ tz WRITE setTz)
        
        Q_PROPERTY(QDateTime utcTime READ utcTime WRITE setutcTime)
        Q_PROPERTY(QDateTime localTime READ localTime WRITE setlocalTime)
        Q_PROPERTY(bool result READ result WRITE setResult)
    public:
        explicit SystemDateAndTime(QObject *parent = NULL);
        
        enum DateTimeType {Manual, NTP};
        void setUtcTime(int year, int month, int day, int hour, int minute, int second);
        void setLocalTime(int year, int month, int day, int hour, int minute, int second);
        QDomElement toxml();
        
        bool daylightSavings() const
        {
            return m_daylightSavings;
        }
        DateTimeType dateTimeType() const
        {
            return m_dateTimeType;
        }
        
        
        
        QDateTime utcTime() const
        {
            return m_utcTime;
        }
        
        QDateTime localTime() const
        {
            return m_localTime;
        }
        
        QString tz() const
        {
            return m_tz;
        }

        bool result() const
        {
            return m_result;
        }

    public slots:
        void setDaylightSavings(bool arg)
        {
            m_daylightSavings = arg;
        }
        void setDateTimeType(DateTimeType arg)
        {
            m_dateTimeType = arg;
        }
        
        void setutcTime(const QDateTime &arg)
        {
            m_utcTime = arg;
        }
        
        void setlocalTime(const QDateTime &arg)
        {
            m_localTime = arg;
        }
        
        void setTz(QString arg)
        {
            m_tz = arg;
        }

        void setResult(bool arg)
        {
            m_result = arg;
        }

    private:
        bool m_daylightSavings;
        DateTimeType m_dateTimeType;
        
        QDateTime m_utcTime;
        QDateTime m_localTime;
        QString m_tz;
        bool m_result;
    };
}

#endif // ONVIF_SYSTEMDATEANDTIME_H
