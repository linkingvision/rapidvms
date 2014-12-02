#ifndef CAPABILITIES_H
#define CAPABILITIES_H
#include <QObject>
#include <QDomElement>
namespace ONVIF {
    class Capabilities : public QObject {
        Q_OBJECT
        Q_ENUMS(Category)
        Q_PROPERTY(QString ptzXAddr READ ptzXAddr WRITE setPtzXAddr)
        Q_PROPERTY(QString imagingXAddr READ imagingXAddr WRITE setImagingXAddr)
        Q_PROPERTY(QString mediaXAddr READ mediaXAddr WRITE setMediaXAddr)
        Q_PROPERTY(bool rtpMulticast READ rtpMulticast WRITE setRtpMulticast)
        Q_PROPERTY(bool rtpTcp READ rtpTcp WRITE setRtpTcp)
        Q_PROPERTY(bool rtpRtspTcp READ rtpRtspTcp WRITE setRtpRtspTcp)
        Q_PROPERTY(QString deviceXAddr READ deviceXAddr WRITE setDeviceXAddr)
        Q_PROPERTY(bool iPFilter READ iPFilter WRITE setIPFilter)
        Q_PROPERTY(bool zeroConfiguration READ zeroConfiguration WRITE setZeroConfiguration)
        Q_PROPERTY(bool iPVersion6 READ iPVersion6 WRITE setIPVersion6)
        Q_PROPERTY(bool dynDNS READ dynDNS WRITE setDynDNS)
        Q_PROPERTY(bool discoveryResolve READ disconveryResolve WRITE setDiscoveryResolve)
        Q_PROPERTY(bool discoveryBye READ discoveryBye WRITE setDiscoveryBye)
        Q_PROPERTY(bool remoteDiscovery READ remoteDiscovery WRITE setRemoteDiscovery)
        Q_PROPERTY(bool systemBackup READ systemBackup WRITE setSystemBackup)
        Q_PROPERTY(bool systemLogging READ systemLogging WRITE setSystemLogging)
        Q_PROPERTY(bool firmwareUpgrade READ firmwareUpgrade WRITE setFirmwareUpgrade)
        Q_PROPERTY(int major READ major WRITE setMajor)
        Q_PROPERTY(bool minor READ minor WRITE setMinor)
        Q_PROPERTY(bool httpFirmwareUpgrade READ httpFirmwareUpgrade WRITE setHttpFrimwareUpgrade)
        Q_PROPERTY(bool httpSystemBackup READ httpSystemBackup WRITE setHttpSystemBackup)
        Q_PROPERTY(bool httpSystemLogging READ httpSystemLogging WRITE setHttpSystemLogging)
        Q_PROPERTY(bool httpSupportInformation READ httpSupportInformation WRITE setHttpSupportInformation)
        Q_PROPERTY(int inputConnectors READ inputConnectors WRITE setInputConnectors)
        Q_PROPERTY(int relayOutputs READ relayOutputs WRITE setRelayOutputs)
        Q_PROPERTY(bool tls11 READ tls11  WRITE setTls11)
        Q_PROPERTY(bool tls22 READ tls22 WRITE setTls22)
        Q_PROPERTY(bool onboardKeyGeneration READ onboardKeyGeneration WRITE setOnboardKeyGeneration)
        Q_PROPERTY(bool accessPolicyConfig READ accessPolicyConfig WRITE setAccessPolicyConfig)
        Q_PROPERTY(bool x509Token READ x509Token WRITE setX509Token)
        Q_PROPERTY(bool samlToken READ samlToken WRITE setSamlToken)
        Q_PROPERTY(bool kerberosToken READ kerberosToken WRITE setKerberosToken)
        Q_PROPERTY(bool relToken READ relToken WRITE setRelToken)
        Q_PROPERTY(bool tls10 READ tls10 WRITE setTls10)
        Q_PROPERTY(bool dot1x READ dot1x WRITE setDot1x)
        Q_PROPERTY(bool remoteUserHanding READ remoteUserHanding WRITE setRemoteUserHanding)

