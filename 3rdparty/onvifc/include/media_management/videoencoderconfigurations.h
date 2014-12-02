#ifndef VIDEOENCODERCONFIGURATIONS_H
#define VIDEOENCODERCONFIGURATIONS_H
#include <QObject>
#include <QList>
namespace ONVIF{
    class VideoEncoderConfigurations : public QObject
    {
        Q_OBJECT
        public:
        explicit VideoEncoderConfigurations(QObject *parent = NULL);
        virtual ~VideoEncoderConfigurations();
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

        QList<int> getWidth()
        {
            return m_width;
        }

        void setWidth(int width)
        {
            m_width.push_back(width);
        }

        QList<int> getHeight()
        {
            return m_height;
        }

        void setHeight(int height)
        {
            m_height.push_back(height);
        }

        QList<int> getQuality()
        {
            return m_quality;
        }

        void setQuality(int quality)
        {
            m_quality.push_back(quality);
        }

        QList<int> getFrameRateLimit()
        {
            return m_frameRateLimit;
        }

        void setFrameRateLimit(int frameRateLimit)
        {
            m_frameRateLimit.push_back(frameRateLimit);
        }

        QList<int> getEncodingInterval()
        {
            return m_encodingInterval;
        }

        void setEncodingInterval(int encodingInterval)
        {
            m_encodingInterval.push_back(encodingInterval);
        }

        QList<int> getBitrateLimit()
        {
            return m_bitrateLimit;
        }

        void setBitrateLimit(int bitrateLimit)
        {
            m_bitrateLimit.push_back(bitrateLimit);
        }

        QList<int> getGovLength()
        {
            return m_govLength;
        }

        void setGovLength(int govLength)
        {
            m_govLength.push_back(govLength);
        }

        QList<QString> getH264Profile()
        {
            return m_h264Profile;
        }

        void setH264Profile(QString h264Profile)
        {
            m_h264Profile.push_back(h264Profile);
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
        QList<int> m_width;
        QList<int> m_height;
        QList<int> m_quality;
        QList<int> m_frameRateLimit;
        QList<int> m_encodingInterval;
        QList<int> m_bitrateLimit;
        QList<int> m_govLength;
        QList<QString> m_h264Profile;
        QList<QString> m_type;
        QList<QString> m_ipv4Address;
        QList<QString> m_ipv6Address;
        QList<int> m_port;
        QList<int> m_ttl;
        QList<bool> m_autoStart;
        QList<QString>m_sessionTimeout;
    };
}
#endif // VIDEOENCODERCONFIGURATIONS_H
