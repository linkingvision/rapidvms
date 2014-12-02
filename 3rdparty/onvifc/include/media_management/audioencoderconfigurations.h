#ifndef AUDIOENCODERCONFIGURATIONS_H
#define AUDIOENCODERCONFIGURATIONS_H
#include <QObject>
#include <QList>
namespace ONVIF{
    class AudioEncoderConfigurations : public QObject
    {
       Q_OBJECT
    public:
        explicit AudioEncoderConfigurations(QObject *parent = NULL);
        virtual ~AudioEncoderConfigurations();
        QList<QString> getToken()
        {
            return m_token;
        }

        void setToken(QString token)
        {
            m_token.push_back(token);
        }

        QList<QString> getName()
        {
            return m_name;
        }
        void setName(QString name)
        {
            m_name.push_back(name);
        }

        QList<int> getUseCount()
        {
            return m_useCount;
        }

        void setUseCount(int useCount)
        {
            m_useCount.push_back(useCount);
        }

        QList<QString> getEncoding()
        {
            return m_encoding;
        }

        void setEncoding(QString encoding)
        {
            m_encoding.push_back(encoding);
        }

        QList<int> getBitrate()
        {
            return m_bitrate;
        }

        void setBitrate(int bitrate)
        {
            m_bitrate.push_back(bitrate);
        }

        QList<int> getSampleRate()
        {
            return m_sampleRate;
        }

        void setSampleRate(int sampleRate)
        {
            m_sampleRate.push_back(sampleRate);
        }

        QList<QString> getType()
        {
            return m_type;
        }

        void setType(QString type)
        {
            m_type.push_back(type);
        }

        QList<QString> getIpv4Address()
        {
            return m_ipv4Address;
        }

        void setIpv4Address(QString ipv4Address)
        {
            m_ipv4Address.push_back(ipv4Address);
        }

        QList<QString> getIpv6Address()
        {
            return m_ipv6Address;
        }

        void setIpv6Address(QString ipv6Address)
        {
            m_ipv6Address.push_back(ipv6Address);
        }

        QList<int> getPort()
        {
            return m_port;
        }

        void setPort(int port)
        {
            m_port.push_back(port);
        }

        QList<int> getTtl()
        {
            return m_ttl;
        }

        void setTtl(int ttl)
        {
            m_ttl.push_back(ttl);
        }

        QList<bool> getAutoStart()
        {
            return m_autoStart;
        }

        void setAutoStart(bool autoStart)
        {
            m_autoStart.push_back(autoStart);
        }

        QList<QString> getSessionTimeout()
        {
            return m_sessionTimeout;
        }

        void setSessionTimeout(QString sessionTimeout)
        {
            m_sessionTimeout.push_back(sessionTimeout);
        }

    private:
        QList<QString> m_token;
        QList<QString> m_name;
        QList<int> m_useCount;
        QList<QString> m_encoding;
        QList<int> m_bitrate;
        QList<int> m_sampleRate;
        QList<QString> m_type;
        QList<QString> m_ipv4Address;
        QList<QString> m_ipv6Address;
        QList<int> m_port;
        QList<int> m_ttl;
        QList<bool> m_autoStart;
        QList<QString> m_sessionTimeout;
    };
}
#endif // AUDIOENCODERCONFIGURATIONS_H