    public:
        explicit Capabilities(QObject *parent = NULL);
        virtual ~Capabilities();
        enum Category {All,Analytics,Device,Events,Imaging,Media,PTZ};
        QString enumToString(Category category);
        QDomElement toxml();
        QString ptzXAddr() const
        {
            return m_ptzXAddr;
        }
        QString imagingXAddr() const
        {
            return m_imagingXAddr;
        }

        QString mediaXAddr() const
        {
            return m_mediaXAddr;
        }

        bool rtpMulticast() const
        {
            return m_rtpMulticast;
        }

        bool rtpTcp() const
        {
            return m_rtpTcp;
        }

        bool rtpRtspTcp() const
        {
            return m_rtpRtspTcp;
        }

        QString deviceXAddr() const
        {
            return m_deviceXAddr;
        }

        bool iPFilter() const
        {
            return m_iPFilter;
        }

        bool zeroConfiguration() const
        {
            return m_zeroConfiguration;
        }

        bool iPVersion6() const
        {
            return m_iPVersion6;
        }

        bool dynDNS() const
        {
            return m_dynDNS;
        }

        bool disconveryResolve() const
        {
            return m_discoveryResolve;
        }

        bool systemLogging() const
        {
            return m_systemLogging;
        }

        bool firmwareUpgrade() const
        {
            return m_firmwareUpgrade;
        }

        int major() const
        {
            return m_major;
        }

        bool minor() const
        {
            return m_minor;
        }

        bool httpFirmwareUpgrade() const
        {
            return m_httpFirmwareUpgrade;
        }

        bool httpSystemBackup() const
        {
            return m_httpSystemBackup;
        }

        bool httpSystemLogging() const
        {
            return m_httpSystemLogging;
        }

        bool httpSupportInformation() const
        {
            return m_httpSupportInformation;
        }

        int inputConnectors() const
        {
            return m_inputConnectors;
        }

        int relayOutputs() const
        {
            return m_relayOutputs;
        }

        bool tls11() const
        {
            return m_tls11;
        }

        bool tls22() const
        {
            return m_tls22;
        }

        bool onboardKeyGeneration() const
        {
            return m_onboardKeyGeneration;
        }

        bool accessPolicyConfig() const
        {
            return m_accessPolicyConfig;
        }

        bool x509Token() const
        {
            return m_x509Token;
        }

        bool samlToken() const
        {
            return m_samlToken;
        }

        bool kerberosToken() const
        {
            return m_kerberosToken;
        }

        bool relToken() const
        {
            return m_relToken;
        }

        bool tls10() const
        {
            return m_tls10;
        }

        bool dot1x() const
        {
            return m_dot1x;
        }

        bool remoteUserHanding() const
        {
            return m_remoteUserHanding;
        }

        bool systemBackup() const
        {
            return m_systemBackup;
        }

        bool discoveryBye() const
        {
            return m_discoveryBye;
        }

        bool remoteDiscovery() const
        {
            return m_remoteDiscovery;
        }

    public slots:
        void setPtzXAddr(QString arg)
        {
            m_ptzXAddr = arg;
        }
        void setImagingXAddr(QString arg)
        {
            m_imagingXAddr = arg;
        }

        void setMediaXAddr(QString arg)
        {
            m_mediaXAddr = arg;
        }

        void setRtpMulticast(bool arg)
        {
            m_rtpMulticast = arg;
        }

        void setRtpTcp(bool arg)
        {
            m_rtpTcp = arg;
        }

        void setRtpRtspTcp(bool arg)
        {
            m_rtpRtspTcp = arg;
        }

        void setDeviceXAddr(QString arg)
        {
            m_deviceXAddr = arg;
        }

