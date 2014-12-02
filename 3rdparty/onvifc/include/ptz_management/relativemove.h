#ifndef RELATIVEMOVE_H
#define RELATIVEMOVE_H
#include <QObject>
#include <QDomElement>
namespace ONVIF {
    class RelativeMove : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString profileToken READ profileToken WRITE setProfileToken)
        Q_PROPERTY(float translationPanTiltX READ translationPanTiltX WRITE setTranslationPanTiltX)
        Q_PROPERTY(float translationPanTiltY READ translationPanTiltY WRITE setTranslationPanTiltY)
        Q_PROPERTY(QString translationPanTiltSpace READ translationPanTiltSpace WRITE setTranslationPanTiltSpace)
        Q_PROPERTY(float translationZoomX READ translationZoomX WRITE setTranslationZoomX)
        Q_PROPERTY(QString translationZoomSpace READ translationZoomSpace WRITE setTranslationZoomSpace)
        Q_PROPERTY(float speedPanTiltX READ speedPanTiltX WRITE setSpeedPanTiltX)
        Q_PROPERTY(float speedPanTiltY READ speedPanTiltY WRITE setSpeedPanTiltY)
        Q_PROPERTY(QString speedPanTiltSpace READ speedPanTiltSpace WRITE speedPanTiltSpace)
        Q_PROPERTY(float speedZoomX READ speedZoomX WRITE setSpeedZoomX)
        Q_PROPERTY(QString speedZoomSpace READ speedZoomSpace WRITE setSpeedZoomSpace)
        Q_PROPERTY(bool result READ result WRITE setResult)
    public:
        explicit RelativeMove(QObject *parent = NULL);
        virtual ~RelativeMove();
        QDomElement toxml();
        QString profileToken() const
        {
            return m_profileToken;
        }

        float translationPanTiltX() const
        {
            return m_translationPanTiltX;
        }

        float translationPanTiltY() const
        {
            return m_translationPanTiltY;
        }

        QString translationPanTiltSpace() const
        {
            return m_translationPanTiltSpace;
        }

        float translationZoomX() const
        {
            return m_translationZoomX;
        }

        QString translationZoomSpace() const
        {
            return m_translationZoomSpace;
        }

        float speedPanTiltX() const
        {
            return m_speedPanTiltX;
        }

        float speedPanTiltY() const
        {
            return m_speedPanTiltY;
        }

        QString speedPanTiltSpace() const
        {
            return m_speedPanTiltSpace;
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

    public slots:
        void setProfileToken(QString arg)
        {
            m_profileToken = arg;
        }

        void setTranslationPanTiltX(float arg)
        {
            m_translationPanTiltX = arg;
        }

        void setTranslationPanTiltY(float arg)
        {
            m_translationPanTiltY = arg;
        }

        void setTranslationPanTiltSpace(QString arg)
        {
            m_translationPanTiltSpace = arg;
        }

        void setTranslationZoomX(float arg)
        {
            m_translationZoomX = arg;
        }

        void setTranslationZoomSpace(QString arg)
        {
            m_translationZoomSpace = arg;
        }

        void setSpeedPanTiltX(float arg)
        {
            m_speedPanTiltX = arg;
        }

        void setSpeedPanTiltY(float arg)
        {
            m_speedPanTiltY = arg;
        }

        void speedPanTiltSpace(QString arg)
        {
            m_speedPanTiltSpace = arg;
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

    private:
        QString m_profileToken;
        float m_translationPanTiltX;
        float m_translationPanTiltY;
        QString m_translationPanTiltSpace;
        float m_translationZoomX;
        QString m_translationZoomSpace;
        float m_speedPanTiltX;
        float m_speedPanTiltY;
        QString m_speedPanTiltSpace;
        float m_speedZoomX;
        QString m_speedZoomSpace;
        bool m_result;
    };
}
#endif // RELATIVEMOVE_H
