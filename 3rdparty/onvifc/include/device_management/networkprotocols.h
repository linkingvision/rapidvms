#ifndef NETWORKPROTOCOLS_H
#define NETWORKPROTOCOLS_H
#include <QObject>
#include <QList>
namespace ONVIF {
    class NetworkProtocols : public QObject {
        Q_OBJECT
    public:
        explicit NetworkProtocols(QObject *parent = NULL);
        virtual ~NetworkProtocols();

        QList<QString> getNetworkProtocolsName()
        {
            return m_networkProtocolsName;
        }

        void setNetworkProtocolsName(QString networkProtocolsName)
        {
            m_networkProtocolsName.push_back(networkProtocolsName);
        }

        QList<bool> getNetworkProtocolsEnabled()
        {
            return m_networkProtocolsEnabled;
        }

        void setNetworkProtocolsEnabled(bool networkProtocolsEnabled)
        {
            m_networkProtocolsEnabled.push_back(networkProtocolsEnabled);
        }

        QList<int> getNetworkProtocolsPort()
        {
            return m_networkProtocolsPort;
        }

        void setNetworkProtocolsPort(int networkProtocolsPort)
        {
            m_networkProtocolsPort.push_back(networkProtocolsPort);
        }

    private:
        QList<QString> m_networkProtocolsName;
        QList<bool> m_networkProtocolsEnabled;
        QList<int> m_networkProtocolsPort;
    };
}
#endif // NETWORKPROTOCOLS_H