        void setIPFilter(bool arg)
        {
            m_iPFilter = arg;
        }

        void setZeroConfiguration(bool arg)
        {
            m_zeroConfiguration = arg;
        }

        void setIPVersion6(bool arg)
        {
            m_iPVersion6 = arg;
        }

        void setDynDNS(bool arg)
        {
            m_dynDNS = arg;
        }

        void setDiscoveryResolve(bool arg)
        {
            m_discoveryResolve = arg;
        }

        void setSystemLogging(bool arg)
        {
            m_systemLogging = arg;
        }

        void setFirmwareUpgrade(bool arg)
        {
            m_firmwareUpgrade = arg;
        }

        void setMajor(int arg)
        {
            m_major = arg;
        }

        void setMinor(bool arg)
        {
            m_minor = arg;
        }

        void setHttpFrimwareUpgrade(bool arg)
        {
            m_httpFirmwareUpgrade = arg;
        }

        void setHttpSystemBackup(bool arg)
        {
            m_httpSystemBackup = arg;
        }

        void setHttpSystemLogging(bool arg)
        {
            m_httpSystemLogging = arg;
        }

        void setHttpSupportInformation(bool arg)
        {
            m_httpSupportInformation = arg;
        }

        void setInputConnectors(int arg)
        {
            m_inputConnectors = arg;
        }

        void setRelayOutputs(int arg)
        {
            m_relayOutputs = arg;
        }

        void setTls11(bool arg)
        {
            m_tls11 = arg;
        }

        void setTls22(bool arg)
        {
            m_tls22 = arg;
        }

        void setOnboardKeyGeneration(bool arg)
        {
            m_onboardKeyGeneration = arg;
        }

        void setAccessPolicyConfig(bool arg)
        {
            m_accessPolicyConfig = arg;
        }

        void setX509Token(bool arg)
        {
            m_x509Token = arg;
        }

        void setSamlToken(bool arg)
        {
            m_samlToken = arg;
        }

        void setKerberosToken(bool arg)
        {
            m_kerberosToken = arg;
        }

        void setRelToken(bool arg)
        {
            m_relToken = arg;
        }

        void setTls10(bool arg)
        {
            m_tls10 = arg;
        }

        void setDot1x(bool arg)
        {
            m_dot1x = arg;
        }

        void setRemoteUserHanding(bool arg)
        {
            m_remoteUserHanding = arg;
        }

        void setSystemBackup(bool arg)
        {
            m_systemBackup = arg;
        }

        void setDiscoveryBye(bool arg)
        {
            m_discoveryBye = arg;
        }

        void setRemoteDiscovery(bool arg)
        {
            m_remoteDiscovery = arg;
        }

    private:
        QString m_ptzXAddr;
        QString m_imagingXAddr;
        QString m_mediaXAddr;
        bool m_rtpMulticast;
        bool m_rtpTcp;
        bool m_rtpRtspTcp;
        QString m_deviceXAddr;
        bool m_iPFilter;
        bool m_zeroConfiguration;
        bool m_iPVersion6;
        bool m_dynDNS;
        bool m_discoveryResolve;
        bool m_systemLogging;
        bool m_firmwareUpgrade;
        int m_major;
        bool m_minor;
        bool m_httpFirmwareUpgrade;
        bool m_httpSystemBackup;
        bool m_httpSystemLogging;
        bool m_httpSupportInformation;
        int m_inputConnectors;
        int m_relayOutputs;
        bool m_tls11;
        bool m_tls22;
        bool m_onboardKeyGeneration;
        bool m_accessPolicyConfig;
        bool m_x509Token;
        bool m_samlToken;
        bool m_kerberosToken;
        bool m_relToken;
        bool m_tls10;
        bool m_dot1x;
        bool m_remoteUserHanding;
        bool m_systemBackup;
        bool m_discoveryBye;
        bool m_remoteDiscovery;
    };
}
#endif // CAPABILITIES_H
