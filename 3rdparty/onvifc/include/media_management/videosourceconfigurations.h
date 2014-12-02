#ifndef VIDEOSOURCECONFIGURATIONS_H
#define VIDEOSOURCECONFIGURATIONS_H
#include <QObject>
#include <QDomElement>
#include <QHash>
#include <QList>
#include <QRect>
namespace ONVIF {
    class VideoSourceConfigurations : public QObject {
        Q_OBJECT
    public:
        explicit VideoSourceConfigurations(QObject *parent = NULL);
        virtual ~VideoSourceConfigurations();
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

        QList<QRect> getBounds()
        {
            return m_bounds;
        }

        void setBounds(QRect rect)
        {
            m_bounds.push_back(rect);
        }
    private:
        QList<QString> m_name;
        QList<int> m_useCount;
        QList<QString> m_sourceToken;
        QList<QRect> m_bounds;
    };
}
#endif
