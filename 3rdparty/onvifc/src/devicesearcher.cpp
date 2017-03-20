#include "devicesearcher.h"
#include "message.h"
#include <QXmlQuery>
#include <QBuffer>
#include "messageparser.h"
#include <QCoreApplication>
#include <QNetworkInterface>
#include <QThread>
#include <string>

#ifdef WIN32
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#endif

using namespace ONVIF;


//DeviceSearcher* DeviceSearcher::searcher = NULL;

DeviceSearcher* DeviceSearcher::instance(QHostAddress &addr) {
    //if(searcher == NULL) {
        DeviceSearcher* searcher = new DeviceSearcher(addr);
    //}
    return searcher;
}

QList<QHostAddress> DeviceSearcher::getHostAddress()
{
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	QList<QHostAddress> ipAddressesIPV4;
	
	// skip IPV6 address
	QList<QHostAddress>::iterator i;
	for(i=ipAddressesList.begin();i!=ipAddressesList.end();i++)
	{
		std::string ip = (*i).toString().toStdString();
		size_t p1 = ip.find("127.");
		if (p1 != std::string::npos) 
		{
			continue;
		}
		size_t p2 = ip.find(":");
		if (p2 != std::string::npos) 
		{
			continue;
		}
		ipAddressesIPV4.append(*i);
	}
	
	return ipAddressesIPV4;
}

DeviceSearcher::DeviceSearcher(QHostAddress &addr, QObject *parent) : QObject(parent) {
    mUdpSocket = new QUdpSocket(this);
    //QHostAddress host("192.168.0.1");
    //mUdpSocket->bind(QHostAddress::Any, 0, QUdpSocket::ShareAddress);
    mUdpSocket->bind(addr, 0, QUdpSocket::ShareAddress);
	//QThread * pCurrThread = QThread::currentThread();
	//mUdpSocket->moveToThread(pCurrThread);
    //this->moveToThread(pCurrThread);

    int opt=4 * 1024 * 1024;
    if (setsockopt(mUdpSocket->socketDescriptor(), SOL_SOCKET, 
        SO_RCVBUF, (char *)&opt, sizeof(int)) < 0)
    {
	printf("Set ----> SO_RCVBUF error\n");
    }
    
    connect(mUdpSocket, SIGNAL(readyRead()),
		this, SLOT(readPendingDatagrams()), Qt::DirectConnection);
}

DeviceSearcher::~DeviceSearcher() {
    if(this->mUdpSocket != NULL) {
        mUdpSocket->close();
        delete mUdpSocket;
        mUdpSocket = NULL;
    }
}


void DeviceSearcher::sendSearchMsg() {
    Message *msg = Message::getOnvifSearchMessage();
    QString msg_str = msg->toXmlStr();
    //qDebug() << msg_str;
    mUdpSocket->writeDatagram(msg_str.toUtf8(), QHostAddress("239.255.255.250"), 3702);
}

