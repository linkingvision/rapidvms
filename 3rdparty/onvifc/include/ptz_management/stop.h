#ifndef STOP_H
#define STOP_H
#include <QObject>
#include <QDomElement>
namespace ONVIF {
    class Stop : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString profileToken READ profileToken WRITE setProfileToken)
        Q_PROPERTY(bool panTilt READ panTilt WRITE setPanTilt)
        Q_PROPERTY(bool zoom READ zoom WRITE setZoom)
        Q_PROPERTY(bool result READ result WRITE setResult)
    public:
        explicit Stop(QObject *parent = NULL);
        virtual ~Stop();
        QDomElement toxml();
        QString profileToken() const
        {
            return m_profileToken;
        }

        bool panTilt() const
        {
            return m_panTilt;
        }

        bool zoom() const
        {
            return m_zoom;
        }

        bool result() const
        {
            return m_result;
        }

    public slots:
        void setProfileToken(QString arg)
        {
            m_profileToken = arg;
        }

        void setPanTilt(bool arg)
        {
            m_panTilt = arg;
        }

        void setZoom(bool arg)
        {
            m_zoom = arg;
        }

        void setResult(bool arg)
        {
            m_result = arg;
        }

    private:
        QString m_profileToken;
        bool m_panTilt;
        bool m_zoom;
        bool m_result;
    };
}
#endif // STOP_H
