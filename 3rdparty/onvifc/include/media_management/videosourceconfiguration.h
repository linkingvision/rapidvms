#ifndef VIDEOSOURCECONFIGURATION_H
#define VIDEOSOURCECONFIGURATION_H
#include <QObject>
#include <QRect>
namespace  ONVIF {
    class VideoSourceConfiguration : public QObject
    {
        Q_OBJECT
    public:
        explicit VideoSourceConfiguration(QObject *parent = NULL);
        virtual ~VideoSourceConfiguration();
        Q_PROPERTY(QString token READ token WRITE setToken)
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(int useCount READ useCount WRITE setUseCount)
        Q_PROPERTY(QString sourceToken READ sourceToken WRITE setSourceToken)
        Q_PROPERTY(QRect bounds READ bounds  WRITE setBounds)
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

        QString sourceToken() const
        {
            return m_sourceToken;
        }

        QRect bounds() const
        {
            return m_bounds;
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

        void setSourceToken(QString arg)
        {
            m_sourceToken = arg;
        }

        void setBounds(QRect arg)
        {
            m_bounds = arg;
        }

    private:
        QString m_token;
        QString m_name;
        int m_useCount;
        QString m_sourceToken;
        QRect m_bounds;
    };
}
#endif // VIDEOSOURCECONFIGURATION_H