void DeviceSearcher::readPendingDatagrams() {
    while (mUdpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(mUdpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        
        mUdpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
        
//        qDebug() << "========> \n" << datagram << "\n++++++++++++++++++++++++\n";
        
        QHash<QString, QString> namespaces;
        namespaces.insert("SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope");
        namespaces.insert("SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding");
        namespaces.insert("xsi", "http://www.w3.org/2001/XMLSchema-instance");
        namespaces.insert("xsd", "http://www.w3.org/2001/XMLSchema");
        namespaces.insert("c14n", "http://www.w3.org/2001/10/xml-exc-c14n#");
        namespaces.insert("wsu", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd");
        namespaces.insert("xenc", "http://www.w3.org/2001/04/xmlenc#");
        namespaces.insert("ds", "http://www.w3.org/2000/09/xmldsig#");
        namespaces.insert("wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd");
        namespaces.insert("wsa5", "http://schemas.xmlsoap.org/ws/2004/08/addressing");
        namespaces.insert("xmime", "http://tempuri.org/xmime.xsd");
        namespaces.insert("xop", "http://www.w3.org/2004/08/xop/include");
        namespaces.insert("wsa", "http://schemas.xmlsoap.org/ws/2004/08/addressing");
        namespaces.insert("tt", "http://www.onvif.org/ver10/schema");
        namespaces.insert("wsbf", "http://docs.oasis-open.org/wsrf/bf-2");
        namespaces.insert("wstop", "http://docs.oasis-open.org/wsn/t-1");
        namespaces.insert("d", "http://schemas.xmlsoap.org/ws/2005/04/discovery");
        namespaces.insert("wsr", "http://docs.oasis-open.org/wsrf/r-2");
        namespaces.insert("dndl", "http://www.onvif.org/ver10/network/wsdl/DiscoveryLookupBinding");
        namespaces.insert("dnrd", "http://www.onvif.org/ver10/network/wsdl/RemoteDiscoveryBinding");
        namespaces.insert("dn", "http://www.onvif.org/ver10/network/wsdl");
        namespaces.insert("tad", "http://www.onvif.org/ver10/analyticsdevice/wsdl");
        namespaces.insert("tanae", "http://www.onvif.org/ver20/analytics/wsdl/AnalyticsEngineBinding");
        namespaces.insert("tanre", "http://www.onvif.org/ver20/analytics/wsdl/RuleEngineBinding");
        namespaces.insert("tan", "http://www.onvif.org/ver20/analytics/wsdl");
        namespaces.insert("tds", "http://www.onvif.org/ver10/device/wsdl");
        namespaces.insert("tetcp", "http://www.onvif.org/ver10/events/wsdl/CreatePullPointBinding");
        namespaces.insert("tete", "http://www.onvif.org/ver10/events/wsdl/EventBinding");
        namespaces.insert("tetnc", "http://www.onvif.org/ver10/events/wsdl/NotificationConsumerBinding");
        namespaces.insert("tetnp", "http://www.onvif.org/ver10/events/wsdl/NotificationProducerBinding");
        namespaces.insert("tetpp", "http://www.onvif.org/ver10/events/wsdl/PullPointBinding");
        namespaces.insert("tetpps", "http://www.onvif.org/ver10/events/wsdl/PullPointSubscriptionBinding");
        namespaces.insert("tev", "http://www.onvif.org/ver10/events/wsdl");
        namespaces.insert("tetps", "http://www.onvif.org/ver10/events/wsdl/PausableSubscriptionManagerBinding");
        namespaces.insert("wsnt", "http://docs.oasis-open.org/wsn/b-2");
        namespaces.insert("tetsm", "http://www.onvif.org/ver10/events/wsdl/SubscriptionManagerBinding");
        namespaces.insert("timg", "http://www.onvif.org/ver20/imaging/wsdl");
        namespaces.insert("timg10", "http://www.onvif.org/ver10/imaging/wsdl");
        namespaces.insert("tls", "http://www.onvif.org/ver10/display/wsdl");
        namespaces.insert("tmd", "http://www.onvif.org/ver10/deviceIO/wsdl");
        namespaces.insert("tptz", "http://www.onvif.org/ver20/ptz/wsdl");
        namespaces.insert("trc", "http://www.onvif.org/ver10/recording/wsdl");
        namespaces.insert("trp", "http://www.onvif.org/ver10/replay/wsdl");
        namespaces.insert("trt", "http://www.onvif.org/ver10/media/wsdl");
        namespaces.insert("trv", "http://www.onvif.org/ver10/receiver/wsdl");
        namespaces.insert("tse", "http://www.onvif.org/ver10/search/wsdl");
        namespaces.insert("tns1", "http://www.onvif.org/ver10/schema");
        namespaces.insert("tnsn", "http://www.eventextension.com/2011/event/topics");
        namespaces.insert("tnsavg", "http://www.avigilon.com/onvif/ver10/topics");

        
        MessageParser parser(QString(datagram), namespaces);
        
        QHash<QString, QString> device_infos;
        device_infos.insert("ep_address", parser.getValue("//d:ProbeMatches/d:ProbeMatch/wsa:EndpointReference/wsa:Address"));
        device_infos.insert("types", parser.getValue("//d:ProbeMatches/d:ProbeMatch/d:Types"));
        device_infos.insert("device_ip", parser.getValue("//d:ProbeMatches/d:ProbeMatch/d:Scopes"));
        device_infos.insert("device_service_address", parser.getValue("//d:ProbeMatches/d:ProbeMatch/d:XAddrs"));
        device_infos.insert("scopes", parser.getValue("//d:ProbeMatches/d:ProbeMatch/wsa:EndpointReference/wsa:Address"));
        device_infos.insert("metadata_version", parser.getValue("//d:ProbeMatches/d:ProbeMatch/d:MetadataVersion"));
        
        qDebug() << "Device =============>\n" << device_infos;
        emit receiveData(device_infos);
    }    
}
