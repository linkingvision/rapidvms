#ifndef AUDIOSOURCECONFIGURATIONS_H
#define AUDIOSOURCECONFIGURATIONS_H
#include <QObject>

namespace  ONVIF {
    class AudioSourceConfigurations : public QObject
    {
        Q_OBJECT
    public:
        explicit AudioSourceConfigurations(QObject *parent = NULL);
        virtual ~AudioSourceConfigurations();
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

        QList<QString> getSourceToken()
        {
            return m_sourceToken;
        }

        void setSourceToken(QString sourceToken)
        {
            m_sourceToken.push_back(sourceToken);
        }
    private:
        QList<QString> m_token;
        QList<QString> m_name;
        QList<int> m_useCount;
        QList<QString> m_sourceToken;
    };
}
#endif // AUDIOSOURCECONFIGURATIONS_H
