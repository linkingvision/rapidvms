#ifndef VIDEOENCODERCONFIGURATION_H
#define VIDEOENCODERCONFIGURATION_H
#include <QObject>

namespace  ONVIF {
    class VideoEncoderConfiguration : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString token READ token WRITE setToken)
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(int useCount READ useCount WRITE setUseCount)
        Q_PROPERTY(QString encoding READ encoding WRITE setEncoding)
        Q_PROPERTY(int width READ width WRITE setWidth)
        Q_PROPERTY(int height READ height WRITE setHeight)
        Q_PROPERTY(int quality READ quality WRITE setQuality)
        Q_PROPERTY(int frameRateLimit READ frameRateLimit WRITE setFrameRateLimit)
        Q_PROPERTY(int encodingInterval READ encodingInterval WRITE setEncodingInterval)
        Q_PROPERTY(int bitrateLimit READ bitrateLimit WRITE setBitrateLimit)
        Q_PROPERTY(int govLength READ govLength WRITE setGovLength)
        Q_PROPERTY(QString h264Profile READ h264Profile WRITE setH264Profile)
        Q_PROPERTY(QString type READ type WRITE setType)
        Q_PROPERTY(QString ipv4Address READ ipv4Address WRITE setIpv4Address)
        Q_PROPERTY(QString ipv6Address READ ipv6Address WRITE setIpv6Address)
        Q_PROPERTY(int port READ port WRITE setPort)
        Q_PROPERTY(int ttl READ ttl WRITE setTtl)
        Q_PROPERTY(bool autoStart READ autoStart WRITE setAutoStart)
        Q_PROPERTY(QString sessionTimeout READ sessionTimeout WRITE setSessionTimeout)
    public:
        explicit VideoEncoderConfiguration(QObject *parent = NULL);
        virtual ~VideoEncoderConfiguration();

        QString token() const
        {
            return m_token;
        }

        QString name() const
        {
            return m_name;
        }

        int useCount() const
        {
            return m_useCount;
        }

        QString encoding() const
        {
            return m_encoding;
        }

        int width() const
        {
            return m_width;
        }

        int height() const
        {
            return m_height;
        }

        int quality() const
        {
            return m_quality;
        }

        int frameRateLimit() const
        {
            return m_frameRateLimit;
        }

        int encodingInterval() const
        {
            return m_encodingInterval;
        }

        int bitrateLimit() const
        {
            return m_bitrateLimit;
        }

        int govLength() const
        {
            return m_govLength;
        }

        QString h264Profile() const
        {
            return m_h264Profile;
        }

        QString type() const
        {
            return m_type;
        }

        QString ipv4Address() const
        {
            return m_ipv4Address;
        }

        QString ipv6Address() const
        {
            return m_ipv6Address;
        }

        int port() const
        {
            return m_port;
        }

        int ttl() const
        {
            return m_ttl;
        }

        bool autoStart() const
        {
            return m_autoStart;
        }

        QString sessionTimeout() const
        {
            return m_sessionTimeout;
        }

    public slots:
        void setToken(QString arg)
        {
            m_token = arg;
        }

        void setName(QString arg)
        {
            m_name = arg;
        }

        void setUseCount(int arg)
        {
            m_useCount = arg;
        }

        void setEncoding(QString arg)
        {
            m_encoding = arg;
        }

        void setWidth(int arg)
        {
            m_width = arg;
        }

        void setHeight(int arg)
        {
            m_height = arg;
        }

        void setQuality(int arg)
        {
            m_quality = arg;
        }

        void setFrameRateLimit(int arg)
        {
            m_frameRateLimit = arg;
        }

        void setEncodingInterval(int arg)
        {
            m_encodingInterval = arg;
        }

        void setBitrateLimit(int arg)
        {
            m_bitrateLimit = arg;
        }

        void setGovLength(int arg)
        {
            m_govLength = arg;
        }

        void setH264Profile(QString arg)
        {
            m_h264Profile = arg;
        }

        void setType(QString arg)
        {
            m_type = arg;
        }

        void setIpv4Address(QString arg)
        {
            m_ipv4Address = arg;
        }

        void setIpv6Address(QString arg)
        {
            m_ipv6Address = arg;
        }

        void setPort(int arg)
        {
            m_port = arg;
        }

        void setTtl(int arg)
        {
            m_ttl = arg;
        }

        void setAutoStart(bool arg)
        {
            m_autoStart = arg;
        }

        void setSessionTimeout(QString arg)
        {
            m_sessionTimeout = arg;
        }

    private:
        QString m_token;
        QString m_name;
        int m_useCount;
        QString m_encoding;
        int m_width;
        int m_height;
        int m_quality;
        int m_frameRateLimit;
        int m_encodingInterval;
        int m_bitrateLimit;
        int m_govLength;
        QString m_h264Profile;
        QString m_type;
        QString m_ipv4Address;
        QString m_ipv6Address;
        int m_port;
        int m_ttl;
        bool m_autoStart;
        QString m_sessionTimeout;
    };
}
#endif // VIDEOENCODERCONFIGURATION_H
