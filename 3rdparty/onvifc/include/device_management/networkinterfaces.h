#ifndef NETWORKINTERFACES_H
#define NETWORKINTERFACES_H
#include <QObject>
#include <QDomElement>
namespace ONVIF {
    class NetworkInterfaces : public QObject {
        Q_OBJECT
        Q_ENUMS(Duplex)
        Q_PROPERTY(bool result READ result WRITE setResult)
        Q_PROPERTY(bool networkInfacesEnabled READ networkInfacesEnabled WRITE setNetworkInfacesEnabled)
        Q_PROPERTY(bool autoNegotiation READ autoNegotiation WRITE setAutoNegotiation)
        Q_PROPERTY(int speed READ speed WRITE setSpeed)
        Q_PROPERTY(Duplex duplex READ duplex WRITE setDuplex)
        Q_PROPERTY(int mtu READ mtu WRITE setMtu)
        Q_PROPERTY(bool ipv4Enabled READ ipv4Enabled WRITE setIpv4Enabled)
        Q_PROPERTY(QString ipv4ManualAddress READ ipv4ManualAddress  WRITE setIpv4ManualAddress)
        Q_PROPERTY(int ipv4ManualPrefixLength READ ipv4ManualPrefixLength WRITE setIpv4ManualPrefixLength)
        Q_PROPERTY(bool ipv4DHCP READ getIpv4DHCP WRITE setIpv4DHCP)
        Q_PROPERTY(QString networkInfacesName READ networkInfacesName WRITE setNetworkInfacesName)
        Q_PROPERTY(QString hwAaddress READ hwAaddress WRITE setHwAaddress)
        Q_PROPERTY(QString ipv4LinkLocalAddress READ ipv4LinkLocalAddress WRITE setIpv4LinkLocalAddress)
        Q_PROPERTY(int ipvLinkLocalPrefixLength READ ipvLinkLocalPrefixLength WRITE setIpvLinkLocalPrefixLength)
        Q_PROPERTY(QString ipv4FromDHCPAddress READ ipv4FromDHCPAddress WRITE setIpv4FromDHCPAddress)
        Q_PROPERTY(int ipv4FromDHCPPrefixLength READ ipv4FromDHCPPrefixLength WRITE setIpv4FromDHCPPrefixLength)
    public:
        explicit NetworkInterfaces(QObject *parent = NULL);
        virtual ~NetworkInterfaces();
        enum Duplex {Full,Half};
        QDomElement toxml();

        bool networkInfacesEnabled() const
        {
            return m_networkInfacesEnabled;
        }

        bool autoNegotiation() const
        {
            return m_autoNegotiation;
        }

        int speed() const
        {
            return m_speed;
        }

        Duplex duplex() const
        {
            return m_duplex;
        }

        int mtu() const
        {
            return m_mtu;
        }

        bool ipv4Enabled() const
        {
            return m_ipv4Enabled;
        }

        QString ipv4ManualAddress() const
        {
            return m_ipv4ManualAddress;
        }

        int ipv4ManualPrefixLength() const
        {
            return m_ipv4ManualPrefixLength;
        }

        QString networkInfacesName() const
        {
            return m_networkInfacesName;
        }

        QString hwAaddress() const
        {
            return m_hwAaddress;
        }

        QString ipv4LinkLocalAddress() const
        {
            return m_ipv4LinkLocalAddress;
        }

        int ipvLinkLocalPrefixLength() const
        {
            return m_ipvLinkLocalPrefixLength;
        }

        QString ipv4FromDHCPAddress() const
        {
            return m_ipv4FromDHCPAddress;
        }

        int ipv4FromDHCPPrefixLength() const
        {
            return m_ipv4FromDHCPPrefixLength;
        }

        bool getIpv4DHCP() const
        {
            return m_ipv4DHCP;
        }

        bool result() const
        {
            return m_result;
        }

    public slots:
        void setNetworkInfacesEnabled(bool arg)
        {
            m_networkInfacesEnabled = arg;
        }

        void setAutoNegotiation(bool arg)
        {
            m_autoNegotiation = arg;
        }

        void setSpeed(int arg)
        {
            m_speed = arg;
        }

        void setDuplex(Duplex arg)
        {
            m_duplex = arg;
        }

        void setMtu(int arg)
        {
            m_mtu = arg;
        }

        void setIpv4Enabled(bool arg)
        {
            m_ipv4Enabled = arg;
        }

        void setIpv4ManualAddress(QString arg)
        {
            m_ipv4ManualAddress = arg;
        }

        void setIpv4ManualPrefixLength(int arg)
        {
            m_ipv4ManualPrefixLength = arg;
        }

        void setNetworkInfacesName(QString arg)
        {
            m_networkInfacesName = arg;
        }

        void setHwAaddress(QString arg)
        {
            m_hwAaddress = arg;
        }

        void setIpv4LinkLocalAddress(QString arg)
        {
            m_ipv4LinkLocalAddress = arg;
        }

        void setIpvLinkLocalPrefixLength(int arg)
        {
            m_ipvLinkLocalPrefixLength = arg;
        }

        void setIpv4FromDHCPAddress(QString arg)
        {
            m_ipv4FromDHCPAddress = arg;
        }

        void setIpv4FromDHCPPrefixLength(int arg)
        {
            m_ipv4FromDHCPPrefixLength = arg;
        }

        void setIpv4DHCP(bool arg)
        {
            m_ipv4DHCP = arg;
        }

        void setResult(bool arg)
        {
            m_result = arg;
        }

    private:
        bool m_networkInfacesEnabled;
        bool m_autoNegotiation;
        int m_speed;
        Duplex m_duplex;
        int m_mtu;
        bool m_ipv4Enabled;
        QString m_ipv4ManualAddress;
        int m_ipv4ManualPrefixLength;
        bool m_ipv4DHCP;
        QString m_networkInfacesName;
        QString m_hwAaddress;
        QString m_ipv4LinkLocalAddress;
        int m_ipvLinkLocalPrefixLength;
        QString m_ipv4FromDHCPAddress;
        int m_ipv4FromDHCPPrefixLength;
        bool m_result;
    };
}
#endif // NETWORKINTERFACES_H
