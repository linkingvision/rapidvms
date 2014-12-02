#ifndef AUDIOENCODERCONFIGURATION_H
#define AUDIOENCODERCONFIGURATION_H
#include <QObject>
namespace ONVIF {
    class AudioEncoderConfiguration : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString token READ token WRITE setToken)
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(int useCount READ useCount WRITE setUseCount)
        Q_PROPERTY(QString encoding READ encoding WRITE setEncoding)
        Q_PROPERTY(int bitrate READ bitrate WRITE setBitrate)
        Q_PROPERTY(int sampleRate READ sampleRate WRITE setSampleRate)
        Q_PROPERTY(QString type READ type WRITE setType)
        Q_PROPERTY(QString ipv4Address READ ipv4Address WRITE setIpv4Address)
        Q_PROPERTY(QString ipv6Address READ ipv6Address WRITE setIpv6Address)
        Q_PROPERTY(int port READ port WRITE setPort)
        Q_PROPERTY(int ttl READ ttl WRITE setTtl)
        Q_PROPERTY(bool autoStart READ autoStart WRITE setAutoStart)
        Q_PROPERTY(QString sessionTimeout READ sessionTimeout WRITE setSessionTimeout)
    public:
        explicit AudioEncoderConfiguration(QObject *parent = NULL);
        virtual ~AudioEncoderConfiguration();
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

        int bitrate() const
        {
            return m_bitrate;
        }

        int sampleRate() const
        {
            return m_sampleRate;
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

        void setBitrate(int arg)
        {
            m_bitrate = arg;
        }

        void setSampleRate(int arg)
        {
            m_sampleRate = arg;
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
        int m_bitrate;
        int m_sampleRate;
        QString m_type;
        QString m_ipv4Address;
        QString m_ipv6Address;
        int m_port;
        int m_ttl;
        bool m_autoStart;
        QString m_sessionTimeout;
    };
}
#endif // AUDIOENCODERCONFIGURATION_H
