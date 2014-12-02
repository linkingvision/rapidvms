#ifndef PROFILE_H
#define PROFILE_H
#include <QObject>
#include <QList>
#include <QRect>
namespace ONVIF{
    class Profile : public QObject
    {
        Q_OBJECT
    public:
        explicit Profile(QObject *parent = NULL);
        virtual ~Profile();
        QList<QString> m_toKenPro;
        QList<bool> m_fixed;
        QList<QString> m_namePro;
        QList<QString> m_nameVsc;
        QList<int> m_useCountVsc;
        QList<QString> m_sourceTokenVsc;
        QList<QRect> m_boundsVsc;
        QList<QString> m_nameVec;
        QList<int> m_useCountVec;
        QList<QString> m_encodingVec;
        QList<int> m_widthVec;
        QList<int> m_heightVec;
        QList<int> m_qualityVec;
        QList<int> m_frameRateLimitVec;
        QList<int> m_encodingIntervalVec;
        QList<int> m_bitrateLimitVec;
        QList<int> m_govLengthVec;
        QList<QString> m_h264ProfileVec;
        QList<QString> m_typeVec;
        QList<QString> m_ipv4AddressVec;
        QList<QString> m_ipv6AddressVec;
        QList<int> m_portVec;
        QList<int> m_ttlVec;
        QList<bool> m_autoStartVec;
        QList<QString> m_sessionTimeoutVec;
        QList<QString> m_namePtz;
        QList<int> m_useCountPtz;
        QList<QString> m_nodeToken;
        QList<QString> m_defaultAbsolutePantTiltPositionSpace;
        QList<QString> m_defaultAbsoluteZoomPositionSpace;
        QList<QString> m_defaultRelativePantTiltTranslationSpace;
        QList<QString> m_defaultRelativeZoomTranslationSpace;
        QList<QString> m_defaultContinuousPantTiltVelocitySpace;
        QList<QString> m_defaultContinuousZoomVelocitySpace;
        QList<QString> m_panTiltSpace;
        QList<int> m_panTiltX;
        QList<int> m_panTiltY;
        QList<QString> m_zoomSpace;
        QList<int> m_zoomX;
        QList<QString> m_defaultPTZTimeout;
        QList<QString> m_panTiltUri;
        QList<int> m_xRangeMinPt;
        QList<int> m_xRangeMaxPt;
        QList<int> m_yRangeMinPt;
        QList<int> m_yRangeMaxPt;

        QList<QString> m_zoomUri;
        QList<int> m_xRangeMinZm;
        QList<int> m_xRangeMaxZm;

        QList<QString> m_nameMc;
        QList<int> m_useCountMc;
        QList<bool> m_status;
        QList<bool> m_position;
        QList<QString> m_filter;
        QList<QString> m_subscriptionPolicy;
        QList<bool> m_analytics;
        QList<QString> m_typeMc;
        QList<QString> m_ipv4AddressMc;
        QList<QString> m_ipv6AddressMc;
        QList<int> m_portMc;
        QList<int> m_ttlMc;
        QList<bool> m_autoStartMc;
        QList<QString> m_sessionTimeoutMc;
    };
}
#endif // PROFILE_H
