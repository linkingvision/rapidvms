#ifndef NODE_H
#define NODE_H
#include <QObject>
#include <QList>
#include <QDomElement>
namespace ONVIF {
    class Node : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString ptzNodeToken READ ptzNodeToken WRITE setPtzNodeToken)
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(QString absolutePanTiltPositionSpaceUri READ absolutePanTiltPositionSpaceUri WRITE setAbsolutePanTiltPositionSpaceUri)
        Q_PROPERTY(float absolutePanTiltPositionSpaceXRangeMin READ absolutePanTiltPositionSpaceXRangeMin WRITE setAbsolutePanTiltPositionSpaceXRangeMin)
        Q_PROPERTY(float absolutePanTiltPositionSpaceXRangeMax READ absolutePanTiltPositionSpaceXRangeMax WRITE setAbsolutePanTiltPositionSpaceXRangeMax)
        Q_PROPERTY(float absolutePanTiltPositionSpaceYRangeMin READ absolutePanTiltPositionSpaceYRangeMin WRITE setAbsolutePanTiltPositionSpaceYRangeMin)
        Q_PROPERTY(float absolutePanTiltPositionSpaceYRangeMax READ absolutePanTiltPositionSpaceYRangeMax WRITE setAbsolutePanTiltPositionSpaceYRangeMax)
        Q_PROPERTY(QString absoluteZoomPositionSpaceUri READ absoluteZoomPositionSpaceUri WRITE setAbsoluteZoomPositionSpaceUri)
        Q_PROPERTY(float absoluteZoomPositionSpaceXRangeMin READ absoluteZoomPositionSpaceXRangeMin WRITE setAbsoluteZoomPositionSpaceXRangeMin)
        Q_PROPERTY(float absoluteZoomPositionSpaceXRangeMax READ absoluteZoomPositionSpaceXRangeMax WRITE setAbsoluteZoomPositionSpaceXRangeMax)
        Q_PROPERTY(QString relativePanTiltTranslationSpaceUri READ relativePanTiltTranslationSpaceUri WRITE setRelativePanTiltTranslationSpaceUri)
        Q_PROPERTY(float relativePanTiltTranslationSpaceXRangeMin READ relativePanTiltTranslationSpaceXRangeMin WRITE setRelativePanTiltTranslationSpaceXRangeMin)
        Q_PROPERTY(float relativePanTiltTranslationSpaceXRangeMax READ relativePanTiltTranslationSpaceXRangeMax WRITE setRelativePanTiltTranslationSpaceXRangeMax)
        Q_PROPERTY(float relativePanTiltTranslationSpaceYRangeMin READ relativePanTiltTranslationSpaceYRangeMin WRITE setRelativePanTiltTranslationSpaceYRangeMin)
        Q_PROPERTY(float relativePanTiltTranslationSpaceYRangeMax READ relativePanTiltTranslationSpaceYRangeMax WRITE setRelativePanTiltTranslationSpaceYRangeMax)
        Q_PROPERTY(QString relativeZoomTranslationSpaceUri READ relativeZoomTranslationSpaceUri WRITE setRelativeZoomTranslationSpaceUri)
        Q_PROPERTY(float relativeZoomTranslationSpaceXRangeMin READ relativeZoomTranslationSpaceXRangeMin WRITE setRelativeZoomTranslationSpaceXRangeMin)
        Q_PROPERTY(float relativeZoomTranslationSpaceXRangeMax READ relativeZoomTranslationSpaceXRangeMax WRITE setRelativeZoomTranslationSpaceXRangeMax)
        Q_PROPERTY(QString continuousPanTiltVelocityUri READ continuousPanTiltVelocityUri WRITE setContinuousPanTiltVelocityUri)
        Q_PROPERTY(float continuousPanTiltVelocityXRangeMin READ continuousPanTiltVelocityXRangeMin WRITE setContinuousPanTiltVelocityXRangeMin)
        Q_PROPERTY(float continuousPanTiltVelocityXRangeMax READ continuousPanTiltVelocityXRangeMax WRITE setContinuousPanTiltVelocityXRangeMax)
        Q_PROPERTY(float continuousPanTiltVelocityYRangeMin READ continuousPanTiltVelocityYRangeMin WRITE setContinuousPanTiltVelocityYRangeMin)
        Q_PROPERTY(float continuousPanTiltVelocityYRangeMax READ continuousPanTiltVelocityYRangeMax WRITE setContinuousPanTiltVelocityYRangeMax)
        Q_PROPERTY(QString continuousZoomVelocitySpaceUri READ continuousZoomVelocitySpaceUri WRITE setContinuousZoomVelocitySpaceUri)
        Q_PROPERTY(float continuousZoomVelocitySpaceXRangeMin READ continuousZoomVelocitySpaceXRangeMin WRITE setContinuousZoomVelocitySpaceXRangeMin)
        Q_PROPERTY(float continuousZoomVelocitySpaceXRangeMax READ continuousZoomVelocitySpaceXRangeMax WRITE setContinuousZoomVelocitySpaceXRangeMax)
        Q_PROPERTY(QString panTiltSpeedSpaceUri READ panTiltSpeedSpaceUri WRITE setPanTiltSpeedSpaceUri)
        Q_PROPERTY(float panTiltSpeedSpaceXRangeMin READ panTiltSpeedSpaceXRangeMin WRITE setPanTiltSpeedSpaceXRangeMin)
        Q_PROPERTY(float panTiltSpeedSpaceXRangeMax READ panTiltSpeedSpaceXRangeMax WRITE setPanTiltSpeedSpaceXRangeMax)
        Q_PROPERTY(QString zoomSpeedSpaceUri READ zoomSpeedSpaceUri WRITE setZoomSpeedSpaceUri)
        Q_PROPERTY(float zoomSpeedSpaceXRangeMin READ zoomSpeedSpaceXRangeMin WRITE setZoomSpeedSpaceXRangeMin)
        Q_PROPERTY(float zoomSpeedSpaceXRangeMax READ zoomSpeedSpaceXRangeMax WRITE setZoomSpeedSpaceXRangeMax)
        Q_PROPERTY(int maximumNumberOfPresets READ maximumNumberOfPresets WRITE setMaximumNumberOfPresets)
        Q_PROPERTY(bool homeSupported READ homeSupport WRITE setHomeSupport)
    public:
        explicit Node(QObject *parent = 0);
        virtual ~Node();
        QDomElement toxml();
        QList< QList<QString> > auxiliaryCommands()
        {
            return m_AuxiliaryCommands;
        }

        void setAuxiliaryCommands(QList<QString> auxiliaryCommands)
        {
           m_AuxiliaryCommands.push_back(auxiliaryCommands);
        }
        QString ptzNodeToken() const
        {
            return m_ptzNodeToken;
        }

        QString name() const
        {
            return m_name;
        }

        QString absolutePanTiltPositionSpaceUri() const
        {
            return m_absolutePanTiltPositionSpaceUri;
        }

        float absolutePanTiltPositionSpaceXRangeMin() const
        {
            return m_absolutePanTiltPositionSpaceXRangeMin;
        }

        float absolutePanTiltPositionSpaceXRangeMax() const
        {
            return m_absolutePanTiltPositionSpaceXRangeMax;
        }

        float absolutePanTiltPositionSpaceYRangeMin() const
        {
            return m_absolutePanTiltPositionSpaceYRangeMin;
        }

        float absolutePanTiltPositionSpaceYRangeMax() const
        {
            return m_absolutePanTiltPositionSpaceYRangeMax;
        }

        QString absoluteZoomPositionSpaceUri() const
        {
            return m_absoluteZoomPositionSpaceUri;
        }

        float absoluteZoomPositionSpaceXRangeMin() const
        {
            return m_absoluteZoomPositionSpaceXRangeMin;
        }

        float absoluteZoomPositionSpaceXRangeMax() const
        {
            return m_absoluteZoomPositionSpaceXRangeMax;
        }

        QString relativePanTiltTranslationSpaceUri() const
        {
            return m_relativePanTiltTranslationSpaceUri;
        }

        float relativePanTiltTranslationSpaceXRangeMin() const
        {
            return m_relativePanTiltTranslationSpaceXRangeMin;
        }

        float relativePanTiltTranslationSpaceXRangeMax() const
        {
            return m_relativePanTiltTranslationSpaceXRangeMax;
        }

        float relativePanTiltTranslationSpaceYRangeMin() const
        {
            return m_relativePanTiltTranslationSpaceYRangeMin;
        }

        float relativePanTiltTranslationSpaceYRangeMax() const
        {
            return m_relativePanTiltTranslationSpaceYRangeMax;
        }

        QString relativeZoomTranslationSpaceUri() const
        {
            return m_relativeZoomTranslationSpaceUri;
        }

        float relativeZoomTranslationSpaceXRangeMin() const
        {
            return m_relativeZoomTranslationSpaceXRangeMin;
        }

        float relativeZoomTranslationSpaceXRangeMax() const
        {
            return m_relativeZoomTranslationSpaceXRangeMax;
        }

        QString continuousPanTiltVelocityUri() const
        {
            return m_continuousPanTiltVelocityUri;
        }

        float continuousPanTiltVelocityXRangeMin() const
        {
            return m_continuousPanTiltVelocityXRangeMin;
        }

        float continuousPanTiltVelocityXRangeMax() const
        {
            return m_continuousPanTiltVelocityXRangeMax;
        }

        float continuousPanTiltVelocityYRangeMin() const
        {
            return m_continuousPanTiltVelocityYRangeMin;
        }

        float continuousPanTiltVelocityYRangeMax() const
        {
            return m_continuousPanTiltVelocityYRangeMax;
        }

        QString continuousZoomVelocitySpaceUri() const
        {
            return m_continuousZoomVelocitySpaceUri;
        }

        float continuousZoomVelocitySpaceXRangeMin() const
        {
            return m_continuousZoomVelocitySpaceXRangeMin;
        }

        float continuousZoomVelocitySpaceXRangeMax() const
        {
            return m_continuousZoomVelocitySpaceXRangeMax;
        }

        QString panTiltSpeedSpaceUri() const
        {
            return m_panTiltSpeedSpaceUri;
        }

        float panTiltSpeedSpaceXRangeMin() const
        {
            return m_panTiltSpeedSpaceXRangeMin;
        }

        float panTiltSpeedSpaceXRangeMax() const
        {
            return m_panTiltSpeedSpaceXRangeMax;
        }

        QString zoomSpeedSpaceUri() const
        {
            return m_zoomSpeedSpaceUri;
        }

        float zoomSpeedSpaceXRangeMin() const
        {
            return m_zoomSpeedSpaceXRangeMin;
        }

        float zoomSpeedSpaceXRangeMax() const
        {
            return m_zoomSpeedSpaceXRangeMax;
        }

        int maximumNumberOfPresets() const
        {
            return m_maximumNumberOfPresets;
        }

        bool homeSupport() const
        {
            return m_homeSupported;
        }

    public slots:
        void setName(QString arg)
        {
            m_name = arg;
        }

        void setAbsolutePanTiltPositionSpaceUri(QString arg)
        {
            m_absolutePanTiltPositionSpaceUri = arg;
        }

        void setAbsolutePanTiltPositionSpaceXRangeMin(float arg)
        {
            m_absolutePanTiltPositionSpaceXRangeMin = arg;
        }

        void setAbsolutePanTiltPositionSpaceXRangeMax(float arg)
        {
            m_absolutePanTiltPositionSpaceXRangeMax = arg;
        }

        void setAbsolutePanTiltPositionSpaceYRangeMin(float arg)
        {
            m_absolutePanTiltPositionSpaceYRangeMin = arg;
        }

        void setAbsolutePanTiltPositionSpaceYRangeMax(float arg)
        {
            m_absolutePanTiltPositionSpaceYRangeMax = arg;
        }

        void setAbsoluteZoomPositionSpaceUri(QString arg)
        {
            m_absoluteZoomPositionSpaceUri = arg;
        }

        void setAbsoluteZoomPositionSpaceXRangeMin(float arg)
        {
            m_absoluteZoomPositionSpaceXRangeMin = arg;
        }

        void setAbsoluteZoomPositionSpaceXRangeMax(float arg)
        {
            m_absoluteZoomPositionSpaceXRangeMax = arg;
        }

        void setRelativePanTiltTranslationSpaceUri(QString arg)
        {
            m_relativePanTiltTranslationSpaceUri = arg;
        }

        void setRelativePanTiltTranslationSpaceXRangeMin(float arg)
        {
            m_relativePanTiltTranslationSpaceXRangeMin = arg;
        }

        void setRelativePanTiltTranslationSpaceXRangeMax(float arg)
        {
            m_relativePanTiltTranslationSpaceXRangeMax = arg;
        }

        void setRelativeZoomTranslationSpaceUri(QString arg)
        {
            m_relativeZoomTranslationSpaceUri = arg;
        }

        void setRelativeZoomTranslationSpaceXRangeMin(float arg)
        {
            m_relativeZoomTranslationSpaceXRangeMin = arg;
        }

        void setRelativeZoomTranslationSpaceXRangeMax(float arg)
        {
            m_relativeZoomTranslationSpaceXRangeMax = arg;
        }

        void setContinuousPanTiltVelocityUri(QString arg)
        {
            m_continuousPanTiltVelocityUri = arg;
        }

        void setContinuousPanTiltVelocityXRangeMin(float arg)
        {
            m_continuousPanTiltVelocityXRangeMin = arg;
        }

        void setContinuousPanTiltVelocityXRangeMax(float arg)
        {
            m_continuousPanTiltVelocityXRangeMax = arg;
        }

        void setContinuousPanTiltVelocityYRangeMin(float arg)
        {
            m_continuousPanTiltVelocityYRangeMin = arg;
        }

        void setContinuousPanTiltVelocityYRangeMax(float arg)
        {
            m_continuousPanTiltVelocityYRangeMax = arg;
        }

        void setContinuousZoomVelocitySpaceUri(QString arg)
        {
            m_continuousZoomVelocitySpaceUri = arg;
        }

        void setContinuousZoomVelocitySpaceXRangeMin(float arg)
        {
            m_continuousZoomVelocitySpaceXRangeMin = arg;
        }

        void setContinuousZoomVelocitySpaceXRangeMax(float arg)
        {
            m_continuousZoomVelocitySpaceXRangeMax = arg;
        }

        void setPanTiltSpeedSpaceUri(QString arg)
        {
            m_panTiltSpeedSpaceUri = arg;
        }

        void setPanTiltSpeedSpaceXRangeMin(float arg)
        {
            m_panTiltSpeedSpaceXRangeMin = arg;
        }

        void setPanTiltSpeedSpaceXRangeMax(float arg)
        {
            m_panTiltSpeedSpaceXRangeMax = arg;
        }

        void setZoomSpeedSpaceUri(QString arg)
        {
            m_zoomSpeedSpaceUri = arg;
        }

        void setZoomSpeedSpaceXRangeMin(float arg)
        {
            m_zoomSpeedSpaceXRangeMin = arg;
        }

        void setZoomSpeedSpaceXRangeMax(float arg)
        {
            m_zoomSpeedSpaceXRangeMax = arg;
        }

        void setMaximumNumberOfPresets(int arg)
        {
            m_maximumNumberOfPresets = arg;
        }

        void setHomeSupport(bool arg)
        {
            m_homeSupported = arg;
        }

        void setPtzNodeToken(QString arg)
        {
            m_ptzNodeToken = arg;
        }

        void setRelativePanTiltTranslationSpaceYRangeMin(float arg)
        {
            m_relativePanTiltTranslationSpaceYRangeMin = arg;
        }

        void setRelativePanTiltTranslationSpaceYRangeMax(float arg)
        {
            m_relativePanTiltTranslationSpaceYRangeMax = arg;
        }

    private:
        QList< QList<QString> > m_AuxiliaryCommands;
        QString m_ptzNodeToken;
        QString m_name;
        QString m_absolutePanTiltPositionSpaceUri;
        float m_absolutePanTiltPositionSpaceXRangeMin;
        float m_absolutePanTiltPositionSpaceXRangeMax;
        float m_absolutePanTiltPositionSpaceYRangeMin;
        float m_absolutePanTiltPositionSpaceYRangeMax;
        QString m_absoluteZoomPositionSpaceUri;
        float m_absoluteZoomPositionSpaceXRangeMin;
        float m_absoluteZoomPositionSpaceXRangeMax;
        QString m_relativePanTiltTranslationSpaceUri;
        float m_relativePanTiltTranslationSpaceXRangeMin;
        float m_relativePanTiltTranslationSpaceXRangeMax;
        float m_relativePanTiltTranslationSpaceYRangeMin;
        float m_relativePanTiltTranslationSpaceYRangeMax;
        QString m_relativeZoomTranslationSpaceUri;
        float m_relativeZoomTranslationSpaceXRangeMin;
        float m_relativeZoomTranslationSpaceXRangeMax;
        QString m_continuousPanTiltVelocityUri;
        float m_continuousPanTiltVelocityXRangeMin;
        float m_continuousPanTiltVelocityXRangeMax;
        float m_continuousPanTiltVelocityYRangeMin;
        float m_continuousPanTiltVelocityYRangeMax;
        QString m_continuousZoomVelocitySpaceUri;
        float m_continuousZoomVelocitySpaceXRangeMin;
        float m_continuousZoomVelocitySpaceXRangeMax;
        QString m_panTiltSpeedSpaceUri;
        float m_panTiltSpeedSpaceXRangeMin;
        float m_panTiltSpeedSpaceXRangeMax;
        QString m_zoomSpeedSpaceUri;
        float m_zoomSpeedSpaceXRangeMin;
        float m_zoomSpeedSpaceXRangeMax;
        int m_maximumNumberOfPresets;
        bool m_homeSupported;
    };
}
#endif // NODE_H
