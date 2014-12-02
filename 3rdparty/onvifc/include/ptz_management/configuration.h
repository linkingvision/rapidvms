#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <QObject>
#include <QDomElement>
namespace ONVIF {
    class Configuration : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString ptzConfigurationToken READ ptzConfigurationToken WRITE setPtzConfigurationToken)
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(int useCount READ useCount WRITE setUseCount)
        Q_PROPERTY(QString nodeToken READ nodeToken WRITE setNodeToken)
        //Q_PROPERTY(QString defaultAbsolutePantTiltPositionSpace READ defaultAbsolutePantTiltPositionSpace WRITE setDefaultAbsolutePanTiltPositionSpace)
        Q_PROPERTY(QString defaultAbsoluteZoomPositionSpace READ defaultAbsoluteZoomPositionSpace WRITE setDefaultAbsoluteZoomPositionSpace)
        Q_PROPERTY(QString defaultRelativePanTiltTranslationSpace READ defaultRelativePanTiltTranslationSpace WRITE setDefaultRelativePanTiltTranslationSpace)
        Q_PROPERTY(QString defaultRelativeZoomTranslationSpace READ defaultRelativeZoomTranslationSpace WRITE setDefaultRelativeZoomTranslationSpace)
        Q_PROPERTY(QString defaultContinuousPanTiltVelocitySpace READ defaultContinuousPanTiltVelocitySpace  WRITE setDefaultContinuousPanTiltVelocitySpace)
        Q_PROPERTY(QString defaultContinuousZoomVelocitySpace READ defaultContinuousZoomVelocitySpace WRITE setDefaultContinuousZoomVelocitySpace)
        Q_PROPERTY(QString panTiltSpace READ panTiltSpace WRITE setPanTiltSpace)
        Q_PROPERTY(float panTiltX READ panTiltX WRITE setPanTiltX)
        Q_PROPERTY(float panTiltY READ panTiltY WRITE setPanTiltY)
        Q_PROPERTY(QString zoomSpace READ zoomSpace WRITE setZoomSpace)
        Q_PROPERTY(float zoomX READ zoomX WRITE setZoomX)
        Q_PROPERTY(QString defaultPTZTimeout READ defaultPTZTimeout WRITE setDefaultPTZTimeout)
        Q_PROPERTY(QString panTiltUri READ panTiltUri WRITE setPanTiltUri)
        Q_PROPERTY(float panTiltXRangeMin READ panTiltXRangeMin WRITE setPanTiltXRangeMin)
        Q_PROPERTY(float panTiltXRangeMax READ panTiltXRangeMax WRITE setPanTiltXRangeMax)
        Q_PROPERTY(float panTiltYRangeMin READ panTiltYRangeMin WRITE setPanTiltYRangeMin)
        Q_PROPERTY(float panTiltYRangeMax READ panTiltYRangeMax WRITE setPanTiltYRangeMax)
        Q_PROPERTY(QString zoomUri READ zoomUri WRITE setZoomUri)
        Q_PROPERTY(float zoomXRangeMin READ zoomXRangeMin WRITE setZoomXRangeMin)
        Q_PROPERTY(float zoomXRangeMax READ zoomXRangeMax WRITE setZoomXRangeMax)


    public:
        explicit Configuration(QObject *parent = 0);
        virtual ~Configuration();
        QDomElement toxml();
        QString name() const
        {
            return m_name;
        }

        int useCount() const
        {
            return m_useCount;
        }

        QString nodeToken() const
        {
            return m_nodeToken;
        }

        QString defaultAbsolutePantTiltPositionSpace() const
        {
            return m_defaultAbsolutePantTiltPositionSpace;
        }

        QString defaultAbsoluteZoomPositionSpace() const
        {
            return m_defaultAbsoluteZoomPositionSpace;
        }

        QString defaultRelativePanTiltTranslationSpace() const
        {
            return m_defaultRelativePanTiltTranslationSpace;
        }

        QString defaultRelativeZoomTranslationSpace() const
        {
            return m_defaultRelativeZoomTranslationSpace;
        }

        QString defaultContinuousPanTiltVelocitySpace() const
        {
            return m_defaultContinuousPanTiltVelocitySpace;
        }

        QString defaultContinuousZoomVelocitySpace() const
        {
            return m_defaultContinuousZoomVelocitySpace;
        }

        float panTiltX() const
        {
            return m_panTiltX;
        }

        float panTiltY() const
        {
            return m_panTiltY;
        }

        QString zoomSpace() const
        {
            return m_zoomSpace;
        }

        QString defaultPTZTimeout() const
        {
            return m_defaultPTZTimeout;
        }

        QString panTiltUri() const
        {
            return m_panTiltUri;
        }

        float panTiltXRangeMin() const
        {
            return m_panTiltXRangeMin;
        }

        float panTiltXRangeMax() const
        {
            return m_panTiltXRangeMax;
        }

        float panTiltYRangeMin() const
        {
            return m_panTiltYRangeMin;
        }

        float panTiltYRangeMax() const
        {
            return m_panTiltYRangeMax;
        }

        QString zoomUri() const
        {
            return m_zoomUri;
        }

        float zoomXRangeMin() const
        {
            return m_zoomXRangeMin;
        }

        float zoomXRangeMax() const
        {
            return m_zoomXRangeMax;
        }

        QString ptzConfigurationToken() const
        {
            return m_ptzConfigurationToken;
        }

        QString panTiltSpace() const
        {
            return m_panTiltSpace;
        }

        float zoomX() const
        {
            return m_zoomX;
        }

    public slots:
        void setName(QString arg)
        {
            m_name = arg;
        }

        void setUseCount(int arg)
        {
            m_useCount = arg;
        }

        void setNodeToken(QString arg)
        {
            m_nodeToken = arg;
        }

        void setDefaultAbsolutePantTiltPositionSpace(QString arg)
        {
            m_defaultAbsolutePantTiltPositionSpace = arg;
        }

        void setDefaultAbsoluteZoomPositionSpace(QString arg)
        {
            m_defaultAbsoluteZoomPositionSpace = arg;
        }

        void setDefaultRelativePanTiltTranslationSpace(QString arg)
        {
            m_defaultRelativePanTiltTranslationSpace = arg;
        }

        void setDefaultRelativeZoomTranslationSpace(QString arg)
        {
            m_defaultRelativeZoomTranslationSpace = arg;
        }

        void setDefaultContinuousPanTiltVelocitySpace(QString arg)
        {
            m_defaultContinuousPanTiltVelocitySpace = arg;
        }

        void setDefaultContinuousZoomVelocitySpace(QString arg)
        {
            m_defaultContinuousZoomVelocitySpace = arg;
        }

        void setPanTiltX(float arg)
        {
            m_panTiltX = arg;
        }

        void setPanTiltY(float arg)
        {
            m_panTiltY = arg;
        }

        void setZoomSpace(QString arg)
        {
            m_zoomSpace = arg;
        }

        void setDefaultPTZTimeout(QString arg)
        {
            m_defaultPTZTimeout = arg;
        }

        void setPanTiltUri(QString arg)
        {
            m_panTiltUri = arg;
        }

        void setPanTiltXRangeMin(float arg)
        {
            m_panTiltXRangeMin = arg;
        }

        void setPanTiltXRangeMax(float arg)
        {
            m_panTiltXRangeMax = arg;
        }

        void setPanTiltYRangeMin(float arg)
        {
            m_panTiltYRangeMin = arg;
        }

        void setPanTiltYRangeMax(float arg)
        {
            m_panTiltYRangeMax = arg;
        }

        void setZoomUri(QString arg)
        {
            m_zoomUri = arg;
        }

        void setZoomXRangeMin(float arg)
        {
            m_zoomXRangeMin = arg;
        }

        void setZoomXRangeMax(float arg)
        {
            m_zoomXRangeMax = arg;
        }

        void setPtzConfigurationToken(QString arg)
        {
            m_ptzConfigurationToken = arg;
        }

        void setPanTiltSpace(QString arg)
        {
            m_panTiltSpace = arg;
        }

        void setZoomX(float arg)
        {
            m_zoomX = arg;
        }

    private:
        QString m_profileToken;
        QString m_name;
        int m_useCount;
        QString m_nodeToken;
        QString m_defaultAbsolutePantTiltPositionSpace;
        QString m_defaultAbsoluteZoomPositionSpace;
        QString m_defaultRelativePanTiltTranslationSpace;
        QString m_defaultRelativeZoomTranslationSpace;
        QString m_defaultContinuousPanTiltVelocitySpace;
        QString m_defaultContinuousZoomVelocitySpace;
        float m_panTiltX;
        float m_panTiltY;
        QString m_zoomSpace;
        QString m_defaultPTZTimeout;
        QString m_panTiltUri;
        float m_panTiltXRangeMin;
        float m_panTiltXRangeMax;
        float m_panTiltYRangeMin;
        float m_panTiltYRangeMax;
        QString m_zoomUri;
        float m_zoomXRangeMin;
        float m_zoomXRangeMax;
        QString m_ptzConfigurationToken;
        QString m_panTiltSpace;
        float m_zoomX;
    };
}
#endif // CONFIGURATION_H
