#ifndef ABSOLUTEMOVE_H
#define ABSOLUTEMOVE_H
#include <QObject>
#include <QDomElement>
namespace ONVIF {
    class AbsoluteMove : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString profileToken READ profileToken WRITE setProfileToken)
        Q_PROPERTY(float positionPanTiltX READ positionPanTiltX WRITE setPositionPanTiltX)
        Q_PROPERTY(float positionPanTiltY READ positionPanTiltY WRITE setPositionPanTiltY)
        Q_PROPERTY(QString positionPanTiltSpace READ positionPanTiltSpace WRITE setPositionPanTiltSpace)
        Q_PROPERTY(float positionZoomX READ positionZoomX WRITE setPositionZoomX)
        Q_PROPERTY(QString positionZoomSpace READ positionZoomSpace WRITE setPositionZoomSpace)
        Q_PROPERTY(float speedPanTiltX READ speedPanTiltX WRITE setSpeedPanTiltX)
        Q_PROPERTY(float speedPanTiltY READ speedPanTiltY WRITE setSpeedPanTiltY)
        Q_PROPERTY(QString speedPanTiltSpace READ speedPanTiltSpace WRITE speedPanTiltSpace)
        Q_PROPERTY(float speedZoomX READ speedZoomX WRITE setSpeedZoomX)
        Q_PROPERTY(QString speedZoomSpace READ speedZoomSpace WRITE setSpeedZoomSpace)
        Q_PROPERTY(bool result READ result WRITE setResult)
    public:
        explicit AbsoluteMove(QObject *parent = NULL);
        virtual ~AbsoluteMove();
        QDomElement toxml();
        QString profileToken() const
        {
            return m_profileToken;
        }

        float positionPanTiltX() const
        {
            return m_positionPanTiltX;
        }

        float positionPanTiltY() const
        {
            return m_positionPanTiltY;
        }

        QString positionPanTiltSpace() const
        {
            return m_positionPanTiltSpace;
        }

        float positionZoomX() const
        {
            return m_positionZoomX;
        }

        QString positionZoomSpace() const
        {
            return m_positionZoomSpace;
        }

        float speedPanTiltX() const
        {
            return m_speedPanTiltX;
        }

        float speedPanTiltY() const
        {
            return m_speedPanTiltY;
        }

        float speedZoomX() const
        {
            return m_speedZoomX;
        }

        QString speedZoomSpace() const
        {
            return m_speedZoomSpace;
        }

        bool result() const
        {
            return m_result;
        }

        QString speedPanTiltSpace() const
        {
            return m_speedPanTiltSpace;
        }

    public slots:
        void setProfileToken(QString arg)
        {
            m_profileToken = arg;
        }

        void setPositionPanTiltX(float arg)
        {
            m_positionPanTiltX = arg;
        }

        void setPositionPanTiltY(float arg)
        {
            m_positionPanTiltY = arg;
        }

        void setPositionPanTiltSpace(QString arg)
        {
            m_positionPanTiltSpace = arg;
        }

        void setPositionZoomX(float arg)
        {
            m_positionZoomX = arg;
        }

        void setPositionZoomSpace(QString arg)
        {
            m_positionZoomSpace = arg;
        }

        void setSpeedPanTiltX(float arg)
        {
            m_speedPanTiltX = arg;
        }

        void setSpeedPanTiltY(float arg)
        {
            m_speedPanTiltY = arg;
        }

        void setSpeedZoomX(float arg)
        {
            m_speedZoomX = arg;
        }

        void setSpeedZoomSpace(QString arg)
        {
            m_speedZoomSpace = arg;
        }

        void setResult(bool arg)
        {
            m_result = arg;
        }

        void speedPanTiltSpace(QString arg)
        {
            m_speedPanTiltSpace = arg;
        }

    private:
        QString m_profileToken;
        float m_positionPanTiltX;
        float m_positionPanTiltY;
        QString m_positionPanTiltSpace;
        float m_positionZoomX;
        QString m_positionZoomSpace;
        float m_speedPanTiltX;
        float m_speedPanTiltY;
        float m_speedZoomX;
        QString m_speedZoomSpace;
        bool m_result;
        QString m_speedPanTiltSpace;
    };
}
#endif // ABSOLUTEMOVE_H
