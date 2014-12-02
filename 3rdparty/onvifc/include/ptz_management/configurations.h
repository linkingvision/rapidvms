#include <QObject>
#include <QList>
namespace ONVIF {
    class Configurations : public QObject
    {
        Q_OBJECT
    public:
        explicit Configurations(QObject *parent = NULL);
        virtual ~Configurations();
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

        QList<QString> getNodeToken()
        {
            return m_nodeToken;
        }

        void setNodeToken(QString nodeToken)
        {
            m_nodeToken.push_back(nodeToken);
        }

        QList<QString> getDefaultAbsolutePantTiltPositionSpace()
        {
            return m_defaultAbsolutePantTiltPositionSpace;
        }

        void setDefaultAbsolutePantTiltPositionSpace(QString defaultAbsolutePantTiltPositionSpace)
        {
            m_defaultAbsolutePantTiltPositionSpace.push_back(defaultAbsolutePantTiltPositionSpace);
        }

        QList<QString> getDefaultAbsoluteZoomPositionSpace()
        {
            return m_defaultAbsoluteZoomPositionSpace;
        }

        void setDefaultAbsoluteZoomPositionSpace(QString defaultAbsoluteZoomPositionSpace)
        {
            m_defaultAbsoluteZoomPositionSpace.push_back(defaultAbsoluteZoomPositionSpace);
        }

        QList<QString> getDefaultRelativePanTiltTranslationSpace()
        {
            return m_defaultRelativePanTiltTranslationSpace;
        }

        void setDefaultRelativePanTiltTranslationSpace(QString defaultRelativePanTiltTranslationSpace)
        {
             m_defaultRelativePanTiltTranslationSpace.push_back(defaultRelativePanTiltTranslationSpace);
        }

        QList<QString> getDefaultRelativeZoomTranslationSpace()
        {
            return m_defaultRelativeZoomTranslationSpace;
        }

        void setDefaultRelativeZoomTranslationSpace(QString defaultRelativeZoomTranslationSpace)
        {
            m_defaultRelativeZoomTranslationSpace.push_back(defaultRelativeZoomTranslationSpace);
        }

        QList<QString> getDefaultContinuousPanTiltVelocitySpace()
        {
            return m_defaultContinuousPanTiltVelocitySpace;
        }

        void setDefaultContinuousPanTiltVelocitySpace(QString defaultContinuousPanTiltVelocitySpace)
        {
            m_defaultContinuousPanTiltVelocitySpace.push_back(defaultContinuousPanTiltVelocitySpace);
        }
        QList<QString> getDefaultContinuousZoomVelocitySpace()
        {
            return m_defaultContinuousZoomVelocitySpace;
        }

        void setDefaultContinuousZoomVelocitySpace(QString defaultContinuousZoomVelocitySpace)
        {
            m_defaultContinuousZoomVelocitySpace.push_back(defaultContinuousZoomVelocitySpace);
        }

        QList<QString> getPanTiltSpace()
        {
            return m_panTiltSpace;
        }

        void setPanTiltSpace(QString panTiltSpace)
        {
            m_panTiltSpace.push_back(panTiltSpace);
        }

        QList<float> getPanTiltY()
        {
            return m_panTiltY;
        }

        void setPanTiltY(float panTiltY)
        {
            m_panTiltY.push_back(panTiltY);
        }

        QList<float> getPanTiltX()
        {
            return m_panTiltX;
        }
        void setPanTiltX(float panTiltX)
        {
            m_panTiltX.push_back(panTiltX);
        }

        QList<QString> getZoomSpace()
        {
            return m_zoomSpace;
        }

        void setZoomSpace(QString zoomSpace)
        {
            m_zoomSpace.push_back(zoomSpace);
        }

        QList<float> getZoomX()
        {
            return m_zoomX;
        }

        void setZoomX(float zoomX)
        {
            m_zoomX.push_back(zoomX);
        }

        QList<QString> getDefaultPTZTimeout()
        {
            return m_defaultPTZTimeout;
        }

        void setDefaultPTZTimeout(QString defaultPTZTimeout)
        {
            m_defaultPTZTimeout.push_back(defaultPTZTimeout);
        }

        QList<QString> getPanTiltRangeUri()
        {
            return m_panTiltRangeUri;
        }

        void setPanTiltRangeUri(QString panTiltRangeUri)
        {
            m_panTiltRangeUri.push_back(panTiltRangeUri);
        }

        QList<float> getPanTiltXRangeMin()
        {
            return m_panTiltXRangeMin;
        }

        void setPanTiltXRangeMin(float panTiltXRangeMin)
        {
            m_panTiltXRangeMin.push_back(panTiltXRangeMin);
        }

        QList<float> getPanTiltXRangeMax()
        {
            return m_panTiltXRangeMax;
        }

        void setPanTiltXRangeMax(float panTiltXRangeMax)
        {
            m_panTiltXRangeMax.push_back(panTiltXRangeMax);
        }

        QList<float> getPanTiltYRangeMin()
        {
            return m_panTiltYRangeMin;
        }

        void setPanTiltYRangeMin(float panTiltYRangeMin)
        {
            m_panTiltYRangeMin.push_back(panTiltYRangeMin);
        }

        QList<float> getPanTiltYRangeMax()
        {
            return m_panTiltYRangeMax;
        }

        void setPanTiltYRangeMax(float panTiltYRangeMax)
        {
            m_panTiltYRangeMax.push_back(panTiltYRangeMax);
        }

        QList<QString> getZoomRangeUri()
        {
            return m_zoomRangeUri;
        }

        void setZoomRangeUri(QString zoomRangeUri)
        {
            m_zoomRangeUri.push_back(zoomRangeUri);
        }

        QList<float> getZoomXRangeMin()
        {
            return m_zoomXRangeMin;
        }

        void setZoomXRangeMin(float zoomXRangeMin)
        {
            m_zoomXRangeMin.push_back(zoomXRangeMin);
        }

        QList<float> getZoomXRangeMax()
        {
            return m_zoomXRangeMax;
        }

        void setZoomXRangeMax(float zoomXRangeMax)
        {
            m_zoomXRangeMax.push_back(zoomXRangeMax);
        }
    private:
        QList<QString> m_token;
        QList<QString> m_name;
        QList<int> m_useCount;
        QList<QString> m_nodeToken;
        QList<QString> m_defaultAbsolutePantTiltPositionSpace;
        QList<QString> m_defaultAbsoluteZoomPositionSpace;
        QList<QString> m_defaultRelativePanTiltTranslationSpace;
        QList<QString> m_defaultRelativeZoomTranslationSpace;
        QList<QString> m_defaultContinuousPanTiltVelocitySpace;
        QList<QString> m_defaultContinuousZoomVelocitySpace;
        QList<QString> m_panTiltSpace;
        QList<float> m_panTiltY;
        QList<float> m_panTiltX;
        QList<QString> m_zoomSpace;
        QList<float> m_zoomX;
        QList<QString> m_defaultPTZTimeout;
        QList<QString> m_panTiltRangeUri;
        QList<float> m_panTiltXRangeMin;
        QList<float> m_panTiltXRangeMax;
        QList<float> m_panTiltYRangeMin;
        QList<float> m_panTiltYRangeMax;
        QList<QString> m_zoomRangeUri;
        QList<float> m_zoomXRangeMin;
        QList<float> m_zoomXRangeMax;


    };
}
